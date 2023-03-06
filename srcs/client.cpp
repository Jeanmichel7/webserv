/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 13:43:13 by lomasson          #+#    #+#             */
/*   Updated: 2023/03/06 13:34:25 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

/*
int main(int argc, char **argv)
{
	int NB_OF_FORK;
	int p = 0;
	struct sockaddr_in interface;
	std::memset(&interface , 0, sizeof(interface));
	int pid = fork();
	if (pid == 0)
	{
		p++;
		interface.sin_port = htons(4241);
	}
	else
		interface.sin_port = htons(4242);
	if (argc >= 2)
		NB_OF_FORK = atoi(argv[1]);
	else
		NB_OF_FORK = 4;
	for(int i = 0; i < NB_OF_FORK; i++)
		fork();

	char buffer[1024] = {0};
	// char test[] = "Hello from client\n";
	char *requestPost;
	if (p == 0)
	{
		char *r = "GET / 127.0 .0.1:4242\r\n";
		requestPost = r;
	}
	else
	{
		char *r = "GET / 127.0.0.1:4241\r\n";
		requestPost = r;
	}

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
	send(socket_fd , requestPost , strlen(requestPost) , 0);
	// std::cout << "Client waiting for data..." << std::endl;
	read( socket_fd , buffer, 1024);
	if (strlen(buffer) > 80)
		std::cout << "Data received ! " << std::endl;
	else
		std::cout << "Data NOT received ! " << std::endl;
	// printf("%s\n",buffer );
	return 0;
}
*/

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