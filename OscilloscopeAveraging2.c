/**
 * OscilloscopeAveraging.c based on cOscilloscopeBlock.c
 * @author Simon Collignon
 *
 * This code performs the averaging of a set of block mode measurments.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <inttypes.h>
#include <libtiepie.h>
#include "CheckStatus.h"
#include "PrintInfo.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
  int status = EXIT_SUCCESS;

  // Initialize library:
  LibInit();

  // Print library information:
  printLibraryInfo();

  // Enable network search:
  NetSetAutoDetectEnabled(BOOL8_TRUE);
  CHECK_LAST_STATUS();

  // Update device list:
  LstUpdate();
  CHECK_LAST_STATUS();

  // Try to open an oscilloscope with block measurement support:
  LibTiePieHandle_t scp = LIBTIEPIE_HANDLE_INVALID;

  for(uint32_t index = 0; index < LstGetCount(); index++)
  {
    if(LstDevCanOpen(IDKIND_INDEX, index, DEVICETYPE_OSCILLOSCOPE))
    {
      scp = LstOpenOscilloscope(IDKIND_INDEX, index);
      CHECK_LAST_STATUS();

      // Check for valid handle and block measurement support:
      if(scp != LIBTIEPIE_HANDLE_INVALID && (ScpGetMeasureModes(scp) & MM_BLOCK))
      {
        break;
      }
      else
      {
        scp = LIBTIEPIE_HANDLE_INVALID;
      }
    }
  }

  if(scp != LIBTIEPIE_HANDLE_INVALID)
  {
    uint16_t channelCount = ScpGetChannelCount(scp);
    CHECK_LAST_STATUS();

    // Set measure mode:
    ScpSetMeasureMode(scp, MM_BLOCK);

    double fs = 200e6;
    // Set sample frequency:
    ScpSetSampleFrequency(scp, fs); // 1 MHz

    // Set pre sample ratio:
    ScpSetPreSampleRatio(scp, 0.0); // 0 %

    // Enable channel 1 to measure it
    ScpChSetEnabled(scp, 0, BOOL8_TRUE);
    CHECK_LAST_STATUS();

    // Enable channel 2
    ScpChSetEnabled(scp, 1, BOOL8_TRUE);
    CHECK_LAST_STATUS();

    // Set record length:
    uint64_t recLength = 32000000;
    uint64_t recordLength = ScpSetRecordLength(scp, recLength); // 64 MPts
    CHECK_LAST_STATUS();

    // Set range:
    double range = 0.8;
    ScpChSetRange(scp, 0, 0.8); // 0.4 V
    CHECK_LAST_STATUS();

    ScpChSetRange(scp, 1, 4); // 2 V
    CHECK_LAST_STATUS();

    // Set coupling:
    ScpChSetCoupling(scp, 0, CK_DCV); // DC Volt
    CHECK_LAST_STATUS();

    ScpChSetCoupling(scp, 1, CK_DCV); // DC Volt
    CHECK_LAST_STATUS();

    // Set trigger timeout:
    ScpSetTriggerTimeOut(scp, 100e-3); // 100 ms
    CHECK_LAST_STATUS();

    // Disable all channel trigger sources:
    for(uint16_t ch = 0; ch < channelCount; ch++)
    {
      ScpChTrSetEnabled(scp, ch, BOOL8_FALSE);
      CHECK_LAST_STATUS();
    }

    // Setup channel trigger:
    const uint16_t ch = 1; // Ch 2

    // Enable trigger source:
    ScpChTrSetEnabled(scp, ch, BOOL8_TRUE);
    CHECK_LAST_STATUS();

    // Kind:
    ScpChTrSetKind(scp, ch, TK_FALLINGEDGE); // Rising edge
    CHECK_LAST_STATUS();

    // Level mode: 
    ScpChTrSetLevelMode(scp, ch, TLM_ABSOLUTE); // We want an absolute level in Volts

    // Level:
    ScpChTrSetLevel(scp, ch, 0, 1); // 1V trigger level
    CHECK_LAST_STATUS();

    // Hysteresis:
    ScpChTrSetHysteresis(scp, ch, 0, 0); // 0 %
    CHECK_LAST_STATUS();

    // Clock source:
    ScpSetClockSource(scp, CS_EXTERNAL);
    CHECK_LAST_STATUS();  

    // Clock output:
    // ScpSetClockOutput(scp, CO_SAMPLE);
    // CHECK_LAST_STATUS(); 

    // Print oscilloscope info:
    printDeviceInfo(scp);

    // modifications start here

    clock_t start, end;
    double cpu_time_used;
    channelCount = 1; // we only want channel 1!
    uint16_t acquisitionCount = 100; // number of acquistions that are averaged
    int cycleLength = 800; // HARDCODED, FIND A BETTER SOLUTION
    float cycleCount = recordLength / cycleLength; // WARNING recordLength HAS to be a multiple of cycleLength for the code to work.
    printf("number of cycle is %f \n", cycleCount);

    start = clock();

    // Create data buffers
    float** averageData = malloc(sizeof(float*) * channelCount);
    float** channelData = malloc(sizeof(float*) * channelCount);
    float** finalData = malloc(sizeof(float*) * channelCount); // changing here to double?

    for(uint16_t ch = 0; ch < channelCount; ch++)
    {
      averageData[ch] = malloc(sizeof(float) * recordLength);
      channelData[ch] = malloc(sizeof(float) * recordLength);
      finalData[ch] = malloc(sizeof(float) * cycleLength);
    }

    // Initialize average buffers to 0
    for(uint16_t ch = 0; ch < channelCount; ch++)
    {
      for(uint64_t i = 0; i < recordLength; i++)
      {
        averageData[ch][i] = 0;
      }
      for(uint64_t i = 0; i < cycleLength; i++)
      {
        finalData[ch][i] = 0;
      }
    }

    // Averaging the acquisitions
    for(uint16_t i = 0; i < acquisitionCount; i++)
    {
      // Start measurement
      ScpStart(scp);
      CHECK_LAST_STATUS();

      // Wait for measurement to complete
      while(!ScpIsDataReady(scp) && !ObjIsRemoved(scp))
      {
        sleepMiliSeconds(10); // 10 ms delay, to save CPU time.
      }

      if(ObjIsRemoved(scp))
      {
        fprintf(stderr, "Device gone!");
        status = EXIT_FAILURE;
      }
      else if(ScpIsDataReady(scp))
      {
        // Get the data from the scope:
        recordLength = ScpGetData(scp, channelData, channelCount, 0, recordLength);
        CHECK_LAST_STATUS();

        for(uint16_t ch = 0; ch < channelCount; ch++)
        {
          for(uint64_t i = 0; i < recordLength; i++)
          {
            averageData[ch][i] += channelData[ch][i]; // we accumulate in averageData buffer
          }
        }
      } 
    }

    // braking averageData into pieces
    for(uint16_t ch = 0; ch < channelCount; ch++)
    {
      for(uint64_t i = 0; i * cycleLength < recordLength; i++)
      {
        for(uint64_t j = 0; j < cycleLength; j++)
        {
          finalData[ch][j] += averageData[ch][i * cycleLength + j]; // we populate finaData buffer
        }
      }
    }

    // timing stop
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Elapsed time is %f seconds \n", (float) cpu_time_used);

    // Open file with write/update permissions:
    const char* filename = "record.csv";
    FILE* csv = fopen(filename, "w");
    if(csv)
    {
      // Write csv header:
      fprintf(csv, "sampling rate [Sa/s]: %f \n", fs);
      fprintf(csv, "record length [Sa]: %" PRIu64, recLength);
      fprintf(csv, "record duration [s]: %f \n", (float) recLength / fs);
      fprintf(csv, "range [V]: %f \n", (float) range);
      fprintf(csv, "acquisition count: %f \n", (float) acquisitionCount);
      fprintf(csv, "FID per acquisition count: %f \n", (float) cycleCount);
      fprintf(csv, "number of averages: %d \n", (int) (acquisitionCount * cycleCount));
      fprintf(csv, "DAQ elapsed time [s]: %f \n", (float) cpu_time_used);
      fprintf(csv, "Time");
      for(uint16_t ch = 0; ch < channelCount; ch++)
      {
        fprintf(csv, ",Ch%" PRIu16, ch + 1);
      }
      fprintf(csv, "\n");

      // Write the data to csv:
      for(uint64_t i = 0; i < cycleLength; i++)
      {
        fprintf(csv, "%.8e", (float) i / fs);
        for(uint16_t ch = 0; ch < channelCount; ch++)
        {
          fprintf(csv, ",%.8e", (float) finalData[ch][i] / (acquisitionCount * cycleCount));
        }
        fprintf(csv, " \n");
      }

      printf("Number of averages is %f \n", acquisitionCount * cycleCount);
      printf("Data written to: %s \n", filename);

      // Close file
      fclose(csv);
    }
    else
    {
      fprintf(stderr, "Couldn't open file: %s" NEWLINE, filename);
      status = EXIT_FAILURE;
    }

    // Free data buffers:
    for(uint16_t ch = 0; ch < channelCount; ch++)
    {
      free(averageData[ch]);
      free(channelData[ch]);
      free(finalData[ch]);
    }

    free(averageData);
    free(channelData);
    free(finalData);

    // Close oscilloscope:
    ObjClose(scp);
    CHECK_LAST_STATUS();
  }
  else
  {
    fprintf(stderr, "No oscilloscope available with block measurement support!" NEWLINE);
    status = EXIT_FAILURE;
  }

  // Exit library:
  LibExit();

  return status;
}
