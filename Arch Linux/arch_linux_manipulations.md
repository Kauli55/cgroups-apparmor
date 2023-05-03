On commence par installer libcgroup.

Instructions à cette adresse : https://aur.chaotic.cx/

Cela nous permet d'utiliser la library libcgroup qui permet une meilleure visualisation des cgroups. (lssubsys -am)

Sur notre arch linux, nous souhaitons utiliser d'abord systemd pour manipuler les cgroups, puis la library libcgroup.

# systemd

Premièrement, si nous souhaitons utiliser systemd pour gérer les cgroups, il faudra effectuer nos opérations dans un répertoire ayant comme chemin /etc/systemd/system

On s'intéressera déjà aux slices et aux services.

Les slices vont mettre une limitations aux services/scopes exécutés à travers eux.
Il est possible qu'un service ait plus de limitations que précisées dans le .slice : ce sont des limitations qui sont rajoutées au service.

cd /etc/systemd/system

nano test.slice (ou équivalent, l'important est de créer le fichier test.slice puis de l'éditer)

On rajoute dans le fichier 'test.slice' :

[Slice]
CPUQuota=30%

Cela veut dire que tous ce qui sera executé dans test.slice aura un quota maximal de CPU de 30%.
Il ne sera pas possible de le dépasser dans ce slice.

Il est possible de lancer des scripts dans un slice particulier. Dans ce cas là, on utilisera la commande : systemd-run --slice=test.slice _commandes_

