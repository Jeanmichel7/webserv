/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:37:00 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/24 21:03:14 by jrasser          ###   ########.fr       */
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

typedef struct s_user_agent
{
	std::string compatibleMozilla;
	std::string version;
	std::string platform;
	std::string os;
	std::string rv;
	std::string geckoVersion;
	std::string browserName;
	std::string browserVersion;
} t_user_agent;

typedef struct s_language
{
	std::string language;
	std::string specificity;
	std::string q;
} t_language;

typedef struct s_methods
{
	bool isGet;
	bool isPost;
	bool isDelete;
} t_methods;

class Request
{
public:
	typedef std::vector< std::pair< std::string, std::string > > 						t_accept;
	typedef std::vector< std::pair< std::string, std::string > >::iterator 	t_accept_it;
	typedef std::vector< t_language > 																		t_languages;
	typedef std::vector< t_language >::iterator 													t_languages_it;
	typedef std::vector< std::pair< std::string, int > > 									t_encodings;
	typedef std::vector< std::pair< std::string, int > >::iterator 				t_encodings_it;

	struct Header
	{
		t_methods 		method;
		std::string 	path;
		std::string 	parameters;
		std::string 	anchor;
		std::string 	protocole;
		std::string 	host;
		t_user_agent 	user_agent;
		t_accept 			accept;
		t_languages 	accept_language;
		t_encodings 	accept_encoding;

		std::string 	connection;
		std::string 	authorization;
		std::string 	cookie;
		std::string 	origin;
		
		std::string 	content_type;
		std::string 	content_length;
		std::string 	content_encoding;
		std::string 	content_language;
		std::string 	content_location;

		Header();
		Header(Header const& src);
		~Header();
		Header &operator= (Header const& src);
	};

	typedef std::map< std::string, std::string > 							t_body;
	typedef std::map< std::string, std::string >::iterator 		t_body_it;

private:
	Header 				header;
	t_body 				body;


public:
	Header 		getHeader() const { return (this->header);}
	t_body 		getBody() const 	{ return (this->body);}

	void parseRequest(const char* brut_request);
	void setHeader(Header header);
	void setBody(t_body body);

	// void printHeader();
	// void printBody();
	// void printRequest();






	Request();
	Request(Request const& src);
	~Request();
	Request &operator= (Request const& src);
	



};



#endif