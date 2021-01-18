#!/bin/bash

OUTDIR=${1:-"../rootfiles/merged_HiForestAOD_MC_skimmed4"}
BASENAME="merged_HiForestAOD_fit"

MULTIFITFILE=$( find $OUTDIR -name "*.multifit" )

ptLow=()
ptHigh=()
BKGTYPE=()
initialN=()
initialAlpha=()
initialSigma=()

CONF=( $( cat $MULTIFITFILE ) )
MOREUPSILON=${CONF[2]}
MASSHIGH=${CONF[5]}
MASSLOW=${CONF[8]}
LAMBDA_BKG=${CONF[11]}
MU_BKG=${CONF[14]}
SIGMA_BKG=${CONF[17]}
CH1=${CONF[20]}
CH2=${CONF[23]}
NSIG1S=${CONF[26]}
NSIG2S=${CONF[29]}
NSIG3S=${CONF[32]}
NBKG=${CONF[35]}
FIXALPHA=${CONF[38]}
FIXN=${CONF[41]}

for i in $( seq 48 6 $((${#CONF[@]} - 1 )))
do
    ptLow=( ${ptLow[@]} ${CONF[$i]} )
    ptHigh=( ${ptHigh[@]} ${CONF[$(($i + 1 ))]} )
    BKGTYPE=( ${BKGTYPE[@]} ${CONF[$(($i + 2 ))]} )
    initialN=( ${initialN[@]} ${CONF[$(($i + 3 ))]} )
    initialAlpha=( ${initialAlpha[@]} ${CONF[$(($i + 4 ))]} )
    initialSigma=( ${initialSigma[@]} ${CONF[$(($i + 5 ))]} )
done

echo "generating fit configuration files in directory ${OUTDIR}:"
#
num=$((${#ptLow[@]} -1 ))
 
for i in $(seq 0 $num )
do
    OUTPUT="massHigh = ${MASSHIGH}\n"
    OUTPUT+="massLow = ${MASSLOW}\n"
    OUTPUT+="cut.ptLow = ${ptLow[$i]}\n"
    OUTPUT+="cut.ptHigh = ${ptHigh[$i]}\n"
    OUTPUT+="cut.yLow = 0.0\n"
    OUTPUT+="cut.yHigh = 2.4\n"
    OUTPUT+="initialValues.moreUpsilon = ${MOREUPSILON}\n"
    OUTPUT+="initialValues.nSigY1S = ${NSIG1S}\n"

    if [ $MOREUPSILON = "true" ]
    then
    OUTPUT+="initialValues.nSigY2S = ${NSIG2S}\n"
    OUTPUT+="initialValues.nSigY3S = ${NSIG3S}\n"
    fi

    OUTPUT+="initialValues.nBkg = ${NBKG}\n"
    OUTPUT+="initialValues.bkg.bkgType = ${BKGTYPE[$i]}\n"

    case ${BKGTYPE[$i]} in
        "chev")
            OUTPUT+="initialValues.bkg.chk4_k1 = ${CH1}\n"
            OUTPUT+="initialValues.bkg.chk4_k2 = ${CH2}\n"
        ;;
        "special")
            OUTPUT+="initialValues.bkg.mu = ${MU_BKG}\n"
            OUTPUT+="initialValues.bkg.sigma = ${SIGMA_BKG}\n"
            OUTPUT+="initialValues.bkg.lambda = ${LAMBDA_BKG}\n"
        ;;
        "exponential")
            OUTPUT+="initialValues.bkg.lambda = ${LAMBDA_BKG}\n"
        ;;
        *)
            echo "incorrect bkgType in ${OUTDIR}/${BASENAME}${i}.fitconf"
            echo "bkgType= ${BKGTYPE[$i]}"
            exit
        ;;
    esac

    OUTPUT+="initialValues.dcb.mean = 9.46\n"
    OUTPUT+="initialValues.dcb.alpha = ${initialAlpha[$i]}\n"
    OUTPUT+="initialValues.dcb.n = ${initialN[$i]}\n"
    OUTPUT+="initialValues.dcb.sigma = ${initialSigma[$i]}\n"
    OUTPUT+="initialValues.dcb.x = 0.5\n"
    OUTPUT+="initialValues.dcb.f = 0.5\n"
    OUTPUT+="fix.dcb.alpha = ${FIXALPHA}\n"
    OUTPUT+="fix.dcb.n = ${FIXN}\n"

    FILENAME="${OUTDIR}/${BASENAME}${i}.fitconf"

    printf "$OUTPUT" > $FILENAME
    echo "generated fit configuration file : ${FILENAME}"
done

#printf "$OUTPUT" "${pt[0]}" "${pt[ $(($num+1)) ]}" > "${OUTDIR}/${BASENAME}_integrated.fitconf"