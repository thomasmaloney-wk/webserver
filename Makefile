CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
CONTROLLER_SRC = /controllers/index_controller.cpp /controllers/about_controller.cpp
SRCS = file_utils.cpp logger.cpp http_response.cpp \
		$(CONTROLLER_SRC) \
		webserver.cpp http_request.cpp main.cpp
OBJ_DIR = obj
SRCS_NODIR := $(notdir $(SRCS))
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS_NODIR))
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: controllers/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: serve-local
serve-local: $(MAKE)
	./server --local