/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:37:00 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/26 00:41:54 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "server.hpp"

// GET /home.html?name=john#title3 HTTP/1.1
// Host: developer.mozilla.org
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:50.0) Gecko/20100101 Firefox/50.0
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
// Accept-Language: en-US,en;q=0.5
// Accept-Encoding: gzip, deflate, br
// Connection: keep-alive

struct Method {
	bool isGet;
	bool isPost;
	bool isDelete;
	bool isValid;
	string method;
	string path;
	string parameters;
	string anchor;
	string protocole;

	Method();
	Method(Method const& src);
	~Method();
	Method &operator= (Method const& src);

	bool parseMethod(string brut_method);
	bool checkMethod(string method);
	bool checkUri(string uri);
	bool checkProtocole(string protocole);
};

struct Header {
	typedef struct s_user_agent
	{
		string compatibleMozilla;
		string version;
		string platform;
		string os;
		string rv;
		string geckoVersion;
		string browserName;
		string browserVersion;
	} t_user_agent;

	typedef struct s_language
	{
		string language;
		string specificity;
		string q;
	} t_language;

	typedef vector< pair< string, string > > 							t_accept;
	typedef vector< pair< string, string > >::iterator 		t_accept_it;
	typedef vector< t_language > 													t_languages;
	typedef vector< t_language >::iterator 								t_languages_it;
	typedef vector< pair< string, int > > 								t_encodings;
	typedef vector< pair< string, int > >::iterator 			t_encodings_it;

	string 				host;
	t_user_agent 	user_agent;
	t_accept 			accept;
	t_languages 	accept_language;
	t_encodings 	accept_encoding;
	
	string 	connection;
	string 	authorization;
	string 	cookie;
	string 	origin;
	
	string 	content_type;
	string 	content_length;
	string 	content_encoding;
	string 	content_language;
	string 	content_location;
	
	bool 					checkedSyntaxe;
	bool 					checkedMethod;
	bool 					checkedUri;
	bool 					checkedProtocole;
	
	Header();
	Header(Header const& src);
	~Header();
	Header &operator= (Header const& src);
	
	void parseHeader(string brut_method, string brut_header);
	bool checkSyntaxe(string request);
	bool checkMethod(string method);
	bool checkUri(string uri);
	bool checkProtocole(string protocole);
};


struct Body {
	typedef map< string, string > 							t_body;
	typedef map< string, string >::iterator 		t_body_it;

	Body();
	Body(Body const& src);
	~Body();
	Body &operator= (Body const& src);

	void parseBody(string brut_body);

	t_body 				body;
};


class Request : public Method, public Header, public Body
{

private:
	Method 		method;
	Header 		header;
	Body 			body;
	
	bool splitRequest(string req, string &method, string &header, string &body);


public:
	Method 		getMethod() const { return (this->method);}
	Header 		getHeader() const { return (this->header);}
	Body 			getBody() 	const { return (this->body);}

	Request();
	Request(Request const& src);
	~Request();
	Request &operator= (Request const& src);

	bool checkRequest(string method, string header, string body);
	bool parseRequest(string brut_request);
};

#endif