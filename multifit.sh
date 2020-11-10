#!/bin/bash

CONFIGFILES=( \
 "merged_HiForestAOD_fit0.fitconf"\
 "merged_HiForestAOD_fit1.fitconf"\
 "merged_HiForestAOD_fit2.fitconf"\
 "merged_HiForestAOD_fit3.fitconf"\
 "merged_HiForestAOD_fit4.fitconf"\
 "merged_HiForestAOD_fit5.fitconf" )

for CONFIG in ${CONFIGFILES[@]}
do
gnome-terminal --tab --title=${CONFIG%.*} -- ./fit.sh $CONFIG
done