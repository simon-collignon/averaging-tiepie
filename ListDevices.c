/**
 * ListDevices.c
 *
 * This example prints all the available devices to the screen.
 *
 * Find more information on http://www.tiepie.com/LibTiePie .
 */

#include <stdlib.h>
#include <stdio.h>
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

  // Get the number of connected devices:
  const uint32_t connectedDevices = LstGetCount();
  CHECK_LAST_STATUS();

  if(connectedDevices != 0)
  {
    printf(NEWLINE "Available devices:" NEWLINE);

    for(uint32_t index = 0; index < connectedDevices; index++)
    {
      // Print device info:
      uint32_t length = LstDevGetName(IDKIND_INDEX, index, NULL, 0) + 1;
      CHECK_LAST_STATUS();
      char* name = malloc(sizeof(char) * length);
      length = LstDevGetName(IDKIND_INDEX, index, name, length);
      CHECK_LAST_STATUS();
      printf("  Name: %s" NEWLINE, name);
      free(name);

      printf("    Serial Number  : %" PRIu32 NEWLINE, LstDevGetSerialNumber(IDKIND_INDEX, index));
      CHECK_LAST_STATUS();

      printf("    Available types: ");
      printDeviceType(LstDevGetTypes(IDKIND_INDEX, index));
      CHECK_LAST_STATUS();
      printf(NEWLINE);

      if(LstDevHasServer(IDKIND_INDEX, index))
      {
        LibTiePieHandle_t server = LstDevGetServer(IDKIND_INDEX, index);
        CHECK_LAST_STATUS();

        length = SrvGetURL(server, NULL, 0) + 1;
        CHECK_LAST_STATUS();
        char* url = malloc(sizeof(char) * length);
        length = SrvGetURL(server, url, length);
        CHECK_LAST_STATUS();

        length = SrvGetName(server, NULL, 0) + 1;
        CHECK_LAST_STATUS();
        name = malloc(sizeof(char) * length);
        length = SrvGetName(server, name, length);
        CHECK_LAST_STATUS();

        printf("    Server         : %s (%s)" NEWLINE, url, name);

        free(url);
        free(name);

printServerInfo(server);
        ObjClose(server);
        CHECK_LAST_STATUS();
      }
    }
  }
  else
  {
    fprintf(stderr, "No devices found!" NEWLINE);
  }

  // Exit library:
  LibExit();

  return status;
}
