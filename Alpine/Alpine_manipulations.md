OpenRC n'active pas les cgroups de base.
Pour les activer, il faudra modifier les valeurs dans le fichier '/etc/rc.conf'.
Beaucoup d'options sont indiqués : nous pouvons activer soit cgroups v1, soit cgroups v2, soit les deux.

Les options pour les services sont aussi indiqués dans ce fichier.
Si des valeurs sont initialisées dans rc.conf, alors ces changements seront répercutés dans tous les cgroups.
Le comportement des cgroups est assez similaire à systemd : nous manipulerons principalement des services qui seront dans ces cgroups.

/etc/rc.conf
```
rc_controller_cgroups="YES"
rc_cgroup_mode="hybrid"
rc_cgroup_controllers="cpuset cpu io memory hugetlb pids"
```

Si les cgroups v1 et v2 ne sont pas présents dans le répertoire '/sys/fs/cgroup/', alors nous pouvons lancer manuellement la création.
Pour se faire, il suffit d'aller dans le répertoire '/etc/init.d/'.
Nous lancerons ensuite la commande : "./cgroups start"
De manière générale avec openRC, nous lancerons un service à l'aide d'une commande ressemblant à : "/etc/init.d/_nomService_ start"
Il est aussi possible d'utiliser la commande : "rc-service _nomService_ start"

Si nous souhaitons que les cgroups soient lancés et montés au démarrage, alors nous pourrons utiliser la commande : "rc-update add cgroups".

Pour faire le lien entre un cgroups et un service, OpenRC lie le répertoire '/etc/conf.d' et le répertoire '/etc/init.d'.
Deux fichiers ayant le même nom seront liés.

Créons d'abord un code php qui permettra de simuler une charge sur le CPU.
Nous reprendrons le code dans le répertoire Arch Linux de ce git.
Ce code sera placé dans le répertoire '/home/test_cgroups/' et gardera le nom de fichier 'charge_cpu.php'.

Créons maintenant un service simple qui se contentera d'exécuter notre code php.
Ce service doit être placé dans le répertoire '/etc/init.d/'

/etc/init.d/testCpu
```
#!/sbin/openrc-run

command=/usr/bin/php
command_args=/home/test_cgroups/charge_cpu.php
```

"openrc-run" est un shell personnalisé qui ne nécessite que les variables 'command' et 'command_args' pour pouvoir fonctionner correctement.
Dans la variable 'command', nous mettons la commande que nous voulons exécuter.
Dans la variable 'command_args', nous mettons les arguments de la commande que nous avons préciser avant.

Pour pouvoir exécuter notre service, il faudra changer les droits afin que le service soit exécutable par tout le monde.
Nous utiliserons donc la commande : "chmod u+x testCpu".

Enfin, nous allons créer un fichier dans le répertoire '/etc/conf.d' afin de mettre en place les limitations.
La variable CPUQuota n'existant pas sous OpenRC, nous devrons écrire le nom des variables correspondants.
Nous manipulerons ici la valeur de cpu.max afin de réduire légèrement les capacités du CPU.

Créons donc le fichier 'testCpu' dans le répertoire '/etc/conf.d' :

/etc/conf.d/testCpu
```
rc_cgroup_settings="
    cpu.max 5000 10000
"
```

Il est évidemment possible de rajouter plus de variables à la suite de celle-ci, par exemple nous pourrions aussi limiter la mémoire en ajoutant : "memory.max 5000" en dessous de "cpu.max".
Une fois ceci fait, nous pouvons reboot la machine afin que les changements soient pris en compte.

Quand nous relançons le service, nous pouvons constater que le service prend plus de temps à finir son exécution.
Les limitations ont donc bien été mises.

Idée pour grouper plusieurs services dans même cgroups --> un service démarrant d'autres services.
Problème : tous les services seront exécutés en même temps et dans le même ordre.

Il y a toujours l'option de créer manuellement un cgroup, mais il ne sera pas créé de façon permanente.

# Manipulation de Cgroup v1

cgroup v1 et cgroup v2 reposent sur les mêmes principes mais les exécutent de manières différentes.
D'abord, il y a moins de contrôleurs pour cgroup v2 par rapport à cgroup v1.
Cela ne veut pas dire que nous ne pouvons utiliser que l'un ou l'autre.
En effet, il est possible de les utiliser en même temps de manière hybride.
C'est le mode que nous utilisons sur notre Alpine.

Essayons maintenant de manipuler le CPU uniquement avec la v1 de cgroups.

Pour ce faire, nous allons même repasser en mode "legacy" afin que cgroup v2 soit désactivé.

Nous retournons pour ce faire dans le fichier '/etc/rc.conf'.

/etc/rc.conf
```
#rc_cgroup_mode="hybrid"
rc_cgroup_mode="legacy"
#rc_cgroup_controllers="cpuset cpu io memory hugetlb pids"
```

Il est important de mettre en commentaire la ligne 'rc_cgroup_controllers' afin que cgroup v1 monte bien tous les contrôleurs.
Si nous ne la mettons pas en commentaire, alors le contrôleur CPU ne sera pas monté et disponible.

Une fois que nous relançons la machine, nous pouvons voir que les changements sont effectués avec la commande : "ls /sys/fs/cgroup/".
Si de nombreux dossiers sont présents et que parmi eux le répertoire /cpu/ existe, alors les changements ont bien été effectué.

Nous allons maintenant créer un nouveau service testCpu1 qui sera identique au service testCpu2.
Nous copions notre fichier 'testCpu' du répertoire '/etc/init.d' et nous le collons dans ce même répertoire avec comme nom 'testCpu1'.
Les changements vont être visibles sur le fichier 'testCpu1' dans le répertoire '/etc/conf.d'

En effet, la façon de préciser les limitations est un peu différente que pour cgroup v2.

/etc/conf.d/testCpu1
```
rc_cgroup_cpu="cpu.cfs_quota_us 60000
cpu.cfs_period_us 100000
"
```

La principale différence est que nous devons créer une variable pour tous les contrôleurs différents s'ils doivent être modifiés.
Nous ne modifions que les valeurs de CPU en l'occurence, il ne faudra donc préciser que 'rc_cgroup_cpu'.

Les options marquées dans testCpu1 permettent de n'utiliser que 60% du CPU.

Pour tester ceci, nous pourrons utiliser la commande : "rc-service testCpu1 start".
Nous savons que sans limitation, la machine sur lequel les tests sont effectués mets environ 11 secondes à exécuter le service.
Avec les limitations, nous nous retrouvons à environ 17-18 secondes.

Les limitations sont donc bien prises en compte.