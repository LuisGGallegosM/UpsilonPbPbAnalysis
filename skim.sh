#!/bin/bash
cd Skimming
./skimcompile.sh
cd ..
INPUT=../rootfiles/merged_HiForestAOD.root
OUTPUTDIR=files
OUTPUT="${OUTPUTDIR}/merged_HiForestAOD_skimmed.root"
CONFIG=../rootfiles/merged_HiForestAOD.txt
mkdir -p $OUTPUTDIR
./Skimming/skim ${INPUT} ${OUTPUT} ${CONFIG}