/**
 * PrintInfo.h
 *
 * This file is part of the LibTiePie programming examples.
 *
 * Find more information on http://www.tiepie.com/LibTiePie .
 */

#ifndef _PRINTINFO_H_
#define _PRINTINFO_H_

#include <libtiepie.h>

// Print library info:
void printLibraryInfo();

// Print device info:
void printDeviceInfo(LibTiePieHandle_t dev);

// Print oscilloscope info:
void printOscilloscopeInfo(LibTiePieHandle_t scp);

// Print generator info:
void printGeneratorInfo(LibTiePieHandle_t gen);

// Print I2C info:
void printI2CInfo(LibTiePieHandle_t i2c);

// Print server info:
void printServerInfo(LibTiePieHandle_t srv);

// Print trigger input info:
void printTriggerInputsInfo(LibTiePieHandle_t dev);

// Print trigger output info:
void printTriggerOutputsInfo(LibTiePieHandle_t dev);

// Print functions for special values/types:
void printGeneratorMode(uint64_t generatorModes);
void printClockOutput(uint32_t clockOutputs);
void printClockSource(uint32_t clockSources);
void printConnectorType(uint32_t connectorTypes);
void printCoupling(uint64_t couplings);
void printDate(TpDate_t date);
void printDeviceType(uint32_t deviceTypes);
void printFrequencyMode(uint32_t frequencyModes);
void printMeasureMode(uint32_t measureModes);
void printAutoResolutionMode(uint32_t autoResolutionModes);
void printSignalType(uint32_t signalTypes);
void printTriggerCondition(uint32_t triggerConditions);
void printTriggerKind(uint64_t triggerKinds);
void printTriggerLevelMode(uint32_t triggerLevelModes);
void printTriggerOutputEvent(uint64_t triggerOutputEvents);
void printVersion(TpVersion_t version);
void printIPv4Address(uint32_t ipv4Address);

// String conversion functions:
const char* boolToStr(bool8_t value);

#endif
