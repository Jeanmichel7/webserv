/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/26 00:58:28 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

#include <iostream>
#include <string>

// #include <iostream>

/*
La taille maximale d'une URI (Uniform Resource Identifier) est de 2048 caractères selon la spécification 
de la norme HTTP/1.1.
La taille maximale des en-têtes de requête est de 8192 octets, selon la même spécification.
La taille maximale du corps de la requête dépend des capacités du serveur, mais généralement elle est de 8 Mo 
pour Apache et de 2 Mo pour IIS.
Il est important de noter qu'il est possible de configurer ces limites en fonction de vos besoins spécifiques 
en utilisant les paramètres de configuration appropriés de votre serveur.
*/

/*
Pour être considérée comme valide, une requête HTTP 1.1 doit respecter les règles définies dans 
la spécification HTTP 1.1 du RFC 7230 à 7235. Les principales exigences sont les suivantes:

La méthode de la requête (GET, POST, etc.) doit être valide et correctement spécifiée.
L'URI de la ressource demandée doit être valide et correctement formatée.
Les en-têtes de la requête doit être valide et correctement formaté.
Le corps de la requête doit être valide s'il est présent.
Le format général de la requête doit respecter la syntaxe HTTP standard.

Il est important de noter que ces exigences ne garantissent pas que la requête sera 
traitée avec succès par le serveur, il est nécessaire de respecter les règles de la spécification 
HTTP pour que la requête soit considérée comme valide.

	//check uri size max 2048 char
	//check header size max 8192 octets
	//check body size max 8Mo
*/

/* *************************************************** */
/*                                                     */
/*                        METHOD                       */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Request::Method::Method()
:
	isGet(false),
	isPost(false),
	isDelete(false),
	isValid(false),
	method(""),
	path(""),
	parameters(""),
	anchor(""),
	protocole("")
{
}

Request::Method::Method(Method const &src)
{
	*this = src;
	return ;
}

Request::Method::~Method()
{
	return ;
}

