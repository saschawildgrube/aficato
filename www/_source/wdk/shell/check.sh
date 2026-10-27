#!/bin/bash

echo ""
echo ""

echo "####################################################################################"
echo "Server Check START"
echo "####################################################################################"
echo ""

echo "uptime:"
uptime
echo ""
echo ""

echo "vmstat:"
vmstat 1 10
echo ""
echo ""

echo "nproc:"
nproc
echo ""
echo ""

echo "free:"
free -h
echo ""
echo ""

echo "top:"
top -b -d 1 -n 5 -w 180 | head -n 160
echo ""
echo ""

echo "ps:"
ps -eo pid,ppid,user,stat,pcpu,pmem,etime,comm --sort=-pcpu | head -25
echo ""
echo ""

echo "Plesk Version:"
plesk version
echo ""
echo ""


echo "apache2 config test:"
apache2ctl configtest
echo ""
echo ""

echo "apache2 run config:"
apache2ctl -t -D DUMP_RUN_CFG
echo ""
echo ""

echo "apache2 loaded modules:"
apachectl -M
echo ""
echo ""

echo "apache2 processes:"
pid=$(ps -eo pid,comm,stat | awk '$2=="apache2" && $3 ~ /^R/ {print $1; exit}')
if [ -n "$pid" ]; then
  echo "Tracing Apache PID $pid"
  timeout -s INT 5 strace -f -s 180 -e trace=execve -p "$pid" 2>&1 | head -60
else
  echo "No runnable Apache worker found; run again."
fi
echo ""
echo ""

echo "nginx:"
nginx -t
echo ""
echo ""


echo "php-fpm processes:"
ps -eo pid,ppid,etime,args | grep '[p]hp-fpm'
echo ""
echo ""

echo "php-fpm services:"
systemctl list-units --type=service --all | grep -E 'php.*fpm'
echo ""
echo ""


echo "traffic:"
./traffic.sh
echo ""
echo ""


echo "fpm slow log:"
tail -n 100 /var/log/php-fpm-slow.log
echo ""
echo ""

echo "####################################################################################"
echo "Server Check END"
echo "####################################################################################"
echo ""
