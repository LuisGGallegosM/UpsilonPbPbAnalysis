#!/bin/bash

MULTIFITDIR="merged_HiForestAOD_DATA_skimjet/multifit_baseline"

./acceff.sh -nocorr $MULTIFITDIR
./crosssection.sh -nocorr $MULTIFITDIR
./yieldfit.sh "$MULTIFITDIR/acceff"
./acceff.sh -corr $MULTIFITDIR
./crosssection.sh -corr $MULTIFITDIR