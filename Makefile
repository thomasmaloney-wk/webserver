CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRCS = file_utils.cpp logger.cpp http_response.cpp /controllers/index_controller.cpp webserver.cpp http_request.cpp main.cpp
#OBJS = $(SRCS:.cpp=.o)
SRCS_NODIR = $(notdir $(SRCS))
OBJS = $(SRCS:.cpp=.o)
#OBJS = $(SRCS_NODIR:.cpp=.o)
#$(patsubst %.cpp,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $(notdir $<) -o $(notdir $@)

clean:
	rm -f $(OBJS) $(EXEC)