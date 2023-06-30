#
# Makefile
#

# Compiler (g++)
CCC = g++

# Compiling flags
CCFLAGS +=  -Wno-deprecated-declarations -Wall -Wextra -pedantic -std=c++17 -Weffc++ -I$(SFML_ROOT)/include

LDFLAGS += -L$(SFML_ROOT)/lib -lsfml-graphics -lsfml-window -lsfml-system

GLFLAGS += -lGL -lX11 -lpthread -lXi -lXrandr -ldl

# Main objetice - created with 'make' or 'make main'.
#main: $(OBJECTS) Makefile 
main: Makefile main.cc
	$(CCC) $(CCFLAGS) main.cc -o main $(LDFLAGS) $(GLFLAGS)

dir:
	@mkdir -p $(OBJDIR)

# 'make clean' removes object files and memory dumps.
clean:
	@ \rm -rf $(OBJDIR)/*.o *.gch core

# 'make zap' also removes the executable and backup files.
zap: clean
	@ \rm -rf main *~

.PHONY: run
run: main
	./main
