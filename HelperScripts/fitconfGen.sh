#!/bin/bash

echo "generating fitconf files..."

MULTIFITFILE=${1:-../rootfiles/confFiles/merged_HiForestAOD_MC_baseline.multifit}
OUTDIR=${2:-../rootfiles/confFiles/multifitmc}
mkdir $OUTDIR
BASENAME="fit"

if [ ! -f "$MULTIFITFILE" ]
then
    echo "Error: ${MULTIFITFILE} file does not exist!"
    exit 1
fi

./Fitting/fit -multifitgen ${MULTIFITFILE} ${OUTDIR}