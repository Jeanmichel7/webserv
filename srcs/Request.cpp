/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/02/10 16:10:18 by ydumaine         ###   ########.fr       */
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

void Method::reset( void ) {
	this->brut_method = "";
	this->isGet = false;
	this->isPost = false;
	this->isDelete = false;
	this->type = "";
	this->url = "";
	this->path = "";
	this->parameters = "";
	this->anchor = "";
	this->protocole = "";
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
	contain_body(false),
	is_valid(false),
	host(""),
	host_ip("127.0.0.1"),
	port("80"),
	str_user_agent(""),
	str_accepts(""),
	str_accept_languages(""),
	str_accept_encodings(""),
	connection(false),
	content_type(""),
	content_length(""),
	content_encoding(""),
	content_language(""),
	content_location(""),
	is_chuncked(false),
	boundary(""),
	list_headers()
{
	t_user_agent user_agent;
	user_agent["product"] = "";
	user_agent["productVersion"] = "";
	user_agent["platform"] = "";
	user_agent["os"] = "";
	user_agent["osVersion"] = "";
	user_agent["browser"] = "";
	user_agent["browserVersion"] = "";
	user_agent["engine"] = "";
	user_agent["engineVersion"] = "";
	this->user_agent = user_agent;

	t_accept		accept;
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
		this->contain_body = rhs.contain_body;
		this->is_valid = rhs.is_valid;
		this->host = rhs.host;
		this->host_ip = rhs.host_ip;
		this->port = rhs.port;
		this->user_agent = rhs.user_agent;
		this->str_user_agent = rhs.str_user_agent;
		this->accepts = rhs.accepts;
		this->str_accepts = rhs.str_accepts;
		this->accept_languages = rhs.accept_languages;
		this->str_accept_languages = rhs.str_accept_languages;
		this->accept_encodings = rhs.accept_encodings;
		this->str_accept_encodings = rhs.str_accept_encodings;
		this->connection = rhs.connection;
		this->is_chuncked = rhs.is_chuncked;
		this->boundary = rhs.boundary;
		this->list_headers = rhs.list_headers;

		this->content_type = rhs.content_type;
		this->content_length = rhs.content_length;
		this->content_language = rhs.content_language;
		this->content_encoding = rhs.content_encoding;
		this->content_location = rhs.content_location;
	}
	return (*this);
}





/* *************   FUNCTION   ************* */

bool Header::checkHeaderKey(const string &key) {
	string 	str(key);

	if (key.empty()) {
		cerr << "Error : key is empty" << endl;
		return 1;
	}
	if (!isalpha(key[0])) {
		cerr << "Error : key '" << key << "' is not valid: Header's key must start with a letter" << endl;
		return 1;
	}
	for(string::iterator it = str.begin(); it != str.end(); it++) {
		if (!isalnum(*it) && *it != '-'){
			cerr << "Error : key '" << str << "' is not valid: Header's key can't have '"<< *it << "'" << endl;
			return (true);
		}
	}
	return 0;
}

bool Header::checkHeaderValue(const string &value) {

	// if (value.empty()) {
	// 	cerr << "Error : value is empty" << endl;
	// 	return 1;
	// }
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
  if (!value.empty() && (value[0] == ' ' || value[0] == '\t' || value[value.size() - 1] == ' ' || value[value.size() - 1] == '\t')) {
		cerr << "Error : value '" << value << "' is not valid: value can't start or end with ' ' or '\t'" << endl;
		return 1;
	}
	return 0;
}

bool Header::checkSyntaxeTag(const string &host, const string &tag) {
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
	// for(string::const_iterator it = tag.begin(); it != tag.end(); ++it) {
	// 	if (!isalnum(*it) && *it != '-'){
			// cerr << "Error : host '" << host << "' is not valid: tag can't have '"<< *it << "'" << endl;
			// return (true);
		// }
	// }
	return 0;
} 

