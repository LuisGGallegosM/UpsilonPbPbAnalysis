#!/bin/bash

CLING="NO"

#input file to skim
INPUTFILE="../rootfiles/datasets/merged_HiForestAOD_MCFix2.root"
INPUTFILENAME=$(basename $INPUTFILE)
#how to folder name skim result file
OUTPUTFOLDER="../rootfiles/skims/${INPUTFILENAME%.*}_skim_test"
#cut configuration file
CONFIG="../rootfiles/confFiles/merged_HiForestAOD_MC.cutconf"
#output skimmed file, put in OUTPUTFOLDER and name based in inputfile
OUTPUTFILENAME="${INPUTFILENAME%.*}_skim.root"
OUTPUTFILE="${OUTPUTFOLDER}/${OUTPUTFILENAME}"

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