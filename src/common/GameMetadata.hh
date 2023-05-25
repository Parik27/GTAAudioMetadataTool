#include "MetadataMgr.hh"
#include "BaseMetadata.hh"
#include "HashManager.hh"
#include "BaseMetadata.hh"
#include "PlaceholderMetadata.hh"
#include "common/BaseTypes.hh"

namespace AMT {

/* Ambient Zone List */
using gameAmbientZoneList
    = BaseMetadataType<21, "gameAmbientZoneList",
                       FieldType<ArrayWrapper<JoaatHash>, "Zones">>;

/* Ambient Zone */
using gameAmbientZone = BaseMetadataType<
    19, "gameAmbientZone", FieldType<float, "__field00">,
    FieldType<float, "MinX">, FieldType<float, "MinY">,
    FieldType<float, "MaxX">, FieldType<float, "MaxY">,
    FieldType<float, "__field14">, FieldType<uint8_t, "__field18">,
    FieldType<ArrayWrapper<JoaatHash>, "Rules">>;

/* Collision */
using gameCollision = PlaceholderMetadataType<1, "gameCollision">;

/* Clothing */
using gameClothing = PlaceholderMetadataType<15, "gameClothing">;

/* Door */
using gameDoor = PlaceholderMetadataType<25, "gameDoor">;

/* Cutscene */
using gameCutscene_Category = BaseMetadataType<0, "gameCutscene_Category",
                                               FieldType<JoaatHash, "Category">,
                                               FieldType<uint8_t, "Intensity">>;

using gameCutscene = BaseMetadataType<
    23, "gameCutscene",
    FieldType<ArrayWrapper<gameCutscene_Category>, "Categories">>;

/* Ambient Emitter */
using gameAmbientEmitter = BaseMetadataType<
    2, "gameAmbientEmitter", FieldType<JoaatHash, "field_0">,
    FieldType<JoaatHash, "field_1">, FieldType<float, "PosX">,
    FieldType<float, "PosY">, FieldType<float, "PosZ">,
    FieldType<float, "field_5">, FieldType<float, "field_6">,
    FieldType<uint32_t, "field_7">, FieldType<uint32_t, "field_7">,
    FieldType<uint16_t, "field_7">, FieldType<JoaatHash, "field_8">,
    FieldType<JoaatHash, "field_9">>;

/* Ambient Emitter List */
using gameAmbientEmitterList = BaseMetadataType<
    17, "gameAmbientEmitterList",
    FieldType<ArrayWrapper<JoaatHash, uint16_t>, "Ambient Emitters">>;

/* Emitter Entity */
using gameEmitterEntity = PlaceholderMetadataType<3, "gameEmitterEntity">;

/* Footsteps */
using gameFootsteps = PlaceholderMetadataType<9, "gameFootsteps">;

/* Interior */
using gameInterior = PlaceholderMetadataType<24, "gameInterior">;

/* melee Combat */
using gameMeleeCombat = PlaceholderMetadataType<5, "gameMeleeCombat">;

/* Crime */
using gameCrime_Instruction
    = BaseMetadataType<0, "gameCrime_Instruction", FieldType<JoaatHash, "Hash">,
                       FieldType<float, "Weight">>;
using gameCrime_Description
    = BaseMetadataType<0, "gameCrime_Description", FieldType<JoaatHash, "Hash">,
                       FieldType<float, "Weight">>;

using gameCrime = BaseMetadataType<
    14, "gameCrime",
    FieldType<ArrayWrapper<gameCrime_Instruction>, "CrimeInstructions">,
    FieldType<ArrayWrapper<gameCrime_Description>, "CrimeDescriptions">>;

/* Radio Station Track Cateegory */
#define X EnumWrapperValue
using eRadioStationTrackCategoryType
    = EnumWrapper<uint8_t, X{"AD", 0}, X{"IDENT", 1}, X{"MUSIC", 2},
                  X{"NEWS", 3}, X{"WEATHER", 4}, X{"DJ_SOLO", 5},
                  X{"USER_INTRO", 6}, X{"USER_OUTRO", 7}, X{"USER_TO_AD", 8},
                  X{"USER_TO_NEWS", 9}>;
#undef X

using gameRadioStationTrackCategory_Track
    = BaseMetadataType<0, "gameRadioStationTrackCategory_Track",
                       FieldType<JoaatHash, "Identifier">,
                       FieldType<JoaatHash, "Hash">>;

using gameRadioStationTrackCategory = BaseMetadataType<
    12, "gameRadioStationTrackCategory",
    FieldType<eRadioStationTrackCategoryType, "Type">,
    FieldType<uint32_t, "">,
    FieldType<uint8_t, "">,
    FieldType<ArrayWrapper<JoaatHash>, "RuntimeRecentTracks">,
    FieldType<uint16_t, "">, FieldType<uint32_t, "__field31">,
    FieldType<ArrayWrapper<gameRadioStationTrackCategory_Track>, "Tracks">>;

/* Radio Station */
using gameRadioStation = BaseMetadataType<
    11, "gameRadioStation", FieldType<int32_t, "">, FieldType<int32_t, "Index">,
    FieldType<uint8_t, "__field08">, FieldType<uint8_t, "">,
    FieldType<int32_t, "">, FieldType<int32_t, "__field14">,
    FieldType<uint8_t, "__field18">, FieldType<StringWrapper<>, "Name">,
    FieldType<ArrayWrapper<JoaatHash>, "TrackCategories">>;

/* Radio Station Category Weights */
using gameRadioStationCategoryWeights_Weight = BaseMetadataType<
    0, "gameRadioStationCategoryWeights_Weight",
    FieldType<eRadioStationTrackCategoryType, "CategoryType">,
    FieldType<int, "Value">>;

using gameRadioStationCategoryWeights = BaseMetadataType<
    13, "gameRadioStationCategoryWeights",
    FieldType<ArrayWrapper<gameRadioStationCategoryWeights_Weight>, "Weights">>;

/* Ped */
using gamePed_struct
    = BaseMetadataType<0, "gamePed_struct", FieldType<JoaatHash, "__field0">,
                       FieldType<uint32_t, "__field4">>;

// using gamePed
//     = BaseMetadataType<16, "gamePed",
//                        FieldType<ArrayWrapper<gamePed_struct>, "__arr1">,
//                        FieldType<ArrayWrapper<gamePed_struct>, "__arr2">>;
using gamePed = PlaceholderMetadataType<16, "gamePed">;

/* Train Station */
using gameTrainStation = PlaceholderMetadataType<22, "gameTrainStation">;

/* Vehicles */
using gameAutomobile = PlaceholderMetadataType<0, "gameAutomobile">;
using gameBoat       = PlaceholderMetadataType<7, "gameBoat">;
using gameHeli       = PlaceholderMetadataType<4, "gameHeli">;

/* Weapon */
using gameWeapon = BaseMetadataType<
    8, "gameWeapon", FieldType<JoaatHash, "FireHash">,
    FieldType<JoaatHash, "EchoHash">, FieldType<JoaatHash, "CasingBounceHash">,
    FieldType<JoaatHash, "SwipeSoundHash">,
    FieldType<JoaatHash, "CollisionHash">,
    FieldType<JoaatHash, "MeleeCollisionHash">,
    FieldType<JoaatHash, "HeftHash">, FieldType<JoaatHash, "PutDownHash">,
    FieldType<JoaatHash, "RattleCollisionHash">,
    FieldType<JoaatHash, "PickupSoundHash">, FieldType<uint8_t, "__field00">,
    FieldType<JoaatHash, "SafetyOnSound">,
    FieldType<JoaatHash, "SafetyOffSound">,
    FieldType<JoaatHash, "SlomoSwooshSound">, FieldType<uint32_t, "__field04">,
    FieldType<JoaatHash, "SlomoXfadeHash">,
    FieldType<JoaatHash, "SlomoCollisionHash">>;

/* Radio Station List */
using gameRadioStationList
    = BaseMetadataType<10, "gameRadioStationList",
                       FieldType<ArrayWrapper<JoaatHash>, "Stations">>;

/* Scripted Report */
using gameScriptedReport = PlaceholderMetadataType<18, "gameScriptedReport">;

/* Sound Rules */
using gameSoundRules = BaseMetadataType<
    20, "gameSoundRules", FieldType<float, "Weight">,
    FieldType<float, "OffsetX">, FieldType<float, "OffsetY">,
    FieldType<uint8_t, "HoursStart">, FieldType<uint8_t, "HoursEnd">,
    FieldType<int16_t, "__field0f">, FieldType<JoaatHash, "SoundHash">,
    FieldType<JoaatHash, "CategoryHash">, FieldType<uint32_t, "">>;

/* Speech Contexts */
using gameSpeechContexts_Context = BaseMetadataType<
    0, "gameSpeechContexts_Context", FieldType<JoaatHash, "ContextHash">,
    FieldType<uint32_t, "_field04">, FieldType<int32_t, "_field08">,
    FieldType<uint32_t, "_field0C">, FieldType<uint8_t, "_field10">,
    FieldType<JoaatHash, "UnkHash">, FieldType<uint32_t, "_field15">>;

using gameSpeechContexts = BaseMetadataType<
    6, "gameSpeechContexts",
    FieldType<ArrayWrapper<gameSpeechContexts_Context, uint16_t>, "Contexts">>;

/*******************************************************/
using GameMetadataHeader = BaseMetadataType<0, "GameMetadataHeader",
                                            FieldType<uint32_t, "__field04">,
                                            FieldType<uint8_t, "__field08">>;

using GameMetadataContainer = BaseMetadataContainer<
    GameMetadataHeader, true, gameAmbientZoneList, gameAmbientZone,
    gameCollision, gameClothing, gameDoor, gameCutscene, gameAmbientEmitter,
    gameAmbientEmitterList, gameEmitterEntity, gameFootsteps, gameInterior,
    gameMeleeCombat, gameCrime, gameRadioStationTrackCategory, gameRadioStation,
    gameRadioStationCategoryWeights, gamePed, gameTrainStation, gameAutomobile,
    gameHeli, gameBoat, gameWeapon, gameRadioStationList, gameSoundRules,
    gameScriptedReport, gameSpeechContexts>;

using GameMetadataMgr = MetadataMgr<GameMetadataContainer, 16>;
}; // namespace AMT
