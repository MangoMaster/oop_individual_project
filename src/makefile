MYSOURCES=$(wildcard *.cpp)
MYOBJECTS=$(MYSOURCES:%.cpp=%.o)
HEADERS=$(wildcard *.h)
LIB=$(wildcard *.lib)
INCLUDES=-Iz3/src/api/c++ -Iz3/src/api
TARGET=DFMBSolver.exe 

all: $(TARGET)

$(TARGET): $(MYOBJECTS) $(LIB) $(HEADERS)
	g++ $(MYOBJECTS) $(LIB) -o $(TARGET)

%.o: %.cpp $(HEADERS)
	g++ $< -c  $(INCLUDES) -o $@

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