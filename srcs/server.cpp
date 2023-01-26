/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/26 00:58:14 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
// #include "Settings.hpp"
// #include "Config.hpp"


int main() {
	Request req;

	const char*requestGet = "GET / HTTP/1.1\r\
Host: www.google.fr\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/109.0\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n\
Accept-Language: fr,fr-FR;q=0.8,en-US;q=0.5,en;q=0.3\r\n\
Accept-Encoding: gzip, deflate, br\r\n\
Alt-Used: www.google.fr\r\n\
Connection: keep-alive\r\n\
Cookie: AEC=ARSKqsKvUfG5zsMM3KX6G_Ic4lpx6N47qUQpVZ4pqtqxPabwToyvxumfDLc; __Secure-ENID=10.SE=oq2mQ0ieZeRlL4O7a6glMPKWDHvt2QIH-WXX1GLZGJ05wg6z3uBg0buLnS4tOcFtUQN1CUKKkUx-adOUuEip1Eh57YeLn8XVaoal_vD5iS-jCchRZzUygFOaDnpUc4d4i6oeIu9YBJc0hMM2MtnpbI0eQqxRS2aSvYb7fWy2iqI; CONSENT=PENDING+844; SOCS=CAESHAgBEhJnd3NfMjAyMzAxMTgtMF9SQzEaAmZyIAEaBgiAnLeeBg\r\n\
Upgrade-Insecure-Requests: 1\r\n\
Sec-Fetch-Dest: document\r\n\
Sec-Fetch-Mode: navigate\r\n\
Sec-Fetch-Site: none\r\n\
Sec-Fetch-User: ?1\r\n\
TE: trailers\r\n";


const char*requestPost = "POST /url_to_post HTTP/1.1\r\n\
Host: www.google.fr\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/109.0\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n\
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

const char *requestPost2 = "POST /users HTTP/1.1\r\n\
Host: www.example.com\r\n\
Content-Type: application/json\r\n\
Content-Length: 45\r\n\
\r\n\
{\r\n\
  \"name\": \"John Doe\",\r\n\
	\"email\": \"johndoe@example.com\"\r\n\
}\r\n";

const char*requestDelete = "DELETE /url_to_delete HTTP/1.1\r\n\
Host: www.google.fr\r\n\
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/109.0\r\n\
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n\
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
{\r\n\
  \"name\": \"John Doe\",\r\n\
	\"email\": \"johndoe@example.com\"\r\n\
}\r\n";



	if (req.parseRequest(requestPost2)) {
		return 1;
	}

	std::cout << req.getMethod().isGet << std::endl;
	std::cout << req.getMethod().isPost << std::endl;
	std::cout << req.getMethod().isDelete << std::endl;

	std::cout << req.getMethod().path << std::endl;
	std::cout << req.getMethod().parameters << std::endl;
	std::cout << req.getMethod().anchor << std::endl;
	std::cout << req.getMethod().protocole << std::endl;
	std::cout << req.getHeader().host << std::endl;
	std::cout << req.getHeader().user_agent.compatibleMozilla << std::endl;
	return 0;
}

// int main( void )
// {
// 	// Config			config();
// 	Settings		server;
// 	int				socket_server;
// 	int				timeout = 0;
// 	struct kevent	srv;

// 	int ke = kqueue();
// 	int fd = open("http/index.html", O_RDWR);
// 	try
// 	{
// 		socket_server = server.build();
// 		EV_SET(&srv, socket_server, EVFILT_READ | EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, NULL);
// 		if (socket_server == -1 || ke == -1 || listen(socket_server, 10) == -1 || kevent(ke, &srv, 1, NULL, 0, NULL) == -1)
// 			throw Settings::badCreation();
// 		char test[550] = {0};
// 		read(fd, &test, 550);
// 		while(1)
// 		{
// 			// char buffer[1024] = {0};
// 			struct kevent event;

// 			printf("\n+++++++ Waiting for new connection ++++++++\n\n");
// 			int nevents = kevent(ke, NULL, 0, &event, 1, NULL);
// 			if (nevents == -1)
// 				printf("KO\n");
// 			else if (nevents > 0)
// 			{
// 				int socket_client = accept(socket_server, (struct sockaddr *)&server.interface, (socklen_t *)&server.interface);

// 				/* get request */
// 				char *brut_request = new char[5000];
// 				recv(socket_client, brut_request, 5000, 0);
// 				std::cout << "brut request : " << brut_request << std::endl;

// 				/* parse request */
// 				Request req;
// 				req.parse(brut_request);
// 				std::cout << "is get : " << req.method.isGet << std::endl;

// 				/* send response */
// 				FILE *file = fopen("/Users/jrasser/cursus/webservtmp/http/index.html", "r");
// 				if (file == NULL) {
// 					printf("Error opening file");
// 					return 1;
// 				}
// 				/* set size */
// 				unsigned int file_size = 0;
// 				fseek(file, 0, SEEK_END);
// 				file_size = ftell(file);
// 				fseek(file, 0, SEEK_SET);
// 				/* read file */
// 				char *buffer = new char[file_size];
// 				if (buffer == NULL) {
// 					printf("Error allocating memory");
// 					return 1;
// 				}
// 				int bytes_read = fread(buffer, 1, file_size, file);
// 				if (bytes_read == -1) {
// 					printf("Error reading file");
// 					return 1;
// 				}
// 				send(socket_client, buffer, file_size, 0);

// 				fclose(file);
// 				close(socket_client);

// 				// printf("%s\n", buffer);
// 				// close(socket_client);
// 				// write(socket_client, test, 550);

// 				EV_SET(&event, socket_client, EVFILT_WRITE , EV_ADD | EV_ENABLE, 0, 0, &timeout);
// 				if (kevent(ke, &event, 1, NULL, 0, NULL) == -1){}
// 			}
// 		}
// 	}
// 	catch(const std::exception& e)
// 	{
// 		close(fd);
// 		close(ke);
// 		std::cerr << e.what() << '\n';
// 	}
// }


/* PLAN
Si GET:
	recuperer le lien vers la ressource voulu
	verifier son existence
	recuperer son contenu
	Si Cgi:
		execve et dup vert fichier tmp
	generer les headers
	assembler headers et contenu
	si la taille du contenu > 2 083 caractères split
	envoyer le ou les paquets
Si POST:
	recuperer la cible
	verifier l'existence du dossier
	upload avec le nom donne
Si DELETE:
	recuperer la cible
	verifier l'existence du fichier
	supprimer la cible
*/