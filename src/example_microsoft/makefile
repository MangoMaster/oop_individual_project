MYSOURCES=$(wildcard *.cpp)
MYOBJECTS=$(MYSOURCES:%.cpp=%.o)
HEADERS=$(wildcard *.h)
LIB=$(wildcard *.dll)
INCLUDES=-I../z3/src/api/c++ -I../z3/src/api
TARGET=example.exe 

all: $(TARGET)

$(TARGET): $(MYOBJECTS) $(LIB) $(HEADERS)
	g++ $(MYOBJECTS) $(LIB) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	g++ $< -c -std=c++11 $(INCLUDES) -o $@

.PHONY: clean
clean: 
	del *.exe *.o

.PHONY: explain
explain:
	@echo "sources: $(MYSOURCES)"
	@echo "objects: $(MYOBJECTS)"
	@echo "headers: $(HEADERS)"
	@echo "lib: $(LIB)"
	@echo "target: $(TARGET)"