/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jrasser <jrasser@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 17:56:22 by jrasser           #+#    #+#             */
/*   Updated: 2023/01/25 18:49:17 by jrasser          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

#include <iostream>
#include <string>

// #include <iostream>

/*
La taille maximale d'une URI (Uniform Resource Identifier) est de 2048 caractères selon la spécification 
de la norme HTTP/1.1.
La taille maximale des en-têtes de requête est de 8192 octets, selon la même spécification.
La taille maximale du corps de la requête dépend des capacités du serveur, mais généralement elle est de 8 Mo 
pour Apache et de 2 Mo pour IIS.
Il est important de noter qu'il est possible de configurer ces limites en fonction de vos besoins spécifiques 
en utilisant les paramètres de configuration appropriés de votre serveur.
*/

/*
Pour être considérée comme valide, une requête HTTP 1.1 doit respecter les règles définies dans 
la spécification HTTP 1.1 du RFC 7230 à 7235. Les principales exigences sont les suivantes:

La méthode de la requête (GET, POST, etc.) doit être valide et correctement spécifiée.
L'URI de la ressource demandée doit être valide et correctement formatée.
Les en-têtes de la requête doit être valide et correctement formaté.
Le corps de la requête doit être valide s'il est présent.
Le format général de la requête doit respecter la syntaxe HTTP standard.

Il est important de noter que ces exigences ne garantissent pas que la requête sera 
traitée avec succès par le serveur, il est nécessaire de respecter les règles de la spécification 
HTTP pour que la requête soit considérée comme valide.

  //check uri size max 2048 char
  //check header size max 8192 octets
  //check body size max 8Mo
*/



/* *************************************************** */
/*                                                     */
/*                        HEADER                       */
/*                                                     */
/* *************************************************** */

/* ************************************* */
/*               CONSTRCUTOR             */
/* ************************************* */
Request::Header::Header() 
:
  host(""),
  checkedMethod(false),
  checkedUri(false),
  checkedProtocole(false)
{
  method.isGet = false;
  method.isPost = false;
  method.isDelete = false;
  method.path = "";
  method.parameters = "";
  method.anchor = "";
  method.protocole = "";
  user_agent.compatibleMozilla = "";
  user_agent.version = "";
  user_agent.platform = "";
  user_agent.os = "";
  user_agent.rv = "";
  user_agent.geckoVersion = "";
  user_agent.browserName = "";
  user_agent.browserVersion = "";

  // t_accept *accept = new t_accept;
  t_accept    accept;
  t_languages accept_language;
  t_encodings accept_encoding;
}

Request::Header::Header(Header const &src)
{
  *this = src;
  return ;
}

Request::Header::~Header()
{
  return ;
}

Request::Header &Request::Header::operator=(Header const &rhs)
{
  if (this != &rhs)
  {
    this->method = rhs.method;
    this->method.path       = rhs.method.path;
    this->method.parameters = rhs.method.parameters;
    this->method.anchor     = rhs.method.anchor;
    this->method.protocole  = rhs.method.protocole;
    this->host = rhs.host;
    this->user_agent = rhs.user_agent;
    this->accept = rhs.accept;
    this->accept_language = rhs.accept_language;
    this->accept_encoding = rhs.accept_encoding;
  }
  return (*this);
}


/* ************************************* */
/*                FUNCTION               */
/* ************************************* */

void Request::Header::parseHeader(std::string brut_header) {
  std::string line;
  std::string pos1;
  std::string pos2;
  std::string pos3;


  //parse line 1


  if (!checkSyntaxe(line) || !checkMethod(pos1) || !checkUri(pos2) || !checkProtocole(pos3)) {
    std::cerr << "Header is not valid" << std::endl;
  }
}

bool Request::Header::checkSyntaxe(std::string str) {
  return (false);
}

bool Request::Header::checkMethod(std::string str) {

  return (false);
}

bool Request::Header::checkUri(std::string str) {

  return (false);
}

bool Request::Header::checkProtocole(std::string str) {

  return (false);
}



/* *************************************************** */
/*                                                     */
/*                         BODY                        */
/*                                                     */
/* *************************************************** */

/* ************************************* */
/*               CONSTRCUTOR             */
/* ************************************* */

Request::Body::Body() {
  return ;
}

Request::Body::Body(Body const &src) {
  *this = src;
  return ;
}

Request::Body::~Body() {
  return ;
}

