/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/13 11:44:18 by lomasson          #+#    #+#             */
/*   Updated: 2023/01/29 22:46:47 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
// #include "Settings.hpp"
// #include "Config.hpp"


int main() {
	Request req;

const char *requestGet = "GET /home?page=12&book=123#456 HTTP/1.1\r\n\
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


/*
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/109.0\r\n\
User-Agent: libwww-perl/6.38\r\n\
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4389.82 Safari/537.36\r\n\
User-Agent: Mozilla/5.0 (iPhone; CPU iPhone OS 14_3 like Mac OS X) AppleWebKit/605.1.15 (KHTML, like Gecko) Version/14.0 Mobile/15E148 Safari/604.1\r\n\
User-Agent: AppleCoreMedia/1.0.0.16E227 (iPhone; U; CPU OS 14_3 like Mac OS X; en_us)\r\n\
*/
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

const char *requestPost2 = "POST /users HTTP/1.1\r\n\
Host: www.example.com-123\r\n\
Content-Type: application/json\r\n\
Content-Length: 45\r\n\
\r\n\
{\r\n\
  \"name\": \"John Doe\",\r\n\
	\"email\": \"johndoe@example.com\"\r\n\
}\r\n";

const char *requestDelete = "DELETE /url_to_delete HTTP/1.1\r\n\
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



	if (req.parseRequest(requestPost)) {
		return 1;
	}

	// cout << req.method.isGet << endl;
	// cout << req.method.isPost << endl;
	// cout << req.method.isDelete << endl;

	cout << "url '" << req.method.url << "'" << endl;
	cout << "path '" << req.method.path << "'" << endl;
	cout << "params '" << req.method.parameters << "'" << endl;
	cout << "anchor '" << req.method.anchor <<  "'" << endl;
	cout << "protocle '" << req.method.protocole << "'" << endl;
	cout << "host '" << req.header.host << "'" << endl;

	cout << "useragent '" << req.header.str_user_agent << "'" << endl;
	Header::t_user_agent_it it = req.header.user_agent.begin();
	for(; it != req.header.user_agent.end(); ++it) {
		cout << "User-Agent "<< it->first << " : " << it->second << endl;
	}

	cout << "str_accept '" << req.header.str_accepts << "'" << endl;
	Header::t_accepts_it it_accept = req.header.accepts.begin();
	while (it_accept != req.header.accepts.end()) {
		cout << "accept '" << it_accept->type << "/" 
		<< it_accept->subtype << "' " 
		<< "q="<< it_accept->q << endl;
		it_accept++;
	}

	cout << "str_accept_language '" << req.header.str_accept_languages << "'" << endl;
	Header::t_languages_it it_lang = req.header.accept_languages.begin();
	for(; it_lang != req.header.accept_languages.end(); ++it_lang) {
		cout << "accept_language "<< it_lang->lang 
		<< (it_lang->spec != "" ? "-" : "") << it_lang->spec 
		<< " q=" << it_lang->q << endl;
	}


	cout << "str_accept_encoding '" << req.header.str_accept_encodings << "'" << endl << endl;
	Header::t_encodings_it it_encod = req.header.accept_encodings.begin();
	for(; it_encod != req.header.accept_encodings.end(); ++it_encod) {
		cout << "accept_encoding '"<< it_encod->type << "' q=" << it_encod->q << endl;
	}

	cout << "Connection : " << req.header.connection << endl;


	// cout << "content_length '" << req.header.content_length << "'" << endl;
	// cout << "content_type '" << req.header.content_type << "'" << endl;
	// cout << "content_encoding '" << req.header.content_encoding << "'" << endl;
	// cout << "content_language '" << req.header.content_language << "'" << endl;
	// cout << "content_location '" << req.header.content_location << "'" << endl  << endl;
	// cout << "content '" << req.header.content << "'" << endl;


	cout << "requete " << (req.header.is_valid ? "valid" : "invalid miss Host") << endl;
	// cout << "body '" << req.body << "'" << endl;


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
// 				cout << "brut request : " << brut_request << endl;

// 				/* parse request */
// 				Request req;
// 				req.parse(brut_request);
// 				cout << "is get : " << req.method.isGet << endl;

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
// 	catch(const exception& e)
// 	{
// 		close(fd);
// 		close(ke);
// 		cerr << e.what() << '\n';
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