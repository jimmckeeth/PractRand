# Makefile for this PractRand checkout.
#
# Upstream PractRand doesn't ship a Makefile (see doc/installation.txt) --
# this one is specific to this fork (see readme.md, "About this copy").
# It just wraps the g++/ar command lines documented in readme.md.
#
# Targets:
#   make            build libPractRand.a, RNG_test, RNG_output, RNG_benchmark,
#                   and the example generators in examples/
#   make lib        build libPractRand.a only
#   make tools      build RNG_test, RNG_output, RNG_benchmark
#   make examples   build the example generators in examples/
#   make test       build everything, then run tests/run_tests.sh
#   make clean      remove all build output
#
# Override CXX, CXXFLAGS, etc. on the command line if needed, e.g.:
#   make CXX=clang++ CXXFLAGS="-O2 -g"

CXX      ?= g++
CXXSTD    = -std=c++14
CXXFLAGS ?= -O3 -Wall
INCLUDES  = -Iinclude
LDFLAGS   = -pthread
AR       ?= ar

OBJDIR = build/obj
LIB    = libPractRand.a

LIB_SRCS = $(wildcard src/*.cpp) $(wildcard src/RNGs/*.cpp) $(wildcard src/RNGs/other/*.cpp)
LIB_OBJS = $(patsubst src/%.cpp,$(OBJDIR)/%.o,$(LIB_SRCS))
LIB_DEPS = $(LIB_OBJS:.o=.d)

TOOLS = RNG_test RNG_output RNG_benchmark

EXAMPLE_SRCS = $(wildcard examples/*.cpp)
EXAMPLES     = $(EXAMPLE_SRCS:.cpp=)

.PHONY: all lib tools examples test clean help

all: lib tools examples

help:
	@echo "Targets:"
	@echo "  make            build libPractRand.a, RNG_test, RNG_output, RNG_benchmark,"
	@echo "                  and the example generators in examples/"
	@echo "  make lib        build libPractRand.a only"
	@echo "  make tools      build RNG_test, RNG_output, RNG_benchmark"
	@echo "  make examples   build the example generators in examples/"
	@echo "  make test       build everything, then run tests/run_tests.sh"
	@echo "  make clean      remove all build output"

lib: $(LIB)

$(LIB): $(LIB_OBJS)
	$(AR) rcs $@ $(LIB_OBJS)

$(OBJDIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXSTD) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@

-include $(LIB_DEPS)

tools: $(TOOLS)

RNG_test: tools/RNG_test.cpp $(LIB)
	$(CXX) $(CXXSTD) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(LIB) $(LDFLAGS)

RNG_output: tools/RNG_output.cpp $(LIB)
	$(CXX) $(CXXSTD) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(LIB) $(LDFLAGS)

RNG_benchmark: tools/RNG_benchmark.cpp $(LIB)
	$(CXX) $(CXXSTD) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(LIB) $(LDFLAGS)

examples: $(EXAMPLES)

examples/%: examples/%.cpp
	$(CXX) $(CXXSTD) $(CXXFLAGS) -o $@ $<

test: all
	@./tests/run_tests.sh

clean:
	rm -rf $(OBJDIR) $(LIB) $(TOOLS) $(EXAMPLES)
