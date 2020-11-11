#!/bin/bash

CLING="NO"

#file path to root fit file for drawing
INPUT=${1:-"../rootfiles/merged_HiForestAOD_MC_skimmed/merged_HiForestAOD_fit0/merged_HiForestAOD_fit0.root"}

#graphics outputfile named same as input file but with .pdf extension
OUTPUT="${INPUT%.*}.pdf"
#path to cut file, one folder up from INPUT file location
CUTFILE="${INPUT%.*}.cutconf"
#path to fit configuration file, it has same name as input but with .fitconf extension
FITFILE="${INPUT%.*}.fitconf"

#execute fit
if [ $CLING = "YES" ]
then
cd Drawing
root -q 'Drawing.cpp("'../${INPUT}'","'../${OUTPUT}'","'../${CUTFILE}'","'../${FITFILE}'")' > "../${INPUT%.*}_Drawing.log" 2>&1 
cd ..
else
./Drawing/draw "${INPUT}" "${OUTPUT}" "${CUTFILE}" "${FITFILE}" > "${INPUT%.*}_Drawing.log" 2>&1 
fi