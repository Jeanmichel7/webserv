/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/29 16:21:14 by jrasser          ###   ########.fr       */
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

Method::Method()
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

Method::Method(Method const &src)
{
	*this = src;
	return ;
}

Method::~Method()
{
	return ;
}

Method &Method::operator=(Method const &rhs)
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

bool Method::parseMethod( void ) {

	size_t pos = 0;
	string str(this->brut_method);
	// cout << "method : " << str << endl;
	
	if ((pos = str.find(" ")) != string::npos
	|| (pos = str.find("	")) != string::npos) {
		this->type = str.substr(0, pos);
		str.erase(0, pos + 1);
	}

	if ((pos = str.find(" ")) != string::npos
	|| (pos = str.find("	")) != string::npos) {
		this->url = str.substr(0, pos);
		str.erase(0, pos + 1);
	}

	this->protocole = str.substr(0);

	cout << "type : '" << this->type << "'" << endl;
	cout << "url : '" << this->url << "'" << endl;
	cout << "protocole : '" << this->protocole << "'" << endl;

	if (this->checkType()
	||	this->checkUri()
	||	this->checkProtocole())
	{
		return 1;
	}
	return 0;
}

bool Method::checkType( void ) {
	if (this->type == "GET") {
		this->isGet = true;
	}
	else if (this->type == "POST") {
		this->isPost = true;
	}
	else if (this->type == "DELETE") {
		this->isDelete = true;
	}
	else {
		cerr << "Error : request method is not valid" << endl;
		return 1;
	}
	return 0;
}

bool Method::checkUri( void ) {
	size_t pos;
	string str(this->url);

	if ((pos = str.find("?")) != string::npos) {
		this->path = str.substr(0, pos);
		str.erase(0, pos + 1);

		if ((pos = str.find("#")) != string::npos) {
			this->parameters = str.substr(0, pos);
			this->anchor = str.substr(pos + 1);
		}
		else {
			this->parameters = str.substr(0);
		}
	}
	else if ((pos = str.find("#")) != string::npos) {
		this->path = str.substr(0, pos);
		this->anchor = str.substr(pos + 1);
	}
	else {
		this->path = str.substr(0);
	}
	return 0;
}

bool Method::checkProtocole( void ) {
	if (this->protocole != "HTTP/1.1") {
		cerr << "Error : HTTP version " << this->protocole << " is not valid" << endl;
		return 1;
	}
	return 0;
}









/* *************************************************** */
/*                                                     */
/*                       HEADER                        */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Header::Header()
:
	brut_header(""),
	is_valid(false),
	host(""),
	str_user_agent(""),
	str_accept(""),
	str_accept_language(""),
	str_accept_encoding(""),
	keep_alive(false)
{
	// user_agent.compatibleMozilla = false;
	// user_agent.platform = "";
	// user_agent.os = "";
	// user_agent.rv = "";
	// user_agent.gecko = "";
	// user_agent.browserName = "";
	// user_agent.browserVersion = "";

	t_user_agent user_agent;
	user_agent["product"] = "";
	user_agent["productVersion"] = "";
	user_agent["platform"] = "";
	user_agent["os"] = "";
	user_agent["osVersion"] = "";
	user_agent["browser"] = "";
	user_agent["browserVersion"] = "";
	// user_agent["device"] = "";
	// user_agent["deviceVersion"] = "";
	user_agent["engine"] = "";
	user_agent["engineVersion"] = "";
	this->user_agent = user_agent;

	t_accept    accept;
	t_languages accept_language;
	t_encodings accept_encoding;
}

Header::Header(Header const &src)
{
	*this = src;
	return ;
}

Header::~Header()
{
	return ;
}

Header &Header::operator=(Header const &rhs)
{
	if (this != &rhs)
	{
		this->brut_header = rhs.brut_header;
		this->is_valid = rhs.is_valid;
		this->host = rhs.host;
		this->user_agent = rhs.user_agent;
		this->str_user_agent = rhs.str_user_agent;
		this->accept = rhs.accept;
		this->str_accept = rhs.str_accept;
		this->accept_language = rhs.accept_language;
		this->str_accept_language = rhs.str_accept_language;
		this->accept_encoding = rhs.accept_encoding;
		this->str_accept_encoding = rhs.str_accept_encoding;
		this->keep_alive = rhs.keep_alive;

		this->content_type = rhs.content_type;
		this->content_length = rhs.content_length;
		this->content_language = rhs.content_language;
		this->content_encoding = rhs.content_encoding;
		this->content_location = rhs.content_location;
	}
	return (*this);
}





/* *************   FUNCTION   ************* */

bool Header::checkHeaderKey(string key) {
	string 	str(key);

	if (key.empty()) {
		cerr << "Error : key is empty" << endl;
		return 1;
	}
	if (!isalpha(key[0])) {
		cerr << "Error : key '" << key << "' is not valid: Header's key must start with a letter" << endl;
		return 1;
	}
	for(string::iterator it = key.begin(); it != key.end(); it++) {
		if (!isalnum(*it) && *it != '-'){
			cerr << "Error : key '" << key << "' is not valid: Header's key can't have '"<< *it << "'" << endl;
			return (true);
		}
	}

	return 0;
}

