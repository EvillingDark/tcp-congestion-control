#/bin/bash
cd lkm
make
cp tcp_satellite.ko /lib/modules/$(uname -r)/
cd ..
modprobe -a tcp_satellite
sysctl net.ipv4.tcp_congestion_control=satellite
echo "done"