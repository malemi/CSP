name := CSP
G4TARGET := $(name)
G4EXLIB := true
G4BIN := ./

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

CPPFLAGS += -I/cern/pro/include/cfortran -Df2cFortran

G4ANALYSIS_USE := true

ifdef G4ANALYSIS_USE
  CPPFLAGS += -DG4ANALYSIS_USE
endif


include $(G4INSTALL)/config/architecture.gmk

ifdef G4ANALYSIS_USE
  # for the aida-config command see the README file
  CPPFLAGS += `aida-config --include`
  LOADLIBS += `aida-config --lib`
endif

visclean:
	rm -f g4*.prim g4*.eps g4*.wrl
	rm -f .DAWN_*

histclean:
	rm ${G4WORKDIR}/tmp/${G4SYSTEM}/${G4TARGET}/HistoManager.o
