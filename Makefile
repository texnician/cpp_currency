CXX := g++
CXXFLAGS += -std=c++11

BINDIR := ./bin
SRCS := $(wildcard *.cpp)
SINGLE_BINS := $(patsubst %.cpp,%,$(SRCS))

$(shell test -d $(BINDIR) || mkdir $(BINDIR))

.PHONY: all install clean distclean

all:
	@echo "Nothing to do"	

install:
	cp -f $(SINGLE_BINS) $(BINDIR)

%: %.cpp
	$(CXX) $(CXXFLAGS) -D__MAIN__ -o $@ $^
	@grep -xq "$@" .gitignore || echo $@ >> .gitignore

clean:
	rm -f $(SINGLE_BINS)

distclean: clean
	rm -f $(BINDIR)/*
