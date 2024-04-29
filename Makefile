IDIR =./include
CXX=g++
CPPFLAGS=-I$(IDIR) -O3


SRC_DIR = ./src
OBJ_DIR = ./obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_SRC_DIR = ./bin_src
BIN_DIR = ./bin

all: Eigen critical_bonds verify
	
critical_bonds: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o critical_bonds.o critical_bonds.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ critical_bonds.o
	perl ./install-scripts.pl scripts

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

Eigen:
	perl ./install-scripts.pl eigen

verify:
	perl ./install-scripts.pl benchmark

clean:
	rm *.o
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
