IDIR =./include
CXX=g++
#CPPFLAGS=-g -I$(IDIR) -I/home/samarth/codes/voronoi/include -std=c++11 -O3 -w -Wall
CPPFLAGS=-g -I$(IDIR) -O3
#CPPFLAGS=-I$(IDIR) -O3
#LDFLAGS=-O3
#POSTFIX=-L/home/samarth/codes/voronoi/lib/ -lvoro++


SRC_DIR = ./src
OBJ_DIR = ./obj
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
BIN_SRC_DIR = ./bin_src
BIN_DIR = ./bin

all: critical_bonds
#all: psd_test
	
critical_bonds: $(OBJ_FILES) | $(BIN_DIR)
	$(CXX) $(CPPFLAGS) -c -o critical_bonds.o target/critical_bonds.cpp
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
