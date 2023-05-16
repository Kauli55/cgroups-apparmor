Nous allons maintenant nous intéresser aux manipulations des cgroups sur libvirt et sur LXC.

Nous commencerons par LXC.

# LXC

Nous installerons LXC puis le configurerons selon la page accessible à l'adresse : https://wiki.archlinux.org/title/Linux_Containers

Une particularité est que le bridge par défaut doit être créé par l'intermédiaire de la commande : "systemctl start lxc-net".
Une autre particularité est que nous devons aller dans le fichier '/etc/default/lxc' et mettre le champ "USE_LXC_BRIDGE" à la valeur "true".

Nous installerons une machine "Ubuntu lunar amd64" à l'aide de la commande : "lxc-create -t download -n _nomContainer_"

Notre nom du container sera "root-container".

Si nous souhaitons créer un container non privilégié pour un utilisateur, alors nous utiliserons la commande : 'systemd-run --user --scope -p "Delegate=yes" lxc-create -t download -n _nomContainer_'

Dans cette commande, l'arguemnt "Delegate=yes" permet d'hériter et d'utiliser des contrôleurs déjà monté autre part.
LXC utilise donc aussi les cgroups, ce qui n'est pas très étonnant puisque les cgroups permettent de limiter et d'isoler certaines fonctionnalités.

Une fois que nous lançons le container avec la commande "lxc-start -n _nomContainer_", nous pouvons nous connecter avec lxc-attach.
Nous utiliserons à l'intérieur la commande "mount" et nous pouvons voir que cgroup2 est monté.

Si nous allons sur le répertoire '/sys/fs/cgroup', nous pouvons voir que nous sommes vraisemblablement bien avec un cgroup v2 puis que les contrôleurs ont la syntaxe de cgroups v2.

Pour vérifier que cgroups fonctionne dans le container, nous allons créer plusieurs processus différents qui voudront envoyer des données à l'hôte.
Nous utiliserons le contrôleur de cgroupv1 "net_prio" afin de réaliser ceci.

Créons donc un répertoire 'net_prio' dans le dossier "/sys/fs/cgroup/".
Si nous essayons la commande "mount -t cgroup -o net_prio net_prio /sys/fs/cgroup/net_prio", nous récupérons l'erreur suivante :

![Erreur récupérée lors de la tentative de mount](./ErreurNetPrio.PNG)

Si nous utilisons la commande lxc "lxc-cgroup", il est possible de modifier la valeur du répertoire "/sys/fs/cgroup" de notre container.
Cependant, ces changements sont temporaires et devront être refait à chaque lancement du container.

Si nous souhaitons ajouter un contrôleur non disponible normalement dans le container, il faudra alors faire des manipulations supplémentaires.
Dans notre cas avec le contrôleur "net_prio", il doit d'abord être monté sur notre machine hôte avant de pouvoir le monter sur notre container.

Plaçons nous dans le répertoire "/sys/fs/cgroup" et créons un sous-cgroups : "mkdir test_netPrio | mount -t cgroup -o net_prio test /sys/fs/cgroup/test_netPrio".

Une fois cela fait, nous allons dans notre répertoire "/var/lib/lxc/_nomContainer_/".
Nous modifions ensuite le fichier "config" afin de pouvoir écrire dans le dossier "/sys/fs/cgroup" du container.

/var/lib/lxc/_nomContainer_/config
```
...
lxc.mount.auto = "proc:rw sys:rw cgroup:rw:force"
...
```

Cette modification va permettre de lire et écrire dans le répertoire des cgroups sans avoir de problèmes d'autorisations.