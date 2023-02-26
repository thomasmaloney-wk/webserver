#pragma once
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>

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
   * Logs string as an error.
   */
  void log_err(std::string err);
};