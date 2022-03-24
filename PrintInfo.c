/**
 * PrintInfo.c
 *
 * This file is part of the LibTiePie programming examples.
 *
 * Find more information on http://www.tiepie.com/LibTiePie .
 */

#include "PrintInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Utils.h" // for NEWLINE

const char* GeneratorModes[GMN_COUNT] = {"Continuous", "Burst count", "Gated periods", "Gated", "Gated period start", "Gated period finish", "Gated run", "Gated run output", "Burst sample count", "Burst sample count output", "Burst segment count", "Burst segment count output"};
const char* ClockOutputTypes[CON_COUNT] = {"Disabled", "Sample", "Fixed"};
const char* ClockSources[CSN_COUNT] = {"External", "Internal"};
const char* ConnectorTypes[CONNECTORTYPE_COUNT] = {"BNC", "Banana", "Power plug"};
const char* Couplings[CKN_COUNT] = {"DCV", "ACV", "DCA", "ACA" , "Ohm"};
const char* DeviceTypes[DEVICETYPE_COUNT] = {"Oscilloscope", "Generator", "I2CHost"};
const char* FrequencyModes [FMN_COUNT] = {"Signal frequency", "Sample frequency"};
const char* MeasureModes[MMN_COUNT] = {"Stream", "Block"};
const char* AutoResolutionModes[ARN_COUNT] = {"Disabled", "Native only", "All"};
const char* SignalTypes[STN_COUNT] = {"Sine", "Triangle", "Square", "DC", "Noise", "Arbitrary", "Pulse"};
const char* TriggerConditions[TCN_COUNT] = {"None", "Smaller", "Larger", "Inside", "Outside"};
const char* TriggerKinds[TKN_COUNT] = {"Rising edge", "Falling edge", "In window", "Out Window", "Any edge", "Enter window", "Exit window", "Pulse width positive", "Pulse width negative", "Pulse width either", "Runt pulse positive", "Runt pulse negative", "Runt pulse either", "Interval rising", "Interval falling"};
const char* TriggerLevelModes[TLMN_COUNT] = {"Relative", "Absolute"};
const char* TriggerOutputEvents[TOEN_COUNT] = {"Generator start", "Generator stop", "Generator new period", "Oscilloscope running", "Oscilloscope triggered"};
const char* ServerErrorCodes[] = {"None", "Unknown", "Connection refused", "Network unreachable", "Timed out", "Hostname lookup failed"};
const char* ServerStatuses[] = {"Disconnected", "Connecting", "Connected", "Disconnecting"};

void printLibraryInfo()
{
  printf("Library:" NEWLINE);

  // Print library version:
  printf("  Version: ");
  printVersion(LibGetVersion());
  printf("%s" NEWLINE, LibGetVersionExtra());

  // Print library configuration:
  uint32_t length = LibGetConfig(NULL, 0);
  uint8_t configuration[sizeof(uint8_t) * length];
  length = LibGetConfig(configuration, length);

  printf("  Configuration: 0x");
  for(uint32_t i = 0; i < length; i++)
    printf("%02" PRIx8, configuration[i]);
  printf(NEWLINE);
}

