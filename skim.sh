#!/bin/bash
cd Skimming
./skimcompile.sh
cd ..

#input file to skim
INPUTFILE="merged_HiForestAOD.root"
#how to name skim result file
OUTPUTFILE="merged_HiForestAOD_skimmed.root"
#cut configuration file
CONFIG="merged_HiForestAOD.cutconf"

#working directory
WORKDIR="../rootfiles"
#directory where to write output
OUTPUTDIR="${WORKDIR}/testskim"

#execute skim
mkdir -p $OUTPUTDIR
./Skimming/skim "${WORKDIR}/${INPUTFILE}" "${OUTPUTDIR}/${OUTPUTFILE}" "${WORKDIR}/${CONFIG}"