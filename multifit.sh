#!/bin/bash

CONFIGFILES=( "merged_HiForestAOD_fit0.fitconf" "merged_HiForestAOD_fit1.fitconf")

for CONFIG in ${CONFIGFILES[@]}
do
gnome-terminal --tab --title=${CONFIG%.*} -- ./fit.sh $CONFIG
done