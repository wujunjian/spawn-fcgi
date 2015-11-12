#!/bin/bash -x
#pkill MoneyCool || true
processnumber=`ps -ef| grep "MoneyCoolMonitor" |grep -v grep | awk '{print $2}'`
kill -9 ${processnumber[@]}
ulimit -c unlimited
usleep 100000
/usr/local/spawnfcgi/bin/spawn-fcgi -a 127.0.0.1 -p 6645 -F 2 -d ./ ./MoneyCoolMonitor
