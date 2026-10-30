#!/bin/bash

echo ""
echo ""

echo "####################################################################################"
echo "Server Check START"
echo "####################################################################################"
echo ""

echo "### uptime:"
uptime
echo ""
echo ""

echo "### vmstat:"
vmstat 1 10
echo ""
echo ""

echo "### nproc:"
nproc
echo ""
echo ""

echo "### free:"
free -h
echo ""
echo ""

echo "### top:"
top -b -d 1 -n 5 -w 180 | head -n 160
echo ""
echo ""

echo "### ps:"
ps -eo pid,ppid,user,stat,pcpu,pmem,etime,comm --sort=-pcpu | head -25
echo ""
echo ""

echo "### OS Version"
cat /etc/os-release
echo ""
echo ""


echo "### Plesk Version:"
plesk version
echo ""
echo ""

echo "### Apache  cat /etc/os-releaseersion:"
apache2 -v
echo ""
echo ""


echo "### Apache Config Test:"
apache2ctl configtest
echo ""
echo ""

echo "### Apache Run Config:"
apache2ctl -t -D DUMP_RUN_CFG
echo ""
echo ""

echo "### Apache Loaded Modules:"
apachectl -M
echo ""
echo ""

echo "### Apache Processes:"
pid=$(ps -eo pid,comm,stat | awk '$2=="apache2" && $3 ~ /^R/ {print $1; exit}')
if [ -n "$pid" ]; then
  echo "Tracing Apache PID $pid"
  timeout -s INT 5 strace -f -s 180 -e trace=execve -p "$pid" 2>&1 | head -60
else
  echo "No runnable Apache worker found; run again."
fi
echo ""
echo ""

echo "### nginx Config Test:"
nginx -t
echo ""
echo ""


echo "### php-fpm Processes:"
ps -eo pid,ppid,etime,args | grep '[p]hp-fpm'
echo ""
echo ""

echo "### php-fpm Services:"
systemctl list-units --type=service --all | grep -E 'php.*fpm'
echo ""
echo ""

echo "### MariaDB Version:"
mariadb --version
echo ""
echo ""

echo "### Traffic:"
./traffic.sh
echo ""
echo ""


echo "### php-fpm Slow Log:"
tail -n 100 /var/log/php-fpm-slow.log
echo ""
echo ""

echo "####################################################################################"
echo "Server Check END"
echo "####################################################################################"
echo ""