Request::Body &Request::Body::operator=(Body const &rhs) {
  if (this != &rhs) {
    this->body = rhs.body;
  }
  return (*this);
}

/* ************************************* */
/*                FUNCTION               */
/* ************************************* */

void Request::Body::parseBody(std::string brut_body) {

}








/* *************************************************** */
/*                                                     */
/*                       REQUEST                       */
/*                                                     */
/* *************************************************** */

Request::Request() : Header(), Body() {
  return ; 
}

Request::Request(Request const &src) {
  *this = src;
  return ;
}

Request::~Request() {
  return ;
}

Request &Request::operator=(Request const &rhs) {
  if (this != &rhs) {
    this->header = rhs.header;
    this->body = rhs.body;
  }
  return (*this);
}


void Request::parseRequest(std::string req) {
  std::cout << "********************* \n" << req << "\n*********************" << std::endl;
  std::cout << "Request Brut size : " << req.size() << std::endl;

  std::string method_line;
  std::string header;
  std::string body;

  std::string::size_type size = req.size();
  std::string::size_type pos;
  std::string::size_type pos_fin_method_line;
  std::string::size_type pos_header;
  std::string::size_type pos_fin_header;
  std::string::size_type pos_body;
  std::string::size_type pos_fin_body;

  pos = req.find("GET");
  if (pos != std::string::npos)
  {
    this->header.method.isGet = true;
    /* split request method */
    method_line = req.substr(pos, req.find("\n"));
    std::cout << "method line : " << method_line << std::endl;

    /* split header */
    pos_fin_method_line = req.find("\n");
    header = req.substr(pos_fin_method_line + 1);
    std::cout << "header : " << header << std::endl;

    // /* split body */
    // body = req.substr(pos_fin_header + 1);
    // std::cout << "body : " << body << std::endl;

    // pos2 = line.find(" ");
    // pos3 = line.find("HTTP");
    // this->path = line.substr(pos2 + 1, pos3 - pos2 - 1);
    // pos4 = this->path.find("?");
    // if (pos4 != std::string::npos)
    // {
    //   this->parameters = this->path.substr(pos4 + 1);
    //   this->path = this->path.substr(0, pos4);
    // }
    // pos5 = this->path.find("#");
    // if (pos5 != std::string::npos)
    // {
    //   this->anchor = this->path.substr(pos5 + 1);
    //   this->path = this->path.substr(0, pos5);
    // }
    // this->protocole = line.substr(pos3 + 1);
  }

  pos = req.find("POST");
  if (pos != std::string::npos)
  {
    this->header.method.isPost = true;
    /* split request method */
    method_line = req.substr(pos, req.find("\n"));
    std::cout << "method line : " << method_line << std::endl;

    /* split header */
    pos_fin_method_line = req.find("\n");
    pos_fin_header = req.substr(pos_fin_method_line + 2).find("\n\n");

    std::cout << "TESTE : " << pos_fin_header << std::endl;

    header = req.substr(pos_fin_method_line + 1, pos_fin_header);
    std::cout << "header : " << header << std::endl;


    /* split body */
    body = req.substr(pos_fin_header + pos_fin_method_line + 4);
    std::cout << "body : " << body << std::endl;
  }


}


/*
  std::string req(req_brut);
  std::string line;
  std::string::size_type pos;
  std::string::size_type pos2;
  std::string::size_type pos3;
  std::string::size_type pos4;
  std::string::size_type pos5;
  std::string::size_type pos6;
  std::string::size_type pos7;
  std::string::size_type pos8;
  std::string::size_type pos9;
  std::string::size_type pos10;
  std::string::size_type pos11;

  pos = req.find("GET");
  if (pos != std::string::npos)
  {
    this->method.isGet = true;
    line = req.substr(pos);
    pos2 = line.find(" ");
    pos3 = line.find("HTTP");
    this->path = line.substr(pos2 + 1, pos3 - pos2 - 1);
    pos4 = this->path.find("?");
    if (pos4 != std::string::npos)
    {
      this->parameters = this->path.substr(pos4 + 1);
      this->path = this->path.substr(0, pos4);
    }
    pos5 = this->path.find("#");
    if (pos5 != std::string::npos)
    {
      this->anchor = this->path.substr(pos5 + 1);
      this->path = this->path.substr(0, pos5);
    }
    this->protocole = line.substr(pos3 + 1);
  }
*/