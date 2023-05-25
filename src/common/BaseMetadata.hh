#pragma once

#include "BaseTypes.hh"
#include "IoUtils.hh"
#include <tuple>
#include <vector>
#include <variant>
#include <string>
#include <cstring>
#include <bitset>

namespace AMT {

template <int id, FixedString name, typename... Args> class BaseMetadataType
{
public:
    std::tuple<Args...>       Data;
    inline static const char *TypeName = name;

    static int
    GetId ()
    {
        return id;
    }

    BaseMetadataType () = default;
    BaseMetadataType (uint8_t *&data, uint32_t size) { data = Read (data); }

    template <unsigned int i = 0>
    void
    GetArchiveOffsets (std::vector<uint32_t> &out,
                       uint32_t               currentOffset = 0) const
    {
        if constexpr (i < sizeof...(Args))
            {
                BasicData::GetArchiveOffsets (out, currentOffset,
                                              std::get<i> (Data));
                currentOffset
                    += BasicData::GetSize (0, std::get<i> (Data).Data);
                GetArchiveOffsets<i + 1> (out, currentOffset);
            }
    }

    template <unsigned int i = 0>
    void
    ToJson (ordered_json &j) const
    {
        if constexpr (i < sizeof...(Args))
            {
                if (std::get<i> (Data).Name[0] != '\0')
                    j[std::get<i> (Data).Name] = std::get<i> (Data).Data;
                ToJson<i + 1> (j);
            }
    }

    template <unsigned int i = 0>
    void
    FromJson (const ordered_json &j)
    {
        if constexpr (i < sizeof...(Args))
            {
                if (std::get<i> (Data).Name[0] != '\0')
                    std::get<i> (Data).Data = j.at (std::get<i> (Data).Name);
                FromJson<i + 1> (j);
            }
    }

    template <unsigned int i = 0>
    void
    GetHashOffsets (std::vector<uint32_t> &out,
                    uint32_t               currentOffset = 0) const
    {
        if constexpr (i < sizeof...(Args))
            {
                BasicData::GetHashOffsets (out, currentOffset,
                                           std::get<i> (Data));

                currentOffset
                    += BasicData::GetSize (0, std::get<i> (Data).Data);
                GetHashOffsets<i + 1> (out, currentOffset);
            }
    }

    template <unsigned int i = 0>
    inline void
    GetArchiveNames (nlohmann::fifo_map<std::string, int> &out) const
    {
        if constexpr (i < sizeof...(Args))
            {
                BasicData::GetArchiveNames (out, std::get<i> (Data));
                GetArchiveNames<i + 1> (out);
            }
    }

    template <unsigned int i = 0>
    uint32_t
    GetSize (uint32_t initialSize = 0) const
    {
        if constexpr (i == sizeof...(Args))
            return initialSize;
        else
            {
                initialSize
                    = BasicData::GetSize (initialSize, std::get<i> (Data).Data);
                return GetSize<i + 1> (initialSize);
            }
    }

    template <unsigned int i = 0>
    uint8_t *
    Read (uint8_t *data)
    {
        if constexpr (i == sizeof...(Args))
            return data;
        else
            {
                data = BasicData::Read (data, std::get<i> (Data).Data);
                return Read<i + 1> (data);
            }
    }

    template <unsigned int i = 0>
    void
    Write (std::ostream &out)
    {
        if constexpr (i == sizeof...(Args))
            return;
        else
            {
                BasicData::Write (out, std::get<i> (Data).Data);
                return Write<i + 1> (out);
            }
    }
};

template <typename... Args>
class OptionalBitfieldContainer : public BaseMetadataType<0, "", Args...>
{
private:
    std::bitset<sizeof...(Args)> m_InternalBitfield;
    uint32_t                     m_BitfieldWritePos = 0;

    using base = BaseMetadataType<0, "", Args...>;

public:
    template <unsigned int i = 0>
    uint8_t *
    Read (uint8_t *data)
    {

        if constexpr (i == 0)
            {
                uint32_t bitset    = IoUtils::ReadData<uint32_t> (data);
                m_InternalBitfield = bitset;
            }

        if constexpr (i == sizeof...(Args))
            return data;
        else
            {
                if (m_InternalBitfield[i])
                    {
                        data = BasicData::Read (data,
                                                std::get<i> (this->Data).Data);
                    }

                return Read<i + 1> (data);
            }
    }