bool Header::checkHostValue(const string &host ) {
	string::size_type pos = 0;
	int 							nbTag = 1;
	string 						tag = "";
	string 						str(host);
	string::size_type pos2 = 0;

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
	// cout << "contain body : " << contain_body << endl;
	/* check tags */
	str = host;
	cout << "test : " << str << endl;

	if ((pos2 = str.find(":")) != string::npos){
		this->host_ip = str.substr(0, pos2);
		this->port = str.substr(pos2 + 1);
	}
	else
		this->host_ip = str;
	while ((pos = str.find(".")) != string::npos) {
		tag = str.substr(0, pos);
		if (checkSyntaxeTag(host, tag))
			return 1;
		str.erase(0, pos + 1);
	}
	if (checkSyntaxeTag(host, str))
		return 1;
	this->host = host;
	if (!contain_body)
		this->is_valid = true;
	return 0;
}

bool Header::parseUserAgent(const string &user_agent ) {
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
		// cerr << "line : " << line << endl;

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
				// cerr << "subline : '" << subline << "'" << endl;

				if (index == 0) {
					if (index_info == 0) 
						this->user_agent["platform"] = subline;
					else if (index_info == 1)
						this->user_agent["os"] = subline;
				} 
				else
					this->user_agent["engineInfo"] = line;
				line.erase(0, pos + 2);
				index_info++;
			}
			// cerr << "subline fin : '" << line << "'" << endl;
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
	// cerr << "line fin : " << str << endl << endl << endl << endl;
	this->user_agent["browser"] = str.substr(0, str.find("/"));
	this->user_agent["browserVersion"] = str.substr(str.find("/") + 1);
	this->str_user_agent = user_agent;
	return 0;
}






bool Header::checkSyntaxeAccept(const string &line) {
	string::size_type pos = 0;

	//  non imprimable et char de controle
	for(string::size_type i = 0; i < line.size(); i++) {
		if (line[i] < 32 || line[i] > 126) {
			cerr << "Error : accept '" << line << "' is not valid: non imprimable char" << endl;
			return 1;
		}
	}

	// 1 seul slash
	if ((pos = line.find("/")) == string::npos) {
		cerr << "Error : accept '" << line << "' is not valid: no slash" << endl;
		return 1;
	}
	if ((pos = line.find("/")) != line.rfind("/")) {
		cerr << "Error : accept '" << line << "' is not valid: more than one slash" << endl;
		return 1;
	}
	return 0;
}



void Header::setAccepts(string &line) {
	string::size_type pos = 0;
	t_accept 					new_accept;

	new_accept.type = line.substr(0, line.find("/"));
	line.erase(0, line.find("/") + 1);
	if ((pos = line.find(";q=")) != string::npos) {
		new_accept.subtype = line.substr(0, pos);
		new_accept.q = line.substr(pos + 3);
	}
	else {
		new_accept.subtype = line;
		new_accept.q = "1";
	}
	this->accepts.push_back(new_accept);
}


// IANA : http://www.iana.org/assignments/media-types/media-types.xhtml
bool Header::parseAccept(const string &value) {
	string::size_type pos = 0;
	string 						str(value);
	string 						line;

	// trim space
	for(string::size_type i = 0; i < str.size(); i++) {
		if ((pos = str.find(", ")) != string::npos)
			str.erase(pos + 1, 1);
		if ((pos = str.find(" ,")) != string::npos)
			str.erase(pos, 1);
	}

	if (str == "") {
		t_accept new_accept;
		new_accept.type = "*";
		new_accept.subtype = "*";
		new_accept.q = "1";
		this->accepts.push_back(new_accept);
		this->str_accepts = "*/*";
		return 0;
	}

	while ((pos = str.find(",")) != string::npos) {
		line = str.substr(0, pos);
		if (checkSyntaxeAccept(line))
			return 1;
		setAccepts(line);
		str.erase(0, pos + 1);
	}
	setAccepts(str);
	this->str_accepts = value;
	return 0;
}


bool Header::checkSyntaxeAcceptLanguage(const string &) {
	return 0;
}


