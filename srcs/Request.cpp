/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/26 15:14:49 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

#include <iostream>
#include <string>

using namespace std;

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

/* *************************************************** */
/*                                                     */
/*                        METHOD                       */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Request::Method::Method()
:
	brut_method(""),
	isGet(false),
	isPost(false),
	isDelete(false),
	isValid(false),
	type(""),
	url(""),
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
		this->brut_method = rhs.brut_method;
		this->isGet = rhs.isGet;
		this->isPost = rhs.isPost;
		this->isDelete = rhs.isDelete;
		this->isValid = rhs.isValid;
		this->type = rhs.type;
		this->url = rhs.url;
		this->path = rhs.path;
		this->parameters = rhs.parameters;
		this->anchor = rhs.anchor;
		this->protocole = rhs.protocole;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */

bool Request::Method::parseMethod( void ) {
	cout << "method : " << this->brut_method << endl;

	size_t pos = 0;
	if ((pos = this->brut_method.find(" ")) != std::string::npos
	|| (pos = this->brut_method.find("	")) != std::string::npos) {
		this->type = this->brut_method.substr(0, pos);
		this->brut_method.erase(0, pos + 1);
	}

	if ((pos = this->brut_method.find(" ")) != std::string::npos
	|| (pos = this->brut_method.find("	")) != std::string::npos) {
		this->url = this->brut_method.substr(0, pos);
		this->brut_method.erase(0, pos + 1);
	}

	this->protocole = this->brut_method.substr(0);

	cout << "type : '" << this->type << "'" << endl;
	cout << "url : '" << this->url << "'" << endl;
	cout << "protocole : '" << this->protocole << "'" << endl;




	if (Request::Method::checkMethod()
	||	Request::Method::checkUri()
	||	Request::Method::checkProtocole())
	{
		cerr << "Error : method syntaxe is not valid" << endl;
		return 1;
	}
	return 0;
}

bool Request::Method::checkMethod( void ) {
	return 0;
}

bool Request::Method::checkUri( void ) {
	return 0;
}

bool Request::Method::checkProtocole( void ) {

	if (this->brut_method.find("HTTP/1.1\r\n") != string::npos) {
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
	brut_header(""),
	host("")
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
	/* a revoir apres */
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
void Request::Header::parseHeader( void ) {

}

bool Request::Header::checkSyntaxe( void ) {
	return (false);
}

bool Request::Header::checkMethod( void ) {
	return (false);
}

bool Request::Header::checkUri( void ) {
	return (false);
}

bool Request::Header::checkProtocole( void ) {
	return (false);
}










/* *************************************************** */
/*                                                     */
/*                         BODY                        */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Request::Body::Body() :
	brut_body("")
{
	t_body body;
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

void Request::Body::parseBody( void ) {

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
bool Request::splitRequest(string req) {

	string::size_type m_pos;
	string::size_type ml_pos;
	string::size_type h_pos;
	string::size_type hl_pos;
	string::size_type b_pos;
	string::size_type bl_pos;

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
		this->isGet = true;
	}
	else if ((m_pos = req.find("POST")) != string::npos) {
		this->isPost = true;
	}
	else if ((m_pos = req.find("DELETE")) != string::npos) {
		this->isDelete = true;
	}
	else {
		cerr << "Error : request method is not valid" << endl;
		return 1;
	}

	if (m_pos != 0) {
		cerr << "Error : request method is not at the beginning of the request" << endl;
		return 1;
	}

	/* split request method */
	ml_pos = req.find("\r\n");
	this->brut_method = req.substr(0, ml_pos);

	/* split header */
	h_pos = ml_pos + 2;
	hl_pos = req.find("\r\n\r\n");
	if (hl_pos == string::npos) {
		if(req.find("POST") == 0) {
			cerr << "Error : header is not valid, miss empty new line beetween headers and body" << endl;
			return 1;
		}
		this->brut_body = "";
		this->brut_header = req.substr(h_pos);
	}
	else {
		hl_pos -= ml_pos + 2;
		this->brut_header = req.substr(h_pos, hl_pos);

		/* split body */
		b_pos = ml_pos + 2 + hl_pos + 4;
		bl_pos = req.size() - ml_pos - 2 - hl_pos - 4;
		this->brut_body = req.substr(b_pos, bl_pos);
	}
	// cerr << "Method : '" << this->brut_method << "'" << endl;
	// cerr << "Header : '" << this->brut_header << "'"<< endl;
	// cerr << "Body : '" << this->brut_body << "'" << endl;

	return 0;
}

// bool Request::checkRequest(string method, string header, string body) {
// 	/* check order */

// 	void parseMethod(string brut_method);

// 	return 0;
// }

bool Request::parseRequest(string req) {
	cout << "********************* \n" << req << "\n*********************" << endl;
	// cout << "Request Brut size : " << req.size() << endl;

	if ( splitRequest(req)
	|| parseMethod() ) {
		return 1;
	}

	this->header.parseHeader();
	this->body.parseBody();

	return 0;
}