SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
HEADERS=$(wildcard *.h)
TARGET=testcase.exe 
SRCLIB=$(wildcard ../src/*.a)
Z3LIB=$(wildcard *.lib)
INCLUDES=-I../src -I../z3/src/api/c++ -I../z3/src/api

all: $(TARGET)

$(TARGET): $(OBJECTS) main.o $(SRCLIB) $(Z3LIB) $(HEADERS)
	g++ $(OBJECTS) main.o $(SRCLIB) $(Z3LIB) -o $(TARGET)

main.o: main.cxx $(HEADERS) $(SRCLIB) $(Z3LIB)
	g++ $< -c $(INCLUDES) -w -std=c++11 -o $@

%.o: %.cpp $(HEADERS) $(SRCLIB) $(Z3LIB)
	g++ $< -c $(INCLUDES) -w -std=c++11 -o $@

.PHONY: clean
clean: 
	del *.exe *.o

.PHONY: explain
explain:
	@echo "sources: $(SOURCES)"
	@echo "objects: $(OBJECTS)"
	@echo "headers: $(HEADERS)"
	@echo "srclib: $(SRCLIB)"
	@echo "z3lib: $(Z3LIB)"
	@echo "target: $(TARGET)"