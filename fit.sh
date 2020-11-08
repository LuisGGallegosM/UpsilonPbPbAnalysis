#!/bin/bash

AUTOCOMPILE="NO"
CLING="YES"

if [ $AUTOCOMPILE = "YES" ] && [ CLING != "YES" ]
then
cd Fitting
./fitcompile.sh
cd ..
fi

#Root file to fit from
INPUTFILE="merged_HiForestAOD_skimmed.root"
#Fit configuration file name
CONFIG="merged_HiForestAOD_fit0.fitconf"

#working directory where all is saved
WORKDIR="../rootfiles/testskim"
#save in a directory with a name generated from config file
OUTPUTDIR="${WORKDIR}/${CONFIG%.*}"
#name of generated file is like CONFIG but with .root extension
OUTPUTFILE="${CONFIG%.*}.root"

mkdir -p $OUTPUTDIR

#execute fitting
if [ $CLING = "YES" ]
then
cd Fitting
root -q 'Fitting.cpp("'../${WORKDIR}/${INPUTFILE}'","'../${OUTPUTDIR}/${OUTPUTFILE}'","'../${WORKDIR}/${CONFIG}'")'
cd ..
else
./Fitting/fit "${WORKDIR}/${INPUTFILE}" "${OUTPUTDIR}/${OUTPUTFILE}" "${WORKDIR}/${CONFIG}"
fi

#also execute drawing
#./draw.sh "${OUTPUTDIR}/${OUTPUTFILE}"