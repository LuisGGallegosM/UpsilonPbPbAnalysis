#!/bin/bash

OUTDIR="../rootfiles/merged_HiForestAOD_skimmed3"
BASENAME="merged_HiForestAOD_fit"

MULTIFITFILE=$( find $OUTDIR -name "*.multifit")

ptLow=()
ptHigh=()
BKGTYPE=()
initialN=(          4.0 6.8 5.4 4.0 4.0 1.8 1.8 6.8 5.4 4.0 6.8 4.0 )
initialAlpha=(      1.5 1.4 1.5 1.5 1.5 1.8 1.8 1.5 1.5 1.8 1.5 1.8 )

CONF=( $( cat $MULTIFITFILE ) )
MOREUPSILON=${CONF[2]}
for i in $( seq 6 3 $((${#CONF[@]} - 1 )))
do
    ptLow=( ${ptLow[@]} ${CONF[$i]} )
    ptHigh=( ${ptHigh[@]} ${CONF[$(($i + 1 ))]} )
    BKGTYPE=( ${BKGTYPE[@]} ${CONF[$(($i + 2 ))]} )
done

echo "generating fit configuration files in directory ${OUTDIR}:"
#
num=$((${#ptLow[@]} -1 ))
 
for i in $(seq 0 $num )
do
    OUTPUT="massHigh = 11.0\n"
    OUTPUT+="massLow = 8.0\n"
    OUTPUT+="cut.ptLow = ${ptLow[$i]}\n"
    OUTPUT+="cut.ptHigh = ${ptHigh[$i]}\n"
    OUTPUT+="cut.yLow = 0.0\n"
    OUTPUT+="cut.yHigh = 2.4\n"
    OUTPUT+="initialValues.moreUpsilon = ${MOREUPSILON}\n"
    OUTPUT+="initialValues.nSigY1S = 600000.0\n"

    if [ $MOREUPSILON = "true" ]
    then
    OUTPUT+="initialValues.nSigY2S = 200000.0\n"
    OUTPUT+="initialValues.nSigY3S = 50000.0\n"
    fi

    OUTPUT+="initialValues.nBkg = 25000.0\n"
    OUTPUT+="initialValues.bkg.bkgType = ${BKGTYPE[$i]}\n"

    case ${BKGTYPE[$i]} in
        "chev")
            OUTPUT+="initialValues.bkg.chk4_k1 =  0.1\n"
            OUTPUT+="initialValues.bkg.chk4_k2 = -0.6\n"
        ;;
        "special")
            OUTPUT+="initialValues.bkg.mu = 12.0\n"
            OUTPUT+="initialValues.bkg.sigma = 0.6\n"
            OUTPUT+="initialValues.bkg.lambda = 0.116\n"
        ;;
        "exponential")
            OUTPUT+="initialValues.bkg.lambda = 0.11\n"
        ;;
        *)
            echo "incorrect bkgType"
            exit 1
        ;;
    esac

    OUTPUT+="initialValues.dcb.mean = 9.46\n"
    OUTPUT+="initialValues.dcb.alpha = ${initialAlpha[$i]}\n"
    OUTPUT+="initialValues.dcb.n = ${initialN[$i]}\n"
    OUTPUT+="initialValues.dcb.sigma = 0.128\n"
    OUTPUT+="initialValues.dcb.x = 0.5\n"
    OUTPUT+="initialValues.dcb.f = 0.5\n"

    FILENAME="${OUTDIR}/${BASENAME}${i}.fitconf"

    printf "$OUTPUT" > $FILENAME
    echo "generated fit configuration file : ${FILENAME}"
done

#printf "$OUTPUT" "${pt[0]}" "${pt[ $(($num+1)) ]}" > "${OUTDIR}/${BASENAME}_integrated.fitconf"