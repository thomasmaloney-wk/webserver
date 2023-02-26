#include <sys/socket.h>
#include <unistd.h>

#include "file_utils.h"
#include "logger.h"
#include "webserver.h"
#include "http_request.h"

void webserver::handle_connection(int newsockfd) {
  std::string buffer(BUFFER_SIZE, '\0');
  const auto bytes_read = read(newsockfd, buffer.data(), buffer.size() - 1);

  if (bytes_read < 0) {
    log->log_err("ERROR reading from socket\n");
    close(newsockfd);
    return;
  }

  std::string_view message(buffer.data(), bytes_read);

  auto httpRequest = parse_request(message);
  log->log_info("[INFO] Requested Method: ");
  log->log_info(httpRequest.method);
  log->log_info("\n");
  log->log_info("[INFO] Requested URL: ");
  log->log_info(httpRequest.url);
  log->log_info("\n");
  
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
      log->log_err("ERROR writing to socket\n");
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
      log->log_err("ERROR writing to socket\n");
    } else {
      log->log_info("Response sent successfully\n");
    }
  }
}

void webserver::run() {
  log->log_info("[INFO] Initializing webserver...\n");
  const int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    log->log_err("ERROR opening socket\n");
    return;
  }

  log->log_info("[INFO] Opened socket.\n");

  serv_addr = {};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if (bind(sockfd, reinterpret_cast<const sockaddr *>(&serv_addr),
           sizeof(serv_addr)) < 0) {
    log->log_err("ERROR on binding\n");
    return;
  }

  log->log_info("[INFO] Binding successful.\n");

  if (listen(sockfd, 5) < 0) {
    log->log_err("ERROR on listening\n");
    return;
  }

  log->log_info("[INFO] Listening successful.\n");

  while (!should_exit) {
    sockaddr_in cli_addr{};
    socklen_t clilen = sizeof(cli_addr);
    const int newsockfd =
        accept(sockfd, reinterpret_cast<sockaddr *>(&cli_addr), &clilen);

    if (newsockfd < 0) {
      log->log_err("ERROR on accept\n");
      continue;
    }

    handle_connection(newsockfd);

    close(newsockfd);
  }
  close(sockfd);
}

void webserver::kill() { should_exit = true; }
