#include <iostream>
#include <string>
#include <span>
#include <string_view>
#include <cstdio>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int BUFFER_SIZE = 1024;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "ERROR, no port provided\n";
        return 1;
    }

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

    if (bind(sockfd, reinterpret_cast<const sockaddr*>(&serv_addr), sizeof(serv_addr)) < 0) {
        std::cerr << "ERROR on binding\n";
        return 1;
    }

    if (listen(sockfd, 5) < 0) {
        std::cerr << "ERROR on listening\n";
        return 1;
    }

    while (true) {
        sockaddr_in cli_addr{};
        socklen_t clilen = sizeof(cli_addr);
        const int newsockfd = accept(sockfd, reinterpret_cast<sockaddr*>(&cli_addr), &clilen);

        if (newsockfd < 0) {
            std::cerr << "ERROR on accept\n";
            continue;
        }

        std::string buffer(BUFFER_SIZE, '\0');
        const auto bytes_read = read(newsockfd, buffer.data(), buffer.size() - 1);

        if (bytes_read < 0) {
            std::cerr << "ERROR reading from socket\n";
            close(newsockfd);
            continue;
        }

        std::string_view message(buffer.data(), bytes_read);
        std::cout << "Here is the message:\n" << message << '\n';

        const auto bytes_written = write(newsockfd, message.data(), message.size());

        if (bytes_written < 0) {
            std::cerr << "ERROR writing to socket\n";
        }

        close(newsockfd);
    }

    close(sockfd);
    return 0;
}
