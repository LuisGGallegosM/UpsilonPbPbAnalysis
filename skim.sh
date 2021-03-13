#!/bin/bash

CLING="NO"

FLAGS=${1:-""}
FLAGS2=${2:-"-nocmd"}

#input file to skim
INPUTFILE="../rootfiles/datasets/merged_HiForestAOD_MCFix2.root"
INPUTFILENAME=$(basename $INPUTFILE)
#how to folder name skim result file
OUTPUTFOLDER="../rootfiles/skims/${INPUTFILENAME%.*}_skim_test"
#cut configuration file
CONFIG="../rootfiles/confFiles/merged_HiForestAOD_MC_jetskim.cutconf"
#output skimmed file, put in OUTPUTFOLDER and name based in inputfile
OUTPUTFILENAME="${INPUTFILENAME%.*}_skim.root"
OUTPUTFILE="${OUTPUTFOLDER}/${OUTPUTFILENAME}"

mkdir -p $OUTPUTFOLDER

if [ $FLAGS2 = "-cmd" ]
then
    echo ${FLAGS} "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}"
else
        #execute skim
    if [ $CLING = "YES" ]
    then
        cd Skimming
        root -q 'Skimming.cpp("'../${INPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")'
        cd ..
    else
        ./Skimming/skim ${FLAGS} "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}"
    fi
fi