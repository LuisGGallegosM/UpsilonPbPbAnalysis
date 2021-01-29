#!/bin/bash

CLING="NO"

#input file to skim
INPUTFILE="../rootfiles/merged_HiForestAOD_MC.root"
#how to folder name skim result file
OUTPUTFOLDER="../rootfiles/merged_HiForestAOD_MC_skimmed5"
#cut configuration file
CONFIG="../rootfiles/merged_HiForestAOD_MC.cutconf"
#output skimmed file, put in OUTPUTFOLDER and same as output folder but with .root extensiom
OUTPUTFILE="${OUTPUTFOLDER}/$(basename ${OUTPUTFOLDER}).root"

mkdir -p $OUTPUTFOLDER

#execute skim
if [ $CLING = "YES" ]
then
cd Skimming
root -q 'Skimming.cpp("'../${INPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
./Skimming/skim "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}"
fi