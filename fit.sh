#!/bin/bash

CLING="NO"
#root file to fit from
INPUTFILE="${1:-../rootfiles/skims/skim/merged_HiForestAOD_MCFix2_skim/SKIM20MAR2021/merged_HiForestAOD_MCFix2_skim.root}"
CUTCONFIG="${2}"
#fit configuration file name
CONFIG="${3:-../rootfiles/confFiles/merged_HiForestAOD_MC_test.fitconf}"
#directory where to save all files
OUTPUTDIR="${4:-../rootfiles/fits/merged_HiForestAOD_MCFix2_skim_fit}"

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
./Fitting/fit -fit "${INPUTFILE}" "${CUTCONFIG}" "${OUTPUTFILE}" "${CONFIG}"
fi