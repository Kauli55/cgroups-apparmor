# Etats de l’art :

Cgroups v2 --> devenu l’utilisation de base de cgroups. Changement par rapport à v1 : interface de contrôle avec une API différente, gestion un peu plus strict des héritages, possibilités de limitations plus grandes, nouvelles fonctionnalités.
Cgroup v1 et cgroup v2 compatibles en grandes parties. L’utilisateur ne remarquera pas de changements sauf s’il souhaite manipuler directement le système de fichiers du cgroup. Dans ce cas-là, les manipulations seront différentes. Cela vaut aussi pour les applications utilisant l’API de la version 1 : elles ne seront pas compatibles si elles ne sont pas mises à niveau pour la version 2.
La version 2 étant beaucoup plus récente, il est fortement recommandé de l’utiliser. Elle est d’ailleurs intégrée automatiquement à beaucoup de distributions sur leurs versions récentes.

Libcgroup --> commandes permettant de simplifier la mise en place de cgroups. Ces commandes trouvent pour la plupart des équivalents en commande shell de base. Libcgroup est surtout utilisé dans les cas où systemd n’a pas de fonctionnement défini.

Des solutions de containeurs et de virtualisation comme LXC et libvirt permettent la manipulation de cgroup.

# Présentation Cgroups :

Cgroups est un mécanisme de gestions des ressources intégré dans le noyau Linux. Ce mécanisme va permettre à l’administrateur la création de hiérarchies ayant des accès aux ressources de l’ordinateur spécifique. Cgroups va pouvoir notamment agir sur les ressources suivantes :

Blkio : le contrôleur agissant sur le nombre d’entrées sorties effectuées sur les appareils d’un bloc.
CPU : le contrôleur agissant sur le nombre de processeurs alloués, à la quantité de temps CPU et les limites de temps d’utilisation du processeur.
Devices : le contrôleur agissant sur l’accès aux périphériques pour les processus.
Memory : le contrôleur agissant sur la quantité de mémoire allouée aux processus.
net_cls : le contrôleur agissant notamment sur la bande passante allouée aux processus et l’accès au réseau.

Les cgroups vont permettre la création de hiérarchies. Ces hiérarchies permettent l’établissement des règles et des limites de ressources pouvant se répercuter sur les sous-groupes appartenant à la hiérarchie. L’ajout et la suppression de sous-groupes dans une hiérarchie se fait simplement afin d’avoir une grande flexibilité.

Cgroups est surtout utilisé pour manipuler des containers afin de limiter les capacités de ceux-ci.

## Pour systemd :

Tous les processus sur le système sont fils du processus systemd.
Scopes : processus lancés et arrêtés par la fonction fork()
Slices : hiérarchie des fichiers. Pas un processus ou un service.
Services : les services lancés pour l'utilisation d'autres processus.

# Utilisation de cgroups :

Cgroups va donc utiliser la limitation de ressources pour permettre une meilleure sécurité :

    • Limitation au niveau des performances : permet d’éviter les problèmes de surcharge de mémoire ainsi que de surcharge de CPU.
    • Limitation au niveau des accès autorisés : permet d’éviter l’accès à des équipements non autorisés voire l’accès au réseau pour certains processus.

Pour les serveurs devant supporter un trafic régulier, il est possible de limiter le débit utilisé par certains processus.