void Header::setAcceptLanguages(const string &str) {
	string 							line(str);
	string::size_type 	pos = 0;
	t_accept_language 	new_accept_language;

	if (str == "") {
		new_accept_language.lang = "*";
		new_accept_language.spec = "";
		new_accept_language.q = "1";
		this->accept_languages.push_back(new_accept_language);
		this->str_accept_languages = "*";
	}
	else if ((pos = line.find("-")) != string::npos) {
		new_accept_language.lang = line.substr(0, pos);
		line.erase(0, pos + 1);

		if ((pos = line.find(";q=")) != string::npos) {
			new_accept_language.spec = line.substr(0, pos);
			new_accept_language.q = line.substr(pos + 3);
		}
		else {
			new_accept_language.spec = line;
			new_accept_language.q = "1";
		}
	}
	else {
		new_accept_language.spec = "";
		if ((pos = line.find(";q=")) != string::npos) {
			new_accept_language.lang = line.substr(0, pos);
			new_accept_language.q = line.substr(pos + 3);
		}
		else {
			new_accept_language.lang = line;
			new_accept_language.q = "1";
		}
	}
	this->accept_languages.push_back(new_accept_language);
}

bool Header::parseAcceptLanguage(const string &value) {
	string::size_type pos = 0;
	string 						str(value);
	string 						line;

	// trim space
	for(string::size_type i = 0; i < str.size(); i++) {
		if ((pos = str.find(", ")) != string::npos)
			str.erase(pos + 1, 1);
		if ((pos = str.find(" ,")) != string::npos)
			str.erase(pos, 1);
	}

	if (str == "") {
		setAcceptLanguages(str);
		return 0;
	}
	while ((pos = str.find(",")) != string::npos) {
		line = str.substr(0, pos);
		if (checkSyntaxeAcceptLanguage(line))
			return 1;
		setAcceptLanguages(line);
		str.erase(0, pos + 1);
	}
	setAcceptLanguages(str);
	this->str_accept_languages = value;
	return 0;
}







bool Header::checkSyntaxeAcceptEncoding(const string &line) {
	string::size_type 	pos = 0;

	if ((pos = line.find(";q=")) != string::npos) {
		if (line.substr(pos + 3).find_first_not_of("0123456789.") != string::npos)
			return 1;
	}
	return 0;
}

void Header::setAcceptEncodings(const string &line) {
	string::size_type 	pos = 0;
	t_accept_encoding 	new_accept_encoding;

	if ((pos = line.find(";q=")) != string::npos) {
		new_accept_encoding.type = line.substr(0, pos);
		new_accept_encoding.q = line.substr(pos + 3);
	}
	else {
		new_accept_encoding.type = line;
		new_accept_encoding.q = "1";
	}
	this->accept_encodings.push_back(new_accept_encoding);
}

bool Header::parseAcceptEncodings(const string &value) {
	string::size_type pos = 0;
	string 						str(value);
	string 						line;

	// trim space
	for(string::size_type i = 0; i < str.size(); i++) {
		if ((pos = str.find(", ")) != string::npos)
			str.erase(pos + 1, 1);
		if ((pos = str.find(" ,")) != string::npos)
			str.erase(pos, 1);
	}

	if (str == "") {
		this->str_accept_encodings = "*";
		return 0;
	}
	if (checkSyntaxeAcceptEncoding(str))
		return 1;
	while ((pos = str.find(",")) != string::npos) {
		line = str.substr(0, pos);
		setAcceptEncodings(line);
		str.erase(0, pos + 1);
	}
	setAcceptEncodings(str);
	this->str_accept_encodings = value;
	return 0;
}



bool Header::parseContentLength(const string &value) {
	for(string::size_type i = 0; i < value.size(); i++) {
		if (!isdigit(value[i])) {
			cerr << "Error : Content-Length must be a number" << endl;
			return 1;
		}
	}

	// if length > ?

	this->content_length = value;
	return 0;
}

