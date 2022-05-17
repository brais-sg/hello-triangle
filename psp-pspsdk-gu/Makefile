TARGET = hello-triangle
OBJS = hello-triangle.o callbacks.o vram.o

INCDIR =
CFLAGS = -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgu -lpspgum

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Hello Triangle

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
