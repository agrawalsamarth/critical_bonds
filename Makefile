IDIR =./include
CXX=g++
CPPFLAGS=-I$(IDIR) -O3


SRC_DIR = ./src
OBJ_DIR = ./obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_SRC_DIR = ./bin_src
BIN_DIR = ./bin

all: critical_bonds
#all: psd_test
	
critical_bonds: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o critical_bonds.o critical_bonds.cpp
	$(CXX) $(LDFLAGS) -o $(BIN_DIR)/$@ $^ critical_bonds.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

clean:
	rm *.o
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
