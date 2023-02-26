#include <atomic>  // for std::atomic_bool
#include <csignal> // for signal()
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"
#include "webserver.h"

// global logger
logger *log;

// global webserver
webserver *server;

// declare a global flag to indicate whether the server should exit
std::atomic_bool should_exit(false);

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
