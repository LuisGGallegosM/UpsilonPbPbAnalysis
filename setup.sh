#!/bin/bash

#clone a needed repository for Jet Energy Corrections in Jet skimming
if [ ! -d ../JECDatabase ]
then
mkdir ../JECDatabase
git clone https://github.com/cms-jet/JECDatabase ../JECDatabase
fi

#create the directories
mkdir -p ../rootfiles
#configuration files dir
mkdir -p ../rootfiles/confFiles
#base dataset root files dir
mkdir -p ../rootfiles/datasets
#base output for analysis dir
mkdir -p ../rootfiles/analysis

./HelperScripts/multifitGen.sh -MC
./HelperScripts/multifitGen.sh -DATA
./HelperScripts/cutconfGen.sh -MC
./HelperScripts/cutconfGen.sh -DATA
./HelperScripts/drawconfGen.sh
cp 