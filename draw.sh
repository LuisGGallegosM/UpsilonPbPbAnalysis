#!/bin/bash
cd Drawing
./drawcompile.sh
cd ..

#file path to root fit file for drawing
INPUT=${1-"../rootfiles/testskim/merged_HiForestAOD_fit0/merged_HiForestAOD_fit0.root"}

#graphics outputfile named same as input file but with .pdf extension
OUTPUT="${INPUT%.*}.pdf"
#path to cut file, one folder up from INPUT file location
CUTFILE="${INPUT%/*}/../merged_HiForestAOD_skimmed.cutconf"
#path to fit configuration file, it has same name as input but with .fitconf extension
FITFILE="${INPUT%.*}.fitconf"
./Drawing/draw "${INPUT}" "${OUTPUT}" "${CUTFILE}" "${FITFILE}"