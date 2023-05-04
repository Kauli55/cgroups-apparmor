On commence par installer libcgroup.

Instructions à cette adresse : https://aur.chaotic.cx/

Cela nous permet d'utiliser la library libcgroup qui permet une meilleure visualisation des cgroups. (lssubsys -am)

Sur notre arch linux, nous souhaitons utiliser d'abord systemd pour manipuler les cgroups, puis la library libcgroup.

# systemd

Premièrement, si nous souhaitons utiliser systemd pour gérer les cgroups, il faudra effectuer nos opérations dans un répertoire ayant comme chemin /etc/systemd/

Si nous nous mettons ensuite dans le dossier /system/ de systemd, alors nos services seront lancés au démarrage du système si activés.
Si nous nous mettons dans le dossier /user/ de systemd, alors nos services seront lancés à la connexion d'un utilisateur.

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

Créons d'abord un simple service qui exécutera un code PHP.
Ce code PHP ne sera compris que d'un simple 'echo' afin de vérifier si le système marche.
Il nous faudra donc un code php, que nous appellerons "index.php" et un fichier .service qui sera situé dans le répertoire /etc/systemd/system/
Ce fichier de service permettra de préciser comment le service doit démarrer, après quel autre service, quels sont les limitations qui lui sont imposées,...
Nous allons aussi pouvoir exécuter un service dans un slice donné.
Toutes les limitations du slice seront donc appliquées au service concerné.
Il est possible également de rajouter d'autres limitations uniques au service.
Il est important de remarquer que le code du service, c'est-à-dire le fichier qui n'est pas un .service, n'a pas besoin d'être dans le même répertoire que son fichier .service.
Il n'a d'ailleurs pas besoin d'être dans un sous-dossier de systemd.

On placera donc dans cet exemple notre fichier "index.php" dans le répertoire /home/test_cgroups/
Son code est le suivant :
index.php
---
<?php

$a="Bienvenue sur l'Arch Linux.";
echo"$a\n";
---

Ce code ne sert qu'à produire un affichage indiquant : Bienvenue sur l'Arch Linux.

Le code du fichier "test_php.service" situé dans le répertoire '/etc/systemd/system' est le suivant:
test_php.service
---
[Unit]
Description=Service de test exécutant un code php

[Service]
Type=simple
Slice=test.slice
MemoryHigh=500M
MemoryMax=800M

ExecStart=/usr/bin/php /home/test_cgroups/index.php
Restart=always

[Install]
WantedBy=multi-user.target
---

Pour une briève explication du fichier : 
Description --> La description du service. Sera affichée quand nous demanderons le status du service.
Type --> Le type de service et comment va-t-il être démarré automatiquement. Simple équivaut à un démarrage dès que possible.
Slice --> Dans quel slice ce service sera exécuté.
MemoryHigh --> Quelle est la limite en mémoire du service. Si le service a absolument besoin de dépasser cette limite, alors ce sera possible mais le service sera ralentit et la mémoire sera récupérée le plus rapidement possible.
MemoryMax --> La limite en mémoire du service. C'est une limite qui n'est pas possible de dépasser.
ExecStart --> Champ obligatoire. Indique la commande que le service doit exécutée pour démarrer.
Restart --> Si plantage, comment le service doit-il redémarrer ? Par défaut, le service essaiera de redémarrer plusieurs fois avant d'arrêter le service.
WantedBy=multi-user.target --> Permet au service d'être lancé automatiquement quand le système a terminé de démarrer. Sans cette ligne, même "systemdctl enable test_php" ne permettrait pas l'exécution automatique du service au démarrage du système.

Mettons à jour la liste des services à l'aide de la commande : "systemctl daemon-reload"
On lance ensuite le service avec la commande : "systemctl start test_php"
Pour voir ce qu'a donné le service, il suffit de faire la commande : "systemctl status test_php"

On peut remarquer que notre service possède l'était "failed" et qu'il a essayé de redémarrer plusieurs fois avant de s'arrêter.
En enlevant la ligne "Restart=always" et en meettant à jour la liste des services, notre service s'exécute bien et s'arrête bien.
Cela est dû au fait que nous souhaitions arrêter le service après l'exécution de son code.
Comme le service s'arrête, le système essaie de le redémarrer mais il n'y a plus de code à exécuter.
Si nous souhaitons que le service s'exécute au démarrage du système, alors il faudra utiliser la commande "git enable test_php".

Il est utile de noter que la manipulation manuelle des cgroups, c'est-à-dire par la modifications des fichiers cpu,memory,... d'un cgroups est différent dans un système sous systemd.
En effet, systemd va monter tous les contrôleurs dans le dossier /sys/fs/cgroup/
Si nous souhaitons créer un cgroup utilisant un de ces contrôleurs, alors il faudra soit démonter le contrôleur de ce dossier, soit créer le cgroup dans ce dossier.

Si nous faisons juste la commande : "mount -t cgroup -o cpu _nom_ _chemin/vers/cgroup_", alors il y a de fortes chances qu'une erreur apparaîsse expliquant que le mount point est occupé.