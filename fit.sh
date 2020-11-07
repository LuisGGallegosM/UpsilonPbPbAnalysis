#!/bin/bash
cd Fitting
./fitcompile.sh
cd ..

#Root file to fit from
INPUTFILE="merged_HiForestAOD_skimmed.root"
#Fit configuration file name
CONFIG="merged_HiForestAOD_fit0.fitconf"

#working directory where all is saved
WORKDIR="../rootfiles/testskim"
#save in a directory with a name generated from config file
OUTPUTDIR="${WORKDIR}/${CONFIG%.*}"
#name of generated file is like CONFIG but with .root extension
OUTPUTFILE="${CONFIG%.*}.root"

#execute fitting
mkdir -p $OUTPUTDIR
./Fitting/fit "${WORKDIR}/${INPUTFILE}" "${OUTPUTDIR}/${OUTPUTFILE}" "${WORKDIR}/${CONFIG}"

#also execute drawing
./draw.sh "${OUTPUTDIR}/${OUTPUTFILE}"