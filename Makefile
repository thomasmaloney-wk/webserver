CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic

SRCS = file_utils.cpp logger.cpp webserver.cpp http_request.cpp server.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)