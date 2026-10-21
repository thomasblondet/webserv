# webserv

Serveur HTTP non bloquant en C++98.
Réalisé en équipe de 3.

J'ai réalisé la partie réseau : création/destruction des connexions, ainsi que l'analyse des requêtes et la construction des réponses.

Le serveur utilise `poll` pour surveiller plusieurs connexions simultanément.

La configuration se trouve dans ```conf/``` et est inspirée de ```nginx```.

Pour le lancer

```
make run
```
Puis ouvrir ```http://127.0.0.1:8080``` dans un navigateur.