void printDeviceInfo(LibTiePieHandle_t dev)
{
  if(dev == LIBTIEPIE_HANDLE_INVALID)
  {
    fprintf(stderr, "Invalid device handle in printDeviceInfo()" NEWLINE);
    return;
  }

  printf("Device:" NEWLINE);

  // Name:
  uint32_t length = DevGetName(dev, NULL, 0) + 1;
  char deviceName[length];
  length = DevGetName(dev, deviceName, length);
  printf("  Name                      : %s" NEWLINE, deviceName);

  // Name short:
  length = DevGetNameShort(dev, NULL, 0) + 1;
  char deviceNameShort[length];
  length = DevGetNameShort(dev, deviceNameShort, length);
  printf("  Short name                : %s" NEWLINE, deviceNameShort);

  printf("  Serial number             : %" PRIu32 NEWLINE, DevGetSerialNumber(dev));

  printf("  Calibration data          : ");
  printDate(DevGetCalibrationDate(dev));
  printf(NEWLINE);

  printf("  Product id                : %" PRIu32 NEWLINE, DevGetProductId(dev));
  printf("  Vendor id                 : %" PRIu32 NEWLINE, DevGetVendorId(dev));

  const TpVersion_t driverVersion = DevGetDriverVersion(dev);
  if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
  {
    printf("  Driver version            : ");
    printVersion(driverVersion);
    printf(NEWLINE);
  }

  const TpVersion_t firmwareVersion = DevGetFirmwareVersion(dev);
  if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
  {
    printf("  Firmware version          : ");
    printVersion(firmwareVersion);
    printf(NEWLINE);
  }

  const uint32_t ipv4Address = DevGetIPv4Address(dev);
  if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
  {
    printf("  IPv4 address              : ");
    printIPv4Address(ipv4Address);
    printf(NEWLINE);
  }

  const uint16_t ipPort = DevGetIPPort(dev);
  if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
  {
    printf("  IP port                   : %" PRIu16 NEWLINE, ipPort);
  }

  printf("  Has battery               : %s" NEWLINE, boolToStr(DevHasBattery(dev)));

  if(DevHasBattery(dev))
  {
    printf("  Battery:" NEWLINE);

    const int8_t batteryCharge = DevGetBatteryCharge(dev);
    if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
    {
      printf("    Charge                  : %" PRIi8 " %%" NEWLINE, batteryCharge);
    }

    const int32_t batteryTimeToEmpty = DevGetBatteryTimeToEmpty(dev);
    if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
    {
      printf("    Time to empty           : %" PRIi32 " minutes" NEWLINE, batteryTimeToEmpty);
    }

    const int32_t batteryTimeToFull = DevGetBatteryTimeToFull(dev);
    if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
    {
      printf("    Time to full            : %" PRIi32 " minutes" NEWLINE, batteryTimeToFull);
    }

    const uint16_t isBatteryChargerConnected = DevIsBatteryChargerConnected(dev);
    if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
    {
      printf("    Charger connected       : %s" NEWLINE, boolToStr(isBatteryChargerConnected));
    }

    const uint16_t isBatteryCharging = DevIsBatteryCharging(dev);
    if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
    {
      printf("    Charging                : %s" NEWLINE, boolToStr(isBatteryCharging));
    }

    const uint16_t isBatteryBroken = DevIsBatteryBroken(dev);
    if(LibGetLastStatus() != LIBTIEPIESTATUS_NOT_SUPPORTED)
    {
      printf("    Broken                  : %s" NEWLINE, boolToStr(isBatteryBroken));
    }
  }

  switch(DevGetType(dev))
  {
    case DEVICETYPE_OSCILLOSCOPE:
      printOscilloscopeInfo(dev);
      break;

    case DEVICETYPE_GENERATOR:
      printGeneratorInfo(dev);
      break;

    case DEVICETYPE_I2CHOST:
      printI2CInfo(dev);
      break;
  }
}

