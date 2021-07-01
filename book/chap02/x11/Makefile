# Makefile for 3ddemo
#

.PHONY: all clean

SRCS = 3ddemo.cpp
OBJS = $(SRCS:.cpp=.o)

EXE = 3ddemo

LIBS = -lX11

CFLAGS ?= -Wall -O2

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CFLAGS) -o $(EXE) $(OBJS) $(LDFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) -MMD -MP -MT $@ -o $@ -c $<

clean:
	rm -f $(OBJS)
	rm -f $(wildcard *.d)
	rm -f $(EXE)

# Include automatically generated dependency files
-include $(wildcard *.d)