Request::Method &Request::Method::operator=(Method const &rhs)
{
	if (this != &rhs)
	{
		this->isGet = rhs.isGet;
		this->isPost = rhs.isPost;
		this->isDelete = rhs.isDelete;
		this->path = rhs.path;
		this->parameters = rhs.parameters;
		this->anchor = rhs.anchor;
		this->protocole = rhs.protocole;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */

bool Request::Method::parseMethod(string method) {
	if (Request::Method::checkMethod(method)
	||	Request::Method::checkUri(method)
	||	Request::Method::checkProtocole(method))
	{
		cerr << "Error : method syntaxe is not valid" << endl;
		return 1;
	}
	return 0;
}

bool Request::Method::checkMethod(string method) {
	return 0;
}

bool Request::Method::checkUri(string method) {
	return 0;
}

bool Request::Method::checkProtocole(string method) {
	if (method.find("HTTP/1.1") == string::npos) {
		cerr << "Error : HTTP version is not valid" << endl;
		return 1;
	}

	/* check 1.12 ?! */
	return 0;
}











/* *************************************************** */
/*                                                     */
/*                       HEADER                        */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Request::Header::Header() 
:
	host(""),
	checkedMethod(false),
	checkedUri(false),
	checkedProtocole(false)
{
	user_agent.compatibleMozilla = "";
	user_agent.version = "";
	user_agent.platform = "";
	user_agent.os = "";
	user_agent.rv = "";
	user_agent.geckoVersion = "";
	user_agent.browserName = "";
	user_agent.browserVersion = "";

	t_accept    accept;
	t_languages accept_language;
	t_encodings accept_encoding;
}

Request::Header::Header(Header const &src)
{
	*this = src;
	return ;
}

Request::Header::~Header()
{
	return ;
}

Request::Header &Request::Header::operator=(Header const &rhs)
{
	if (this != &rhs)
	{
		this->host = rhs.host;
		this->user_agent = rhs.user_agent;
		this->accept = rhs.accept;
		this->accept_language = rhs.accept_language;
		this->accept_encoding = rhs.accept_encoding;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */
void Request::Header::parseHeader(string brut_method ,string brut_header) {
	string line;
	string pos1;
	string pos2;

	// if (!checkSyntaxe(line) || !checkMethod(pos1) || !checkUri(pos2) || !checkProtocole(pos3)) {
	//   cerr << "Header is not valid" << endl;
	// }
}

bool Request::Header::checkSyntaxe(string str) {
	(void)str;
	return (false);
}

bool Request::Header::checkMethod(string str) {
	(void)str;
	return (false);
}

bool Request::Header::checkUri(string str) {
	(void)str;
	return (false);
}

bool Request::Header::checkProtocole(string str) {
	(void)str;
	return (false);
}










/* *************************************************** */
/*                                                     */
/*                         BODY                        */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Request::Body::Body() {
	return ;
}

Request::Body::Body(Body const &src) {
	*this = src;
	return ;
}

Request::Body::~Body() {
	return ;
}

Request::Body &Request::Body::operator=(Body const &rhs) {
	if (this != &rhs) {
		this->body = rhs.body;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */

void Request::Body::parseBody(string brut_body) {

}







/* *************************************************** */
/*                                                     */
/*                       REQUEST                       */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */
Request::Request() : Method(), Header(), Body() {
	return ; 
}

Request::Request(Request const &src) : Method(src.method), Header(src.header), Body(src.body) {
	return ;
}

Request::~Request() {
	return ;
}

Request &Request::operator=(Request const &rhs) {
	if (this != &rhs) {
		this->method = Method(rhs.method);
		this->header = Header(rhs.header);
		this->body = Body(rhs.body);
	}
	return (*this);
}


/* *************   FUNCTION   ************* */
bool Request::splitRequest(string req, string &method_line, string &header, string &body) {

	string::size_type m_pos;
	string::size_type ml_pos;
	string::size_type h_pos;
	string::size_type hl_pos;
	string::size_type b_pos;
	string::size_type bl_pos;
	bool isGet = false;
	bool isPost = false;
	bool isDelete = false;

	if (req.size() > 8192) {
		cerr << "Error : request size is too big" << endl;
		return 1;
	}
	if (req.size() == 0) {
		cerr << "Error : request is empty" << endl;
		return 1;
	}
	if (req.substr(req.size() - 2, 2) != "\r\n") {
		cerr << "Error : the end of the request do not include a line break" << endl;
		return 1;
	}

	if ((m_pos = req.find("GET")) != string::npos) {
		isGet = true;
	}
	else if ((m_pos = req.find("POST")) != string::npos) {
		isPost = true;
	}
	else if ((m_pos = req.find("DELETE")) != string::npos) {
		isDelete = true;
	}
	else {
		cerr << "Error : request method is not valid" << endl;
		return 1;
	}

	if (m_pos != 0) {
		cerr << "Error : request method is not at the beginning of the request" << endl;
		return 1;
	}

	if (isGet) {
		/* split request method */
		ml_pos = req.find("\r\n");
		method_line = req.substr(0, ml_pos);
		cout << "method line : " << method_line << endl;

		/* split header */
		h_pos = ml_pos + 2;
		hl_pos = req.size() - 2 - ml_pos - 2;
		header = req.substr(h_pos, hl_pos);
		cout << "header : '" << header << "'" << endl;
	}
	if (isPost || isDelete) {
		/* split request method */
		ml_pos = req.find("\r\n");
		method_line = req.substr(0, ml_pos);
		cout << "method line : " << method_line << endl;

		/* split header */
		h_pos = ml_pos + 2;
		hl_pos = req.find("\r\n\r\n");
		if(hl_pos == string::npos) {
			cerr << "Error : header is not valid, miss empty new line beetween headers and body" << endl;
			return 1;
		}
		hl_pos -= ml_pos + 2;
		header = req.substr(h_pos, hl_pos);
		cout << "header : '" << header << "'" << endl;

		/* split body */
		b_pos = ml_pos + 2 + hl_pos + 4;
		bl_pos = req.size() - ml_pos - 2 - hl_pos - 4 - 2;
		body = req.substr(b_pos, bl_pos);
		cout << "body : '" << body << "'" << endl;
	}
	return 0;
}

bool Request::checkRequest(string method, string header, string body) {
	/*
			Vérifiez que la première ligne contient la méthode HTTP correcte (GET, POST, PUT, DELETE, etc.), l'URI de la ressource cible et la version HTTP utilisée (HTTP/1.1).
	Vérifiez que l'en-tête Host est présent et contient le nom d'hôte du serveur cible.
	Vérifiez que les autres en-têtes de la requête sont présents et valides (par exemple, Content-Type pour les requêtes avec un corps, Content-Length pour les requêtes avec un corps, Authorization pour les requêtes nécessitant une authentification, etc.).
	Vérifiez que le corps de la requête est valide et contient les informations appropriées pour la requête (si présent).
		Vérifiez que les retours à la ligne (\r\n) sont utilisés pour séparer les en-têtes
			et pour indiquer la fin de la requête (\r\n à la fin de la requête).
			Vérifiez que la longueur de la requête est conforme aux limites spécifiées par le protocole HTTP (généralement 8190 octets ou moins).
	Vérifiez que les valeurs des en-têtes ne dépassent pas les limites spécifiées par le protocole HTTP (généralement 8190 octets ou moins).
	Vérifiez que les URI de la requête sont encodés de manière appropriée, en utilisant les codes d'échappement pour les caractères spéciaux tels que les espaces et les caractères non ASCII.
	Vérifiez que les valeurs des en-têtes sont encodées de manière appropriée, en utilisant les codes d'échappement pour les caractères spéciaux tels que les espaces et les caractères non ASCII.
	Vérifiez que la requête est conforme aux règles de sécurité établies pour l'application ou le service utilisé, par exemple en vérifiant que l'authentification est valide et que les autorisations appropriées ont été accordées pour l'accès à la ressource demandée.
	Vérifiez que la requête contient les informations de contrôle de la version nécessaire (If-Match, If-None-Match, If-Modified-Since, If-Unmodified-Since) pour les méthodes PUT et DELETE qui nécessitent une condition d'écriture.
	*/

	/* check order */

	// void parseMethod(string brut_method);

	return 0;
}

bool Request::parseRequest(string req) {
	cout << "********************* \n" << req << "\n*********************" << endl;
	// cout << "Request Brut size : " << req.size() << endl;

	string method;
	string header;
	string body;

	if ( splitRequest(req, method, header, body)
	|| parseMethod(method) ) {
		return 1;
	}



	this->header.parseHeader(method, header);
	this->body.parseBody(body);



	return 0;
}