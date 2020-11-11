#!/bin/bash

CLING="NO"

#input file to skim
INPUTFILE="../rootfiles/merged_HiForestAOD.root"
#how to folder name skim result file
OUTPUTFOLDER="../rootfiles/merged_HiForestAOD_MC_skimmed"
#cut configuration file
CONFIG="../rootfiles/merged_HiForestAOD.cutconf"

OUTPUTFILE="${OUTPUTFOLDER}/$(basename ${OUTPUTFOLDER}).root"

mkdir -p $OUTPUTFOLDER

#execute skim
if [ $CLING = "YES" ]
then
cd Skimming
root -q 'Skimming.cpp("'../${INPUTFILE}'","'../${OUTPUTFOLDER}/${OUTPUTFOLDER%/*}.root'","'../${CONFIG}'")'
cd ..
else
./Skimming/skim "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}" > "${OUTPUTFILE%.*}_Skimming.log" 2>&1
fi