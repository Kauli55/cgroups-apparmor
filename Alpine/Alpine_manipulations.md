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