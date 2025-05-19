#!/bin/bash
echo "[*] Listening for raw packets on tap0..."

# sudo ip tuntap add dev tap0 mode tap user $USER
# sudo ip link set tap0 up
# sudo ip addr add 192.168.100.1/24 dev tap0

tcpdump -i tap0 -xx -e -n -vvv ether proto 0x0806
