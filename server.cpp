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

    //creating socket file descriptor
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // attaching socket to the port
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    //binding the socket to the port
    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    //Start listening for connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Server started on port " << port << endl;

    //Accepting connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addressLen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    //Creating a directory to store received files
    fs::path dir_path("/home/josiah/Documents/C++ DEV/FILE-TRANSFER/received_files");
    if(!fs::exists(dir_path)) {
        fs::create_directories(dir_path);
    }

    //Receiving a file
    int bytes_received;
    int total_bytes_received = 0;
    int file_size = 0;

    //Receive the file size first
    recv(new_socket, &file_size, sizeof(file_size), 0);

    string file_name = "received_file_" + to_string(time(0));
    fs::path file_path = dir_path / (file_name + ".txt");

    ofstream outfile(file_path, ios::binary);
    while (total_bytes_received < file_size) {
        bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
        outfile.write(buffer, bytes_received);
        total_bytes_received += bytes_received;

        //Display progress message
        cout << "\rProgress: " << (double)total_bytes_received / file_size * 100.0 << "%";
        cout.flush();
    }

    cout << endl;
    cout << "File received successfully. You will find the received file in: " << file_path << endl;

    //cleanup code
    outfile.close();
    close(new_socket);
    close(server_fd);

    return 0;
}