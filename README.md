# LibTiePie examples for C
[![License](https://img.shields.io/github/license/tiepie/python-libtiepie.svg)](LICENSE)

C examples for the [LibTiePie SDK](https://www.tiepie.com/libtiepie-sdk). The LibTiePie SDK is a library to easily interface with TiePie engineering [WiFi oscilloscopes](https://www.tiepie.com/wifi-oscilloscope) and [USB oscilloscopes](https://www.tiepie.com/usb-oscilloscope). Using the LibTiePie SDK the user has full control over all aspects of the oscilloscope and can perform measurements easily on Windows and Linux. Examples for different measurements are available to get started easily.

## Requirements
- These examples.
- [Download the LibTiePie SDK](https://www.tiepie.com/libtiepie-sdk/download)
- A suitable compiler.

## Building the examples

The examples are tested to work with the following tools:

### GNU Make

#### Windows
The `libtiepie.dll` file and the `libtiepie.h` header file supplied with the library need to be placed in a location where the compiler can find them, e.g. in the folder with the example files.

To build the examples, execute `mingw32-make` in the folder with the examples.

#### Linux
To build the examples, execute `make` in the folder with the examples.

### Qt Creator

#### Windows
The `libtiepie.dll` file and the `libtiepie.h` header file supplied with the library need to be placed in a location where the compiler can find them, e.g. in the folder with the example files.

To build the examples, open and build the project file `LibTiePie_C_examples.pro` in the main folder of the examples.

#### Linux
To build the examples, open and build the project file `LibTiePie_C_examples.pro` in the main folder of the examples.
