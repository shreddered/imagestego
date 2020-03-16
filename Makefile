.LIBPATTERNS=
VPATH=src

CXX=g++
CPPFLAGS=-fPIC -Wall --std=c++11 -Iinclude/ -I/usr/include/opencv4
LDFLAGS=-shared
LIBS= #TODO: add opencv here
OPENCV_LIBS=-lopencv_core -lopencv_imgcodecs
OPENCV_CPPFLAGS=-I/usr/include/opencv4

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=lib/%.o)

TARGET=
#example
target_example_SRC=examples/example.cpp
target_example_OUTPUT=

OUTPUT=

ifdef ($(OS))
	OUTPUT=lib/libimagestego.dll
	RM=rd
	target_example_OUTPUT+=bin/example.exe
else
	RM=rm
	target_example_OUTPUT=bin/example
	ifeq ($(shell uname -s), Darwin)
		OUTPUT=lib/libimagestego.dylib
	else
		OUTPUT=lib/libimagestego.so
	endif
endif

$(OUTPUT): libimagestego
	$(RM) $(OBJECTS)

libimagestego: $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OPENCV_LIBS) -o $(OUTPUT) $(OBJECTS)

lib/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

cleanup: $(OBJECTS)
	$(RM) $(OBJECTS)

example: TARGET=target_example

example: $(OUTPUT)
	$(CXX) -Iinclude/ $(OPENCV_CPPFLAGS) -Llib/ -limagestego $(OPENCV_LIBS) -D_DEBUG $($(TARGET)_SRC) -o $($(TARGET)_OUTPUT)
