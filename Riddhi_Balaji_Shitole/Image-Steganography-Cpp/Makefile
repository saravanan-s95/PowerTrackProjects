CXX = g++
CXXFLAGS = -Wall -std=c++11

TARGET = stego

SRCS = main.cpp image.cpp encoder.cpp decoder.cpp utility.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	g++ $(OBJS) -o $(TARGET)

%.o: %.cpp
	g++ $(CXXFLAGS) -c $<

clean:
	rm -f *.o $(TARGET)
