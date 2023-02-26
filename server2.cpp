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

#include "http_request.h"

// declare a global flag to indicate whether the server should exit
std::atomic_bool should_exit(false);

// declare a signal handler function to catch SIGINT
void handle_signal(int signum) {
  if (signum == SIGINT) {
    std::cout << "Received SIGINT, shutting down...\n";
    should_exit.store(true);
  }
}

const int BUFFER_SIZE = 1024;

// Function to load the contents of a file into a string
std::string load_file(const std::filesystem::path &path) {
  std::ifstream file(path);
  if (!file) {
    throw std::runtime_error("Failed to open file: " + path.string());
  }
  return std::string(std::istreambuf_iterator<char>(file), {});
}

void handle_connection(int newsockfd) {
  std::string buffer(BUFFER_SIZE, '\0');
  const auto bytes_read = read(newsockfd, buffer.data(), buffer.size() - 1);

  if (bytes_read < 0) {
    std::cerr << "ERROR reading from socket\n";
    close(newsockfd);
    return;
  }

  std::string_view message(buffer.data(), bytes_read);
  std::cout << "Here is the message:\n" << message << '\n';

  // Check if the request is for the root path ("/")
  if (message.find("GET / HTTP/1.1") != std::string_view::npos) {
    const auto response_body = load_file("index.html");

    // Construct the HTTP response
    std::string response = "HTTP/1.1 200 OK\r\n"
                           "Content-Length: " +
                           std::to_string(response_body.size()) +
                           "\r\n"
                           "Content-Type: text/html\r\n"
                           "\r\n" +
                           response_body;

    // Send the response to the client
    const auto bytes_written =
        write(newsockfd, response.data(), response.size());

    if (bytes_written < 0) {
      std::cerr << "ERROR writing to socket\n";
    }
  } else {
    // Construct a 404 Not Found response
    const auto response_body = load_file("404.html");
    const std::string not_found = "HTTP/1.1 404 Not Found\r\n"
                                  "Content-Length: " +
                                  std::to_string(response_body.size()) +
                                  "\r\n"
                                  "Content-Type: text/html\r\n"
                                  "\r\n" +
                                  response_body;

    const auto bytes_written =
        write(newsockfd, not_found.data(), not_found.size());

    if (bytes_written < 0) {
      std::cerr << "ERROR writing to socket\n";
    } else {
      std::cout << "Response sent successfully\n";
    }
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

  const int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    std::cerr << "ERROR opening socket\n";
    return 1;
  }

  sockaddr_in serv_addr{};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, reinterpret_cast<const sockaddr *>(&serv_addr),
           sizeof(serv_addr)) < 0) {
    std::cerr << "ERROR on binding\n";
    return 1;
  }

  if (listen(sockfd, 5) < 0) {
    std::cerr << "ERROR on listening\n";
    return 1;
  }

  while (!should_exit.load()) {
    sockaddr_in cli_addr{};
    socklen_t clilen = sizeof(cli_addr);
    const int newsockfd =
        accept(sockfd, reinterpret_cast<sockaddr *>(&cli_addr), &clilen);

    if (newsockfd < 0) {
      std::cerr << "ERROR on accept\n";
      continue;
    }

    handle_connection(newsockfd);

    close(newsockfd);
  }

  close(sockfd);
  return 0;
}
