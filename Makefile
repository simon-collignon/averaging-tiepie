# GNU Makefile
#
# This file is part of the LibTiePie programming examples.
#
# Find more information on http://www.tiepie.com/LibTiePie .

CC = gcc
CFLAGS = -O2 -I. -Wall
LD = gcc
LFLAGS = -ltiepie
ODIR = build

ifeq ($(OS),Windows_NT)
  CFLAGS += -std=c99
  LFLAGS += -L./
  TARGET_EXT = .exe
  RM = del
else
  CFLAGS += -std=gnu99
  LFLAGS += -lm
  TARGET_EXT =
  RM = rm -f
endif

SOURCES = $(wildcard Generator*.c) \
          $(wildcard Oscilloscope*.c) \
          $(wildcard I2C*.c) \
          ListDevices.c

DEPENDENCIES = CheckStatus.c \
               PrintInfo.c \
               Utils.c

OBJECTS = $(SOURCES:.c=.o)
DEPOBJECTS = $(DEPENDENCIES:.c=.o)

TARGETS = $(SOURCES:.c=$(TARGET_EXT))

.PHONY : all clean

all : $(TARGETS)

clean :
	$(RM) $(DEPOBJECTS) $(OBJECTS) $(TARGETS)

%.o : %.c
	$(CC) $(CFLAGS) $< -c -o $@

$(TARGETS) : %$(TARGET_EXT) : $(DEPOBJECTS) %.o
	$(LD) $+ -o $@ $(LFLAGS)
