#!/bin/bash

CLING="NO"

#input file to skim
INPUTFILE="../rootfiles/datasets/merged_HiForestAOD_MCFix2.root"
#how to folder name skim result file
OUTPUTFOLDER="../rootfiles/merged_HiForestAOD_MCFix2_skimmed0"
#cut configuration file
CONFIG="../rootfiles/confFiles/merged_HiForestAOD_MC.cutconf"
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