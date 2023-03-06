#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include "controllers/about_controller.h"
#include "controllers/controller.h"
#include "controllers/index_controller.h"
#include "file_utils.h"
#include "http/http_request.h"
#include "http/http_response.h"
#include "logger.h"
#include "routing/route_handler.h"
#include "webserver.h"

void webserver::start() {
  if (running) {
    std::cerr << "Webserver is already running." << std::endl;
    return;
  }
  running = true;
  server_thread = std::thread(&webserver::run, this);
}

void webserver::stop() {
  if (!running) {
    std::cerr << "Webserver not running." << std::endl;
    return;
  }
  running = false;
  server_thread.join();
}

void webserver::handle_connection(int client_socket) {
  auto message = receive_message(client_socket);
  if (message.empty())
    return;

  auto httpRequest = http_request::parse_request(message);
  log->log_http_request(httpRequest);

  http_response *response = router->handle_request(httpRequest);

  send_message(client_socket, response->to_string());
  if (response != nullptr &&
      response->headers.contains(http_response::SHUTDOWN_HEADER_KEY)) {
    stop();
  }
}

void webserver::send_message(int client_socket, std::string message) {
  const auto bytes_written =
      write(client_socket, message.data(), message.size());

  if (bytes_written < 0) {
    log->log_err("writing to socket");
  } else {
    log->log_info("Response sent successfully");
  }
}

std::string_view webserver::receive_message(int client_socket) {
  std::string buffer(BUFFER_SIZE, '\0');
  const auto bytes_read = read(client_socket, buffer.data(), buffer.size() - 1);

  if (bytes_read < 0) {
    log->log_err("Reading from socket failed.");
    close(client_socket);
    return std::string_view{};
  }

  std::string_view message(buffer.data(), bytes_read);
  return message;
}

void webserver::run() {
  log->log_info("Initializing webserver...");
  const int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket < 0) {
    log->log_err("Opening socket failed");
    return;
  }

  log->log_info("Opened socket.");

  auto inaddr = address.empty() ? INADDR_ANY : inet_addr(address.c_str());
  serv_addr = {};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inaddr;
  serv_addr.sin_port = htons(portno);

  if (bind(server_socket, reinterpret_cast<const sockaddr *>(&serv_addr),
           sizeof(serv_addr)) < 0) {
    log->log_err("Binding failed. " + std::string(std::strerror(errno)));
    return;
  }

  log->log_info("Binding successful.");

  if (listen(server_socket, 5) < 0) {
    log->log_err("Listening failed.");
    return;
  }

  log->log_info("Listening successful.");

  log->log_info("Initializing router...");
  router = route_handler::create_route_handler();
  log->log_info("Router init successful");

  while (running) {
    sockaddr_in cli_addr{};
    socklen_t clilen = sizeof(cli_addr);
    const int client_socket =
        accept(server_socket, reinterpret_cast<sockaddr *>(&cli_addr), &clilen);

    if (client_socket < 0) {
      log->log_err("Unable to accept.");
      continue;
    }

    handle_connection(client_socket);

    close(client_socket);
  }
  close(server_socket);
}

void webserver::kill() { stop(); }

bool webserver::is_running() { return running; }
