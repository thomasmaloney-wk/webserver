CXX = clang++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
HTTP_SRC = /http/http_response.cpp /http/http_request.cpp
CONTROLLER_SRC = /controllers/index_controller.cpp /controllers/about_controller.cpp
SRCS = file_utils.cpp logger.cpp \
		$(HTTP_SRC) \
		$(CONTROLLER_SRC) \
		webserver.cpp main.cpp
OBJ_DIR = obj
SRCS_NODIR := $(notdir $(SRCS))
OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS_NODIR))
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJS)
	@echo "Linking $(EXEC)"
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

$(OBJ_DIR)/%.o: %.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: controllers/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: http/%.cpp
	@echo "Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo "Removing build objects"
	@rm -f $(OBJS) $(EXEC)

.PHONY: serve-local
serve-local: $(MAKE)
	./server --local