#!/bin/bash

CLING="NO"

#Root file to fit from
INPUTFILE="${1:-../rootfiles/merged_HiForestAOD_MC_skimmed2/merged_HiForestAOD_MC_skimmed2.root}"
#Fit configuration file name
CONFIG="${2:-../rootfiles/merged_HiForestAOD_MC_skimmed2/merged_HiForestAOD_fit0.fitconf}"
#Draw configuration file name
DRAWCONFIG="${INPUTFILE%.*}.drawconf"

#save in a directory with a name generated from config file
OUTPUTDIR="${CONFIG%.*}"
#name of generated file is like CONFIG but with .root extension
OUTPUTFILE="${OUTPUTDIR}/$( basename $OUTPUTDIR ).root"
#path of file where to save log
LOGFILE="${OUTPUTFILE%.*}_Fitting.log"

mkdir -p $OUTPUTDIR

#execute fitting
if [ $CLING = "YES" ]
then
cd Fitting
root -q 'Fitting.cpp("'../${INPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")' > "../${LOGFILE}" 2>&1
cd ..
else
./Fitting/fit "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}" > "${LOGFILE}" 2>&1
fi

#also execute drawing
./draw.sh "${OUTPUTFILE}" "${DRAWCONFIG}"