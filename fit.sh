#!/bin/bash
cd Fitting
./fitcompile.sh
cd ..
INPUT=files/merged_HiForestAOD_skimmed.root
OUTPUTDIR=files/test
OUTPUT="${OUTPUTDIR}/merged_HiForestAOD_fit.root"
CONFIG="${OUTPUTDIR}/merged_HiForestAOD_fit.txt"
mkdir -p $OUTPUTDIR
./Fitting/fit ${INPUT} ${OUTPUT} ${CONFIG}