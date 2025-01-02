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

