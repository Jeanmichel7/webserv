/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:37:00 by jrasser           #+#    #+#             */
/*   Updated: 2023/03/01 19:59:02ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

using namespace std;

# include "server.hpp"

class Config;




struct Method {
	string 	brut_method;
	bool		isGet;
	bool		isPost;
	bool		isDelete;
	bool		isValid;
	string	type;
	string	url;
	string	path;
	string	parameters;
	string	anchor;
	string	protocole;

	Method();
	Method(Method const& src);
	~Method();
	Method &operator= (Method const& src);

	bool parseMethod( void );
	bool checkType( void);
	bool checkUri( void );
	bool checkProtocole( void );
	void reset( void );
};



struct Header {

	typedef struct s_accept
	{
		string 	type;
		string 	subtype;
		string 	q;
	} t_accept;

	typedef struct s_accept_language
	{
		string lang;
		string spec;
		string q;
	} t_accept_language;

	typedef struct s_accept_encoding
	{
		string 	type;
		string	q;
	} t_accept_encoding;

	typedef map< string, string > 									t_user_agent;
	typedef map< string, string >::const_iterator 					t_user_agent_it;
	typedef vector< t_accept > 										t_accepts;
	typedef vector< t_accept >::const_iterator 						t_accepts_it;
	typedef vector< t_accept_language > 							t_languages;
	typedef vector< t_accept_language >::const_iterator 			t_languages_it;
	typedef vector< t_accept_encoding > 							t_encodings;
	typedef vector< t_accept_encoding >::const_iterator 			t_encodings_it;
	typedef map< const string, string > 							t_list_header;
	typedef map< const string, string >::const_iterator 			t_list_header_it;
	typedef map< const string, string >					 			t_cookie;
	typedef map< const string, string >::const_iterator 			t_cookie_it;

	string 				brut_header;
	bool 				contain_body;
	bool 				is_valid;
	string 				host;
	string				host_ip;
	string				port;
	t_user_agent 		user_agent;
	string				str_user_agent;
	t_accepts 			accepts;
	string				str_accepts;
	t_languages 		accept_languages;
	string				str_accept_languages;
	t_encodings 		accept_encodings;
	string				str_accept_encodings;
	bool 				connection;
	string				str_cookie;
	t_cookie			cookies;

	string				content_type;
	string				content_length;
	string				content_encoding;
	string				content_language;
	string				content_location;
	bool 				is_chuncked;
	string				boundary;

	t_list_header 	list_headers;

	Header();
	Header(Header const& src);
	~Header();
	Header &operator= (const Header &src);
	
	bool parseHeader( void );
	bool checkHeaderKey(const string &key);
	bool checkHeaderValue(const string &value);
	bool checkHostValue(const string &host );
	bool checkSyntaxeTag(const string &host, const string &tag);
	bool parseUserAgent(const string &user_agent);

	bool parseAccept(const string &accept);
	bool checkSyntaxeAccept(const string &accept);
	void setAccepts(string &accept);

	bool parseAcceptLanguage(const string &accept_language);
	bool checkSyntaxeAcceptLanguage(const string &);
	void setAcceptLanguages(const string &);

	bool parseAcceptEncodings(const string &);
	bool checkSyntaxeAcceptEncoding(const string &line);
	void setAcceptEncodings(const string &);

	bool parseContentLength(const string &);
	// bool setContentLength(const string &);

	bool parseContentType(const string &);
	bool parseCookies(const string &value);

	bool setAllHeaders(const string &, const string &);
	void reset( void );
};



struct Body {

	Body();
	Body(Body const& src);
	~Body();
	Body &operator= (Body const& src);

	string 	brut_body;
	std::vector<char> *vector_body;
	string	content;
	// string  concat_body;
	bool 		is_chuncked;
	// string 	chuncked_size;
	string 	boundary;
	// Request	req;
	int		socket_client;

	bool 	parseBody( void );
	bool 	parseChuncked( void );
	bool 	parseMultipartBody( void );
	void	reset( void );
};



struct Request {

	Method 		method;
	Header 		header;
	Body 		body;
	bool 		contain_body;
	char		buffer[8192];
	int 		socket_client;
	bool		isFinished;
	bool		isChunked;
	
	bool splitRequest(string req);

	Request();
	Request(Request const& src);
	~Request();
	Request &operator= (Request const& src);

	bool parseRequest(std::vector<char> &req);
	void printRequest( void );
	void resetBuffer( void );
	void reset( void );
	// bool isFinishedRequest( string const &buff );
	bool isFinishedRequest(std::vector<char> const &req, unsigned int octet_read);
	bool check_header_buffer(string buffer, Config & config);
};

#endif