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
#include "webserver.h"

std::string create_html_response_from_string(std::string response_body) {
  // Construct the HTTP response
  auto response =
      http_response(http_response_code::OK, response_body, "text/html");
  return response.to_string();
}

// Route handlers

std::string shutdown_route_handler(const http_request &request) {
  const auto response_body = load_file("web/shutdown.html");
  const std::string response = create_html_response_from_string(response_body);
  return response;
}

std::string not_found_route_handler(const http_request &request) {
  const auto response_body = load_file("web/404.html");
  const auto not_found_response = http_response::not_found(response_body);
  return not_found_response->to_string();
}

std::string echo_route_handler(const http_request &request) {
  auto url = request.url;
  auto route = url.substr(5);
  const auto response_body = "<html><head/><body>" + route + "</body></html>";
  const std::string response = create_html_response_from_string(response_body);
  return response;
}

// Webserver method implementations

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

  auto httpRequest = parse_request(message);
  log->log_http_request(httpRequest);

  // find a matching route and call the handler
  route _route = {std::regex("/404"), not_found_route_handler};
  for (const auto &route : routes) {
    if (std::regex_match(httpRequest.url, route.pattern)) {
      _route = route;
      break;
    }
  }

  std::string response =
      _route.controller_handler != nullptr
          ? _route.controller_handler->handle_request(&httpRequest)->to_string()
          : _route.handler(httpRequest);

  send_message(client_socket, response);
  if (httpRequest.url == "/Shutdown") {
    stop();
  }
}

void webserver::send_message(int client_socket, std::string message) {
  const auto bytes_written =
      write(client_socket, message.data(), message.size());

  if (bytes_written < 0) {
    log->log_err(LOG_ERR + " writing to socket\n");
  } else {
    log->log_info(LOG_INFO + " Response sent successfully\n");
  }
}

std::string_view webserver::receive_message(int client_socket) {
  std::string buffer(BUFFER_SIZE, '\0');
  const auto bytes_read = read(client_socket, buffer.data(), buffer.size() - 1);

  if (bytes_read < 0) {
    log->log_err(LOG_ERR + " Reading from socket failed.\n");
    close(client_socket);
    return std::string_view{};
  }

  std::string_view message(buffer.data(), bytes_read);
  return message;
}

void webserver::add_route(
    const std::string &pattern,
    std::function<std::string(const http_request &)> handler) {
  routes.push_back({std::regex(pattern), handler});
}

void webserver::add_route(const std::string &pattern, controller *handler) {
  routes.push_back({std::regex(pattern), nullptr, handler});
}

void webserver::run() {
  log->log_info(LOG_INFO + " Initializing webserver...\n");
  const int server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (server_socket < 0) {
    log->log_err(LOG_ERR + " Opening socket failed\n");
    return;
  }

  log->log_info(LOG_INFO + " Opened socket.\n");

  auto inaddr = address.empty() ? INADDR_ANY : inet_addr(address.c_str());
  serv_addr = {};
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inaddr;
  serv_addr.sin_port = htons(portno);

  if (bind(server_socket, reinterpret_cast<const sockaddr *>(&serv_addr),
           sizeof(serv_addr)) < 0) {
    log->log_err(LOG_ERR + " Binding failed. ");
    log->log_err(std::strerror(errno));
    return;
  }

  log->log_info(LOG_INFO + " Binding successful.\n");

  if (listen(server_socket, 5) < 0) {
    log->log_err(LOG_ERR + " Listening failed.\n");
    return;
  }

  log->log_info(LOG_INFO + " Listening successful.\n");

  // initialize routes
  add_route("/", new index_controller());
  add_route("/About", new about_controller());
  add_route("/Shutdown", shutdown_route_handler);
  add_route("/Echo/.*", echo_route_handler);

  while (running) {
    sockaddr_in cli_addr{};
    socklen_t clilen = sizeof(cli_addr);
    const int client_socket =
        accept(server_socket, reinterpret_cast<sockaddr *>(&cli_addr), &clilen);

    if (client_socket < 0) {
      log->log_err(LOG_ERR + " Unable to accept.\n");
      continue;
    }

    handle_connection(client_socket);

    close(client_socket);
  }
  close(server_socket);
}

void webserver::kill() { stop(); }

bool webserver::is_running() { return running; }
