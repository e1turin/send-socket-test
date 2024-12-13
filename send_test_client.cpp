#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <iostream>
#include <memory>


#define log (std::cout << __LINE__ << ": ")

int open_socket(std::string address, std::string port) {
  int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
  if (sockfd < 0) {
    log << "socket error" << std::endl;
    return -1;
  }

  addrinfo hints = {};
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_V4MAPPED;

  addrinfo *res;
  if (getaddrinfo(address.c_str(), port.c_str(), &hints, &res) < 0) {
    log << "getaddrinfo error: " << strerror(errno) << std::endl;
    return -1;
  }
  auto ai_defer =
      std::unique_ptr<addrinfo, decltype(&freeaddrinfo)>(res, &freeaddrinfo);

  if (connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    log << "connect error: " << strerror(errno) << std::endl;
    return -1;
  }

  return sockfd;
}

int main(int argc, char *argv[]) {
  (void)argc;
  (void)argv;

  std::string address = "localhost";
  std::string port = "3000";
  std::string input;

  int sockfd = open_socket(address, port);
  if (sockfd < 0) {
    log << "open socket error" << std::endl;
    exit(errno);
  }
  signal(SIGPIPE, [](int sig) { log << "SIGPIPE! " << sig << std::endl; });

  int recvsize;
  char *recvbuf;
  ssize_t errc;

  std::cout << "type ? for help" << std::endl;
  while (true) {
    std::cout << "> ";
    std::cin >> input;
    if (input[0] == '?') {
      std::cout << "\tc = close read & write\n"
                << "\tcr = close read\n"
                << "\tcw = close write\n"
                << "\tx = close & exit\n"
                << "\tsn {str} = send string {str} nonblocking\n"
                << "\tsb {n} = send big string with size {n}\n"
                << "\tsnb {n} = send big string with size {n} nonblocking\n"
                << "\tr {n} = receive {n} bytes\n"
                << "\trn {n} = receive {n} bytes nonblocking\n"
                << std::endl;
      continue;
    }
    if (input[0] == 'x') {
      break;
    }
    if (input[0] == 'c') {
      if (input == "c") {
        shutdown(sockfd, SHUT_RDWR);
      } else if (input == "cr") {
        shutdown(sockfd, SHUT_RD);
      } else if (input == "cw") {
        shutdown(sockfd, SHUT_WR);
      }
      log << "close socket" << std::endl;
      continue;
    }

    if (input[0] == 's') {
      int mode = 0;
      if (input == "sn") {
        mode = MSG_DONTWAIT;
        std::cin >> input;
      } else if (input == "sb") {
        std::cin >> input;
        input.resize(std::stoi(input));
      } else if (input == "snb") {
        mode = MSG_DONTWAIT;
        std::cin >> input;
        input.resize(std::stoi(input));
      } else {
        std::cin >> input;
      }

      log << "send..." << std::endl;
      errc = send(sockfd, input.c_str(), input.size(), mode);

      if (errc == 0) {
        log << "send returned 0" << std::endl;
        perror("error?");
      } else if (errc < 0) {
        log << "send returned error " << errc << std::endl;
        perror("error?");
      } else {
        log << "send returned " << errc << std::endl;
        perror("error?");
      }

      input.clear();
      input.shrink_to_fit();
    } else if (input[0] == 'r') {
      int mode = 0;
      if (input == "rn") {
        mode = MSG_DONTWAIT;
      }

      std::cin >> input;

      recvsize = std::stoi(input);
      recvbuf = new char[recvsize + 1]{};

      log << "recv..." << std::endl;
      errc = recv(sockfd, recvbuf, recvsize, mode);

      if (errc == 0) {
        log << "recv returned 0" << std::endl;
        perror("error?");
      } else if (errc < 0) {
        log << "recv returned error" << std::endl;
        perror("error?");
      } else {
        log << "recv returned " << errc << ", res: " << std::string(recvbuf)
            << std::endl;
      }

      delete[] recvbuf;
    }
  }

  shutdown(sockfd, SHUT_RDWR);

  return 0;
}