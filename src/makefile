SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
HEADERS=$(wildcard *.h)
LIB=src.a
TARGET=DFMBSolver.exe 
Z3LIB=$(wildcard ../z3/build/*.lib ../z3/build/*.dll)
INCLUDES=-I../z3/src/api/c++ -I../z3/src/api

all: $(TARGET)

$(TARGET): $(OBJECTS) $(Z3LIB) $(HEADERS)
	g++ $(OBJECTS) $(Z3LIB) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	g++ $< -c  $(INCLUDES) -o $@

$(LIB): $(OBJECTS) $(HEADERS)
	ar -rv $(LIB) $(OBJECTS)
	ranlib $(LIB)

.PHONY: lib
lib: $(LIB)

.PHONY: clean
clean: 
	del *.exe *.o *.a

.PHONY: explain
explain:
	@echo "sources: $(SOURCES)"
	@echo "objects: $(OBJECTS)"
	@echo "headers: $(HEADERS)"
	@echo "z3lib: $(Z3LIB)"
	@echo "lib: $(LIB)"
	@echo "target: $(TARGET)"