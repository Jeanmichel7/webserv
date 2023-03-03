/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 09:47:09 by ydumaine          #+#    #+#             */
/*   Updated: 2023/03/02 19:16:24 by ydumaine         ###   ########.fr       */
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
#include "Config.hpp"
#include "Request.hpp"


class CGI
{
	public :
		CGI();
		void build(Config &conf, const Request &req, struct sockaddr_in const &client_addr);
		static std::vector<char> execute_cgi(Config &config, Request const &req, struct sockaddr_in const &client_addr);
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
};

#endif