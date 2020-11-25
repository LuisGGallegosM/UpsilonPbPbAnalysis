#!/bin/bash

OUTDIR="../rootfiles/merged_HiForestAOD_MC_skimmed2"
BASENAME="merged_HiForestAOD_fit"

pt=( 0.0 2.0 4.0 6.0 9.0 12.0 30.0 )

BKGTYPE="special"

OUTPUT+="moreUpsilon = false\n"
OUTPUT+="massHigh = 10.0\n"
OUTPUT+="massLow = 8.0\n"
OUTPUT+="cut.ptLow = %s\n"
OUTPUT+="cut.ptHigh = %s\n"
OUTPUT+="cut.yLow = 0.0\n"
OUTPUT+="cut.yHigh = 2.4\n"
OUTPUT+="initialValues.nSigY1S = 50000.0\n"
OUTPUT+="initialValues.nSigY2S = 16000.0\n"
OUTPUT+="initialValues.nSigY3S = 8000.0\n"
OUTPUT+="initialValues.nBkg = 120000.0\n"
OUTPUT+="initialValues.bkg.bkgType = ${BKGTYPE}\n"

if [ $BKGTYPE = "chev" ] 
then
OUTPUT+="initialValues.bkg.chk4_k1 =  0.1\n"
OUTPUT+="initialValues.bkg.chk4_k2 = -0.6\n"
fi
if [ $BKGTYPE = "special" ] 
then
OUTPUT+="initialValues.bkg.mu = 7.86\n"
OUTPUT+="initialValues.bkg.sigma = 1.02\n"
OUTPUT+="initialValues.bkg.lambda = 6.08\n"
fi

OUTPUT+="initialValues.dcb.mean = 9.46\n"
OUTPUT+="initialValues.dcb.alpha = 1.722\n"
OUTPUT+="initialValues.dcb.n = 2.405\n"
OUTPUT+="initialValues.dcb.sigma = 0.02\n"
OUTPUT+="initialValues.dcb.x = 0.5\n"
OUTPUT+="initialValues.dcb.f = 0.5\n"

num=$((${#pt[@]} -2 ))
 
for i in $(seq 0 $num )
do
 printf "$OUTPUT" "${pt[$i]}" "${pt[ $(($i+1)) ]}" > "${OUTDIR}/${BASENAME}${i}.fitconf"
done

printf "$OUTPUT" "${pt[0]}" "${pt[ $(($num+1)) ]}" > "${OUTDIR}/${BASENAME}_integrated.fitconf"