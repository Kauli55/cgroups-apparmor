#!bin/bash

#Insertion du PID dans le cgroup:
echo $$ > /sys/fs/cgroup/test_manu/cgroup.procs

#Test du script
$(/home/test_cgroups/testMemoire)

exit 0