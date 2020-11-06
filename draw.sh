#!/bin/bash
OUTPUTDIR=files/test
INPUT="${OUTPUTDIR}/merged_HiForestAOD_fit.root"
OUTPUT="${OUTPUTDIR}/merged_HiForestAOD_fit.pdf"
./Drawing/draw ${INPUT} ${OUTPUT}