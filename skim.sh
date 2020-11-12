#!/bin/bash

CLING="NO"

#input file to skim
INPUTFILE="../rootfiles/merged_HiForestAOD.root"
#how to folder name skim result file
OUTPUTFOLDER="../rootfiles/merged_HiForestAOD_MC_skimmed"
#cut configuration file
CONFIG="../rootfiles/merged_HiForestAOD.cutconf"
#path of file where to save log
LOGFILE="${OUTPUTFILE%.*}_Skimming.log"
#output skimmed file, put in OUTPUTFOLDER and same as output folder but with .root extensiom
OUTPUTFILE="${OUTPUTFOLDER}/$(basename ${OUTPUTFOLDER}).root"
mkdir -p $OUTPUTFOLDER

#execute skim
if [ $CLING = "YES" ]
then
cd Skimming
root -q 'Skimming.cpp("'../${INPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")' > "${LOGFILE}" 2>&1
cd ..
else
./Skimming/skim "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}" > "${LOGFILE}" 2>&1
fi