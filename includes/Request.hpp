/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 13:37:00 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/24 17:33:42 by jrasser          ###   ########.fr       */
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
	std::string _compatibleMozilla;
	std::string _version;
	std::string _platform;
	std::string _os;
	std::string _rv;
	std::string _geckoVersion;
	std::string _browserName;
	std::string _browserVersion;
} t_user_agent;

typedef struct s_language
{
	std::string _language;
	std::string _specificity;
	std::string _q;
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
	typedef std::vector< t_language > 																		t_language;
	typedef std::vector< t_language >::iterator 													t_language_it;
	typedef std::vector< std::pair< std::string, int > > 									t_encoding;
	typedef std::vector< std::pair< std::string, int > >::iterator 				t_encoding_it;

	Request();
	Request(Request const& src);
	~Request();
	Request &operator= (Request const& src);
	
	void parse(char* brut_request);

	void setMethod(std::string str_method) {
		if (str_method == "GET")
			method.isGet = true;
		else if (str_method == "POST")
			method.isPost = true;
		else if (str_method == "DELETE")
			method.isDelete = true;
	}

	t_methods			method;
	std::string   str_method;
	std::string 	path;
	std::string 	parameters;
	std::string 	anchor;
	std::string 	protocole;
	std::string 	host;
	t_user_agent 	user_agent;
	t_accept 			accept;
	t_language 		accept_language;
	t_encoding 		accept_encoding;

};


#endif