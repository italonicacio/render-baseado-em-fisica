EXECUTABLE = swpathtracer

IMAGE = output_image

CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra -Wpedantic -fopenmp -DDEBUG -O0 -g3

# ifeq ($(DEBUG), 1)
# CXXFLAGS += -DDEBUG -O0 -g3
# else
# CXXFLAGS += -DNDEBUG -O3 -g0
# endif

CPPFLAGS += -MMD \
			-I../../../../work/common/glm-0.9.7.0 \
			-I/usr/local/include

LDFLAGS = -L/usr/lib/gcc/x86_64-linux-gnu/4.8 \
		  -L/usr/lib/x86_64-linux-gnu \
		  -L/usr/lib \
	      -L/usr/local/lib

LDLIBS = -lassimp -fopenmp

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDLIBS)

.cpp.o:

-include $(OBJECTS:.o=.d)

.PHONY: clean distclean run png clpng

clean:
	rm *.o *.d

distclean: clean
	rm $(EXECUTABLE)

run: swpathtracer
	./swpathtracer

png:
	convert $(IMAGE).ppm $(IMAGE).png

clpng: $(IMAGE).png
	rm $(IMAGE).png



