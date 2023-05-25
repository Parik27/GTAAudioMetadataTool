#include "common/SoundsMetadata.hh"
#include "common/MetadataMgr.hh"
#include <fstream>
#include "common/HashManager.hh"
#include "common/CategoryMetadata.hh"
#include "common/GameMetadata.hh"
#include "common/EffectsMetadata.hh"
#include "common/CurvesMetadata.hh"

template <typename T>
void
DeserialiseMetadata (const std::string &file)
{
    T             mgr;
    std::ifstream input (file, std::ios_base::binary);

    if (!input.good ())
        return;

    mgr.Read (input);

    AMT::ordered_json j = mgr;
    std::ofstream     out (file + ".json");

    out << j.dump (4);
}

template <typename T>
void
SerialiseMetadata (const std::string &file)
{
    std::ifstream input (file + ".json");
    if (!input.good ())
        return;

    AMT::ordered_json j;
    input >> j;

    T             mgr = j;
    std::ofstream out (file + ".GEN", std::ios_base::binary);
    mgr.Write (out);
}

void
ReadHashes (const std::string &file)
{
    std::ifstream input (file);
    std::string   key;
    while (std::getline (input, key))
        AMT::HashManager::Instance ()->AddHash (key);
}

int
main (int argc, char **argv)
{
    ReadHashes ("Hashes.txt");
    if (argc == 2 && std::string (argv[1]) == "gen")
        {
            SerialiseMetadata<AMT::CategoriesMetadataMgr> ("CATEGORIES.DAT15");
            SerialiseMetadata<AMT::CurvesMetadataMgr> ("CURVES.DAT12");
            SerialiseMetadata<AMT::EffectsMetadataMgr> ("EFFECTS.DAT11");

            SerialiseMetadata<AMT::SoundsMetadataMgr> ("SOUNDS.DAT15");
            SerialiseMetadata<AMT::SoundsMetadataMgr> ("EP1_RADIO_SOUNDS.DAT15");
            SerialiseMetadata<AMT::SoundsMetadataMgr> ("EP2_RADIO_SOUNDS.DAT15");

            SerialiseMetadata<AMT::GameMetadataMgr> ("GAME.DAT16");
            SerialiseMetadata<AMT::GameMetadataMgr> ("EP1_GAME.DAT16");
            SerialiseMetadata<AMT::GameMetadataMgr> ("EP1_RADIO_GAME.DAT16");
            SerialiseMetadata<AMT::GameMetadataMgr> ("EP2_GAME.DAT16");
            SerialiseMetadata<AMT::GameMetadataMgr> ("EP2_RADIO_GAME.DAT16");
            // SerialiseMetadata ("speech.dat");
        }
    else
        {
            DeserialiseMetadata<AMT::CategoriesMetadataMgr> ("CATEGORIES.DAT15");
            DeserialiseMetadata<AMT::CurvesMetadataMgr> ("CURVES.DAT12");
            DeserialiseMetadata<AMT::EffectsMetadataMgr> ("EFFECTS.DAT11");

            DeserialiseMetadata<AMT::SoundsMetadataMgr> ("SOUNDS.DAT15");
            DeserialiseMetadata<AMT::SoundsMetadataMgr> ("EP1_RADIO_SOUNDS.DAT15");
            DeserialiseMetadata<AMT::SoundsMetadataMgr> ("EP2_RADIO_SOUNDS.DAT15");

            DeserialiseMetadata<AMT::GameMetadataMgr> ("GAME.DAT16");
            DeserialiseMetadata<AMT::GameMetadataMgr> ("EP1_GAME.DAT16");
            DeserialiseMetadata<AMT::GameMetadataMgr> ("EP1_RADIO_GAME.DAT16");
            DeserialiseMetadata<AMT::GameMetadataMgr> ("EP2_GAME.DAT16");
            DeserialiseMetadata<AMT::GameMetadataMgr> ("EP2_RADIO_GAME.DAT16");
            // DeserialiseMetadata ("speech.dat");
        }
}