bool Header::parseHeader( void ) {

	string::size_type pos = 0;
	string 						str(this->brut_header);
	string 						line;
	string 						key;
	string 						value;

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
				if (parseUserAgent(value))
					return 1;
			}
			else if (key == "Accept") {
				if (parseAccept(value))
					return 1;
			}
			else if (key == "Accept-Language"){
				if (parseAcceptLanguage(value))
					return 1;
			}
			else if (key == "Accept-Encoding") {
				if (parseAcceptEncodings(value))
					return 1;
			}
			else if (key == "Connection") {
				if (value == "keep-alive")
					this->connection = true;
			} //"Content-Length" ou "Transfer-Encoding".
			else if (key == "Content-Length") {
				if (parseContentLength(value))
					return 1;
				if (this->contain_body)
					this->is_valid = true;
			}
			else if (key == "Transfer-Encoding") {
				if (value == "chunked")
					this->is_chuncked = true;
				if (this->contain_body)
					this->is_valid = true;
			}
			else if (key == "Content-Type") {
				if (parseContentType(value))
					return 1;
			}
			else if (key == "Content-Encoding")
				this->content_encoding = value;
			else if (key == "Content-Language")
				this->content_language = value;
			else if (key == "Content-Location")
				this->content_location = value;

			if (setAllHeaders(key, value))
				return 1;
			// else {
			// 	cerr << "Header '" << key << "' non implemente" << endl;
			// }
		}
		// else {
		// 	cerr << "Error : Error syntaxe separator \": \" in '" << line << "'" << endl;
		// 	return 1;
		// }
	}
	return 0;
}
bool Header::setAllHeaders(const string &key, const string &value) {

	t_list_header_it it_all_headers = this->list_headers.begin();
	for (; it_all_headers != this->list_headers.end(); ++it_all_headers)
	{
		// cout << key << " == ?" << it_all_headers->first << endl;
		// if (it_all_headers->first == key) {
		// 	cerr << "Error : Header '" << key << "' already exist" << endl;
		// 	return 1;
		// }
	}
	this->list_headers[key] = value;
	return 0;
}

bool Header::parseContentType(const string &value) {
	string::size_type pos = 0;
	string 						str(value);
	string 						line;

	// cout << "parseContentType : " << value << endl;
	if (str == "") {
		this->content_type = "*";
		return 0;
	}
	if ((pos = str.find("boundary=")) != string::npos) {
		str.erase(0, pos + 9);
		this->is_chuncked = true;
		this->boundary = str;
	}
	this->content_type = value;
	
	return 0;
}

void Header::reset( void ) {
	this->brut_header = "";
	this->contain_body = false;
	this->is_valid = false;
	this->host = "";
	this->host_ip = "127.0.0.1"; // modif
	this->port = "80";
	this->user_agent.clear();
	this->str_user_agent = "";
	this->accepts.clear();
	this->str_accepts = "";
	this->accept_languages.clear();
	this->str_accept_languages = "";
	this->accept_encodings.clear();
	this->str_accept_encodings = "";
	this->connection = false;
	this->content_type = "";
	this->content_length = "";
	this->content_encoding = "";
	this->content_language = "";
	this->content_location = "";
	this->is_chuncked = false;
	this->boundary = "";
}


/* *************************************************** */
/*                                                     */
/*                         BODY                        */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