void printOscilloscopeInfo(LibTiePieHandle_t scp)
{
  if(scp == LIBTIEPIE_HANDLE_INVALID)
  {
    fprintf(stderr, "Invalid device handle in printOscilloscopeInfo()" NEWLINE);
    return;
  }

  const uint16_t channelCount = ScpGetChannelCount(scp);

  printf("Oscilloscope:" NEWLINE);
  printf("  Channel count             : %" PRIu16 NEWLINE, channelCount);
  printf("  Connection test           : %s" NEWLINE, boolToStr(ScpHasConnectionTest(scp)));
  printf("  Measure modes             : ");
  printMeasureMode(ScpGetMeasureModes(scp));
  printf(NEWLINE);
  printf("  Measure mode              : ");
  printMeasureMode(ScpGetMeasureMode(scp));
  printf(NEWLINE);
  printf("  Auto resolution modes     : ");
  printAutoResolutionMode(ScpGetAutoResolutionModes(scp));
  printf(NEWLINE);
  printf("  Auto resolution mode      : ");
  printAutoResolutionMode(ScpGetAutoResolutionMode(scp));
  printf(NEWLINE);

  uint8_t resolutionCount = ScpGetResolutions(scp, NULL, 0);
  uint8_t resolutions[sizeof(uint8_t) * resolutionCount];
  resolutionCount = ScpGetResolutions(scp, resolutions, resolutionCount);

  printf("  Resolutions               : ");
  for(uint8_t i = 0; i < resolutionCount; i++)
  {
    if(i != 0)
      printf(", ");

    printf("%" PRIu8, resolutions[i]);
  }
  printf(NEWLINE);

  printf("  Resolution                : %" PRIu8 NEWLINE, ScpGetResolution(scp));
  printf("  Resolution enhanced       : %s" NEWLINE, boolToStr(ScpIsResolutionEnhanced(scp)));
  printf("  Clock outputs             : ");
  printClockOutput(ScpGetClockOutputs(scp));
  printf(NEWLINE);
  printf("  Clock output              : ");
  printClockOutput(ScpGetClockOutput(scp));
  printf(NEWLINE);

  uint32_t clockOutputFrequencyCount = ScpGetClockOutputFrequencies(scp, NULL, 0);
  if(clockOutputFrequencyCount > 0)
  {
    double clockOutputFrequencies[sizeof(double) * clockOutputFrequencyCount];
    clockOutputFrequencyCount = ScpGetClockOutputFrequencies(scp, clockOutputFrequencies, clockOutputFrequencyCount);

    printf("  Clock output frequencies  : ");
    for(uint32_t i = 0; i < clockOutputFrequencyCount; i++)
    {
      if(i != 0)
        printf(", ");

      printf("%f", clockOutputFrequencies[i]);
    }
    printf(NEWLINE);
    printf("  Clock output frequency    : %f" NEWLINE, ScpGetClockOutputFrequency(scp));
  }

  printf("  Clock sources             : ");
  printClockSource(ScpGetClockSources(scp));
  printf(NEWLINE);
  printf("  Clock source              : ");
  printClockSource(ScpGetClockSource(scp));
  printf(NEWLINE);

  uint32_t clockSourceFrequencyCount = ScpGetClockSourceFrequencies(scp, NULL, 0);
  if(clockSourceFrequencyCount > 0)
  {
    double clockSourceFrequencies[sizeof(double) * clockSourceFrequencyCount];
    clockSourceFrequencyCount = ScpGetClockSourceFrequencies(scp, clockSourceFrequencies, clockSourceFrequencyCount);

    printf("  Clock source frequencies  : ");
    for(uint32_t i = 0; i < clockSourceFrequencyCount; i++)
    {
      if(i != 0)
        printf(", ");

      printf("%f", clockSourceFrequencies[i]);
    }
    printf(NEWLINE);
    printf("  Clock source frequency    : %f" NEWLINE, ScpGetClockSourceFrequency(scp));
  }

  printf("  Record length max         : %" PRIu64 NEWLINE, ScpGetRecordLengthMax(scp));
  printf("  Record length             : %" PRIu64 NEWLINE, ScpGetRecordLength(scp));
  printf("  Sample frequency max      : %f" NEWLINE, ScpGetSampleFrequencyMax(scp));
  printf("  Sample frequency          : %f" NEWLINE, ScpGetSampleFrequency(scp));

  if(ScpGetMeasureMode(scp) == MM_BLOCK)
  {
    printf("  Segment count max         : %" PRIu32 NEWLINE, ScpGetSegmentCountMax(scp));
    printf("  Segment count             : %" PRIu32 NEWLINE, ScpGetSegmentCount(scp));
  }

  if(ScpHasTrigger(scp))
  {
    printf("  Pre sample ratio          : %f" NEWLINE, ScpGetPreSampleRatio(scp));

    double triggerTimeout = ScpGetTriggerTimeOut(scp);
    printf("  Trigger time out          : ");
    if(triggerTimeout == TO_INFINITY)
      printf("Infinite" NEWLINE);
    else
      printf("%f" NEWLINE, triggerTimeout);

    if(ScpHasTriggerDelay(scp))
    {
      printf("  Trigger delay max         : %f" NEWLINE, ScpGetTriggerDelayMax(scp));
      printf("  Trigger delay             : %f" NEWLINE, ScpGetTriggerDelay(scp));
    }

    if(ScpHasTriggerHoldOff(scp))
    {
      printf("  Trigger hold off count max: %" PRIu64 NEWLINE, ScpGetTriggerHoldOffCountMax(scp));
      printf("  Trigger hold off count    : %" PRIu64 NEWLINE, ScpGetTriggerHoldOffCount(scp));
    }
  }

  for(uint16_t ch = 0; ch < channelCount; ch++)
  {
    printf("  Channel%" PRIu16 ":" NEWLINE, (ch + 1));
    printf("    Connector type          : ");
    printConnectorType(ScpChGetConnectorType(scp, ch));
    printf(NEWLINE);
    printf("    Differential            : %s" NEWLINE, boolToStr(ScpChIsDifferential(scp, ch)));
    printf("    Impedance               : %f" NEWLINE, ScpChGetImpedance(scp, ch));
    printf("    Connection test         : %s" NEWLINE, boolToStr(ScpChHasConnectionTest(scp, ch)));
    printf("    Available               : %s" NEWLINE, boolToStr(ScpChIsAvailable(scp, ch)));
    printf("    Enabled                 : %s" NEWLINE, boolToStr(ScpChGetEnabled(scp, ch)));

    uint32_t bandwidthCount = ScpChGetBandwidths(scp, ch, NULL, 0);
    double bandwidths[sizeof(double) * bandwidthCount];
    bandwidthCount = ScpChGetBandwidths(scp, ch, bandwidths, bandwidthCount);

    printf("    Bandwidths              : ");
    for(uint32_t i = 0; i < bandwidthCount; i++)
    {
      if(i != 0)
        printf(", ");

      printf("%f", bandwidths[i]);
    }
    printf(NEWLINE);
    printf("    Bandwidth               : %f" NEWLINE, ScpChGetBandwidth(scp, ch));

    printf("    Couplings               : ");
    printCoupling(ScpChGetCouplings(scp, ch));
    printf(NEWLINE);
    printf("    Coupling                : ");
    printCoupling(ScpChGetCoupling(scp, ch));
    printf(NEWLINE);
    printf("    Auto ranging            : %s" NEWLINE, boolToStr(ScpChGetAutoRanging(scp, ch)));

    uint32_t rangeCount = ScpChGetRanges(scp, ch, NULL, 0);
    double ranges[sizeof(double) * rangeCount];
    rangeCount = ScpChGetRanges(scp, ch, ranges, rangeCount);

    printf("    Ranges                  : ");
    for(uint32_t i = 0; i < rangeCount; i++)
    {
      if(i != 0)
        printf(", ");

      printf("%f", ranges[i]);
    }
    printf(NEWLINE);

    printf("    Range                   : %f" NEWLINE, ScpChGetRange(scp, ch));
    printf("    Probe gain              : %f" NEWLINE, ScpChGetProbeGain(scp, ch));
    printf("    Probe offset            : %f" NEWLINE, ScpChGetProbeOffset(scp, ch));
    if(ScpChHasSafeGround(scp, ch))
    {
      printf("    SafeGround enabled      : %s" NEWLINE, boolToStr(ScpChGetSafeGroundEnabled(scp, ch)));
      printf("    SafeGround threshold max: %f" NEWLINE, ScpChGetSafeGroundThresholdMax(scp, ch));
      printf("    SafeGround threshold min: %f" NEWLINE, ScpChGetSafeGroundThresholdMin(scp, ch));
      printf("    SafeGround threshold    : %f" NEWLINE, ScpChGetSafeGroundThreshold(scp, ch));
    }

    if(ScpChHasTrigger(scp, ch))
    {
      printf("  Trigger:" NEWLINE);
      printf("    Available               : %s" NEWLINE, boolToStr(ScpChTrIsAvailable(scp, ch)));
      printf("    Enabled                 : %s" NEWLINE, boolToStr(ScpChTrGetEnabled(scp, ch)));
      printf("    Kinds                   : ");
      printTriggerKind(ScpChTrGetKinds(scp, ch));
      printf(NEWLINE);
      printf("    Kind                    : ");
      printTriggerKind(ScpChTrGetKind(scp, ch));
      printf(NEWLINE);
      printf("    Level modes             : ");
      printTriggerLevelMode(ScpChTrGetLevelModes(scp, ch));
      printf(NEWLINE);
      printf("    Level mode              : ");
      printTriggerLevelMode(ScpChTrGetLevelMode(scp, ch));
      printf(NEWLINE);

      const uint32_t triggerLevelCount = ScpChTrGetLevelCount(scp, ch);
      printf("    Levels                  : ");
      for(uint32_t i = 0; i < triggerLevelCount; i++)
      {
        if(i != 0)
          printf(", ");

        printf("%f", ScpChTrGetLevel(scp, ch, i));
      }
      printf(NEWLINE);

      const uint32_t triggerHysteresisCount = ScpChTrGetHysteresisCount(scp, ch);
      printf("    Hystereses              : ");
      for(uint32_t i = 0; i < triggerHysteresisCount; i++)
      {
        if(i != 0)
          printf(", ");

        printf("%f", ScpChTrGetHysteresis(scp, ch, i));
      }
      printf(NEWLINE);

      printf("    Conditions              : ");
      printTriggerCondition(ScpChTrGetConditions(scp, ch));
      printf(NEWLINE);
      if(ScpChTrGetConditions(scp, ch) != TCM_NONE)
      {
        printf("    Condition               : ");
        printTriggerCondition(ScpChTrGetCondition(scp, ch));
        printf(NEWLINE);
      }

      const uint32_t triggerTimeCount = ScpChTrGetTimeCount(scp, ch);
      if(triggerTimeCount > 0)
      {
        printf("    Times                   : ");
        for(uint32_t i = 0; i < triggerTimeCount; i++)
        {
          if(i != 0)
            printf(", ");

          printf("%f" NEWLINE, ScpChTrGetTime(scp, ch, i));
        }
        printf(NEWLINE);
      }
    }
  }

  printTriggerInputsInfo(scp);
  printTriggerOutputsInfo(scp);
}

