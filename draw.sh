#!/bin/bash

CLING="NO"

WORKDIR="test/Fitting/Drawing"
DEFAULTFILE="merged_HiForestAOD_fit1"
#directory where fit files are located
INPUT=${1:-"../rootfiles/${WORKDIR}/${DEFAULTFILE}"}
#path to draw configuration file
DRAWFILE=${2:-"../rootfiles/${WORKDIR}/merged_HiForestAOD.drawconf"}

#execute fit
if [ $CLING = "YES" ]
then
cd Drawing
root -q 'Drawing.cpp("'../${INPUT}'","'../${DRAWFILE}'")'
cd ..
else
./Fitting/fit -draw "${INPUT}" "${DRAWFILE}"
fi