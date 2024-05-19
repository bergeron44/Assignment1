# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

# Directories
BIN_DIR = ./bin
SRC_DIR = ./src

# Source files
SRC_FILES = $(SRC_DIR)/main.cpp \
            $(SRC_DIR)/Beneficiary.cpp \
            $(SRC_DIR)/Volunteer.cpp \
            $(SRC_DIR)/SupplyRequest.cpp \
            $(SRC_DIR)/MedicalWareHouse.cpp \
            $(SRC_DIR)/Action.cpp

# Object files
OBJ_FILES = $(BIN_DIR)/main.o \
            $(BIN_DIR)/Beneficiary.o \
            $(BIN_DIR)/Volunteer.o \
            $(BIN_DIR)/SupplyRequest.o \
            $(BIN_DIR)/MedicalWareHouse.o \
            $(BIN_DIR)/Action.o

# Target executable
TARGET = $(BIN_DIR)/executable

all: clean compile link

compile: $(OBJ_FILES)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

link: $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ_FILES)

clean:
	rm -f $(BIN_DIR)/*

.PHONY: all clean compile link
