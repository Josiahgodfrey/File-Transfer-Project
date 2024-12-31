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

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported: " << strerror(errno) << endl;
        return -1;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "Connection failed: " << strerror(errno) << endl;
        return -1;
    }

    // Loop to allow user to select multiple files
    while (true) {
        string file_path;
        cout << "Enter the file path or 'exit' to quit: ";
        cin >> file_path;

        if (file_path == "exit") {
            break;
        }

        // Validate and sanitize file path
        fs::path path(file_path);
        if (!fs::exists(path) || !fs::is_regular_file(path)) {
            cerr << "Invalid file path" << endl;
            continue;
        }

        // Check if the file exists and is readable
        std::ifstream infile(file_path, std::ios::binary);
        if (!infile.is_open()) {
            cerr << "Error opening file: " << strerror(errno) << endl;
            continue;
        }

        // Check file size limit
        infile.seekg(0, ios::end);
        size_t file_size = infile.tellg();
        infile.seekg(0, ios::beg);
        if (file_size > MAX_FILE_SIZE) {
            cerr << "File size exceeds maximum limit" << endl;
            infile.close();
            continue;
        }

        // Send the file size to the server
        send(sock, &file_size, sizeof(file_size), 0);

        // Calculate file checksum (SHA-256)
        unsigned char checksum[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        while (infile) {
            infile.read(buffer, BUFFER_SIZE);
            SHA256_Update(&sha256, buffer, infile.gcount());
        }
        SHA256_Final(checksum, &sha256);

        // Send the file checksum to the server
        send(sock, checksum, SHA256_DIGEST_LENGTH, 0);
        // creating a directory for storing sent files
        fs::path dir_path("/home/josiah/Documents/C++ DEV/FILE-TRANSFER/sent_files");
        if (!fs::exists(dir_path)) {
            fs::create_directories(dir_path);
        }

        string file_name = "sent_file_" + to_string(time(nullptr));

        // Send the file data
        while (infile) {
            infile.read(buffer, BUFFER_SIZE);
            send(sock, buffer, infile.gcount(), 0);
        }

        cout << "File sent successfully" << endl;

        // Cleanup
        infile.close();
    }

    close(sock);

    return 0;
}