void printGeneratorInfo(LibTiePieHandle_t gen)
{
  if(gen == LIBTIEPIE_HANDLE_INVALID)
  {
    fprintf(stderr, "Invalid device handle in printGeneratorInfo()" NEWLINE);
    return;
  }

  printf("Generator:" NEWLINE);
  printf("  Connector type            : ");
  printConnectorType(GenGetConnectorType(gen));
  printf(NEWLINE);
  printf("  Differential              : %s" NEWLINE, boolToStr(GenIsDifferential(gen)));
  printf("  Controllable              : %s" NEWLINE, boolToStr(GenIsControllable(gen)));
  printf("  Impedance                 : %f" NEWLINE, GenGetImpedance(gen));
  printf("  Resolution                : %" PRIu8 NEWLINE, GenGetResolution(gen));
  printf("  Output value min          : %f" NEWLINE, GenGetOutputValueMin(gen));
  printf("  Output value max          : %f" NEWLINE, GenGetOutputValueMax(gen));
  printf("  Output on                 : %s" NEWLINE, boolToStr(GenGetOutputOn(gen)));
  if(GenHasOutputInvert(gen))
  {
    printf("  Output invert             : %s" NEWLINE, boolToStr(GenGetOutputInvert(gen)));
  }

  printf("  Modes native              : ");
  printGeneratorMode(GenGetModesNative(gen));
  printf(NEWLINE);
  printf("  Modes                     : ");
  printGeneratorMode(GenGetModes(gen));
  printf(NEWLINE);
  if(GenGetModes(gen) != GMM_NONE)
  {
    printf("  Burst mode                : ");
    printGeneratorMode(GenGetMode(gen));
    printf(NEWLINE);
    if(GenGetMode(gen) & GMM_BURST_COUNT)
    {
      printf("  Burst active              : %s" NEWLINE, boolToStr(GenIsBurstActive(gen)));
      printf("  Burst count max           : %" PRIu64 NEWLINE, GenGetBurstCountMax(gen));
      printf("  Burst count               : %" PRIu64 NEWLINE, GenGetBurstCount(gen));
    }
    if(GenGetMode(gen) & GMM_BURST_SAMPLE_COUNT)
    {
      printf("  Burst sample max          : %" PRIu64 NEWLINE, GenGetBurstSampleCountMax(gen));
      printf("  Burst sample              : %" PRIu64 NEWLINE, GenGetBurstSampleCount(gen));
    }
    if(GenGetMode(gen) & GMM_BURST_SEGMENT_COUNT)
    {
      printf("  Burst segment max         : %" PRIu64 NEWLINE, GenGetBurstSegmentCountMax(gen));
      printf("  Burst segment             : %" PRIu64 NEWLINE, GenGetBurstSegmentCount(gen));
    }
  }

  printf("  Signal types              : ");
  printSignalType(GenGetSignalTypes(gen));
  printf(NEWLINE);
  printf("  Signal type               : ");
  printSignalType(GenGetSignalType(gen));
  printf(NEWLINE);

  if(GenHasAmplitude(gen))
  {
    printf("  Amplitude min             : %f" NEWLINE, GenGetAmplitudeMin(gen));
    printf("  Amplitude max             : %f" NEWLINE, GenGetAmplitudeMax(gen));
    printf("  Amplitude                 : %f" NEWLINE, GenGetAmplitude(gen));

    uint32_t rangeCount = GenGetAmplitudeRanges(gen, NULL, 0);
    double ranges[sizeof(double) * rangeCount];
    rangeCount = GenGetAmplitudeRanges(gen, ranges, rangeCount);

    printf("  Amplitude ranges          : ");
    for(uint32_t i = 0; i < rangeCount; i++)
    {
      if(i != 0)
        printf(", ");

      printf("%f", ranges[i]);
    }
    printf(NEWLINE);

    printf("  Amplitude range           : %f" NEWLINE, GenGetAmplitudeRange(gen));
    printf("  Amplitude auto ranging    : %s" NEWLINE, boolToStr(GenGetAmplitudeAutoRanging(gen)));
  }

  if(GenHasFrequency(gen))
  {
    printf("  Frequency modes           : ");
    printFrequencyMode(GenGetFrequencyModes(gen));
    printf(NEWLINE);
    printf("  Frequency mode            : ");
    printFrequencyMode(GenGetFrequencyMode(gen));
    printf(NEWLINE);
    printf("  Frequency min             : %f" NEWLINE, GenGetFrequencyMin(gen));
    printf("  Frequency max             : %f" NEWLINE, GenGetFrequencyMax(gen));
    printf("  Frequency                 : %f" NEWLINE, GenGetFrequency(gen));
  }

  if(GenHasOffset(gen))
  {
    printf("  Offset min                : %f" NEWLINE, GenGetOffsetMin(gen));
    printf("  Offset max                : %f" NEWLINE, GenGetOffsetMax(gen));
    printf("  Offset                    : %f" NEWLINE, GenGetOffset(gen));
  }

  if(GenHasPhase(gen))
  {
    printf("  Phase min                 : %f" NEWLINE, GenGetPhaseMin(gen));
    printf("  Phase max                 : %f" NEWLINE, GenGetPhaseMax(gen));
    printf("  Phase                     : %f" NEWLINE, GenGetPhase(gen));
  }

  if(GenHasSymmetry(gen))
  {
    printf("  Symmetry min              : %f" NEWLINE, GenGetSymmetryMin(gen));
    printf("  Symmetry max              : %f" NEWLINE, GenGetSymmetryMax(gen));
    printf("  Symmetry                  : %f" NEWLINE, GenGetSymmetry(gen));
  }

  if(GenHasWidth(gen))
  {
    printf("  Width min                 : %f" NEWLINE, GenGetWidthMin(gen));
    printf("  Width max                 : %f" NEWLINE, GenGetWidthMax(gen));
    printf("  Width                     : %f" NEWLINE, GenGetWidth(gen));
  }

  if(GenHasEdgeTime(gen))
  {
    printf("  Leading edge time min     : %f" NEWLINE, GenGetLeadingEdgeTimeMax(gen));
    printf("  Leading edge time max     : %f" NEWLINE, GenGetLeadingEdgeTimeMin(gen));
    printf("  Leading edge time         : %f" NEWLINE, GenGetLeadingEdgeTime(gen));
    printf("  Trailing edge time min    : %f" NEWLINE, GenGetTrailingEdgeTimeMax(gen));
    printf("  Trailing edge time max    : %f" NEWLINE, GenGetTrailingEdgeTimeMin(gen));
    printf("  Trailing edge time        : %f" NEWLINE, GenGetTrailingEdgeTime(gen));
  }

  if(GenHasData(gen))
  {
    printf("  DataLength min            : %" PRIu64 NEWLINE, GenGetDataLengthMin(gen));
    printf("  DataLength max            : %" PRIu64 NEWLINE, GenGetDataLengthMax(gen));
    printf("  DataLength                : %" PRIu64 NEWLINE, GenGetDataLength(gen));
  }

  printTriggerInputsInfo(gen);
  printTriggerOutputsInfo(gen);
}

