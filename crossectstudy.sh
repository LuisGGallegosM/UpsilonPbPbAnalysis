#!/bin/bash

MULTIFITDIR="merged_HiForestAOD_DATA_skim/multifit_baseline_data"

./acceff.sh -nocorr $MULTIFITDIR
./crosssection.sh -nocorr $MULTIFITDIR
./yieldfit.sh "$MULTIFITDIR/acceff"
./acceff.sh -corr $MULTIFITDIR
./crosssection.sh -corr $MULTIFITDIR