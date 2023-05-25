#include "MetadataMgr.hh"
#include "BaseMetadata.hh"
#include "HashManager.hh"
#include "BaseMetadata.hh"

namespace AMT {

class SpeechMetadataMgr
{
public:
    void Write (std::ostream &out);

    void ToJson (ordered_json &j) const;

    void FromJosn (const ordered_json &j);

    void Read (std::istream &in);
};

}; // namespace AMT
