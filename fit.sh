#!/bin/bash

CLING="NO"
#root file to fit from
INPUTFILE="${1:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/merged_HiForestAOD_MC_skimjet.root}"
CUTCONFIG="${2:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/merged_HiForestAOD_MC_skimjet.cutconf}"
#fit configuration file name
CONFIG="${3:-../rootfiles/confFiles/MC/fit0.fitconf}"
#directory where to save all files
OUTPUTDIR="${4:-../rootfiles/analysis/merged_HiForestAOD_MC_skim/testfit}"
CORRFILE="${5}"

#name of generated file is like CONFIG but with .root extension
OUTPUTFILE="${OUTPUTDIR}/$( basename $OUTPUTDIR ).root"

mkdir -p $OUTPUTDIR

#execute fitting
if [ $CLING = "YES" ]
then
cd Fitting
root -q 'Fitting.cpp("'../${INPUTFILE}'","'../${CUTCONFIG}'","'../${OUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
./Fitting/fit -fit "${INPUTFILE}" "${CUTCONFIG}" "${CONFIG}" "${OUTPUTFILE}"# > "${OUTPUTFILE%.*}.log"
#./Fitting/fit -fitw "${INPUTFILE}" "${CUTCONFIG}" "${CONFIG}" "${OUTPUTFILE}" ${CORRFILE}  > "${OUTPUTFILE%.*}.log"
fi