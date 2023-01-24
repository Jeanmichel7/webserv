/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 13:43:13 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/24 16:43:12 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

// int main(int argc, char **argv)
// {
// 	struct sockaddr_in interface;
// 	char buffer[1024];
// 	char test[] = "Hello from client\n";
	
// 	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
	
// 	interface.sin_family = AF_INET;
// 	interface.sin_port = htons(80);
// 	if(inet_pton(AF_INET, "127.0.0.1", &interface.sin_addr) <= 0)
// 	{
// 	    printf("\nInvalid address/ Address not supported \n");
// 	    return -1;
// 	}
// 	if (connect(socket_fd, (struct sockaddr *)&interface, sizeof(interface)) < 0)
// 	{
// 	    printf("\nConnection Failed \n");
// 	    return -1;
// 	}
// 	if (argc == 1)
// 		send(socket_fd , test , strlen(test) , 0);
// 	else
// 		send(socket_fd , argv[1] , strlen(argv[1]) , 0);
// 	read( socket_fd , buffer, 1024);
// 	printf("%s\n",buffer );
// 	return 0;
// }


int main()
{
	struct sockaddr_in interface;
	interface.sin_family = AF_INET;
	interface.sin_port = htons(80);
	
	// char buffer[1024];
	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if(inet_pton(AF_INET, "127.0.0.1", &interface.sin_addr) <= 0) {
	    printf("\nInvalid address/ Address not supported \n");
	    return 1;
	}
	socklen_t socket_len = sizeof(interface);
	if (connect(socket_fd, (struct sockaddr *)&interface, socket_len) < 0) {
	    printf("\nConnection Failed %d, %s\n", errno, strerror(errno));
	    // return 1;
	}
	
	//Sending request
	// const char *request = "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: Keep-Alive\r\n\r\n";
	const char *request = "GET / HTTP/1.1\r\n\
User-Agent: PostmanRuntime/7.29.0\r\n\
Accept: */*\r\n\
Postman-Token: f7f5f1c0-bbc1-44ef-8303-18bd70d322f2bla\r\n\
Host: 127.0.0.1\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Connection: keep-alive";


	int sendBytesAmount = send(socket_fd, request, strlen(request), 0);
	if (sendBytesAmount < (int)strlen(request) || sendBytesAmount < 0) {
		std::cout << "Error sending request : " << errno << strerror(errno)<< std::endl;
		return 1;
	}
	else {
		std::cout << "Request sent" << std::endl;
	}


	//Receiving response
	int bytesReceived = 0;
	char buffer[1024];

	bytesReceived = recv(socket_fd, buffer, 1024, 0);
	if (bytesReceived < 0) {
		std::cout << "Error receiving response : " << errno << strerror(errno)<< std::endl;
		return 1;
	}
	else {
		std::cout << "Response received" << std::endl;
		bytesReceived = fread(buffer, 1, 1024, stdout);
		std::cout << buffer << std::endl;
	}
	return 0;
}
