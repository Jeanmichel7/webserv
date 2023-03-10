/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lomasson <lomasson@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 09:47:09 by ydumaine          #+#    #+#             */
/*   Updated: 2023/03/10 16:03:33 by lomasson         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <cstdio>
#include <algorithm>
#include <stdio.h>


struct Sbuffer;
class Config;
struct Request;
class CGI
{
	public :
		CGI();
		void build(Config &conf, const Request &req, struct sockaddr_in const &client_addr);
		static void execute_cgi(Config &config, Sbuffer &client, struct sockaddr_in const &client_addr);
		static std::vector<char> launchProcess(Sbuffer &client,  Config &config, struct sockaddr_in const &client_addr);
		static void handleProcessResponse(Sbuffer &client);
		static std::vector<char> error_500();
		static std::vector<char> error_404();
		~CGI();
		std::string _body;
		char **_env; 
		char **_arg;
		std::FILE *_file_stdin;
		std::FILE *_file_stdout;
		int _fd_stdin;
		int _fd_stdout;
		size_t _readed;
		bool _cgi_process_body_ready;
};

#endif