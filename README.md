# Webserv - 42 School Project

## 📋Résumé

Ce projet est une implémentation d'un serveur HTTP en C++ 98. Il vise à fournir un aperçu de la complexité sous-jacente derrière le protocole HTTP, qui est la base de la communication de données pour le World Wide Web.

## 💻Technologies Utilisées

- C++ 98
- Sockets
- POSIX API

## 📦Installation

```bash
git clone https://github.com/Jeanmichel7/webserv.git
cd webserv
make
```

## 📜Règles du projet

- Le code doit être compilé avec les flags `-Wall -Wextra -Werror`.
- Pas d'utilisation de bibliothèques externes.
- Les fonctions externes autorisées sont limitées à la norme C++ 98 et certaines fonctions POSIX.
- Le serveur ne doit jamais bloquer et doit être capable d'écouter sur plusieurs ports.
- Il doit être compatible avec les navigateurs web majeurs.

## 🛠️Utilisation

1. Lancez le serveur avec le fichier de configuration:

```bash
./webserv config/default.conf
```

2. Ouvrez votre navigateur web et allez à `http://localhost:4241`, port configuré dans le fichier de configuration.

### Méthodes HTTP supportées

- GET
- POST
- DELETE

## 📝Note pour les utilisateurs Unix
Ne fonctionne que sur MacOS en raison de l'utilisation de sys/event.


## 📃Fichier de configuration

Le fichier de configuration permet de:

- Choisir le port et l'host de chaque "serveur".
- Définir les pages d'erreur par défaut.
- Limiter la taille du body des clients.
- Configurer diverses routes avec diverses règles.