// Body::Body() : brut_body(), content(""), concat_body(""), is_chuncked(false) {
Body::Body() : brut_body(), content(""), is_chuncked(false), socket_client(-1) {
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
		this->brut_body = rhs.brut_body;
		this->content = rhs.content;
		this->is_chuncked = rhs.is_chuncked;
		// this->concat_body = rhs.concat_body;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */

bool Body::parseBody( void ) {
	// cout << "start parse body " << this->brut_body << endl;
	if (this->is_chuncked) {
		if (parseTransferEncoding())
			return 1;
	} else if (!this->boundary.empty()) {
		if (parseMultipartBody())
			return 1;
	} else
		this->content = this->brut_body;
	return 0;
}

bool 	Body::parseMultipartBody( void ){
	// string::size_type 	pos_in_line = 0;
	string 				str(this->brut_body);
	string 				line;
	std::stringstream 	body_parsed;

	// cout << "parseMultipartBody : " << this->brut_body << endl;
	// while ((pos = str.find("\r\n")) != string::npos) {
	// 	pos_in_line += pos + 2; 
	// 	line = str.substr(0, pos);
	// 	if (line != "--" + this->boundary && line != "--" + this->boundary + "--")
	// 		body_parsed << line << "\r\n";
	// 	else
	// 		body_parsed << "\r\n";
	// 	str.erase(0, pos + 2);
	// }
	// this->content = body_parsed.str();
	this->content = this->brut_body;
	return 0;
}

bool Body::parseTransferEncoding( void ) {
	string::size_type pos = 0;
	string 						str(this->brut_body);
	string						nb_hexa;
	string::size_type size = 0;
	string 						line;
	string 						line_concat;

	cout << "TransferEncoding, body size : " << str.length() << endl;

	this->content = this->brut_body;


	// for(string::size_type i = 0; i < str.length() - 1; i++) {
	// 	if (str[i] == 'A' && str[i + 1] != 'A')
	// 		cout << "A";
	// 	else if (str[i] == 'A' && str[i + 1] == 'A') 
	// 		{}
	// 	else
	// 		cout << str[i];
	// }
	// cout << str[str.length() - 1] << endl;


	// std::filebuf fb;
 	// fb.open ("testout2.txt",std::ios::out);
 	// std::ostream os(&fb);
 	// os << this->brut_body;
 	// fb.close();

	// cout << "coucou je suis bien la et je dois regarder si je read and dechunck dans parseTransferEncoding() ou dans le server.cpp, et toi quel est ton avis ?" << endl;
	// if ((pos = str.find("\r\n")) != string::npos)
	// {
	// 	nb_hexa = str.substr(0, pos);
	// 	str.erase(0, pos + 2);

	// 	string::size_type x;
	// 	std::stringstream ss;
	// 	std::stringstream ssbody;
	// 	ss << std::hex << nb_hexa;
	// 	ss >> x;
	// 	size = static_cast<string::size_type>(x);
	// 	char buffer[REQ_MAX_SIZE] = {0};

	// 	// read(this->socket_client, buffer, req.header.);

	// 	ssbody << buffer;

	// 	// cout << "TEST : " << ret.str() << endl;

	// 	// output it as a signed type
	// 	// cout << "str: " << str	<< endl;
	// 	// std::cout << "TESTE : "<< size << std::endl;
	// 	// this->concat_body = ret.str();
	// }
	// else {
	// 	cerr << "Error : Error syntaxe separator \": \" in '" << nb_hexa << "'" << endl;
	// 	return 1;
	// }
	return 0;
}

void Body::reset( void ) {
	this->brut_body = "";
	this->content = "";
	// this->concat_body = "";
	this->is_chuncked = false;
	this->boundary = "";
}







/* *************************************************** */
/*                                                     */
/*                       REQUEST                       */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */
Request::Request() : method(Method()), header(Header()), body(Body()), contain_body(false), socket_client(-1)
{
	for(int i = 0; i < REQ_MAX_SIZE; i++)
		buffer[i] = 0;
	return ; 
}

Request::Request(Request const &src){
	this->method = src.method;
	this->header = src.header;
	this->body = src.body;
	this->socket_client = src.socket_client;
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

	// cout << "size request : " << req.size() << endl;
	// if (req.size() > 8200) {
	// 	cerr << "Error : request size is too big" << endl;
	// 	return 1;
	// }
	if (req.size() == 0) {
		cerr << "Error : request is empty" << endl;
		return 1;
	}
	// if (req.substr(req.size() - 2, 2) != "\r\n") {
	// 	cerr << "Error : the end of the request do not include a line break" << endl;
	// 	return 1;
	// }


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
	else if (hl_pos == req.size() - 4) {
		this->body.brut_body = "";
		this->header.brut_header = req.substr(h_pos);
	}
	else {
		hl_pos -= ml_pos;
		this->header.brut_header = req.substr(h_pos, hl_pos);

		/* split body */
		b_pos = ml_pos + 2 + hl_pos + 2;
		bl_pos = req.size() - ml_pos - 2 - hl_pos - 4;
		this->body.brut_body = req.substr(b_pos);
		this->contain_body = true;
		this->header.contain_body = true;
	}
	// cout << "split ok : " << this->body.brut_body << endl;
	return 0;
}

bool Request::parseRequest(string req) {
	// cout << "********************* \n" << req << "\n*********************" << endl;
	// cout << "Request Brut size : " << req.size() << endl;

	if (splitRequest(req) || method.parseMethod() || header.parseHeader()) {
		return 1;
	}
	this->body.is_chuncked = this->header.is_chuncked;
	this->body.boundary = this->header.boundary;
	if (body.parseBody()) {
		return 1;
	}
	return 0;
}

void Request::printRequest()
{

	cout << "Header brut : \n"
		 << this->method.brut_method << endl
		 << this->header.brut_header << endl;
	//   cout << this->method.isGet << endl;
	//   cout << this->method.isPost << endl;
	//   cout << this->method.isDelete << endl;

	//   cout << "url '" << this->method.url << "'" << endl;
	//   cout << "path '" << this->method.path << "'" << endl;
	//   cout << "params '" << this->method.parameters << "'" << endl;
	//   cout << "anchor '" << this->method.anchor <<  "'" << endl;
	//   cout << "protocle '" << this->method.protocole << "'" << endl;
	//   cout << "port '" << this->header.port << "'" << endl;
	//   cout << "host '" << this->header.host << "'" << endl;

	// cout << "useragent '" << this->header.str_user_agent << "'" << endl;
	// Header::t_user_agent_it it = this->header.user_agent.begin();
	// for(; it != this->header.user_agent.end(); ++it) {
	//   cout << "User-Agent "<< it->first << " : " << it->second << endl;
	// }

	// cout << "str_accept '" << this->header.str_accepts << "'" << endl;
	// Header::t_accepts_it it_accept = this->header.accepts.begin();
	// while (it_accept != this->header.accepts.end()) {
	//   cout << "accept '" << it_accept->type << "/"
	//   << it_accept->subtype << "' "
	//   << "q="<< it_accept->q << endl;
	//   it_accept++;
	// }

	// cout << "str_accept_language '" << this->header.str_accept_languages << "'" << endl;
	// Header::t_languages_it it_lang = this->header.accept_languages.begin();
	// for(; it_lang != this->header.accept_languages.end(); ++it_lang) {
	//   cout << "accept_language "<< it_lang->lang
	//   << (it_lang->spec != "" ? "-" : "") << it_lang->spec
	//   << " q=" << it_lang->q << endl;
	// }

	// cout << "str_accept_encoding '" << this->header.str_accept_encodings << "'" << endl << endl;
	// Header::t_encodings_it it_encod = this->header.accept_encodings.begin();
	// for(; it_encod != this->header.accept_encodings.end(); ++it_encod) {
	//   cout << "accept_encoding '"<< it_encod->type << "' q=" << it_encod->q << endl;
	// }

	// cout << "Connection : " << this->header.connection << endl;

	// cout << "content_length '" << this->header.content_length << "'" << endl;
	// cout << "content_type '" << this->header.content_type << "'" << endl;
	// cout << "content_encoding '" << this->header.content_encoding << "'" << endl;
	// cout << "content_language '" << this->header.content_language << "'" << endl;
	// cout << "content_location '" << this->header.content_location << "'" << endl  << endl;

	cout << "All Header : " << endl;
	Header::t_list_header_it it_all_headers = this->header.list_headers.begin();
	for (; it_all_headers != this->header.list_headers.end(); ++it_all_headers)
	{
		cout << it_all_headers->first << " : " << it_all_headers->second << endl;
	}

	// this->contain_body ? cout << "body\n" << "'" << this->body.content << "'" << endl : cout << "no body" << endl;
	cout << "\n\nrequete " << (this->header.is_valid ? "valid" : "invalid") << endl;
}

void Request::resetBuffer( void ) {
	for(int i = 0; i < REQ_MAX_SIZE; i++){
		this->buffer[i] = 0;
	}
}

void Request::reset( void ) {
	this->method.reset();
	this->header.reset();
	this->body.reset();
	this->contain_body = false;
	this->resetBuffer();
}



