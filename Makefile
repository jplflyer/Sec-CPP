#----------------------------------------------------------------------
# The default target.
#----------------------------------------------------------------------
all: directories makelib makeprograms

#----------------------------------------------------------------------
# A ton of boilerplate copied from ShowLib.
#----------------------------------------------------------------------
UNAME := $(shell uname)
MACAPPEND=
AUTO_ARGUMENT=

THREADING_ARG=-j4

ifeq ($(UNAME), Darwin)
		MACAPPEND=-mac
		CXX=clang++
else
		CORES := $(shell lscpu | egrep Core.s | cut -d: -f2)
		ifeq ($(shell test ${CORES} -gt 4; echo $$?),0)
				THREADING_ARG=-j${CORES}
		endif
endif

SRCDIR := src
OBJDIR := obj${MACAPPEND}
DEPDIR := .d
BINDIR := bin${MACAPPEND}


# This gives us some starter values. Makefiles should use += to add to these.
# Be careful with CXX flags and use = instead of := so that INCLUDES gets late expansion.
INCLUDES := -I/usr/local/include -I${SRCDIR}
CXXFLAGS = ${INCLUDES} --std=c++17 -g -Wno-unused-local-typedefs -Wno-deprecated-declarations ${AUTO_ARGUMENT}
LDFLAGS += -L/usr/local/lib

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

#======================================================================
# If we have files in the DEPDIR, include them.
#======================================================================
ifneq (,$(wildcard ${DEPDIR}/*))
include .d/*
endif

#======================================================================
# Build the directories needed.
# You shouldn't need to invoke these directly.
#======================================================================
.PHONY: directories
directories: ${DEPDIR} ${OBJDIR}

${DEPDIR}:
	mkdir -p $@

${OBJDIR}:
	mkdir -p $@

${BINDIR}:
	mkdir -p $@

#======================================================================
# How to make the .o files as. This is a little complicated but is
# derived from:
# http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
#======================================================================

${OBJDIR}/%.o : %.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d


ifneq (,$(wildcard ${DEPDIR}/*))
include .d/*
endif

#----------------------------------------------------------------------
# Rules.
#----------------------------------------------------------------------
TEST_SRC=tests
VPATH := ${SRCDIR}:${TEST_SRC}:${SRCDIR}/OpenSSL-CPP

CXXFLAGS += -I${SRCDIR}

SRC := $(shell find ${SRCDIR} -name "*.cpp" | sort )

SRC_NODIR := $(notdir ${SRC})
OBJ := $(patsubst %.cpp,${OBJDIR}/%.o,${SRC_NODIR})

LIBNAME=sec-cpp${MACAPPEND}
LIB=lib${LIBNAME}.a
LIB_ARGS= -cvrU
ifeq ($(UNAME), Darwin)
    LIB_ARGS= -cvr
endif

INSTALL_BASE=/usr/local


LDFLAGS += -L. -l${LIBNAME}

#----------------------------------------------------------------------
# Clean.
#----------------------------------------------------------------------
.PHONY: clean
clean:
	rm -f ${DEPDIR}/* ${OBJDIR}/* ${LIB} .generated

#----------------------------------------------------------------------
# Dependencies. We'll install jwt-cpp and complain if we're missing
# openssl or libcrypto.
#----------------------------------------------------------------------
dependencies: /usr/local/include/jwt-cpp

/usr/local/include/jwt-cpp:
	rm -rf /tmp/jwt-cpp
	git clone https://github.com/Thalhammer/jwt-cpp.git /tmp/jwt-cpp
	sudo mkdir $@
	sudo cp -R /tmp/jwt-cpp/include/jwt-cpp/* $@
	rm -rf /tmp/jwt-cpp

#======================================================================
# Making the library.
#======================================================================
.PHONY: makelib
makelib:
	@$(MAKE) ${THREADING_ARG} --output-sync=target --no-print-directory lib

lib: ${LIB}

${LIB}: ${OBJ}
	@mkdir -p lib
	ar ${LIB_ARGS} ${LIB} ${OBJ}
	ranlib ${LIB}

#======================================================================
# Making any programs.
#======================================================================
directories: ${BINDIR}

.PHONY: makeprograms
makeprograms:
	@$(MAKE) ${THREADING_ARG} --output-sync=target --no-print-directory programs

programs:

${BINDIR}/%: ${OBJDIR}/%.o
	$(CXX) $^ ${LDFLAGS} ${LIB_DIRS} ${LIBS} $(OUTPUT_OPTION)
