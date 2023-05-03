Etude des solutions de sécurisation des processus sous Linux (cgroups, apparmor ...). Vous proposerez des cas d'usage et des bonnes pratiques 

CGROUPS :

https://man7.org/linux/man-pages/man7/cgroups.7.html

    • Documentation officielle sur cgroups.


https://access.redhat.com/documentation/fr-fr/red_hat_enterprise_linux/7/html/resource_management_guide/chap-introduction_to_control_groups

    • Utilisation de cgroups, libcgroup et libcgroup-tools

https://docs.kernel.org/admin-guide/cgroup-v1/index.html

    • Explication du fonctionnement de la version 1 de cgroups

https://docs.kernel.org/admin-guide/cgroup-v2.html

    • Explication du fonctionnement de la version 2 de cgroups

https://zarak.fr/linux/exploration-des-cgroups/

    • En supplément, pas la documentation officielle. Exemples repris

https://libvirt.org/cgroups.html#required-controllers

    • Utilisation des cgroups avec libvirt

Pas encore testé

https://linuxcontainers.org/lxc/manpages/man1/lxc-cgroup.1.html

    • Cgroups avec LXC

Pas encore testé

https://wiki.gentoo.org/wiki/OpenRC/CGroups

    • Pour Alpine car systemd n’est pas utilisé, c’est OpenRC qui l’est. --> Voir les différences avec systemd

https://www.freedesktop.org/wiki/Software/systemd/

https://www.freedesktop.org/software/systemd/man/systemd.resource-control.html

    • Pour tous les arguments possibles dans un _fichier_.splice 

https://wiki.archlinux.org/title/cgroups#With_the_cgroup_virtual_filesystem

    • Exemples de cgroups avec systemd et libcgroup

APPARMOR :

https://doc.ubuntu-fr.org/apparmor#:~:text=AppArmor%20(%22Application%20Armor%22),les%20capacit%C3%A9s%20de%20celui%2Dci.

https://gitlab.com/apparmor/apparmor/-/wikis/Documentation