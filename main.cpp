#include <csignal> // for signal()
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "logger.h"
#include "webserver.h"

// Define a UDL operator for string_view literals
constexpr std::string_view operator""_sv(const char *str,
                                         std::size_t len) noexcept {
  return std::string_view(str, len);
}

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

struct prog_args {
  bool help;
  int port;
  std::string log_file;
  std::string address;
};

const std::string usage_info =
    "Usage: server [OPTIONS]\n"
    "Options:\n"
    "  --help\t\tShow this message\n"
    "  --logfile <LOG_FILE>\tSpecify a file to output logs to\n"
    "  --port <PORT_NUMBER>\tSpecify a port number\n"
    "  --addr <ADDRESS>\tSpecify an address\n"
    "  --local\t\tdefault port to 8080\n";

prog_args parse_prog_args(int argc, char *argv[]) {
  auto args = std::vector<std::string_view>(argv + 1, argv + argc);
  auto parsed_args = prog_args{};
  parsed_args.port = -1;
  // Loop through the arguments and process them
  for (auto it = args.begin(); it != args.end(); ++it) {
    if (*it == "--help"_sv) {
      parsed_args.help = true;
    } else if (*it == "--logfile"_sv && ++it != args.end()) {
      parsed_args.log_file = std::string(*it);
    } else if (*it == "--port"_sv && ++it != args.end()) {
      parsed_args.port = std::stoi(std::string(*it));
    } else if (*it == "--addr"_sv && ++it != args.end()) {
      parsed_args.address = std::string(*it);
    } else if (*it == "--local"_sv) {
      parsed_args.port = 8080;
    } else {
      std::cerr << "Unknown option: " << *it << "\n";
      return prog_args{};
    }
  }
  return parsed_args;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cout << usage_info;
    return 1;
  }

  auto args = parse_prog_args(argc, argv);
  if (args.help) {
    std::cout << usage_info;
    return 0;
  }

  if (args.port == -1) {
    std::cerr << "No port provided.";
    return 1;
  }
  // register the signal handler
  signal(SIGINT, handle_signal);

  if (!args.log_file.empty())
    log = new logger(args.log_file);
  else
    log = new logger();
  server = new webserver(args.port, args.address, log);
  server->start();
  while (server->is_running())
    ;

  return 0;
}
