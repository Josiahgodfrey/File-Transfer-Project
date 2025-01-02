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

    return 0;
}