bool Header::checkHeaderValue(string value) {
	string 	str(value);

	if (value.empty()) {
		cerr << "Error : value is empty" << endl;
		return 1;
	}
  for (string::const_iterator it = value.begin(); it != value.end(); ++it) {
    if (!isprint(*it) && *it != '\t' && *it != ' ') {
			cerr << "Error : value '" << value << "' is not valid: value can't have '"<< *it << "'" << endl;
      return 1;
		}
  }
  for (string::const_iterator it = value.begin(); it != value.end(); ++it) {
    if (iscntrl(*it)){
			cerr << "Error : value '" << value << "' is not valid: value can't have '"<< *it << "'" << endl;
			return 1;
		}
  }
  if (value[0] == ' ' || value[0] == '\t' || value[value.size() - 1] == ' ' || value[value.size() - 1] == '\t') {
		cerr << "Error : value '" << value << "' is not valid: value can't start or end with ' ' or '\t'" << endl;
		return 1;
	}
	return 0;
}

bool Header::checkSyntaxeTag(string host, string tag) {
	if (tag.size() > 63) {
		cerr << "Error : host '" << host << "' is not valid: > 63 char per tag" << endl;
		return 1;
	}
	if (tag.empty()) {
		cerr << "Error : host '" << host << "' is not valid: tag is empty" << endl;
		return 1;
	}
	if (tag[0] == '-' || tag[tag.size() - 1] == '-') {
		cerr << "Error : host '" << host << "' is not valid: tag can start or end with '-'" << endl;
		return 1;
	}
	if (tag.find("--") != string::npos) {
		cerr << "Error : host '" << host << "' is not valid: tag can't have '--'" << endl;
		return 1;
	}
	for(string::iterator it = tag.begin(); it != tag.end(); ++it) {
		if (!isalnum(*it) && *it != '-'){
			cerr << "Error : host '" << host << "' is not valid: tag can't have '"<< *it << "'" << endl;
			return (true);
		}
	}
	return 0;
}

bool Header::checkHostValue( string host ) {
	string::size_type pos = 0;
	int 							nbTag = 1;
	string 						tag = "";
	string 						str(host);

	if (str.empty()) {
		cerr << "Error : host is empty" << endl;
		return 1;
	}
	while ((pos = str.find(".")) != string::npos) {
		str.erase(0, pos + 1);
		nbTag++;
		if (nbTag > 127) {
			cerr << "Error : host '" << host << "' is not valid: > 127 Tags" << endl;
			return 1;
		}
	}

	/* check tags */
	str = host;
	while ((pos = str.find(".")) != string::npos) {
		tag = str.substr(0, pos);
		if (checkSyntaxeTag(host, tag))
			return 1;
		str.erase(0, pos + 1);
	}
	if (checkSyntaxeTag(host, str))
		return 1;
	this->host = host;
	this->is_valid = true;
	return 0;
}

void Header::setUserAgent(string::size_type index, string value) {
		t_user_agent_it it = this->user_agent.begin();

		cerr << "Index : " << index << "	" ;
		for(string::size_type i = 0; i < index; i++) { it++; }
		it->second = value;
		cerr << it->first << " " << it->second << endl;
}

bool Header::parseUserAgentValue( string user_agent ) {
	// retour a la ligne ?

	string::size_type pos = 0;
	string::size_type subpos = 0;
	string str(user_agent);
	string line = "";
	string subline = "";
	string tmp_line  = "";
	std::stringstream engine_info_nb;

	int index = 0;
	int index_info = 0;

	if (str.size() > 512) {
		cerr << "Error : user_agent '" << user_agent << "' is not valid: > 512 char, don't want to be DDoS" << endl;
		return 1;
	}
	while(((pos = str.find(" ")) != string::npos || (pos = str.find("	")) != string::npos)) {
		line = str.substr(0, pos);
		cerr << "line : " << line << endl;

		if (index == 0) {
			this->user_agent["product"] = line.substr(0, line.find("/"));
			this->user_agent["productVersion"] = line.substr(line.find("/") + 1);
		}
		else if (index == 1) {
			this->user_agent["engine"] = line.substr(0, line.find("/"));
			this->user_agent["engineVersion"] = line.substr(line.find("/") + 1);
		}

		tmp_line = str.substr(pos + 1);
		str.erase(0, pos + 1);

		if ((pos = (tmp_line.find("("))) != string::npos) {
			if ((subpos = tmp_line.find(")")) == string::npos) {
				cerr << "Error: bracket no close" << endl;
			}
			index_info = 0;
			line = tmp_line.substr(pos + 1, subpos - 1);
			while((pos = line.find("; ")) != string::npos) {
				subline = line.substr(0, pos);
				cerr << "subline : '" << subline << "'" << endl;

				if (index == 0) {
					if (index_info == 0) 
						this->user_agent["platform"] = subline;
					else if (index_info == 1)
						this->user_agent["os"] = subline;
				} 
				else {
					this->user_agent["engineInfo"] = line;
				}
				line.erase(0, pos + 2);
				index_info++;
			}
			cerr << "subline fin : '" << line << "'" << endl;
			if (index == 0)
				this->user_agent["osVersion"] = line;
			else {
					engine_info_nb << " " << line;
					this->user_agent["engineInfo"] = engine_info_nb.str();
			}
			str.erase(0, subpos + 2);
		}
		line = str.substr(0, pos);
		str.erase(0, pos + 1);
		index++;
	}
	cerr << "line fin : " << str << endl << endl << endl << endl;
	this->user_agent["browser"] = str.substr(0, str.find("/"));
	this->user_agent["browserVersion"] = str.substr(str.find("/") + 1);

	this->str_user_agent = user_agent;
	return 0;
}

