#!/bin/bash

#configuration flags
FLAGS=${1:-"-skimjet"}
#input file to skim
INPUTFILE=${2:-"../rootfiles/datasets/merged_HiForestAOD_DATA.root"}
#cut configuration file
CONFIG=${3:-"../rootfiles/confFiles/merged_HiForestAOD_DATA_baseline.cutconf"}

INPUTFILENAME=$(basename $INPUTFILE)
#folder name skim result file
OUTPUTFOLDER="../rootfiles/analysis/${INPUTFILENAME%.*}_skimjet_test5"

CLING="NO"
#output skimmed file, put in OUTPUTFOLDER and name based in inputfile
OUTPUTFILENAME="${INPUTFILENAME%.*}_${FLAGS:1}.root"
OUTPUTFILE=${4:-"${OUTPUTFOLDER}/${OUTPUTFILENAME}"}

mkdir -p $OUTPUTFOLDER

#execute skim
if [ $CLING = "YES" ]
then
    cd Skimming
    root -q 'Skimming.cpp("'../${INPUTFILE}'","'../${OUTPUTFILE}'","'../${CONFIG}'")'
    cd ..
else
    ./Skimming/skim ${FLAGS} "${INPUTFILE}" "${OUTPUTFILE}" "${CONFIG}"
fi