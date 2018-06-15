SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
HEADERS=$(wildcard *.h)
LIB=DFMBsrc.a
Z3LIB=$(wildcard *.lib)
INCLUDES=-I../z3/src/api/c++ -I../z3/src/api

all: $(LIB)

%.o: %.cpp $(HEADERS)
	g++ $< -c  $(INCLUDES) -std=c++11 -o $@

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
	@echo "target: $(LIB)"