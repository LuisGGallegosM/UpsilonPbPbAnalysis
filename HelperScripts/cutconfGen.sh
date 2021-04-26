#!/bin/bash

if [ ${1} = "-DATA" ]
then
    cp HelperScripts/examplefiles/exCutDATA.cutconf ${2:-../rootfiles/confFiles/merged_HiForestAOD_DATA_baseline.cutconf}
fi

if [ ${1} = "-MC" ]
then
    cp HelperScripts/examplefiles/exCutMC.cutconf ${2:-../rootfiles/confFiles/merged_HiForestAOD_MC_baseline.cutconf}
fi