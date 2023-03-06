/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 15:20:31 by jrasser           #+#    #+#             */
/*   Updated: 2023/03/06 19:12:42 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# define REQ_MAX_SIZE 8192 
# define MAX_REQUESTS 1024



# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/ip.h>
# include <arpa/inet.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <sys/event.h>
# include <netdb.h>

# include <algorithm>
# include <iostream>
# include <string>
# include <cstring>
# include <ctime>
# include <sstream>
# include <vector>
# include <exception>
# include <cerrno>
# include <map>
# include <fstream>
# include <cstdio>
# include <cstdlib>
# include <dirent.h>


# include "Config.hpp"
# include "Cgi.hpp"
# include "Request.hpp"


# include "Settings.hpp"
#endif