
#define project name
TARGET              =bin/app.app
#define compiler
CXX                 =g++
#define warn level
WARN                =
#defile optimisation flags
OFLAGS              =-MMD
#defile aditional libraries flags
LDFLAGS             =-lboost_system -lboost_iostreams
#define compilation flags
CFLAGS              =-Wall -O0 -ggdb -std=c++11

#define sources. all files with .cpp extansions are assumed to be an source.
#Sources are looked for in folder "sources" and in its subfolders
SOURCES:=$(wildcard sources/*.cpp ) $(wildcard sources/*/*.cpp)
#defile headers with the same way.
#headers are looked for in filders "headers" and its subfolders
HEADERS:=$(wildcard headers/*.hpp) $(wildcard headers/*/*.hpp)
#objects files have a name same as their appropriate .cpp file.
#They would be placed in "objects" folder. Folder hierarchy would be kept.
OBJECTS:=$(subst sources/,objects/,$(SOURCES:.cpp=.o))

.PHONY: all
all: $(TARGET)

projectDetails:
	@echo HEADERS are: $(HEADERS)
	@echo SOURCES are: $(SOURCES)
	@echo OBJECTS are: $(OBJECTS)


$(TARGET): $(OBJECTS)
	@echo //////////////  Project Linking \\\\\\\\\\\\\\\\\\\\
	$(CXX) -o $@ $^ $(LDFLAGS)

objects/%.o: sources/%.cpp $(HEADERS)
	$(CXX) $(OFLAGS) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f objects/*.o objects/*.d objects/*/*.o objects/*/*.d bin/app
rclean:
	rm -f results/tdat/*/*.tdat results/png/*/*.png results/bindat/*/*.bin
