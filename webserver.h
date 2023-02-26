#pragma once
#include <netinet/in.h>

class logger; // forward declaration

class webserver {
private:
  bool should_exit;
  const int BUFFER_SIZE = 1024;
  sockaddr_in serv_addr;
  int portno;
  logger *log;

  /*
   * Handles an incoming connection.
   */
  void handle_connection(int newsockfd);

public:
  webserver(int port, logger *logger) : portno(port), log(logger) {}

  /*
   * Run the webserver.
   */
  void run();

  /*
   * Stops the server.
   */
  void kill();
};