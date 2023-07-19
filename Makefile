#
# Makefile
#

# Folders
SRC = src

# this is the directory I will compile from (i.e. the argument to -I)
IDIR := include

# all object files will be put here
OBJDIR := objdir

# Compiler (g++)
CCC = g++

# Compiling flags
CCFLAGS +=  -Wno-deprecated-declarations -Wall -Wextra -pedantic -std=c++1z -Weffc++ -I$(SFML_ROOT)/include
LDFLAGS += -L$(SFML_ROOT)/lib -lsfml-graphics -lsfml-window -lsfml-system
GLFLAGS += -lGL -lX11 -lpthread -lXi -lXrandr -ldl

# Object modules
OBJECTS = $(OBJDIR)/main.o $(OBJDIR)/Camera.o $(OBJDIR)/stb_image.o

# Main objetice - created with 'make' or 'make main'.
main: $(OBJECTS) Makefile
	$(CCC) -I$(IDIR) $(CCFLAGS) -o main $(OBJECTS) $(LDFLAGS) $(GLFLAGS)

# Part objectives
$(OBJDIR)/main.o:
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/main.cc -o $(OBJDIR)/main.o

$(OBJDIR)/Camera.o:
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Camera.cc -o $(OBJDIR)/Camera.o

$(OBJDIR)/stb_image.o:
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/stb_image.cc -o $(OBJDIR)/stb_image.o

.PHONY: run dir clean zap
run: main
	./main

dir:
	@mkdir -p $(OBJDIR)

# 'make clean' removes object files and memory dumps.
clean:
	@ \rm -rf $(OBJDIR)/*.o *.gch core

# 'make zap' also removes the executable and backup files.
zap: clean
	@ \rm -rf main *~
