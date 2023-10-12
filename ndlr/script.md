
Requete GET:
- /
- /wrongpath
- /data/images/favicon.ico


Requete POST:
- normale
- qui upload le fichier /mnt/nfs/homes/nfauconn/repos/webserv/www/testwebsite.com/data/images/favicon.ico vers /mnt/nfs/homes/nfauconn/repos/webserv/www/testwebsite.com/data/upload/
- qui post avec un body de taille superieure a 1M

Requete DELETE:
- qui delete le fichier /mnt/nfs/homes/nfauconn/repos/webserv/www/nyah

Request UNKNOWN to verify no crash if bad request