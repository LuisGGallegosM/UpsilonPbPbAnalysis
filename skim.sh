#!/bin/bash

AUTOCOMPILE="YES"
CLING="YES"

if [ $AUTOCOMPILE = "YES" ] && [ CLING != "YES" ]
then
cd Skimming
./skimcompile.sh
cd ..
fi

#input file to skim
INPUTFILE="merged_HiForestAOD.root"
#how to name skim result file
OUTPUTFILE="merged_HiForestAOD_skimmed.root"
#cut configuration file
CONFIG="merged_HiForestAOD.cutconf"

#working directory
WORKDIR="../rootfiles"
#directory where to write output
OUTPUTDIR="${WORKDIR}/testskim"

mkdir -p $OUTPUTDIR

#execute skim
if [ $CLING = "YES" ]
then
cd Skimming
root -q 'Skimming.cpp("'../${WORKDIR}/${INPUTFILE}'","'../${OUTPUTDIR}/${OUTPUTFILE}'","'../${WORKDIR}/${CONFIG}'")'
cd ..
else
./Skimming/skim "${WORKDIR}/${INPUTFILE}" "${OUTPUTDIR}/${OUTPUTFILE}" "${WORKDIR}/${CONFIG}"
fi