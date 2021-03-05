#!/bin/bash

#output directory
OUTDIR="../rootfiles/merged_HiForestAOD_MC_skimmed2"
BASENAME="$( basename $OUTDIR )"

#generated file
OUTPUT="isMC = false\n"
OUTPUT+="checkSign = true\n"
OUTPUT+="trigSelect = 1\n"
OUTPUT+="sign = 0\n"
OUTPUT+="prescale = 1\n"
OUTPUT+="selectionBits = 10\n"
OUTPUT+="minTracks = 6\n"
OUTPUT+="minPixels = 1\n"
OUTPUT+="maxDxy = 0.3\n"
OUTPUT+="maxDz = 20\n"
OUTPUT+="minVtxProb = 0.01\n"
OUTPUT+="ptLow = 0\n"
OUTPUT+="ptHigh = 50\n"
OUTPUT+="yLow = 0\n"
OUTPUT+="yHigh = 2.4\n"
OUTPUT+="singleMuPtLow = 3.5\n"
OUTPUT+="singleMuEtaHigh = 2.4\n"

#save file in OUTDIR with same name but with drawconf extension
printf "$OUTPUT" > "${OUTDIR}/${BASENAME}.cutconf"

