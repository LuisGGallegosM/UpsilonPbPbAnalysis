#!/bin/bash

CLING="NO"

#Root file to fit from
INPUTFILE="../rootfiles/merged_HiForestAOD_MC_skimmed/merged_HiForestAOD_MC_skimmed.root"
#Fit configuration file name
CONFIG="${1:-../rootfiles/merged_HiForestAOD_MC_skimmed/merged_HiForestAOD_fit0.fitconf}"

#save in a directory with a name generated from config file
OUTPUTDIR="${CONFIG%.*}"
#name of generated file is like CONFIG but with .root extension
OUTPUTFILE="${OUTPUTDIR}/$( basename $OUTPUTDIR ).root"

mkdir -p $OUTPUTDIR

#execute fitting
if [ $CLING = "YES" ]
then
cd Fitting
root -q 'Fitting.cpp("'../${INPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")' > "../${OUTPUTFILE%.*}_Fitting.log" 2>&1
cd ..
else
./Fitting/fit "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}" > "${OUTPUTFILE%.*}_Fitting.log" 2>&1
fi

#also execute drawing
./draw.sh "${OUTPUTFILE}"