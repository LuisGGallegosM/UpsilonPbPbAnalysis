#!/bin/bash

OUTPUT="bkgOn = true\n"
OUTPUT+="moreUpsilon = true\n"
OUTPUT+="massHigh = 12.0\n"
OUTPUT+="massLow = 8.0\n"
OUTPUT+="cut.ptLow = %s\n"
OUTPUT+="cut.ptHigh = %s\n"
OUTPUT+="cut.yLow = 0.0\n"
OUTPUT+="cut.yHigh = 2.4\n"
OUTPUT+="initialValues.nSigY1S = 50000.0\n"
OUTPUT+="initialValues.nSigY2S = 16000.0\n"
OUTPUT+="initialValues.nSigY3S = 8000.0\n"
OUTPUT+="initialValues.nBkg = 120000.0\n"
OUTPUT+="initialValues.chk4_k1 =  0.0\n"
OUTPUT+="initialValues.chk4_k2 = -0.2\n"
OUTPUT+="initialValues.dcb.mean = 9.46\n"
OUTPUT+="initialValues.dcb.alpha = 1.6\n"
OUTPUT+="initialValues.dcb.n = 2.0\n"
OUTPUT+="initialValues.dcb.sigma = 0.13\n"
OUTPUT+="initialValues.dcb.x = 0.5\n"
OUTPUT+="initialValues.dcb.f = 0.5\n"

OUTDIR="../rootfiles/merged_HiForestAOD_skimmed"
BASENAME="merged_HiForestAOD_fit"

pt=( 0.0 2.0 4.0 6.0 9.0 12.0 30.0 )

num=$((${#pt[@]} -2 ))
 
for i in $(seq 0 $num )
do
 printf "$OUTPUT" "${pt[$i]}" "${pt[ $(($i+1)) ]}" > "${OUTDIR}/${BASENAME}${i}.fitconf"
done
