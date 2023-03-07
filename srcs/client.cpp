/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 13:43:13 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/07 11:45:34 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Error: socket creation failed" << endl;
        return;
    }
    struct hostent* he = gethostbyname(host);
    if (he == NULL) {
        cerr << "Error: host name resolution failed" << endl;
        close(sock);
        return;
    }
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr = *((struct in_addr*)he->h_addr);
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Error: connection failed" << endl;
        close(sock);
        return;
    }
    ostringstream oss;
    oss << "POST " << path << " HTTP/1.1\r\n";
    oss << "Host: " << host << ":" << port << "\r\n";
    oss << "Content-Type: text/plain\r\n";
    oss << "Transfer-Encoding: chunked\r\n";
    oss << "\r\n";
    send(sock, oss.str().c_str(), oss.str().length(), 0);
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
    send(sock, "0\r\n\r\n", 5, 0);
    char buffer[1024];
    int bytes_read = recv(sock, buffer, sizeof(buffer), 0);
    while (bytes_read > 0) {
        cout << string(buffer, bytes_read);
        bytes_read = recv(sock, buffer, sizeof(buffer), 0);
    }
    close(sock);
}
/*

int main(int argc, char **argv)
{
	int NB_OF_FORK = 4;

	if (argc > 2)
		NB_OF_FORK = atoi(argv[1]);
	
	struct sockaddr_in interface;
	std::memset(&interface , 0, sizeof(interface));
	interface.sin_port = htons(4241);
	char buffer[1024] = {0};
	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
	
	interface.sin_family = AF_INET;
	for(int i = 0; i < NB_OF_FORK; i++)
	{
		if (fork() == 0)
			interface.sin_port = htons(4241);
		else
			interface.sin_port = htons(4241);
	}
	char r[] = "GET / 127.0.0.1:4242\r\n";
	if(inet_pton(AF_INET, "127.0.0.1", &interface.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	if (connect(socket_fd, (struct sockaddr *)&interface, sizeof(interface)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	send(socket_fd , r , strlen(r), 0);
	// std::cout << "Client waiting for data..." << std::endl;
	read( socket_fd , buffer, 1024);
	printf("%s\n",buffer );
	return 0;
}


*/
int main() {
    const char* host = "localhost";
    int port = 4241;
    const char* path = "/";
    const char* data = "Lorem ipsum dolor sit amet, consectetur adipiscing elit.";
    int length = strlen(data);
    send_chunked_request(host, port, path, data, length);
    return 0;
}