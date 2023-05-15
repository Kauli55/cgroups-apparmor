Nous allons maintenant nous intéresser aux manipulations des cgroups sur libvirt et sur LXC.

Nous commencerons par LXC.

# LXC

Nous installerons LXC puis le configurerons selon la page accessible à l'adresse : https://wiki.archlinux.org/title/Linux_Containers

Une particularité est que le bridge par défaut doit être créé par l'intermédiaire de la commande : "systemctl start lxc-net".
Une autre particularité est que nous devons aller dans le fichier '/etc/default/lxc' et mettre le champ "USE_LXC_BRIDGE" à la valeur "true".

Nous installerons une machine "Ubuntu lunar amd64" à l'aide de la commande : "lxc-create -t download -n _nomContainer_"

Notre nom du conteneur sera "root-container".

Une fois que nous lançons le conteneur avec la commande "lxc-start -n _nomContainer_", nous pouvons nous connecter avec lxc-attach.
Nous utiliserons à l'intérieur la commande "mount" et nous pouvons voir que cgroup2 est monté.

Si nous allons sur le répertoire '/sys/fs/cgroup', nous pouvons voir que nous sommes vraisemblablement bien avec un cgroup v2 puis que les contrôleurs ont la syntaxe de cgroups v2.

Pour vérifier que cgroups fonctionne dans le conteneur, nous allons créer plusieurs processus différents qui voudront envoyer des données à l'hôte.
Nous utiliserons le contrôleur de cgroupv1 "net_prio" afin de réaliser ceci.

Créons donc un répertoire 'net_prio' dans le dossier "/sys/fs/cgroup/".
Si nous essayons la commande "mount -t cgroup -o net_prio net_prio /sys/fs/cgroup/net_prio", nous récupérons l'erreur suivante :

![Erreur récupérée lors de la tentative de mount](./ErreurNetPrio.PNG)

