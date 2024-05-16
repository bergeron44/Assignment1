# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Source files
SRCS = \
    main.cpp \
    CoreAction.cpp \
    SimulateStep.cpp \
    AddRequset.cpp \
    RegisterBeneficiary.cpp \
    PrintRequestStatus.cpp \
    PrintBeneficiaryStatus.cpp \
    PrintVolunteerStatus.cpp \
    PrintActionsLog.cpp \
    Close.cpp \
    BackupWareHouse.cpp \
    RestoreWareHouse.cpp \
    Beneficiary.cpp \
    HospitalBeneficiary.cpp \
    ClinicBeneficiary.cpp \
    MedicalWareHouse.cpp \
    SupplyRequest.cpp \
    Volunteer.cpp \
    InventoryManagerVolunteer.cpp \
    CourierVolunteer.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = warehouse

# Make rules
all: $(TARGET)

$(TARGET): $(OBJS)
    $(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
    $(CXX) $(CXXFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)