    template <unsigned int i = 0>
    void
    Write (std::ostream &out)
    {

        if constexpr (i == 0)
            {
                m_BitfieldWritePos = out.tellp ();
                out.seekp (4, std::ios_base::cur);
                m_InternalBitfield.reset ();
            }

        if constexpr (i == sizeof...(Args))
            {
                size_t retPos = out.tellp ();
                out.seekp (m_BitfieldWritePos);

                IoUtils::WriteData (out,
                                    uint32_t (m_InternalBitfield.to_ulong ()));
                out.seekp (retPos);
                return;
            }
        else
            {
                if (std::get<i> (this->Data).Data.HasValue ())
                    {
                        BasicData::Write (out, std::get<i> (this->Data).Data);
                        m_InternalBitfield[i] = true;
                    }

                Write<i + 1> (out);
            }
    }

    uint32_t
    GetSize (uint32_t initialSize = 0) const
    {
        return base::GetSize (initialSize) + 4;
    }

    template <unsigned int i = 0>
    void
    ToJson (ordered_json &j) const
    {
        if constexpr (i < sizeof...(Args))
            {
                if (std::get<i> (this->Data).Data.HasValue ())
                    j[std::get<i> (this->Data).Name]
                        = std::get<i> (this->Data).Data;
                ToJson<i + 1> (j);
            }
    }

    template <unsigned int i = 0>
    void
    FromJson (const ordered_json &j)
    {
        if constexpr (i < sizeof...(Args))
            {
                if (j.count (std::get<i> (this->Data).Name))
                    std::get<i> (this->Data).Data
                        = j.at (std::get<i> (this->Data).Name);

                FromJson<i + 1> (j);
            }
    }
};

template <typename HeaderType, bool hasNameOffset, typename... Types>
class BaseMetadataContainer
{
    std::string            Name;
    HeaderType             Header;
    std::variant<Types...> Data;

public:
    BaseMetadataContainer () = default;
    BaseMetadataContainer (std::string name, uint32_t size) : Name (name) {}

    std::string
    GetName () const
    {
        return Name;
    }

    void
    SetName (const std::string &name)
    {
        Name = name;
    }

    void
    Read (uint8_t *in, uint32_t size)
    {
        uint8_t Id = IoUtils::ReadData<uint8_t> (in);
        if constexpr (hasNameOffset)
            {
                IoUtils::ReadData<uint32_t> (in);
            }

        in = Header.Read (in);

        ((Id == Types::GetId ()
              ? (Data = Types (in, size - GetHeaderSize ()), true)
              : false),
         ...);
    }

    void
    Write (std::ostream &out, uint32_t nameOffset)
    {
        uint8_t Id = 0;
        std::visit ([&] (auto &&arg) { Id = arg.GetId (); }, Data);

        IoUtils::WriteData(out, Id);
        if constexpr (hasNameOffset)
            {
                IoUtils::WriteData (out, nameOffset);
            }

        Header.Write (out);
        std::visit ([&] (auto &&arg) { arg.Write (out); }, Data);
    }

    inline uint32_t
    GetSize () const
    {
        return std::visit (
            [&] (auto &&arg) { return arg.GetSize () + GetHeaderSize (); },
            Data);
    }

    void
    ToJson (ordered_json &j) const
    {
        std::visit (
            [&] (auto &&arg) {
                j["Type"] = arg.TypeName;
                to_json (j, Header);
                j["Metadata"] = arg;
            },
            Data);
    }

    void
    FromJson (const ordered_json &j)
    {
        from_json (j, Header);

        (..., (Types::TypeName == j.at ("Type")
                   ? (Data = j.at ("Metadata").get<Types> (), false)
                   : true));
    }

    uint32_t
    GetHeaderHeaderSize () const
    {
        uint32_t size = 1; // for the type
        if constexpr (hasNameOffset)
            {
                size += 4;
            }
        return size;
    }

    
    uint32_t
    GetHeaderSize () const
    {
        return BasicData::GetSize (GetHeaderHeaderSize (), Header);
    }

    void
    GetHashOffsets (std::vector<uint32_t> &out,
                    uint32_t               currentOffset = 0) const
    {
        BasicData::GetHashOffsets (out, GetHeaderHeaderSize () + currentOffset,
                                   Header);

        std::visit (
            [&] (auto &&arg) {
                BasicData::GetHashOffsets (out,
                                           GetHeaderSize () + currentOffset,
                                           arg);
            },
            Data);
    }

    void
    GetArchiveOffsets (std::vector<uint32_t>& out,
                       uint32_t              currentOffset = 0) const
    {
        BasicData::GetArchiveOffsets (out,
                                      GetHeaderHeaderSize () + currentOffset,
                                      Header);

        std::visit (
            [&] (auto &&arg) {
                BasicData::GetArchiveOffsets (out,
                                              GetHeaderSize () + currentOffset,
                                              arg);
            },
            Data);
    }

    void
    GetArchiveNames (nlohmann::fifo_map<std::string, int> &out) const
    {
        return std::visit (
            [&] (auto &&arg) { return BasicData::GetArchiveNames (out, arg); },
            Data);
    }
};

}; // namespace AMT
