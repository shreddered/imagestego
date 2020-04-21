.LIBPATTERNS=
VPATH=src

CXX=g++
CPPFLAGS=-ggdb -fPIC -Wall --std=c++11 -Iinclude/ -I/usr/include/opencv4
LDFLAGS=-shared
LIBS=
OPENCV_LIBS=-lopencv_core -lopencv_imgcodecs
OPENCV_CPPFLAGS=-I/usr/include/opencv4

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=lib/%.o)
OUT_DIRS=bin/ lib/

TARGET=
#example
target_example_SRC=examples/example.cpp
target_example_OUTPUT=
#lsb
target_lsb_SRC=examples/lsb.cpp
target_lsb_OUTPUT=
#huffman
target_huffman_SRC=examples/huffman.cpp
target_huffman_OUTPUT=
#archived
target_archived_SRC=examples/archived_data_stego.cpp
target_archived_OUTPUT=bin/stego

OUTPUT=

ifdef ($(OS))
	OUTPUT=lib/libimagestego.dll
	RM=rd
	target_example_OUTPUT+=bin/example.exe
	target_huffman_OUTPUT=bin/huffman.exe
	target_lsb_OUTPUT=bin/lsb.exe
else
	RM=rm
	target_example_OUTPUT=bin/example
	target_huffman_OUTPUT=bin/huffman
	target_lsb_OUTPUT=bin/lsb
	ifeq ($(shell uname -s), Darwin)
		OUTPUT=lib/libimagestego.dylib
	else
		OUTPUT=lib/libimagestego.so
	endif
endif

.PHONY: directories clean

$(OUTPUT): libimagestego
	$(RM) $(OBJECTS)

libimagestego: directories $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OPENCV_LIBS) -o $(OUTPUT) $(OBJECTS)

lib/%.o: %.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

directories: ${OUT_DIRS}

$(OUT_DIRS):
	@mkdir -p ${OUT_DIRS} 2> /dev/null

clean: $(OBJECTS)
	$(RM) $(OBJECTS)

example: TARGET=target_example

example: 
	$(CXX) -Iinclude/ $(OPENCV_CPPFLAGS) -Llib/ -limagestego $(OPENCV_LIBS) -D_DEBUG $($(TARGET)_SRC) -o $($(TARGET)_OUTPUT)

lsb: TARGET=target_lsb

lsb: 
	$(CXX) -ggdb -Iinclude/ $(OPENCV_CPPFLAGS) -Llib/ -limagestego $(OPENCV_LIBS) -D_DEBUG $($(TARGET)_SRC) -o $($(TARGET)_OUTPUT)

huffman: TARGET=target_huffman

huffman: 
	$(CXX) -ggdb -Iinclude/ $(OPENCV_CPPFLAGS) -Llib/ -limagestego $(OPENCV_LIBS) -D_DEBUG $($(TARGET)_SRC) -o $($(TARGET)_OUTPUT)

archived: TARGET=target_archived

archived:
	$(CXX) -ggdb -Iinclude/ $(OPENCV_CPPFLAGS) -Llib/ -limagestego $(OPENCV_LIBS) -D_DEBUG $($(TARGET)_SRC) -o $($(TARGET)_OUTPUT)
