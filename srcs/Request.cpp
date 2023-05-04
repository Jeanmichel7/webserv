/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/03/02 13:36:50y ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
#include "Config.hpp"

/* *************************************************** */
/*                                                     */
/*                        METHOD                       */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Method::Method()
		: brut_method(""),
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
	return;
}

Method::~Method()
{
	// cerr << "Method destructor called" << std::endl;
	return;
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

bool Method::parseMethod(void)
{

	size_t pos = 0;
	string str(this->brut_method);

	if ((pos = str.find(" ")) != string::npos || (pos = str.find("	")) != string::npos)
	{
		this->type = str.substr(0, pos);
		str.erase(0, pos + 1);
	}

	if ((pos = str.find(" ")) != string::npos || (pos = str.find("	")) != string::npos)
	{
		this->url = str.substr(0, pos);
		str.erase(0, pos + 1);
	}
	this->protocole = str.substr(0);

	if (this->checkType() || this->checkUri() || this->checkProtocole())
	{
		return 1;
	}
	return 0;
}

 bool Method::checkType(void)
 {
 	if (this->type == "GET")
 	{
 		this->isGet = true;
 	}
 	else if (this->type == "POST")
 	{
 		this->isPost = true;
 	}
 	else if (this->type == "DELETE")
 	{
 		this->isDelete = true;
 	}
 	return 0;
 }

bool Method::checkUri(void)
{
	size_t pos;
	string str(this->url);

	if ((pos = str.find("?")) != string::npos)
	{
		this->path = str.substr(0, pos);
		str.erase(0, pos + 1);

		if ((pos = str.find("#")) != string::npos)
		{
			this->parameters = str.substr(0, pos);
			this->anchor = str.substr(pos + 1);
		}
		else
		{
			this->parameters = str.substr(0);
		}
	}
	else if ((pos = str.find("#")) != string::npos)
	{
		this->path = str.substr(0, pos);
		this->anchor = str.substr(pos + 1);
	}
	else
	{
		this->path = str.substr(0);
	}
	return 0;
}

bool Method::checkProtocole(void)
{
	if (this->protocole != "HTTP/1.1")
	{
		cerr << "Error : HTTP version " << this->protocole << " is not valid" << std::endl;
		return 1;
	}
	return 0;
}

void Method::reset(void)
{
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
		: brut_header(""),
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

	t_accept accept;
	t_languages accept_language;
	t_encodings accept_encoding;
	t_cookie cookies;
}

Header::Header(Header const &src)
{
	*this = src;
	return;
}

Header::~Header()
{
	// cerr << "Header destructor called" << std::endl;
	return;
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

bool Header::checkHeaderKey(const string &key)
{
	string str(key);

	if (key.empty())
	{
		cerr << "Error : key is empty" << std::endl;
		return 1;
	}
	if (!isalpha(key[0]))
	{
		cerr << "Error : key '" << key << "' is not valid: Header's key must start with a letter" << std::endl;
		return 1;
	}
	for (string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (!isalnum(*it) && *it != '-')
		{
			cerr << "Error : key '" << str << "' is not valid: Header's key can't have '" << *it << "'" << std::endl;
			return (true);
		}
	}
	return 0;
}

bool Header::checkHeaderValue(const string &value)
{

	for (string::const_iterator it = value.begin(); it != value.end(); ++it)
	{
		if (!isprint(*it) && *it != '\t' && *it != ' ')
		{
			cerr << "Error : value '" << value << "' is not valid: value can't have '" << *it << "'" << std::endl;
			return 1;
		}
	}
	for (string::const_iterator it = value.begin(); it != value.end(); ++it)
	{
		if (iscntrl(*it))
		{
			cerr << "Error : value '" << value << "' is not valid: value can't have '" << *it << "'" << std::endl;
			return 1;
		}
	}
	if (!value.empty() && (value[0] == ' ' || value[0] == '\t' || value[value.size() - 1] == ' ' || value[value.size() - 1] == '\t'))
	{
		cerr << "Error : value '" << value << "' is not valid: value can't start or end with ' ' or '\t'" << std::endl;
		return 1;
	}
	return 0;
}

bool Header::checkSyntaxeTag(const string &host, const string &tag)
{
	if (tag.size() > 63)
	{
		cerr << "Error : host '" << host << "' is not valid: > 63 char per tag" << std::endl;
		return 1;
	}
	if (tag.empty())
	{
		cerr << "Error : host '" << host << "' is not valid: tag is empty" << std::endl;
		return 1;
	}
	if (tag[0] == '-' || tag[tag.size() - 1] == '-')
	{
		cerr << "Error : host '" << host << "' is not valid: tag can start or end with '-'" << std::endl;
		return 1;
	}
	if (tag.find("--") != string::npos)
	{
		cerr << "Error : host '" << host << "' is not valid: tag can't have '--'" << std::endl;
		return 1;
	}
	return 0;
}

bool Header::checkHostValue(const string &host)
{
	string::size_type pos = 0;
	int nbTag = 1;
	string tag = "";
	string str(host);
	string::size_type pos2 = 0;

	if (str.empty())
	{
		cerr << "Error : host is empty" << std::endl;
		return 1;
	}
	while ((pos = str.find(".")) != string::npos)
	{
		str.erase(0, pos + 1);
		nbTag++;
		if (nbTag > 127)
		{
			cerr << "Error : host '" << host << "' is not valid: > 127 Tags" << std::endl;
			return 1;
		}
	}
	str = host;

	if ((pos2 = str.find(":")) != string::npos)
	{
		this->host_ip = str.substr(0, pos2);
		this->port = str.substr(pos2 + 1);
	}
	else
		this->host_ip = str;
	while ((pos = str.find(".")) != string::npos)
	{
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

bool Header::parseUserAgent(const string &user_agent)
{

	string::size_type pos = 0;
	string::size_type subpos = 0;
	string str(user_agent);
	string line = "";
	string subline = "";
	string tmp_line = "";
	std::stringstream engine_info_nb;

	int index = 0;
	int index_info = 0;

	if (str.size() > 512)
	{
		cerr << "Error : user_agent '" << user_agent << "' is not valid: > 512 char, don't want to be DDoS" << std::endl;
		return 1;
	}
	while (((pos = str.find(" ")) != string::npos || (pos = str.find("	")) != string::npos))
	{
		line = str.substr(0, pos);

		if (index == 0)
		{
			this->user_agent["product"] = line.substr(0, line.find("/"));
			this->user_agent["productVersion"] = line.substr(line.find("/") + 1);
		}
		else if (index == 1)
		{
			this->user_agent["engine"] = line.substr(0, line.find("/"));
			this->user_agent["engineVersion"] = line.substr(line.find("/") + 1);
		}

		tmp_line = str.substr(pos + 1);
		str.erase(0, pos + 1);

		if ((pos = (tmp_line.find("("))) != string::npos)
		{
			if ((subpos = tmp_line.find(")")) == string::npos)
			{
				cerr << "Error: bracket no close" << std::endl;
			}
			index_info = 0;
			line = tmp_line.substr(pos + 1, subpos - 1);
			while ((pos = line.find("; ")) != string::npos)
			{
				subline = line.substr(0, pos);

				if (index == 0)
				{
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
			if (index == 0)
				this->user_agent["osVersion"] = line;
			else
			{
				engine_info_nb << " " << line;
				this->user_agent["engineInfo"] = engine_info_nb.str();
			}
			str.erase(0, subpos + 2);
		}
		line = str.substr(0, pos);
		str.erase(0, pos + 1);
		index++;
	}
	this->user_agent["browser"] = str.substr(0, str.find("/"));
	this->user_agent["browserVersion"] = str.substr(str.find("/") + 1);
	this->str_user_agent = user_agent;
	return 0;
}

bool Header::checkSyntaxeAccept(const string &line)
{
	string::size_type pos = 0;

	for (string::size_type i = 0; i < line.size(); i++)
	{
		if (line[i] < 32 || line[i] > 126)
		{
			cerr << "Error : accept '" << line << "' is not valid: non imprimable char" << std::endl;
			return 1;
		}
	}

	if ((pos = line.find("/")) == string::npos)
	{
		cerr << "Error : accept '" << line << "' is not valid: no slash" << std::endl;
		return 1;
	}
	if ((pos = line.find("/")) != line.rfind("/"))
	{
		cerr << "Error : accept '" << line << "' is not valid: more than one slash" << std::endl;
		return 1;
	}
	return 0;
}

void Header::setAccepts(string &line)
{
	string::size_type pos = 0;
	t_accept new_accept;

	new_accept.type = line.substr(0, line.find("/"));
	line.erase(0, line.find("/") + 1);
	if ((pos = line.find(";q=")) != string::npos)
	{
		new_accept.subtype = line.substr(0, pos);
		new_accept.q = line.substr(pos + 3);
	}
	else
	{
		new_accept.subtype = line;
		new_accept.q = "1";
	}
	this->accepts.push_back(new_accept);
}

// IANA : http://www.iana.org/assignments/media-types/media-types.xhtml
bool Header::parseAccept(const string &value)
{
	string::size_type pos = 0;
	string str(value);
	string line;

	// trim space
	for (string::size_type i = 0; i < str.size(); i++)
	{
		if ((pos = str.find(", ")) != string::npos)
			str.erase(pos + 1, 1);
		if ((pos = str.find(" ,")) != string::npos)
			str.erase(pos, 1);
	}

	if (str == "")
	{
		t_accept new_accept;
		new_accept.type = "*";
		new_accept.subtype = "*";
		new_accept.q = "1";
		this->accepts.push_back(new_accept);
		this->str_accepts = "*/*";
		return 0;
	}

	while ((pos = str.find(",")) != string::npos)
	{
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

bool Header::checkSyntaxeAcceptLanguage(const string &)
{
	return 0;
}

void Header::setAcceptLanguages(const string &str)
{
	string line(str);
	string::size_type pos = 0;
	t_accept_language new_accept_language;

	if (str == "")
	{
		new_accept_language.lang = "*";
		new_accept_language.spec = "";
		new_accept_language.q = "1";
		this->accept_languages.push_back(new_accept_language);
		this->str_accept_languages = "*";
	}
	else if ((pos = line.find("-")) != string::npos)
	{
		new_accept_language.lang = line.substr(0, pos);
		line.erase(0, pos + 1);

		if ((pos = line.find(";q=")) != string::npos)
		{
			new_accept_language.spec = line.substr(0, pos);
			new_accept_language.q = line.substr(pos + 3);
		}
		else
		{
			new_accept_language.spec = line;
			new_accept_language.q = "1";
		}
	}
	else
	{
		new_accept_language.spec = "";
		if ((pos = line.find(";q=")) != string::npos)
		{
			new_accept_language.lang = line.substr(0, pos);
			new_accept_language.q = line.substr(pos + 3);
		}
		else
		{
			new_accept_language.lang = line;
			new_accept_language.q = "1";
		}
	}
	this->accept_languages.push_back(new_accept_language);
}

bool Header::parseAcceptLanguage(const string &value)
{
	string::size_type pos = 0;
	string str(value);
	string line;

	for (string::size_type i = 0; i < str.size(); i++)
	{
		if ((pos = str.find(", ")) != string::npos)
			str.erase(pos + 1, 1);
		if ((pos = str.find(" ,")) != string::npos)
			str.erase(pos, 1);
	}

	if (str == "")
	{
		setAcceptLanguages(str);
		return 0;
	}
	while ((pos = str.find(",")) != string::npos)
	{
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

bool Header::checkSyntaxeAcceptEncoding(const string &line)
{
	string::size_type pos = 0;

	if ((pos = line.find(";q=")) != string::npos)
	{
		if (line.substr(pos + 3).find_first_not_of("0123456789.") != string::npos)
			return 1;
	}
	return 0;
}

void Header::setAcceptEncodings(const string &line)
{
	string::size_type pos = 0;
	t_accept_encoding new_accept_encoding;

	if ((pos = line.find(";q=")) != string::npos)
	{
		new_accept_encoding.type = line.substr(0, pos);
		new_accept_encoding.q = line.substr(pos + 3);
	}
	else
	{
		new_accept_encoding.type = line;
		new_accept_encoding.q = "1";
	}
	this->accept_encodings.push_back(new_accept_encoding);
}

bool Header::parseAcceptEncodings(const string &value)
{
	string::size_type pos = 0;
	string str(value);
	string line;

	for (string::size_type i = 0; i < str.size(); i++)
	{
		if ((pos = str.find(", ")) != string::npos)
			str.erase(pos + 1, 1);
		if ((pos = str.find(" ,")) != string::npos)
			str.erase(pos, 1);
	}

	if (str == "")
	{
		this->str_accept_encodings = "*";
		return 0;
	}
	if (checkSyntaxeAcceptEncoding(str))
		return 1;
	while ((pos = str.find(",")) != string::npos)
	{
		line = str.substr(0, pos);
		setAcceptEncodings(line);
		str.erase(0, pos + 1);
	}
	setAcceptEncodings(str);
	this->str_accept_encodings = value;
	return 0;
}

bool Header::parseContentLength(const string &value)
{
	for (string::size_type i = 0; i < value.size(); i++)
	{
		if (!isdigit(value[i]))
		{
			cerr << "Error : Content-Length must be a number" << std::endl;
			return 1;
		}
	}

	this->content_length = value;
	return 0;
}

bool Header::parseHeader(void)
{
	string::size_type pos = 0;
	string str(this->brut_header);
	string line;
	string key;
	string value;

	while ((pos = str.find("\r\n")) != string::npos)
	{
		line = str.substr(0, pos);
		str.erase(0, pos + 2);
		if ((pos = line.find(": ")) != string::npos)
		{
			key = line.substr(0, pos);
			value = line.substr(pos + 2);
			// check non doublon
			if (checkHeaderKey(key) || checkHeaderValue(value))
				return 1;
			if (key == "Host")
			{
				if (checkHostValue(value))
					return 1;
			}
			else if (key == "User-Agent")
			{
				if (parseUserAgent(value))
					return 1;
			}
			else if (key == "Accept")
			{
				if (parseAccept(value))
					return 1;
			}
			else if (key == "Accept-Language")
			{
				if (parseAcceptLanguage(value))
					return 1;
			}
			else if (key == "Accept-Encoding")
			{
				if (parseAcceptEncodings(value))
					return 1;
			}
			else if (key == "Connection")
			{
				if (value == "keep-alive")
					this->connection = true;
			}
			else if (key == "Content-Length")
			{
				if (parseContentLength(value))
					return 1;
				if (this->contain_body)
					this->is_valid = true;
			}
			else if (key == "Transfer-Encoding")
			{
				if (value == "chunked")
					this->is_chuncked = true;
				if (this->contain_body)
					this->is_valid = true;
			}
			else if (key == "Content-Type")
			{
				if (parseContentType(value))
					return 1;
			}
			else if (key == "Content-Encoding")
				this->content_encoding = value;
			else if (key == "Content-Language")
				this->content_language = value;
			else if (key == "Content-Location")
				this->content_location = value;
			else if (key == "Cookie")
			{
				if (parseCookies(value))
					return 1;
			}
			if (setAllHeaders(key, value))
				return 1;
		}
	}
	return 0;
}

bool Header::parseCookies(const string &value)
{
	string::size_type pos = 0;
	string str(value);
	string line;
	string key;
	string val;

	this->str_cookie = value;
	while ((pos = str.find("; ")) != string::npos)
	{
		line = str.substr(0, pos);
		str.erase(0, pos + 2);
		if ((pos = line.find("=")) != string::npos)
		{
			key = line.substr(0, pos);
			val = line.substr(pos + 1);
			this->cookies[key] = val;
		}
	}
	if ((pos = str.find("=")) != string::npos)
	{
		key = str.substr(0, pos);
		val = str.substr(pos + 1);
		this->cookies[key] = val;
	}
	return 0;
}

bool Header::setAllHeaders(const string &key, const string &value)
{

	t_list_header_it it_all_headers = this->list_headers.begin();
	for (; it_all_headers != this->list_headers.end(); ++it_all_headers)
	{
		// std::cout << key << " == ?" << it_all_headers->first << std::endl;
		if (it_all_headers->first == key)
		{
			cerr << "Error : Header '" << key << "' already exist" << std::endl;
			return 1;
		}
	}
	this->list_headers[key] = value;
	return 0;
}

bool Header::parseContentType(const string &value)
{
	string::size_type pos = 0;
	string str(value);
	string line;

	if (str == "")
	{
		this->content_type = "*";
		return 0;
	}
	if ((pos = str.find("boundary=")) != string::npos)
	{
		str.erase(0, pos + 9);
		this->boundary = str;
	}
	this->content_type = value;

	return 0;
}

void Header::reset(void)
{
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
	this->list_headers.clear();
	this->cookies.clear();
}

/* *************************************************** */
/*                                                     */
/*                         BODY                        */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */

Body::Body() : brut_body(), content(""), is_chuncked(false), socket_client(-1)
{
	return;
}

Body::Body(Body const &src)
{
	*this = src;
	return;
}

Body::~Body()
{
	// cerr << "Body destructor" << std::endl;
	return;
}

Body &Body::operator=(Body const &rhs)
{
	if (this != &rhs)
	{
		this->brut_body = rhs.brut_body;
		this->content = rhs.content;
		this->is_chuncked = rhs.is_chuncked;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */

bool Body::parseBody(void)
{
	if (!this->boundary.empty())
	{
		if (parseMultipartBody())
			return 1;
	}
	else
		this->content = this->brut_body;
	return 0;
}

bool Body::parseMultipartBody(void)
{
	string str(this->brut_body);
	string line;
	std::stringstream body_parsed;

	this->content = this->brut_body;
	return 0;
}

void Body::reset(void)
{
	this->brut_body = "";
	this->content = "";
	this->is_chuncked = false;
	this->boundary = "";
}

/* *************************************************** */
/*                                                     */
/*                       REQUEST                       */
/*                                                     */
/* *************************************************** */

/* *************   CONSTRCUTOR   ************* */
Request::Request() : method(Method()), header(Header()), body(Body()), contain_body(false), socket_client(-1), isFinished(false)
{
	for (int i = 0; i < REQ_MAX_SIZE; i++)
		buffer[i] = 0;
	return;
}

Request::Request(Request const &src)
{
	this->method = src.method;
	this->header = src.header;
	this->body = src.body;
	this->socket_client = src.socket_client;
	this->contain_body = src.contain_body;
	this->isFinished = src.isFinished;
	return;
}

Request::~Request()
{
	// cerr << "Request destructor" << std::endl;
	return;
}

Request &Request::operator=(Request const &rhs)
{
	if (this != &rhs)
	{
		this->method = Method(rhs.method);
		this->header = Header(rhs.header);
		this->body = Body(rhs.body);
		this->socket_client = rhs.socket_client;
		this->contain_body = rhs.contain_body;
		this->isFinished = rhs.isFinished;
	}
	return (*this);
}

/* *************   FUNCTION   ************* */
bool Request::splitRequest(string req)
{

	string::size_type ml_pos;
	string::size_type h_pos;
	string::size_type hl_pos;
	string::size_type b_pos;
	string::size_type bl_pos;

	if (req.size() == 0)
	{
		cerr << "Error : request is empty" << std::endl;
		return 1;
	}

	/* split request method */
	ml_pos = req.find("\r\n");
	method.brut_method = req.substr(0, ml_pos);

	/* split header */
	h_pos = ml_pos + 2;
	hl_pos = req.find("\r\n\r\n");

	if (hl_pos == string::npos)
	{
		if (req.find("POST") == 0)
		{
			cerr << "Error : header is not valid, miss empty new line beetween headers and body" << std::endl;
			return 1;
		}
		this->body.brut_body = "";
		this->header.brut_header = req.substr(h_pos);
	}
	else if (hl_pos == req.size() - 4)
	{
		this->body.brut_body = "";
		this->header.brut_header = req.substr(h_pos);
	}
	else
	{
		hl_pos -= ml_pos;
		this->header.brut_header = req.substr(h_pos, hl_pos);

		/* split body */
		b_pos = ml_pos + 2 + hl_pos + 2;
		bl_pos = req.size() - ml_pos - 2 - hl_pos - 4;
		this->body.brut_body = req.substr(b_pos);
		this->contain_body = true;
		this->header.contain_body = true;
	}
	return 0;
}

bool Request::parseRequest(std::vector<char> &req)
{
	size_t header_size = 0;
	if (req.size() < 4)
	{
		cerr << "Error : request is empty" << std::endl;
		return 1;
	}
	char sep[4] = {'\r', '\n', '\r', '\n'};
	for (size_t i = 0; i < req.size() - 3; i++)
	{
		if (req[i] == sep[0] && req[i + 1] == sep[1] && req[i + 2] == sep[2] && req[i + 3] == sep[3])
		{
			header_size = i + 4;
			break;
		}
	}
	std::string header_str = "";
	for (size_t i = 0; i < header_size; i++)
	{
		header_str.push_back(req[i]);
	}
	std::vector<char>::const_iterator end = req.begin() + header_size;
	req.erase(req.begin(), end);
	this->body.vector_body = &req;
	if (splitRequest(header_str) || method.parseMethod() || header.parseHeader())
	{
		return 1;
	}
	this->body.is_chuncked = this->header.is_chuncked;
	this->body.boundary = this->header.boundary;
	// this->printRequest();
	return 0;
}

void Request::printRequest()
{
	cout << "\n\nHeader brut : \n"
			 << this->method.brut_method << std::endl
			 << this->header.brut_header << std::endl;
	if (this->contain_body)
		cout << "Body brut : \n"
				 << this->body.brut_body << std::endl
				 << std::endl;
	//   std::cout << this->method.isGet << std::endl;
	//   std::cout << this->method.isPost << std::endl;
	//   std::cout << this->method.isDelete << std::endl;

	//   std::cout << "url '" << this->method.url << "'" << std::endl;
	//   std::cout << "path '" << this->method.path << "'" << std::endl;
	//   std::cout << "params '" << this->method.parameters << "'" << std::endl;
	//   std::cout << "anchor '" << this->method.anchor <<  "'" << std::endl;
	//   std::cout << "protocle '" << this->method.protocole << "'" << std::endl;
	//   std::cout << "port '" << this->header.port << "'" << std::endl;
	//   std::cout << "host '" << this->header.host << "'" << std::endl;

	// cout << "useragent '" << this->header.str_user_agent << "'" << std::endl;
	// Header::t_user_agent_it it = this->header.user_agent.begin();
	// for(; it != this->header.user_agent.end(); ++it) {
	//   std::cout << "User-Agent "<< it->first << " : " << it->second << std::endl;
	// }

	// cout << "str_accept '" << this->header.str_accepts << "'" << std::endl;
	// Header::t_accepts_it it_accept = this->header.accepts.begin();
	// while (it_accept != this->header.accepts.end()) {
	//   std::cout << "accept '" << it_accept->type << "/"
	//   << it_accept->subtype << "' "
	//   << "q="<< it_accept->q << std::endl;
	//   it_accept++;
	// }

	// cout << "str_accept_language '" << this->header.str_accept_languages << "'" << std::endl;
	// Header::t_languages_it it_lang = this->header.accept_languages.begin();
	// for(; it_lang != this->header.accept_languages.end(); ++it_lang) {
	//   std::cout << "accept_language "<< it_lang->lang
	//   << (it_lang->spec != "" ? "-" : "") << it_lang->spec
	//   << " q=" << it_lang->q << std::endl;
	// }

	// cout << "str_accept_encoding '" << this->header.str_accept_encodings << "'" << std::endl << std::endl;
	// Header::t_encodings_it it_encod = this->header.accept_encodings.begin();
	// for(; it_encod != this->header.accept_encodings.end(); ++it_encod) {
	//   std::cout << "accept_encoding '"<< it_encod->type << "' q=" << it_encod->q << std::endl;
	// }

	// cout << "str cookies : " << this->header.str_cookie << std::endl;
	// Header::t_cookie_it it_cookies = this->header.cookies.begin();
	// for (; it_cookies != this->header.cookies.end(); ++it_cookies)
	// {
	// 	cout << "cookie '" << it_cookies->first << "' : '" << it_cookies->second << "'" << std::endl;
	// }
	// cout << "Connection : " << this->header.connection << std::endl;

	// cout << "content_length '" << this->header.content_length << "'" << std::endl;
	// cout << "content_type '" << this->header.content_type << "'" << std::endl;
	// cout << "content_encoding '" << this->header.content_encoding << "'" << std::endl;
	// cout << "content_language '" << this->header.content_language << "'" << std::endl;
	// cout << "content_location '" << this->header.content_location << "'" << std::endl  << std::endl;

	// cout << "All Header : " << std::endl;
	// Header::t_list_header_it it_all_headers = this->header.list_headers.begin();
	// for (; it_all_headers != this->header.list_headers.end(); ++it_all_headers)
	// {
	// 	cout << it_all_headers->first << " : " << it_all_headers->second << std::endl;
	// }

	// this->contain_body ? std::cout << "body\n" << "'" << this->body.content << "'" << std::endl : std::cout << "no body" << std::endl;
	// cout << "\n\nrequete " << (this->header.is_valid ? "valid" : "invalid") << std::endl;
}

void Request::resetBuffer(void)
{
	for (int i = 0; i < REQ_MAX_SIZE; i++)
	{
		this->buffer[i] = 0;
	}
}

void Request::reset(void)
{
	this->method.reset();
	this->header.reset();
	this->body.reset();
	this->contain_body = false;
	this->resetBuffer();
	this->isFinished = false;
}

bool Request::check_header_buffer(string buffer, Config &config, sockaddr_in &client_net)
{
	Request tmp;

	string::size_type pos = buffer.find("\r\n");
	tmp.method.brut_method = buffer.substr(0, pos);
	buffer.erase(0, pos + 2);
	tmp.method.parseMethod();
	tmp.header.brut_header = buffer;
	tmp.header.parseHeader();

	config.selectServ(client_net.sin_addr.s_addr, ntohs(client_net.sin_port), tmp.header.host);
	if (tmp.header.content_length == "")
		return 0;

	unsigned int nb;
	std::istringstream(tmp.header.content_length) >> nb;

	if (nb > config.getMaxSize())
		return 1;
	return 0;
}

int Request::isFinishedRequest(std::vector<char> const &req)
{

	size_t header_size = 0;
	char sep[4] = {'\r', '\n', '\r', '\n'};
	for (size_t i = 0; i < req.size() - 3; i++)
	{
		if (req[i] && req[i + 1] && req[i + 2] && req[i + 3] && req[i] == sep[0] && req[i + 1] == sep[1] && req[i + 2] == sep[2] && req[i + 3] == sep[3])
		{
			header_size = i;
			break;
		}
	}
	std::string header = "";
	for (size_t i = 0; i < header_size; i++)
	{
		header.push_back(req[i]);
	}

	/* get content-length */
	string::size_type cl_pos = header.find("Content-Length: ");
	if (cl_pos == string::npos)
	{
		if (header.size() == req.size() - 4)
			return 1;
		else
			return 0;
	}
	else
	{
		string::size_type cl_end = header.find("\r\n", cl_pos);
		string content_length = header.substr(cl_pos + 16, cl_end - cl_pos - 16);
		int cl = atoi(content_length.c_str());
		if (cl == 0)
			return 1;
		else if (req.size() - header.length() - 4 == (unsigned int)cl)
			return 1;
		else if (req.size() - header.length() - 4 >= (unsigned int)cl)
		{
			return 2;
		}
	}
	return 0;
}
