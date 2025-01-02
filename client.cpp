#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <limits>
#include <filesystem>
#include <openssl/sha.h> // For file integrity check
using namespace std;
namespace fs = filesystem;

#define PORT 8511
#define BUFFER_SIZE 1024
#define MAX_FILE_SIZE 10000000 // Maximum file size limit (10 MB)

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cerr << "Socket creation error: " << strerror(errno) << endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    return 0;
}