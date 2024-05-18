# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Source directory
SRC_DIR = src

# Build directory
BUILD_DIR = build

# Source files (listed in the specified order)
SRCS = \
    $(SRC_DIR)/Beneficiary.cpp \
    $(SRC_DIR)/Volunteer.cpp \
    $(SRC_DIR)/SupplyRequest.cpp \
	$(SRC_DIR)/MedicalWarehouse.cpp \
    $(SRC_DIR)/Action.cpp \
    $(SRC_DIR)/main.cpp

# Object files
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Executable name
TARGET = warehouse

# Make rules
all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking $(TARGET) from $(OBJS)"
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "Compiling $< to $@"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "Cleaning up..."
	rm -f $(OBJS) $(TARGET)