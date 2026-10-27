#!/bin/bash

echo "Drop IP addresses: $1"
echo
echo "sudo iptables -A INPUT -s $1 -j DROP"
sudo iptables -A INPUT -s $1 -j DROP