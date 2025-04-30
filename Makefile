CXX = g++
CXXFLAGS = -std=c++11
TARGET = game

# Source files
SRCS = Item.cpp guide.cpp main.cpp quiz_challenge.cpp shop_menu.cpp invincibility.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# type: make clean
# to clean up all generated files and executable
clean:
	rm -f $(OBJS) $(TARGET)
