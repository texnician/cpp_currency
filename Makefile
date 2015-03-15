CXX := g++
CXXFLAGS += -std=c++11 -arch x86_64
GCC_LIBRARY_PATH=/usr/local/Cellar/gcc49/4.9.2_1/lib/gcc/4.9

BINDIR := ./bin
SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
SINGLE_BINS := $(patsubst %.cpp,%,$(SRCS))
DEBUG_SYMBOLS := $(patsubst %.cpp,%.dSYM,$(SRCS))

DEP_DIR := .deps
DEPS := $(addprefix $(DEP_DIR)/,$(subst .cpp,.Tpo,$(SRCS)))

$(shell test -d $(DEP_DIR) || mkdir $(DEP_DIR))
$(shell test -d $(BINDIR) || mkdir $(BINDIR))

.PHONY: all install clean distclean

.SUFFIXES:
.SUFFIXES: .o .h
.PRECIOUS: %.c %.cpp %.o

all: $(SINGLE_BINS)

-include $(DEPS)

install:
	cp -f $(SINGLE_BINS) $(BINDIR)

%: %.o
	$(CXX) $(CXXFLAGS) -MD -MP -MF $(DEP_DIR)/$*.Tpo -o $@ $<
	@dsymutil $@
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
