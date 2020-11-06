#!/bin/bash
INPUT=files/merged_HiForestAOD_skimmed.root
OUTPUTDIR=files/test
OUTPUT="${OUTPUTDIR}/merged_HiForestAOD_fit.root"
mkdir -p $OUTPUTDIR
cd Fitting
root -q 'Fitting.cpp("'../${INPUT}'","'../${OUTPUT}'")'
cd ..