void printI2CInfo(LibTiePieHandle_t i2c)
{
  if(i2c == LIBTIEPIE_HANDLE_INVALID)
  {
    fprintf(stderr, "Invalid device handle in printI2CInfo()" NEWLINE);
    return;
  }

  printf("I2C Host:" NEWLINE);

  uint32_t internalAddressCount = I2CGetInternalAddresses(i2c, NULL, 0);
  uint16_t internalAddresses[sizeof(uint16_t) * internalAddressCount];
  internalAddressCount = I2CGetInternalAddresses(i2c, internalAddresses, internalAddressCount);

  printf("  Internal addresses        : ");
  for(uint32_t i = 0; i < internalAddressCount; i++)
  {
    if(i != 0)
      printf(", ");

    printf("%u", internalAddresses[i]);
  }
  printf(NEWLINE);

  printf("  Speed max                 : %f" NEWLINE, I2CGetSpeedMax(i2c));
  printf("  Speed                     : %f" NEWLINE, I2CGetSpeed(i2c));

  printTriggerInputsInfo(i2c);
  printTriggerOutputsInfo(i2c);
}

void printServerInfo(LibTiePieHandle_t srv)
{
  if(srv == LIBTIEPIE_HANDLE_INVALID)
  {
    fprintf(stderr, "Invalid server handle in printServerInfo()" NEWLINE);
    return;
  }

  printf("Server:" NEWLINE);

  uint32_t length = SrvGetURL(srv, NULL, 0) + 1;
  char url[length];
  SrvGetURL(srv, url, length);
  printf("  URL                       : %s" NEWLINE, url);

  length = SrvGetName(srv, NULL, 0) + 1;
  char name[length];
  SrvGetName(srv, name, length);
  printf("  Name                      : %s" NEWLINE, name);

  length = SrvGetDescription(srv, NULL, 0) + 1;
  char description[length];
  SrvGetDescription(srv, description, length);
  printf("  Description               : %s" NEWLINE, description);

  printf("  IPv4 address              : ");
  printIPv4Address(SrvGetIPv4Address(srv));
  printf(NEWLINE);

  printf("  IP port                   : %" PRIu16 NEWLINE, SrvGetIPPort(srv));

  length = SrvGetID(srv, NULL, 0) + 1;
  char id[length];
  SrvGetID(srv, id, length);
  printf("  Id                        : %s" NEWLINE, id);

  printf("  Version                   : ");
  printVersion(SrvGetVersion(srv));
  printf(NEWLINE);

  printf("  Status                    : %s" NEWLINE, ServerStatuses[SrvGetStatus(srv)]);

  if(SrvGetLastError(srv) != LIBTIEPIE_SERVER_ERROR_NONE)
    printf("  Last error                : %s" NEWLINE, ServerErrorCodes[SrvGetLastError(srv)]);
}

