# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -g -Wall -Weffc++ -std=c++11 -Iinclude

all: clean compile link


compile:
	${CXX} ${CXXFLAGS} -c -o ./bin/main.o ./src/main.cpp
	${CXX} ${CXXFLAGS} -c -o ./bin/Beneficiary.o ./src/Beneficiary.cpp
	${CXX} ${CXXFLAGS} -c -o ./bin/Volunteer.o ./src/Volunteer.cpp
	${CXX} ${CXXFLAGS} -c -o ./bin/SupplyRequest.o ./src/SupplyRequest.cpp
	${CXX} ${CXXFLAGS} -c -o ./bin/MedicalWareHouse.o ./src/MedicalWareHouse.cpp
	${CXX} ${CXXFLAGS} -c -o ./bin/Action.o ./src/Action.cpp

link:
	g++ -o ./bin/*.o

clean:
	rm -f ./bin/*
