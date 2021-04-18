
#!/bin/bash

if [ ${1} = "-DATA" ]
then
    cp HelperScripts/examplefiles/exMultifitDATA.multifit ${2:-../rootfiles/confFiles/merged_HiForestAOD_baseline.multifit}
fi

if [ ${1} = "-MC" ]
then
    cp HelperScripts/examplefiles/exMultifitMC.multifit ${2:-../rootfiles/confFiles/merged_HiForestAOD_MC_baseline.multifit}
fi
