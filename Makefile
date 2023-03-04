CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

# Source directories
SRC_DIRS := src src/http
# Object directory
OBJ_DIR := bin

# Source files
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
# Object files
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))

#SRCS = file_utils.cpp logger.cpp webserver.cpp http_request.cpp main.cpp
#OBJS = $(SRCS:.cpp=.o)
EXEC = server

all: $(EXEC)

#$(EXEC): $(OBJS)
#	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $^

# Compile object files from source files
#$(OBJ_DIR)/%.o: $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.h)) $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
#	$(CXX) $(CXXFLAGS) -c $*.cpp -o $@
# Compile object files from source files
$(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# %.o: %.cpp
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean object files and executable
clean:
	rm -f $(OUTPUT) $(OBJS)