/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 13:43:13 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/19 12:19:58 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
	struct sockaddr_in interface;
	char buffer[1024];
	char test[] = "Hello from client\n";
	
	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0); 
	
	interface.sin_family = AF_INET;
	interface.sin_port = htons(80);
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
	if (argc == 1)
		send(socket_fd , test , strlen(test) , 0);
	else
		send(socket_fd , argv[1] , strlen(argv[1]) , 0);
	read( socket_fd , buffer, 1024);
	printf("%s\n",buffer );
	return 0;
}