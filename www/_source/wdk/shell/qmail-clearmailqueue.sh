/var/qmail/bin/qmail-qstat
service qmail stop
find /var/qmail/queue/mess -type f -exec rm {} \;
find /var/qmail/queue/info -type f -exec rm {} \;
find /var/qmail/queue/local -type f -exec rm {} \;
find /var/qmail/queue/intd -type f -exec rm {} \;
find /var/qmail/queue/todo -type f -exec rm {} \;
find /var/qmail/queue/remote -type f -exec rm {} \;
service qmail start
/var/qmail/bin/qmail-qstat