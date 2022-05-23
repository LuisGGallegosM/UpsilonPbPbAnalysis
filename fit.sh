#!/bin/bash

CLING="NO"
#root file to fit from
INPUTFILE="${1:-../rootfiles/analysis/merged_HiForestAOD_MC_skim/merged_HiForestAOD_MC_skim.root}"
CUTCONFIG="${2:-../rootfiles/analysis/merged_HiForestAOD_MC_skim/merged_HiForestAOD_MC_skim.cutconf}"
#fit configuration file name
CONFIG="${3:-../rootfiles/confFiles/MC/fit0.fitconf}"
#directory where to save all files
OUTPUTDIR="${4:-../rootfiles/analysis/merged_HiForestAOD_MC_skim/testfit}"

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
fi