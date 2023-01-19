/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/19 12:25:13 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main( void )
{
	struct sockaddr_in interface;
	
	interface.sin_addr.s_addr = htonl(INADDR_ANY);
	interface.sin_port = htons(80);
	
	
	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	bind(socket_fd, (const struct sockaddr *)&interface, sizeof(interface));
	
	listen(socket_fd, 5);

	while(1)
	{
		int fd = open("srcs/site/test.html", O_RDWR);
		char test[550] = {0};
		read(fd, &test, 550);
		char buffer[1024] = {0};

		
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		int listen_socket = accept(socket_fd, (struct sockaddr *)&interface, (socklen_t *)&interface);
		read(listen_socket, buffer, 1024);
		
		printf("%s\n", buffer);
		write(listen_socket, test, 550);
		printf("------------------Hello message sent-------------------\n");
		close(listen_socket);
		close(fd);
	}
}