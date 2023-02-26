#pragma once
#include <netinet/in.h>

class logger; // forward declaration

class webserver {
private:
  bool should_exit;
  const int BUFFER_SIZE = 1024;
  sockaddr_in serv_addr;
  logger *log;
  int portno;

public:
  webserver(int port, logger *logger) : portno(port), log(logger) {}

  /*
   * Initialize the webserver.
   */
  void init();

  /*
   * Run the webserver.
   */
  void run();
};