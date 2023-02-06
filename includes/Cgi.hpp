/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 09:47:09 by ydumaine          #+#    #+#             */
/*   Updated: 2023/02/06 12:09:48 by ydumaine         ###   ########.fr       */
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
		void build(char *const scriptName, const Config &conf, const request &req);
		static std::string execute_cgi(std::string const &request_content, std::string const &path, char *const scriptName, Config const &config, Request const &req);
		~CGI();
		std::string _body;
		char **_env; 
		char **_arg;
		std::FILE *_tmpf;
		int _fd;
};

#endif