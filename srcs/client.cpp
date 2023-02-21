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
#define BLU "\033[0;34m"
#define GRN "\033[0;32m"
#define RED "\033[0;31m"
#define RST "\033[2K\r"
#define END "\033[0m"

#include "client.hpp"

int main(int argc, char **argv)
{

	const char *requestPost =  "GET / HTTP/1.1\r\nHost: 127.0.0.1:4241\r\n";
	const char *requestPost2 = "GET / HTTP/1.1\r\nHost: 127.0.0.1:4242\r\n";

	struct sockaddr_in interface;
	std::memset(&interface, 0, sizeof(interface));

	int pid = fork();
	if (pid == 0)
		interface.sin_port = htons(4241);
	else
		interface.sin_port = htons(4242);
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	interface.sin_family = AF_INET;

	char buffer[8024] = {0};

	if (inet_pton(AF_INET, "127.0.0.1", &interface.sin_addr) <= 0)
	{
		if (pid == 0)
			cout << BLU "\nInvalid address/ Address not supported \n" END << endl;
		else
			cout << GRN "\nInvalid address/ Address not supported \n" END << endl;
		return -1;
	}
	if (connect(socket_fd, (struct sockaddr *)&interface, sizeof(interface)) < 0)
	{
		if (pid == 0)
			cout << BLU "Connection Failed" END << endl;
		else
			cout << GRN "Connection Failed" END << endl;
		return -1;
	}

	if (pid == 0)
		send(socket_fd, requestPost, strlen(requestPost), 0);
	else
		send(socket_fd, requestPost2, strlen(requestPost2), 0);

	if (pid == 0)
	{
		std::cout << BLU "Client waiting for data..." END << std::endl;
		read(socket_fd, buffer, 8024);
		std::cout << BLU "Data received ! " << endl <<  buffer << END << endl;
	}
	else
	{
		std::cout << GRN "Client waiting for data..." END << std::endl;
		read(socket_fd, buffer, 8024);
		std::cout << GRN "Data received ! " << endl <<  buffer << END << endl;
	}

	while (wait(NULL) != -1 && errno != ECHILD) {}
	return 0;
}
