#pragma once
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

struct http_request;
static const std::string LOG_INFO = "\033[0;36m[INFO]\033[m";
static const std::string LOG_ERR = "\033[0;31m[ERROR]\033[m";
class logger {
private:
  std::ostream *outstream;
  bool is_stdio;

public:
  /*
   * Initializes a logger with default output stream of cout.
   */
  logger() : outstream(&std::cout), is_stdio(true) {}

  /*
   * Initializes a logger with output to file "filename".
   */
  logger(std::string filename)
      : outstream(new std::ofstream(filename)), is_stdio(false) {}

  ~logger();

  /*
   * Logs string as info.
   */
  void log_info(std::string info);

  /*
   * Logs data pertaining to an http request.
   */
  void log_http_request(http_request request);

  /*
   * Logs string as an error.
   */
  void log_err(std::string err);
};