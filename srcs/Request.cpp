/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ydumaine <ydumaine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/31 08:37:33 by ydumaine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
// #include <iostream>

Request::Request()
:
  str_method(""),
  path(""),
  parameters(""),
  anchor(""),
  protocole(""),
  host(""),
  user_agent(),
  accept(),
  accept_language(),
  accept_encoding()
{
  method.isGet = false;
  method.isPost = false;
  method.isDelete = false;
  return ;
}

Request::Request(Request const &src)
{
  *this = src;
  return ;
}

Request::~Request()
{
  return ;
}

Request &Request::operator=(Request const &rhs)
{
  if (this != &rhs)
  {
    this->method = rhs.method;
    this->path = rhs.path;
    this->parameters = rhs.parameters;
    this->anchor = rhs.anchor;
    this->protocole = rhs.protocole;
    this->host = rhs.host;
    this->user_agent = rhs.user_agent;
    this->accept = rhs.accept;
    this->accept_language = rhs.accept_language;
    this->accept_encoding = rhs.accept_encoding;
  }
  return (*this);
}

Request Request::parse(char *req_brut) {
  Request req;
  std::cout << "parsing request" << std::endl;

  std::cout << "Request Brut : \n" << req_brut << std::endl;
  std::cout << "Request Brut size : " << strlen(req_brut) << std::endl;











  return req;
}