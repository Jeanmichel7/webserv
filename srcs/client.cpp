/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 13:43:13 by lomasson          #+#    #+#             */
/*   Updated: 2023/02/13 12:01:28 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

int main(int argc, char **argv)
{
	struct sockaddr_in interface;
	std::memset(&interface , 0, sizeof(interface));
	char buffer[1024] = {0};
	// char test[] = "Hello from client\n";
	const char *requestPost = "Test GET 127.0.0.1:4241/";

	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
	
	interface.sin_family = AF_INET;
	interface.sin_port = htons(4241);
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
	if (argc == 1){
		send(socket_fd , requestPost , strlen(requestPost) , 0);

	}
	else
		send(socket_fd , argv[1] , strlen(argv[1]) , 0);
	std::cout << "Client waiting for data..." << std::endl;
	read( socket_fd , buffer, 1024);
	std::cout << "Data received ! " << std::endl;
	printf("%s\n",buffer );
	return 0;
}
