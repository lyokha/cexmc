
name := cexmc
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

EXTRALIBS = `cernlib-g77 geant321 phtools packlib kernlib`
EXTRALIBS += -L/usr/lib/gcc/x86_64-redhat-linux/3.4.6 -lg2c

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*
