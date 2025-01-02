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