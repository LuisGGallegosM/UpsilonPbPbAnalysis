#!/bin/bash

#output directory
OUTDIR="../rootfiles/merged_HiForestAOD_MC_skimmed2"
BASENAME="$( basename $OUTDIR )"

#generated file
OUTPUT="nBins = 100\n"

#save file in OUTDIR with same name but with drawconf extension
printf "$OUTPUT" > "${OUTDIR}/${BASENAME}.drawconf"