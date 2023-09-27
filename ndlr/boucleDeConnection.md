
### boucle de connection

1. erreur
    > delete node de request correspondant au event.data.fd 
    > delete node de reponse correspondant au event.data.fd 
    > close event.data.fd

2. nouvelle connection
    > accept connection
    > setsockopt
    > add socket to epoll

3. bout de requete a lire
    > read from client
        > recv dans vecteur de chars
            > si vide : return
            > add chunk request
                > add a la map des chunks = recherche du fd en tant que cle
                    > s'il y est : append le vecteur au vecteur existant
                    > sinon : creer un node
                        > si isEncoding (cf requete) : traitement specifique. a voir
                        > sinon : normal
            > si la requete est terminee / pas morcelee : 
                > reponse
                > edit socket in epoll --> EPOLLOUT
                > delete node 
    /!\ catch les erreurs de  sorte a close fd + delete node 

4. bout de reponse a envoyer

