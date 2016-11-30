#!/bin/bash

for i in {101..250}
do
	for j in {0..5}
	do
# 		printf -v hex '%02x' $i
		printf -v mac '08:00:27:78:5%d:%02x' $j $i
# 		ifconfig eth0 down
		ifconfig eth0 192.168.$j.$i/22
# 		ifconfig eth0 hw ether $mac
# 		ifconfig eth0 up
		route add default gw 192.168.0.1

		echo "set 192.168.$j.$i"

		wget -a wget.log -d "http://192.168.0.1:2060/api/login.cgi?token=2b022580d9a77cabe8c9fcfe6cb8fe2e982fd232f01224b2&jsonpcallback=jQuery111308767621802741068_1479957225898&_=1479957225899"

####### test httpd ########
# 		wget -q "http://192.168.0.1/cgi-bin/userLogin.cgi" --post-data="username=admin&password=admin&login=true&portalname=CommonPortal" --keep-session-cookies --save-cookies cookie.txt -p
# 		wget -q "http://192.168.0.1/default.htm" --load-cookie="cookie.txt" -p
# 		wget -q "http://192.168.0.1/home.htm" --load-cookie="cookie.txt" -p
# 		wget -q "http://192.168.0.1/network.htm" --load-cookie="cookie.txt" -p
		#sleep 1
	done
done

# wget -a wget.log -d "http://192.168.0.1/cgi-bin/userLogin.cgi" --post-data="username=admin&password=admin&login=true&portalname=CommonPortal" --keep-session-cookies --save-cookies cookie.txt
# wget -a wget.log -d "http://192.168.0.1/default.htm" --load-cookie="cookie.txt"