void printTriggerInputsInfo(LibTiePieHandle_t dev)
{
  if(dev == LIBTIEPIE_HANDLE_INVALID)
  {
    fprintf(stderr, "Invalid device handle in printTriggerInputsInfo()" NEWLINE);
    return;
  }

  const uint16_t count = DevTrGetInputCount(dev);

  for(uint16_t i = 0; i < count; i++)
  {
    printf("  TriggerInput %" PRIu16 ":" NEWLINE, i);
    printf("    Id                      : %" PRIu32 NEWLINE, DevTrInGetId(dev, i));

    uint32_t length = DevTrInGetName(dev, i, NULL, 0) + 1;
    char name[sizeof(char) * length];
    length = DevTrInGetName(dev, i, name, length);
    printf("    Name                    : %s" NEWLINE, name);

    printf("    Available               : %s" NEWLINE, boolToStr(DevTrInIsAvailable(dev, i)));
    if(DevTrInIsAvailable(dev, i))
    {
      printf("    Enabled                 : %s" NEWLINE, boolToStr(DevTrInGetEnabled(dev, i)));
      printf("    Kinds                   : ");
      printTriggerKind(DevTrInGetKinds(dev, i));
      printf(NEWLINE);
      if(DevTrInGetKinds(dev, i) != TKM_NONE)
      {
        printf("    Kind                    : ");
        printTriggerKind(DevTrInGetKind(dev, i));
        printf(NEWLINE);
      }
    }
  }
}

