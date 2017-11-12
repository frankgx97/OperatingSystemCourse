#!/bin/bash
#add */15 * * * * /path/to/script/task1.sh to crontab to run this script every 15 minutes
if [ -z $1 ];
then
    echo "Parameter missing."
    exit 1
fi

process_no=`ps -ef |grep $1 |grep -v "grep" |grep -v $0 |wc -l`
if [ $process_no -eq 0 ];
then
    echo $(date)
    echo $1 "is down."
    echo "starting" $1
    sudo service $1 start
else
    echo $(date)
    echo $1 "is up."
fi
exit 0
