#pragma once
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

class http_request;
static const std::string LOG_INFO_COLOR = "\033[0;36m[INFO]\033[m";
static const std::string LOG_ERR_COLOR = "\033[0;31m[ERROR]\033[m";
static const std::string LOG_INFO = "[INFO]";
static const std::string LOG_ERR = "[ERROR]";
class logger {
private:
  std::ostream *outstream;
  bool is_stdio;
  const std::string info_prefix;
  const std::string error_prefix;

public:
  /*
   * Initializes a logger with default output stream of cout.
   */
  logger()
      : outstream(&std::cout), is_stdio(true), info_prefix(LOG_INFO_COLOR),
        error_prefix(LOG_ERR_COLOR) {}

  /*
   * Initializes a logger with output to file "filename".
   */
  logger(std::string filename)
      : outstream(new std::ofstream(filename)), is_stdio(false),
        info_prefix(LOG_INFO), error_prefix(LOG_ERR) {}

  ~logger();

  /*
   * Logs string as info.
   */
  void log_info(std::string info);

  /*
   * Logs data pertaining to an http request.
   */
  void log_http_request(http_request *request);

  /*
   * Logs string as an error.
   */
  void log_err(std::string err);
};