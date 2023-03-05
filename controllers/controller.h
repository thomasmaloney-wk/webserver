#pragma once
#include <string>
class http_request;
class http_response;

class controller {
protected:
  const std::string _controller_root;
  controller(std::string root) : _controller_root(root){}

public:
  virtual http_response *handle_request(http_request *request) = 0;
};