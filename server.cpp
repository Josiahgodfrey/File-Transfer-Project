#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <filesystem>
using namespace std;
namespace fs = filesystem;

#define PORT 8510
#define BUFFER_SIZE 1024

void print_usage() {
    cout << "usage: ./server <port>" << endl;
}

int main(int argc, char *argv[]) {
    int port = 8510;    //default port

    if(argc == 2) {
        port = stoi(argv[1]);
    }

    if(argc != 2) {
        print_usage();
        return 1;
    }

    port = stoi(argv[1]);

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 11;
    int addressLen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};


    return 0;
}