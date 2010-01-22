
name := cexmc
G4TARGET := $(name)
G4EXLIB := true
GCC_VERSION := $(shell gcc --version | head -1 | awk '{ printf $$3 }' | awk -F"." '{ printf $$1 }')

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

CPPFLAGS += -DCEXMC_DEBUG_TP
CPPFLAGS += -I`root-config --incdir`

EXTRALIBS = `cernlib geant321 phtools packlib kernlib`

EXTRALIBS += `root-config --libs` -lboost_serialization

ifeq ($(GCC_VERSION),3)
  EXTRALIBS += -lg2c
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
