#pragma once

#include <cstdint>
#include <tuple>
#include <optional>
#include <cstring>
#include <vector>
#include "IoUtils.hh"
#include <fifo_map.hpp>
#include <nlohmann/json.hpp>

namespace AMT {

template <class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map
    = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using ordered_json = nlohmann::basic_json<my_workaround_fifo_map>;

template <typename T>
concept HasArchiveOffsets = requires(T a, std::vector<uint32_t> &b, uint32_t c)
{
    a.GetArchiveOffsets (b, c);
};
template <typename T>
concept HasHashOffsets = requires(T a, std::vector<uint32_t> &b, uint32_t c)
{
    a.GetHashOffsets (b, c);
};
template <typename T> concept HasReadFunction = requires(T a, uint8_t *in)
{
    a.Read (in);
};
template <typename T> concept HasWriteFunction = requires(T a, std::ostream &b)
{
    a.Write (b);
};
template <typename T> concept HasGetSizeFunction = requires(T a, uint32_t b)
{
    a.GetSize (b);
};
template <typename T>
concept HasGetArchiveNames
    = requires(T a, nlohmann::fifo_map<std::string, int> &b)
{
    a.GetArchiveNames (b);
};
    
class BasicData
{
public:
    template <typename T>
    static inline uint8_t *
    Read (uint8_t *data, T &out)
    {
        if constexpr (!HasReadFunction<T>)
            {
                memcpy (&out, data, sizeof (decltype (out)));
                data += sizeof (decltype (out));
            }
        else
            {
                data = out.Read (data);
            }

        return data;
    }

    template <typename T>
    static inline void
    Write (std::ostream &out, T &data)
    {
        if constexpr (!HasWriteFunction<T>)
            {
                IoUtils::WriteData (out, data);
            }
        else
            {
                data.Write (out);
            }
    }

    template <typename T>
    static uint32_t
    GetSize (uint32_t initialSize, const T &data)
    {
        if constexpr (!HasGetSizeFunction<T>)
            {
                return initialSize + sizeof (decltype (data));
            }
        else
            {
                return data.GetSize (initialSize);
            }
    }

    template <typename T>
    static void
    GetArchiveOffsets (std::vector<uint32_t> &out, uint32_t currentOffset,
                       const T &data)
    {
        if constexpr (HasArchiveOffsets<T>)
            {
                data.GetArchiveOffsets (out, currentOffset);
            }
    }

    template <typename T>
    static void
    GetHashOffsets (std::vector<uint32_t>& out, uint32_t currentOffset,
                    const T &data)
    {
        if constexpr (HasHashOffsets<T>)
            {
                data.GetHashOffsets (out, currentOffset);
            }
    }

    template <typename T>
    static void
    GetArchiveNames (nlohmann::fifo_map<std::string, int> &out, const T &data)
    {
        if constexpr (HasGetArchiveNames<T>)
            {
                data.GetArchiveNames (out);
            }
    }
};

/* For string literal template arguments (C++20 onwards only) */
template <unsigned N> struct FixedString
{
    char buf[N + 1]{};
    constexpr FixedString (char const *s)
    {
        for (unsigned i = 0; i != N; ++i)
            buf[i] = s[i];
    }
    constexpr operator char const * () const { return buf; }
};
template <unsigned N> FixedString (char const (&)[N]) -> FixedString<N - 1>;

/* A class to store information about a specific field */
template <typename T, FixedString name, bool isHash = false,
          bool isArchive = false>
struct FieldType
{
    using TypeName                 = T;
    inline static const char *Name = name;

    T Data;

    inline void
    GetArchiveOffsets (std::vector<uint32_t>& out,
                       uint32_t              currentOffset) const
    {
        if constexpr (isArchive)
            {
                if (Data.Hash != 0xFFFFFFFF)
                    out.push_back (currentOffset);
            }
        BasicData::GetArchiveOffsets (out, currentOffset, Data);
    }

    inline void
    GetHashOffsets (std::vector<uint32_t>& out, uint32_t currentOffset) const
    {
        if constexpr (isHash)
            {
                if (Data.Hash != 0xFFFFFFFF)
                    out.push_back (currentOffset);
            }
        BasicData::GetHashOffsets (out, currentOffset, Data);
    }

