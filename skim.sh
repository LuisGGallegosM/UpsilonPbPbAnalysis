#!/bin/bash

CLING="NO"

FLAGS=${1:-"-skimjet"}
FLAGS2=${2:-"-nocmd"}

#input file to skim
INPUTFILE="../rootfiles/datasets/merged_HiForestAOD_DATA.root"
#cut configuration file
CONFIG="../rootfiles/confFiles/merged_HiForestAOD_DATA_v2.cutconf"

INPUTFILENAME=$(basename $INPUTFILE)
#how to folder name skim result file
OUTPUTFOLDER="../rootfiles/analysis/${INPUTFILENAME%.*}_${FLAGS:1}_test"
#output skimmed file, put in OUTPUTFOLDER and name based in inputfile
OUTPUTFILENAME="${INPUTFILENAME%.*}_${FLAGS:1}.root"
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