#include <csignal> // for signal()
#include <iostream>
#include <string>
#include <string_view>

#include "logger.h"
#include "webserver.h"

// global logger
logger *log;

// global webserver
webserver *server;

// declare a signal handler function to catch SIGINT
void handle_signal(int signum) {
  if (signum == SIGINT) {
    std::cout << "Received SIGINT, shutting down...\n";
    server->kill();
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "ERROR, no port provided\n";
    return 1;
  }
  // register the signal handler
  signal(SIGINT, handle_signal);

  const int portno = std::stoi(argv[1]);

  if (argc == 3)
    log = new logger(std::string(argv[2]));
  else
    log = new logger();
  server = new webserver(portno, log);
  server->run();

  return 0;
}