bool Header::parseHeader( void ) {
	string::size_type pos = 0;
	string str(this->brut_header);
	string line;
	string key;
	string value;

	while ((pos = str.find("\r\n")) != string::npos) {
		line = str.substr(0, pos);
		// cout << "line : " << line << endl;
		str.erase(0, pos + 2);

		if ((pos = line.find(": ")) != string::npos) {
			key = line.substr(0, pos);
			value = line.substr(pos + 2);

			// check non doublon
			if (checkHeaderKey(key) || checkHeaderValue(value))
				return 1;

			if (key == "Host") {
				if (checkHostValue(value))
					return 1;
			}
			else if (key == "User-Agent") {
				if (parseUserAgentValue(value))
					return 1;
			}
			else if (key == "Accept")
				this->str_accept = value;
			else if (key == "Accept-Language")
				this->str_accept_language = value;
			else if (key == "Accept-Encoding")
				this->str_accept_encoding = value;
			else if (key == "Content-Type")
				this->content_type = value;
			else if (key == "Content-Length")
				this->content_length = value;
			else if (key == "Content-Encoding")
				this->content_encoding = value;
			else if (key == "Content-Language")
				this->content_language = value;
			else if (key == "Content-Location")
				this->content_location = value;
			// else {
			// 	cerr << "Header '" << key << "' non implemente, is a problem ?" << endl;
			// 	// cerr << "Error : Header's key '" << key << "' is not valid" << endl;
			// 	// return 1;
			// }
		}
		else {
			cerr << "Error : Error syntaxe separator \": \" in '" << line << "'" << endl;
			return 1;
		}
	}
	return 0;
}








/* *************************************************** */
/*                                                     */
/*                         BODY                        */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Body::Body() :
	brut_body("")
{
	t_body body;
	return ;
}

Body::Body(Body const &src) {
	*this = src;
	return ;
}

Body::~Body() {
	return ;
}

Body &Body::operator=(Body const &rhs) {
	if (this != &rhs) {
		this->body = rhs.body;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */

bool Body::parseBody( void ) {

	// size_t pos = 0;
	// string str(this->brut_body);
	// string line;
	// string key;
	// string value;

	// while ((pos = str.find("\r\n")) != string::npos) {
	// 	line = str.substr(0, pos);
	// 	str.erase(0, pos + 2);

	// 	if ((pos = line.find("=")) != string::npos) {
	// 		key = line.substr(0, pos);
	// 		value = line.substr(pos + 1);

	// 		this->body[key] = value;
	// 	}
	// 	else {
	// 		cerr << "Error : body line is not valid" << endl;
	// 		return 1;
	// 	}
	// }










	return 0;
}







/* *************************************************** */
/*                                                     */
/*                       REQUEST                       */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */
Request::Request(){
	method = Method();
	header = Header();
	body = Body();
	return ; 
}

Request::Request(Request const &src){
	this->method = src.method;
	this->header = src.header;
	this->body = src.body;
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

	/* split request method */
	ml_pos = req.find("\r\n");
	method.brut_method = req.substr(0, ml_pos);

	/* split header */
	h_pos = ml_pos + 2;
	hl_pos = req.find("\r\n\r\n");
	if (hl_pos == string::npos) {
		if(req.find("POST") == 0) {
			cerr << "Error : header is not valid, miss empty new line beetween headers and body" << endl;
			return 1;
		}
		this->body.brut_body = "";
		this->header.brut_header = req.substr(h_pos);
	}
	else {
		hl_pos -= ml_pos;
		this->header.brut_header = req.substr(h_pos, hl_pos);

		/* split body */
		b_pos = ml_pos + 2 + hl_pos + 4;
		bl_pos = req.size() - ml_pos - 2 - hl_pos - 4;
		this->body.brut_body = req.substr(b_pos, bl_pos);
	}
	// cerr << "Method : '" << this->method.brut_method << "'" << endl;
	// cerr << "Header : '" << this->header.brut_header << "'"<< endl;
	// cerr << "Body : '" << this->body.brut_body << "'" << endl;

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
	|| method.parseMethod() 
	|| header.parseHeader()
	|| body.parseBody() ) {
		return 1;
	}
	return 0;
}