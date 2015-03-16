CXX := g++
CXXFLAGS += -std=c++11
GCC_LIBRARY_PATH ?= /usr/local/gcc49/lib64
BOOST_PATH ?= $(HOME)/buildutil

BINDIR := ./bin
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
SINGLE_BINS := $(patsubst %.cpp,%,$(SRCS))
DEBUG_SYMBOLS := $(patsubst %.cpp,%.dSYM,$(SRCS))

DEP_DIR := .deps
DEPS := $(addprefix $(DEP_DIR)/,$(subst .cpp,.Tpo,$(SRCS)))

$(shell test -d $(DEP_DIR) || mkdir $(DEP_DIR))
$(shell test -d $(BINDIR) || mkdir $(BINDIR))

ifeq ($(shell uname -s),Linux)
	CXXFLAGS += -I$(BOOST_PATH)/include
	LDFLAGS += -Wl,-rpath=$(GCC_LIBRARY_PATH)
endif

.PHONY: all install clean distclean

.SUFFIXES:
.SUFFIXES: .o .h
.PRECIOUS: %.c %.cpp %.o

all: $(SINGLE_BINS)

-include $(DEPS)

install:
	cp -f $(SINGLE_BINS) $(BINDIR)

%: %.o
	$(CXX) $(LDFLAGS) -o $@ $<
	@which dsymutil &> /dev/null ; if [ $$? -eq 0 ] ; then dsymutil $@ ; fi
	@grep -xq "$@" .gitignore || echo $@ >> .gitignore

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MD -MP -MF $(DEP_DIR)/$*.Tpo -D__MAIN__ -g -o $@ -c $<

clean:
	rm -f $(SINGLE_BINS)
	rm -rf *.dSYM
	rm -f $(OBJS)

distclean: clean
	rm -f $(BINDIR)/*
	rm -f $(DEPS)
