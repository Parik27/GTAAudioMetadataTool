#include "MetadataMgr.hh"
#include "BaseMetadata.hh"
#include "HashManager.hh"
#include "BaseMetadata.hh"

namespace AMT {

/* Simple sound */
using audSimpleSound
    = BaseMetadataType<12, "audSimpleSound", FieldType<uint32_t, "__field00">,
                       FieldType<JoaatHash, "ArchiveHash", false, true>,
                       FieldType<JoaatHash, "SoundHash">>;

/* Multitrack sound */
using audMultitrackSound_Track
    = BaseMetadataType<0, "audMultitrackSound_Track",
                       FieldType<JoaatHash, "TrackHash", true>,
                       FieldType<uint32_t, "__field04">>;

using audMultitrackSound = BaseMetadataType<
    13, "audMultitrackSound",
    FieldType<ArrayWrapper<audMultitrackSound_Track>, "Tracks">>;

/* Looping Sound */
using audLoopingSound
    = BaseMetadataType<1, "audLoopingSound", FieldType<uint16_t, "__field00">,
                       FieldType<uint16_t, "__field02">,
                       FieldType<JoaatHash, "SoundHash", true>>;

/* Envelope Sound */
using audEnvelopeSound = BaseMetadataType<
    2, "audEnvelopeSound", FieldType<uint16_t, "__field00">,
    FieldType<uint16_t, "__field02">, FieldType<uint8_t, "__field04">,
    FieldType<int32_t, "__field05">, FieldType<int32_t, "__field09">,
    FieldType<JoaatHash, "UnkCurveHash1">,
    FieldType<JoaatHash, "UnkCurveHash2">,
    FieldType<JoaatHash, "UnkCurveHash3">,
    FieldType<JoaatHash, "VariableHash1">,
    FieldType<JoaatHash, "VariableHash2">,
    FieldType<JoaatHash, "VariableHash3">,
    FieldType<JoaatHash, "VariableHash4">,
    FieldType<JoaatHash, "VariableHash5">,
    FieldType<JoaatHash, "SoundHash", true>>;

/* Variable Block */
using audVariableBlockSound_Variable
    = BaseMetadataType<0, "audVariableBlockSound_Variable",
                       FieldType<JoaatHash, "Hash">, FieldType<float, "Data">,
                       FieldType<uint8_t, "__field08">>;

using audVariableBlockSound = BaseMetadataType<
    21, "audVariableBlockSound", FieldType<JoaatHash, "SoundHash", true>,
    FieldType<ArrayWrapper<audVariableBlockSound_Variable>, "Variables">>;

/* Math Operation */

#define X EnumWrapperValue
using eMathOperation = EnumWrapper<
    uint8_t, X{"MATH_OPERATION_ADD", 0}, X{"MATH_OPERATION_SUBTRACT", 1},
    X{"MATH_OPERATION_MULTIPLY", 2}, X{"MATH_OPERATION_DIVIDE", 3},
    X{"MATH_OPERATION_SET", 4}, X{"MATH_OPERATION_MOD", 5},
    X{"MATH_OPERATION_MIN", 6}, X{"MATH_OPERATION_MAX", 7},
    X{"MATH_OPERATION_ABS", 8}, X{"MATH_OPERATION_SIGN", 9},
    X{"MATH_OPERATION_FLOOR", 10}, X{"MATH_OPERATION_CEIL", 11},
    X{"MATH_OPERATION_RAND", 12}, X{"MATH_OPERATION_SIN", 13},
    X{"MATH_OPERATION_COS", 14}, X{"MATH_OPERATION_SQRT", 15},
    X{"MATH_OPERATION_DBTOLINEAR", 16}, X{"MATH_OPERATION_LINEARTODB", 17},
    X{"MATH_OPERATION_PITCHTORATIO", 18}, X{"MATH_OPERATION_RATIOTOPITCH", 19},
    X{"MATH_OPERATION_GETTIME", 20}, X{"MATH_OPERATION_FSEL", 21},
    X{"MATH_OPERATION_VALUEINRANGE", 22}, X{"MATH_OPERATION_CLAMP", 23},
    X{"MATH_OPERATION_POW", 24}, X{"MATH_OPERATION_ROUND", 25},
    X{"MATH_OPERATION_SCALEDSIN", 26}, X{"MATH_OPERATION_SCALEDTRI", 27},
    X{"MATH_OPERATION_SCALEDSAW", 28}, X{"MATH_OPERATION_SCALEDSQUARE", 29},
    X{"MATH_OPERATION_SMOOTH", 30}, X{"MATH_OPERATION_GETSCALEDTIME", 31}>;
#undef X

using audMathOperationSound_Operation
    = BaseMetadataType<0, "audMathOperationSound_Operation",
                       FieldType<eMathOperation, "Operation">,
                       FieldType<float, "OperandAStatic">,
                       FieldType<JoaatHash, "OperandAVariable">,
                       FieldType<float, "OperandBStatic">,
                       FieldType<JoaatHash, "OperandBVariable">,
                       FieldType<float, "OperandCStatic">,
                       FieldType<JoaatHash, "OperandCVariable">,
                       FieldType<JoaatHash, "OutputVariable">>;

using audMathOperationSound = BaseMetadataType<
    24, "audMathOperationSound", FieldType<JoaatHash, "SoundHash", true>,
    FieldType<ArrayWrapper<audMathOperationSound_Operation>, "Operations">>;

/* Variable Curve */
using audVariableCurveSound = BaseMetadataType<
    17, "audVariableCurveSound", FieldType<JoaatHash, "SoundHash", true>,
    FieldType<JoaatHash, "UnkVariableA">, FieldType<JoaatHash, "UnkVariableB">,
    FieldType<JoaatHash, "UnkCurvesHash">>;

/* Randomized Sound */
using audRandomizedSound_Sound
    = BaseMetadataType<0, "audRandomizedSound_Sound",
                       FieldType<JoaatHash, "SoundHash", true>,
                       FieldType<float, "Weight">>;

using audRandomizedSound = BaseMetadataType<
    14, "audRandomizedSound", FieldType<uint32_t, "__field00">,
    FieldType<uint8_t, "__field04">,
    FieldType<ArrayWrapper<uint8_t>, "__field06">,
    FieldType<ArrayWrapper<audRandomizedSound_Sound>, "Sounds">>;

/* Switch Sound */
using audSwitchSound = BaseMetadataType<
    16, "audSwitchSound", FieldType<JoaatHash, "ControlVariable">,
    FieldType<ArrayWrapper<JoaatHash, uint8_t, true>, "Sounds">>;

/* Twin Loop Sound */

using audTwinLoopSound_Sound
    = BaseMetadataType<0, "audTwinLoopSound_Sound",
                       FieldType<JoaatHash, "SoundHash", true>,
                       FieldType<uint32_t, "__field04">>;

using audTwinLoopSound = BaseMetadataType<
    3, "audTwinLoopSound", FieldType<int16_t, "__field00">,
    FieldType<int16_t, "__field02">, FieldType<int16_t, "__field04">,
    FieldType<int16_t, "__field06">, FieldType<JoaatHash, "Mode">,
    FieldType<JoaatHash, "VariableHash1">,
    FieldType<JoaatHash, "VariableHash2">,
    FieldType<JoaatHash, "VariableHash3">,
    FieldType<JoaatHash, "VariableHash4">,
    FieldType<ArrayWrapper<audTwinLoopSound_Sound>, "Sounds">>;

/* Sequential Sound */
using audSequentialSound = BaseMetadataType<
    7, "audSequentialSound",
    FieldType<ArrayWrapper<audTwinLoopSound_Sound>, "Sounds">>;

/* OnStop Sound Sound */
using audOnStopSound
    = BaseMetadataType<5, "audOnStopSound",
                       FieldType<JoaatHash, "ChildSound", true>,
                       FieldType<JoaatHash, "OnPauseSound", true>,
                       FieldType<JoaatHash, "OnEndSound", true>>;

/* Retriggered Overlapped Sound */
using audRetriggeredOverlappedSound = BaseMetadataType<
    9, "audRetriggeredOverlappedSound", FieldType<int16_t, "__field00">,
    FieldType<uint16_t, "__field02">, FieldType<JoaatHash, "VariableHash1">,
    FieldType<JoaatHash, "VariableHash2">,
    FieldType<JoaatHash, "SoundHash", true>>;

/* If sound */
#define X EnumWrapperValue
using eIfCondition = EnumWrapper<uint8_t, X{"IF_CONDITION_LESS_THAN", 0},
                                 X{"IF_CONDITION_LESS_THAN_OR_EQUAL_TO", 1},
                                 X{"IF_CONDITION_GREATER_THAN", 2},
                                 X{"IF_CONDITION_GREATER_THAN_OR_EQUAL_TO", 3},
                                 X{"IF_CONDITION_EQUAL_TO", 4},
                                 X{"IF_CONDITION_NOT_EQUAL_TO", 5}>;
#undef X

using audIfSound = BaseMetadataType<
    22, "audIfSound", FieldType<JoaatHash, "TrueSound", true>,
    FieldType<JoaatHash, "FalseSound", true>, FieldType<JoaatHash, "VariableA">,
    FieldType<eIfCondition, "Operator">, FieldType<float, "OperandBStatic">,
    FieldType<JoaatHash, "OperandBVariable">>;

/* For loop sound */
using audForLoopSound
    = BaseMetadataType<23, "audForLoopSound",
                       FieldType<JoaatHash, "SoundHash", true>,
                       FieldType<float, "LoopCounterInitialValue">,
                       FieldType<JoaatHash, "LoopCounterInitialVariable">,
                       FieldType<float, "LoopCounterConditionValue">,
                       FieldType<JoaatHash, "LoopCounterConditionVariable">,
                       FieldType<float, "LoopCounterIncrementValue">,
                       FieldType<JoaatHash, "LoopCounterIncrementVariable">,
                       FieldType<JoaatHash, "LoopCounterVariable">>;

/* Variable Print Value Sound */
using audVariablePrintValueSound
    = BaseMetadataType<18, "audVariablePrintValueSound",
                       FieldType<JoaatHash, "VariableHash">,
                       FieldType<FixedArrayWrapper<uint8_t, 15>, "__field04">>;

/* Crossfade Sound */
using audCrossfadeSound = BaseMetadataType<
  10, "audCrossfadeSound", FieldType<JoaatHash, "NearSound", true>,
  FieldType<JoaatHash, "FarSound", true>, FieldType<uint8_t, "__field08">,
    FieldType<int32_t, "__field09">, FieldType<int32_t, "__field0d">,
    FieldType<int32_t, "__field11">, FieldType<JoaatHash, "VariableHash1">,
    FieldType<JoaatHash, "VariableHash2">, FieldType<int32_t, "__field1d">,
    FieldType<JoaatHash, "VariableHash3">,
    FieldType<JoaatHash, "UnkCurvesHash">>;

/* Streaming Sound */
using audStreamingSound = BaseMetadataType<
    8, "audStreamingSound", FieldType<uint32_t, "__field00">,
    FieldType<ArrayWrapper<audTwinLoopSound_Sound>, "Sounds">>;

/* Collapsing Stereo */
using audCollapsingStereoSound = BaseMetadataType<
    11, "audCollapsingStereoSound", FieldType<JoaatHash, "SoundHash1", true>,
    FieldType<JoaatHash, "SoundHash2", true>, FieldType<float, "__field08">,
    FieldType<float, "__field0c">, FieldType<JoaatHash, "VariableHash1">,
    FieldType<JoaatHash, "VariableHash2">,
    FieldType<JoaatHash, "VariableHash3">,
    FieldType<JoaatHash, "VariableHash4">,
    FieldType<JoaatHash, "VariableHash5">, FieldType<uint8_t, "__field24">>;

/* Wrapper Sound */
using audWrapperSound
    = BaseMetadataType<6, "audWrapperSound",
                       FieldType<JoaatHash, "SoundHash", true>>;

/* Variable Set Time Sound */
using audVariableSetTimeSound
    = BaseMetadataType<20, "audVariableSetTimeSound",
                       FieldType<JoaatHash, "VariableHash">>;

/* Stub sounds */
using audSpeechSound = BaseMetadataType<4, "audSpeechSound">;
using audAssertSound = BaseMetadataType<19, "audAssertSound">;

/*******************************************************/
using SoundsMetadataHeader = BaseMetadataType<
    0, "SoundsMetadataHeader", FieldType<uint32_t, "Flags">,
    FieldType<uint16_t, "__unk09">,

