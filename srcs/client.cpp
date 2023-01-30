/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 13:43:13 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/30 15:10:05 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.hpp"

int main(int argc, char **argv)
{
	struct sockaddr_in interface;
	char buffer[1024] = {0};
	char test[] = "Hello from client\n";
	const char *requestPost = "POST /url_to_post HTTP/1.1\r\n\
Host: www.google.fr\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.82 Safari/537.36\r\n\
Accept: text/html, application/xhtml+xml, application/xml;q=0.9, image/avif,image/webp,*/*;q=0.8\r\n\
Accept-Language: fr,fr-FR;q=0.8,en-US;q=0.5,en;q=0.3\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Alt-Used: www.google.fr\r\n\
Connection: keep-alive\r\n\
Content-Length: 11\r\n\
Content-Type: application/x-www-form-urlencoded\r\n\
Content-Type: text/plain\r\n\
Content-Encoding: gzip\r\n\
Content-Language: fr\r\n\
Content-Location: www.google.fr\r\n\
\r\n\
\"Simple string\"\r\n";

	
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
		send(socket_fd , requestPost , strlen(requestPost) , 0);
	else
		send(socket_fd , argv[1] , strlen(argv[1]) , 0);
	read( socket_fd , buffer, 1024);
	printf("%s\n",buffer );
	return 0;
}
