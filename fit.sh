#!/bin/bash

CLING="NO"
DEFAULTNAME="merged_HiForestAOD_skimmed3"
#root file to fit from
INPUTFILE="${1:-../rootfiles/${DEFAULTNAME}/${DEFAULTNAME}.root}"
#fit configuration file name
CONFIG="${2:-../rootfiles/${DEFAULTNAME}/merged_HiForestAOD_fit0.fitconf}"
#directory where to save all files
OUTPUTDIR="${3:-../rootfiles/${DEFAULTNAME}/merged_HiForestAOD_fit0}"

#draw configuration file name, must have same name as INPUTFILE but with drawconf extension
DRAWCONFIG="${INPUTFILE%.*}.drawconf"
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