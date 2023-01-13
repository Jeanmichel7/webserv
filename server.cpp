/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/13 14:22:43 by lomasson         ###   ########.fr       */
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

int main(int argc, char **argv, char **env)
{
	struct sockaddr_in interface;
	
	int	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	interface.sin_addr.s_addr = htonl(INADDR_ANY);
	interface.sin_port = htons(80);
	memset(interface.sin_zero, '\0', sizeof interface.sin_zero);
	bind(socket_fd, (const struct sockaddr *)&interface, sizeof(interface));
	listen(socket_fd, 5);
	int fd = open("test.html", O_RDWR);
	char test[10000] = {0};
	read(fd, &test, 10000);
	int len = strlen(test);
	while(1)
	{
		char buffer[1024] = {0};
		printf("\n+++++++ Waiting for new connection ++++++++\n\n");
		int listen_socket = accept(socket_fd, (struct sockaddr *)&interface, (socklen_t *)&interface);
		read(listen_socket, buffer, 1024);
		printf("%s\n", buffer);
		
		write(listen_socket, test, len);
		printf("------------------Hello message sent-------------------\n");
		close(listen_socket);
	}
}