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
OBJECTS =  $(OBJDIR)/main.o $(OBJDIR)/Camera.o $(OBJDIR)/Shader.o $(OBJDIR)/Program.o $(OBJDIR)/Voxel.o $(OBJDIR)/Texture.o $(OBJDIR)/Grass.o $(OBJDIR)/Tree.o $(OBJDIR)/Flower.o $(OBJDIR)/Chunk.o $(OBJDIR)/stb_image.o

# Main objetice - created with 'make' or 'make main'.
main: $(OBJECTS) Makefile
	$(CCC) -I$(IDIR) $(CCFLAGS) -o main $(OBJECTS) $(LDFLAGS) $(GLFLAGS)

# Part objectives
$(OBJDIR)/main.o: $(SRC)/main.cc
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/main.cc -o $(OBJDIR)/main.o

$(OBJDIR)/Camera.o: $(SRC)/Camera.cc $(IDIR)/Camera.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Camera.cc -o $(OBJDIR)/Camera.o

$(OBJDIR)/Shader.o: $(SRC)/Shader.cc $(IDIR)/Shader.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Shader.cc -o $(OBJDIR)/Shader.o

$(OBJDIR)/Program.o: $(SRC)/Program.cc $(IDIR)/Program.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Program.cc -o $(OBJDIR)/Program.o

$(OBJDIR)/Voxel.o: $(SRC)/Voxel.cc $(IDIR)/Voxel.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Voxel.cc -o $(OBJDIR)/Voxel.o

$(OBJDIR)/Texture.o: $(SRC)/Texture.cc $(IDIR)/Texture.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Texture.cc -o $(OBJDIR)/Texture.o

$(OBJDIR)/Grass.o: $(SRC)/Grass.cc $(IDIR)/Grass.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Grass.cc -o $(OBJDIR)/Grass.o

$(OBJDIR)/Tree.o: $(SRC)/Tree.cc $(IDIR)/Tree.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Tree.cc -o $(OBJDIR)/Tree.o

$(OBJDIR)/Flower.o: $(SRC)/Flower.cc $(IDIR)/Flower.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Flower.cc -o $(OBJDIR)/Flower.o

$(OBJDIR)/Chunk.o: $(SRC)/Chunk.cc $(IDIR)/Chunk.h
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/Chunk.cc -o $(OBJDIR)/Chunk.o

$(OBJDIR)/stb_image.o: $(SRC)/stb_image.cc $(IDIR)/stb_image.h
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