void printTriggerOutputsInfo(LibTiePieHandle_t dev)
{
  if(dev == LIBTIEPIE_HANDLE_INVALID)
  {
    fprintf(stderr, "Invalid device handle in printTriggerOutputsInfo()" NEWLINE);
    return;
  }

  const uint16_t count = DevTrGetOutputCount(dev);

  for(uint16_t i = 0; i < count; i++)
  {
    printf("  TriggerOutput %" PRIu16 ":" NEWLINE, i);
    printf("    Id                      : %" PRIu32 NEWLINE, DevTrOutGetId(dev, i));

    uint32_t length = DevTrOutGetName(dev, i, NULL, 0) + 1;
    char name[sizeof(char) * length];
    length = DevTrOutGetName(dev, i, name, length);
    printf("    Name                    : %s" NEWLINE, name);

    printf("    Enabled                 : %s" NEWLINE, boolToStr(DevTrOutGetEnabled(dev, i)));
    printf("    Events                  : ");
    printTriggerOutputEvent(DevTrOutGetEvents(dev, i));
    printf(NEWLINE);
    printf("    Event                   : ");
    printTriggerOutputEvent(DevTrOutGetEvent(dev, i));
    printf(NEWLINE);
  }
}

// Print functions for special values/types:
void printGeneratorMode(uint64_t generatorModes)
{
  if(generatorModes == GM_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < GMN_COUNT; i++)
  {
    if(generatorModes & (1ULL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", GeneratorModes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printClockOutput(uint32_t clockOutputs)
{
  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < CON_COUNT; i++)
  {
    if(clockOutputs & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", ClockOutputTypes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printClockSource(uint32_t clockSources)
{
  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < CSN_COUNT; i++)
  {
    if(clockSources & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", ClockSources[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printConnectorType(uint32_t connectorTypes)
{
  if(connectorTypes == CONNECTORTYPE_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < CONNECTORTYPE_COUNT; i++)
  {
    if(connectorTypes & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", ConnectorTypes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printCoupling(uint64_t couplings)
{
  if(couplings == CK_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < CKN_COUNT; i++)
  {
    if(couplings & (1ULL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", Couplings[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printDate(TpDate_t date)
{
  if(date != 0)
    printf("%04" PRIu16 "-%02" PRIu8 "-%02" PRIu8, (uint16_t)TPDATE_YEAR(date), (uint8_t)TPDATE_MONTH(date), (uint8_t)TPDATE_DAY(date));
  else
    printf("Unavailable");
}

void printDeviceType(uint32_t deviceTypes)
{
  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < DEVICETYPE_COUNT; i++)
  {
    if(deviceTypes & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", DeviceTypes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printFrequencyMode(uint32_t frequencyModes)
{
  if(frequencyModes == FM_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < FMN_COUNT; i++)
  {
    if(frequencyModes & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", FrequencyModes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printMeasureMode(uint32_t measureModes)
{
  if(measureModes == MM_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < MMN_COUNT; i++)
  {
    if(measureModes & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", MeasureModes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printAutoResolutionMode(uint32_t autoResolutionModes)
{
  if(autoResolutionModes == AR_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < ARN_COUNT; i++)
  {
    if(autoResolutionModes & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", AutoResolutionModes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printSignalType(uint32_t signalTypes)
{
  if(signalTypes == ST_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < STN_COUNT; i++)
  {
    if(signalTypes & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", SignalTypes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printTriggerCondition(uint32_t triggerConditions)
{
  if(triggerConditions == TC_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < TCN_COUNT; i++)
  {
    if(triggerConditions & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", TriggerConditions[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printTriggerKind(uint64_t triggerKinds)
{
  if(triggerKinds == TK_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < TKN_COUNT; i++)
  {
    if(triggerKinds & (1ULL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", TriggerKinds[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printTriggerLevelMode(uint32_t triggerLevelModes)
{
  if(triggerLevelModes == TLM_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < TLMN_COUNT; i++)
  {
    if(triggerLevelModes & (1UL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", TriggerLevelModes[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printTriggerOutputEvent(uint64_t triggerOutputEvents)
{
  if(triggerOutputEvents == TOE_UNKNOWN)
  {
    printf("Unknown");
    return;
  }

  bool8_t first = BOOL8_TRUE;

  for(unsigned int i = 0; i < TOEN_COUNT; i++)
  {
    if(triggerOutputEvents & (1ULL << i))
    {
      if(!first)
        printf(", ");

      printf("%s", TriggerOutputEvents[i]);

      first = BOOL8_FALSE;
    }
  }
}

void printVersion(TpVersion_t version)
{
  printf("%" PRIu16 ".%" PRIu16 ".%" PRIu16 ".%" PRIu16, (uint16_t)TPVERSION_MAJOR(version), (uint16_t)TPVERSION_MINOR(version), (uint16_t)TPVERSION_RELEASE(version), (uint16_t)TPVERSION_BUILD(version));
}

void printIPv4Address(uint32_t ipv4Address)
{
  printf("%" PRIu32 ".%" PRIu32 ".%" PRIu32 ".%" PRIu32, ipv4Address >> 24, (ipv4Address >> 16) & 0xff, (ipv4Address >> 8) & 0xff, ipv4Address & 0xff);
}

// String conversion functions:
const char* boolToStr(bool8_t value)
{
  return (value == BOOL8_FALSE) ? "false" : "true";
}
