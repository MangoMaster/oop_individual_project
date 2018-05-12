SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
HEADERS=$(wildcard *.h)
TARGET=example.exe 
SRCLIB=$(wildcard ../src/*.a)
Z3LIB=$(wildcard *.lib *.dll)
INCLUDES=-I../src -I../z3/src/api/c++ -I../z3/src/api

all: $(TARGET)

$(TARGET): $(OBJECTS) $(SRCLIB) $(Z3LIB) $(HEADERS)
	g++ $(OBJECTS) $(SRCLIB) $(Z3LIB) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	g++ $< -c  $(INCLUDES) -o $@

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