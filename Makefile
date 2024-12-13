# DO NOT TOUCH IT FILE !!!

SHELL := /bin/bash
CXX := clang++

test: test_client test_server

test_client: send_test_client.cpp
	$(CXX) send_test_client.cpp -std=c++17 -g -O3 -Werror -Wall -Wextra -pthread -pedantic -o test_client
test_server: send_test_server.cpp
	$(CXX) send_test_server.cpp -std=c++17 -g -O3 -Werror -Wall -Wextra -pthread -pedantic -o test_server

