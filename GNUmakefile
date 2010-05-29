name := cexmc
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

EXTRALIBS = `cernlib geant321 phtools packlib kernlib`
EXTRALIBS += -lboost_serialization

USE_ROOT := $(shell which root-config 2>/dev/null)

ifneq ($(USE_ROOT)),)
  CPPFLAGS += -I`root-config --incdir`
  EXTRALIBS += `root-config --libs`
  CPPFLAGS += -DCEXMC_USE_ROOT
  USE_ROOTQT := $(shell root-config --features | grep qt)
  ifneq ($(USE_ROOTQT),)
    EXTRALIBS += -lGQt
    CPPFLAGS += -DCEXMC_USE_ROOTQT
  endif
endif

GCC_VERSION := $(shell gcc --version | head -1 | awk '{ printf $$3 }' | awk -F"." '{ printf $$1 }')

ifdef CEXMC_FORTRAN_LIB
  EXTRALIBS += $(CEXMC_FORTRAN_LIB)
else
# try to setup fortran lib automatically
# WARNING: the following is not robust check because cernlib can be built
# against libg2c even when using gcc-4 series
# Please define CEXMC_FORTRAN_LIB if the check fails
  ifeq ($(GCC_VERSION),3)
    EXTRALIBS += -lg2c
  else
    EXTRALIBS += -lgfortran
  endif
endif

#CPPFLAGS += -DCEXMC_USE_QGSP_BIC_EMY
CPPFLAGS += -DCEXMC_DEBUG_TP

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
