#!/bin/bash
time=$(date +"%Y-%m-%d-%H-%M")
/usr/sbin/logrotate -vf /etc/logrotate.d/MoneyCoolMonitor

cd /home/117bakup/daijun/MoneyCoolMonitor/log
for i in $(ls ./ | grep "^\(.*\)\.[[:digit:]]$")
do
mv $i ./$(echo $i|sed -n 's/^\(.*\)\.\([[:digit:]]\)$/\1/p').$(echo $time)
done

