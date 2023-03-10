#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

using namespace std;

void send_chunked_request(const char* host, int port, const char* path, const char* data, int length) {
    // create a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error: socket creation failed" << endl;
        return;
    }
    // resolve the host name
    struct hostent* he = gethostbyname(host);
    if (he == NULL) {
        cerr << "Error: host name resolution failed" << endl;
        close(sock);
        return;
    }
    // set up the server address
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr*)he->h_addr);
    // connect to the server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Error: connection failed" << endl;
        close(sock);
        return;
    }
    // send the HTTP request
    ostringstream oss;
    oss << "POST " << path << " HTTP/1.1\r\n";
    oss << "Host: " << host << ":" << port << "\r\n";
    oss << "Content-Type: text/plain\r\n";
    oss << "Transfer-Encoding: chunked\r\n";
    oss << "\r\n";
    send(sock, oss.str().c_str(), oss.str().length(), 0);
    // send the chunks of data
    int offset = 0;
    while (offset < length) {
        int chunk_size = min(8, length - offset); // use 8 bytes per chunk
        ostringstream chunk_header;
        chunk_header << hex << chunk_size << "\r\n";
        send(sock, chunk_header.str().c_str(), chunk_header.str().length(), 0);
        send(sock, data + offset, chunk_size, 0);
        send(sock, "\r\n", 2, 0);
        offset += chunk_size;
    }
    // send the final chunk
    send(sock, "0\r\n\r\n", 5, 0);
    // read the response
    char buffer[1024];
    int bytes_read = recv(sock, buffer, sizeof(buffer), 0);
    while (bytes_read > 0) {
        cout << string(buffer, bytes_read);
        bytes_read = recv(sock, buffer, sizeof(buffer), 0);
    }
    // close the socket
    close(sock);
}

int main() {
    const char* host = "localhost";
    int port = 4241;
    const char* path = "/a.out";
    const char* data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    int length = strlen(data);
    send_chunked_request(host, port, path, data, length);
    return 0;
}