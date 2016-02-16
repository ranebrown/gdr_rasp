# makefile /bin/sh

# directories
SRCDIR = ../src
BUILDDIR = build
TESTDIR = ../test
INCDIR = ../inc

# Compiler to use
CC = g++

# Include paths for header files
INCLUDES = -I $(INCDIR) -I /usr/local/include/opencv -I /usr/local/include/opencv2

# Compiler flags
# pkg-config pulls in needed libraries
CFLAGS = -Wall -Wextra -g $(INCLUDES) `pkg-config --cflags --libs opencv`

# Paths to required libraries -L flags
LFLAGS =

# The specific libraries that project depends on -l flags
LIBS =

# All source files
SRCS = $(wildcard $(SRCDIR)/*.cpp)

# All object files
OBJS := $(SRCS:$(SRCDIR)/%.cpp=%.o)

# name of executable
MAIN = exc

# make all
all: $(MAIN)

$(MAIN): $(OBJS)
	@$(CC) $(CFLAGS) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	@echo "Compiling executable $(MAIN)..."
	@echo "Operation complete :)"

# Automatically builds all object files from source files
# -c option compiles but does not link (create object files)
# -o is output filename
$(OBJS): %.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiling $@..."

# This is used in case there is a file named clean
# @ suppresses command at shell
.PHONY: clean
clean:
	@if [ -e $(MAIN) ]; then rm $(MAIN); fi;
	@if [ `find . -name "*.o" | head -1` ]; then rm *.o; fi;
	@if [ `find . -name "*.txt" | head -1` ]; then rm *.txt; fi;
	@echo "Cleaning..."
	@echo "Operation complete :)"

