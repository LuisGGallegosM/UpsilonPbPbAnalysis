#!/bin/bash

CLING="NO"

DEFAULTFILE="merged_HiForestAOD_fit1"
#directory where fit files are located
INPUT=${1:-"../rootfiles/fits/merged_HiForestAOD_MCFix2_skim_fit"}
#path to draw configuration file
DRAWFILE=${2:-"../rootfiles/confFiles/merged_HiForestAOD.drawconf"}

#execute fit
if [ $CLING = "YES" ]
then
cd Drawing
root -q 'Drawing.cpp("'../${INPUT}'","'../${DRAWFILE}'")'
cd ..
else
./Fitting/fit -draw "${INPUT}" "${DRAWFILE}"
fi