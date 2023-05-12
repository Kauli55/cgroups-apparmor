OpenRC n'active pas les cgroups de base.

/etc/rc.conf
```
rc_controller_cgroups="YES"
rc_cgroup_mode="hybrid"
rc_cgroup_controllers="cpuset cpu io memory hugetlb pids"
```