    // Flags for the header
    FieldType<OptionalBitfieldContainer<
                  FieldType<OptionalWrapper<uint16_t>, "__field0">,
                  FieldType<OptionalWrapper<uint16_t>, "__field1">,
                  FieldType<OptionalWrapper<uint16_t>, "__field2">,
                  FieldType<OptionalWrapper<uint16_t>, "__field3">,
                  FieldType<OptionalWrapper<uint16_t>, "__field4">,
                  FieldType<OptionalWrapper<uint16_t>, "__field5">,
                  FieldType<OptionalWrapper<uint16_t>, "__field6">,
                  FieldType<OptionalWrapper<uint16_t>, "__field7">,
                  FieldType<OptionalWrapper<uint32_t>, "__field8">,
                  FieldType<OptionalWrapper<uint32_t>, "__field9">,
                  FieldType<OptionalWrapper<uint16_t>, "__field10">,
                  FieldType<OptionalWrapper<uint16_t>, "__field11">,
                  FieldType<OptionalWrapper<uint16_t>, "__field12">,
                  FieldType<OptionalWrapper<JoaatHash>, "CategoryHash">,
                  FieldType<OptionalWrapper<uint32_t>, "__field14">,
                  FieldType<OptionalWrapper<uint16_t>, "__field15">,
                  FieldType<OptionalWrapper<uint8_t>, " __field16">,
                  FieldType<OptionalWrapper<uint8_t>, " __field17">,
                  FieldType<OptionalWrapper<JoaatHash>, "VolumeVariable">,
                  FieldType<OptionalWrapper<JoaatHash>, "PitchVariable">,
                  FieldType<OptionalWrapper<JoaatHash>, "PanVariable">,
                  FieldType<OptionalWrapper<JoaatHash>, "UnkVariable2">,
                  FieldType<OptionalWrapper<JoaatHash>, "UnkVariable3">,
                  FieldType<OptionalWrapper<JoaatHash>, "CutoffVariable">>,
              "Header">>;

using SoundsMetadataContainer = BaseMetadataContainer<
    SoundsMetadataHeader, true, audLoopingSound, audMultitrackSound,
    audSimpleSound, audEnvelopeSound, audMathOperationSound,
    audVariableBlockSound, audRandomizedSound, audVariableCurveSound,
    audSwitchSound, audTwinLoopSound, audSequentialSound, audOnStopSound,
    audRetriggeredOverlappedSound, audIfSound, audForLoopSound,
    audVariablePrintValueSound, audCrossfadeSound, audStreamingSound,
    audCollapsingStereoSound, audWrapperSound, audVariableSetTimeSound,
    audSpeechSound, audAssertSound>;

using SoundsMetadataMgr = MetadataMgr<SoundsMetadataContainer, 15>;
}; // namespace AMT
