/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:37:00 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/28 15:46:57 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

using namespace std;

# include "server.hpp"

struct Method {
	string brut_method;
	bool isGet;
	bool isPost;
	bool isDelete;
	bool isValid;
	string type;
	string url;
	string path;
	string parameters;
	string anchor;
	string protocole;

	Method();
	Method(Method const& src);
	~Method();
	Method &operator= (Method const& src);

	bool parseMethod( void );
	bool checkType( void);
	bool checkUri( void );
	bool checkProtocole( void );
};


struct Header {
	typedef struct s_user_agent
	{
		bool		compatibleMozilla;
		string	version;
		string	platform;
		string	os;
		string	rv;
		string	geckoVersion;
		string	browserName;
		string	browserVersion;
	} t_user_agent;

	typedef struct s_language
	{
		string language;
		string specificity;
		string q;
	} t_language;

	typedef vector< pair< string, string > > 						t_accept;
	typedef vector< pair< string, string > >::iterator 	t_accept_it;
	typedef vector< t_language > 												t_languages;
	typedef vector< t_language >::iterator 							t_languages_it;
	typedef vector< pair< string, int > > 							t_encodings;
	typedef vector< pair< string, int > >::iterator 		t_encodings_it;

	string 				brut_header;
	bool 					is_valid;
	string 				host;
	t_user_agent 	user_agent;
	string				str_user_agent;
	t_accept 			accept;
	string				str_accept;
	t_languages 	accept_language;
	string				str_accept_language;
	t_encodings 	accept_encoding;
	string				str_accept_encoding;
	bool 					keep_alive;
	
	string 	content_type;
	string 	content_length;
	string 	content_encoding;
	string 	content_language;
	string 	content_location;
	
	// string 	connection;
	// string 	authorization;
	// string 	cookie;
	// string 	origin;
	
	Header();
	Header(Header const& src);
	~Header();
	Header &operator= (Header const& src);
	
	bool parseHeader( void );
	bool checkHeaderKey(string key);
	bool checkHeaderValue(string value);
	bool checkHostValue( string host );
	bool checkSyntaxeTag(string host, string tag);
	bool parseUserAgentValue( string user_agent );
};


struct Body {
	typedef map< string, string > 							t_body;
	typedef map< string, string >::iterator 		t_body_it;

	Body();
	Body(Body const& src);
	~Body();
	Body &operator= (Body const& src);

	string 		brut_body;
	t_body 		body;

	bool parseBody( void );
};


struct Request
{

	Method 		method;
	Header 		header;
	Body 			body;
	
	bool splitRequest(string req);


	// Method 		getMethod() const { return (this->method);}
	// Header 		getHeader() const { return (this->header);}
	// Body 			getBody() 	const { return (this->body);}

	Request();
	Request(Request const& src);
	~Request();
	Request &operator= (Request const& src);

	// bool checkRequest(string method, string header, string body);
	bool parseRequest(string brut_request);
};

#endif