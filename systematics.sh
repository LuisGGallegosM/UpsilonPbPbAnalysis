#!/bin/bash

INPUTDIR="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_pt50/weighted/acceff"
MULTIFITFILE="../rootfiles/confFiles/merged_HiForestAOD_DATA_baseline_z.multifit"
DRAWCONFIG="../rootfiles/confFiles/merged_HiForestAOD.drawconf"
CUTFILE="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_pt50/merged_HiForestAOD_DATA_skimjet.cutconf"

FILES=$( find ${INPUTDIR} -maxdepth 1 -name  "*.root" ) 

for FILE in $FILES
do
    echo "FITTING: ${FILE}"
    mkdir -p "$(dirname $FILE )/multifit/$(basename $FILE)"
    ./multifit.sh ${MULTIFITFILE} ${FILE} ${DRAWCONFIG} ${CUTFILE} "$(dirname $FILE )/multifit/$(basename $FILE)" > "$(dirname $FILE )/multifit/$(basename $FILE)/$(basename $FILE).log"
done
