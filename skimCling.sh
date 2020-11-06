#!/bin/bash
INPUT=../rootfiles/merged_HiForestAOD.root
OUTPUTDIR=files
OUTPUT="${OUTPUTDIR}/merged_HiForestAOD_skimmed.root"
mkdir -p $OUTPUTDIR
cd Skimming
root -q 'Skimming.cpp("'../${INPUT}'","'../${OUTPUT}'")'
cd ..