    inline void
    GetArchiveNames (nlohmann::fifo_map<std::string, int> &out) const
    {
        if constexpr (isArchive)
            {
                if (Data.Hash != 0xFFFFFFFF)
                    out[Data.ToString ()] = 1;
            }
        else
            {
                BasicData::GetArchiveNames (out, Data);
            }
    }
};

template <typename S = uint8_t> class StringWrapper
{
public:
    std::string Data;

    uint8_t *
    Read (uint8_t *in)
    {
        S size = IoUtils::ReadData<S> (in);
        Data.resize (size);

        IoUtils::ReadData (in, &Data[0], size);
        return in;
    }

    void
    Write (std::ostream &out)
    {
        IoUtils::WriteData (out, (S) Data.size ());
        out.write (Data.c_str (), Data.size ());
    }

    uint32_t
    GetSize (uint32_t initialSize = 0) const
    {
        return Data.size () + 1 + initialSize;
    }

    void
    ToJson (ordered_json &j) const
    {
        j = Data;
    }

    void
    FromJson (const ordered_json &j)
    {
        Data = j;
    }
};

template <typename T> class OptionalWrapper
{
public:
    std::optional<T> Data;

    uint8_t *
    Read (uint8_t *in)
    {
        T data;
        in = BasicData::Read (in, data);

        Data = data;
        return in;
    }

    void
    Write (std::ostream& out)
    {
        if (HasValue ())
            IoUtils::WriteData (out, *Data);
    }

    
    void
    ToJson (ordered_json &j) const
    {
        if (HasValue ())
            j = *Data;
    }

    void
    FromJson (const ordered_json &j)
    {
        Data = j;
    }

    bool
    HasValue () const
    {
        return Data.has_value ();
    }

    uint32_t
    GetSize (uint32_t initialSize = 0) const
    {
        if (HasValue ())
            return BasicData::GetSize (initialSize, *Data);
        return initialSize;
    }
};

template <typename T, int size> class FixedArrayWrapper
{
public:
    T Data[size];
    void
    ToJson (ordered_json &j) const
    {
        j = Data;
    }

    void
    FromJson (const ordered_json &j)
    {
        std::vector<T> vec = j;
        for (int i = 0; i < size; i++)
            Data[i] = vec[i];
    }
};

    template <typename T, typename S = uint8_t, bool storesHashes = false> class ArrayWrapper
{
public:
    std::vector<T> Data;

    uint8_t *
    Read (uint8_t *in)
    {
        Data.clear ();

        S size = IoUtils::ReadData<S> (in);
        Data.resize (size);

        for (int i = 0; i < size; i++)
            in = BasicData::Read (in, Data[i]);

        return in;
    }

    void
    Write (std::ostream& out)
    {
        IoUtils::WriteData(out, S(Data.size()));

        for (auto& i : Data)
            BasicData::Write (out, i);
    }

    
    void
    GetHashOffsets (std::vector<uint32_t>& out,
                    uint32_t              currentOffset = 0) const
    {
        currentOffset += sizeof (S);
        for (auto &i : Data)
            {
                if constexpr (storesHashes)
                    {
                        if (i.Hash != 0xFFFFFFFF)
                            out.push_back (currentOffset);
                    }
                else
                    BasicData::GetHashOffsets (out, currentOffset, i);

                currentOffset += BasicData::GetSize (0, i);
            }
    }

    void
    GetArchiveOffsets (std::vector<uint32_t>& out,
                       uint32_t              currentOffset = 0) const
    {
        currentOffset += sizeof (S);
        for (auto &i : Data)
            {
                BasicData::GetArchiveOffsets (out, currentOffset, i);
                currentOffset += BasicData::GetSize (0, i);
            }
    }

    uint32_t
    GetSize (uint32_t initialSize = 0) const
    {
        initialSize += sizeof (S);
        for (const auto &i : Data)
            initialSize = BasicData::GetSize (initialSize, i);

        return initialSize;
    }

    void
    ToJson (ordered_json &j) const
    {
        j = Data;
    }

    void
    FromJson (const ordered_json &j)
    {
        Data = j.get<std::vector<T>>();
    }
};

template <typename T, unsigned int N> struct EnumWrapperValue
{
    FixedString<N> Name;
    const T        Value;
};
template <typename T, unsigned N>
EnumWrapperValue (char const (&)[N], T value) -> EnumWrapperValue<T, N - 1>;

template <typename T, EnumWrapperValue... Values>
class EnumWrapper
{
public:
    T Data;

    void
    ToJson (ordered_json &j) const
    {
        (..., (Values.Value == Data ? (j = Values.Name, false) : true));
    }

    void
    FromJson (const ordered_json &j)
    {
        (..., (Values.Name.buf == j.get<std::string> ()
                   ? (Data = Values.Value, false)
                   : true));
    }
};

template<typename T>
requires requires (const T &x, AMT::ordered_json& j) { x.ToJson (j); }
void to_json (AMT::ordered_json &j, const T &p)
{
    p.ToJson (j);
}

template<typename T>
requires requires (T &x, const AMT::ordered_json& j) { x.FromJson (j); }
void from_json (const AMT::ordered_json &j, T &p)
{
    p.FromJson (j);
}

}; // namespace AMT
