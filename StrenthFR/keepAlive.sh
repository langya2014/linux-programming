#!/bin/sh
program=V_smile
sn=`ps -ef | grep $program | grep -v grep | awk '{print $2}'`
if [ "${sn}" = "" ]
then 
	nohup ./main/V_smile
fi