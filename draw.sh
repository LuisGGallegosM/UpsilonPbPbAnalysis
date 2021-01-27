#!/bin/bash

CLING="NO"

WORKDIR="merged_HiForestAOD_skimmed4"
DEFAULTFILE="merged_HiForestAOD_fit0"
#file path to root fit file for drawing
INPUT=${1:-"../rootfiles/${WORKDIR}/output/${DEFAULTFILE}/${DEFAULTFILE}.root"}
#path to draw configuration file
DRAWFILE=${2:-"../rootfiles/${WORKDIR}/${WORKDIR}.drawconf"}

#graphics outputfile named same as input file but with .pdf extension
OUTPUT="${INPUT%.*}.pdf"
#path to cut file, one folder up from INPUT file location
CUTFILE="${INPUT%.*}.cutconf"
#path to fit configuration file, it has same name as input but with .fitconf extension
FITFILE="${INPUT%.*}.fitconf"
#path of file where to save log
LOGFILE="${INPUT%.*}_Drawing.log"

#execute fit
if [ $CLING = "YES" ]
then
cd Drawing
root -q 'Drawing.cpp("'../${INPUT}'","'../${OUTPUT}'","'../${DRAWFILE}'","'../${CUTFILE}'","'../${FITFILE}'")' > "../${LOGFILE}" 2>&1 
cd ..
else
./Drawing/draw "${INPUT}" "${OUTPUT}" "${DRAWFILE}" "${CUTFILE}" "${FITFILE}" > "${LOGFILE}" 2>&1 
fi