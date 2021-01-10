#!/bin/bash

OUTDIR="../rootfiles/merged_HiForestAOD_skimmed3"
BASENAME="merged_HiForestAOD_fit"

ptLow=(   0.0 0.0 2.0 4.0 6.0  9.0 12.0 0.0 4.0  9.0 0.0  6.0)
ptHigh=( 30.0 2.0 4.0 6.0 9.0 12.0 30.0 4.0 9.0 30.0 6.0 30.0)
BKGTYPE=( "special" "special" "special" "special" "special"\
 "exponential" "exponential" "special" "special" "exponential"\
 "special" "exponential" )
MOREUPSILON="true"

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
    OUTPUT+="initialValues.nSigY1S = 50000.0\n"

    if [ $MOREUPSILON = "true" ]
    then
    OUTPUT+="initialValues.nSigY2S = 16000.0\n"
    OUTPUT+="initialValues.nSigY3S = 8000.0\n"
    fi

    OUTPUT+="initialValues.nBkg = 120000.0\n"
    OUTPUT+="initialValues.bkg.bkgType = ${BKGTYPE[$i]}\n"

    case ${BKGTYPE[$i]} in
        "chev")
            OUTPUT+="initialValues.bkg.chk4_k1 =  0.1\n"
            OUTPUT+="initialValues.bkg.chk4_k2 = -0.6\n"
        ;;
        "special")
            OUTPUT+="initialValues.bkg.mu = 7.86\n"
            OUTPUT+="initialValues.bkg.sigma = 1.02\n"
            OUTPUT+="initialValues.bkg.lambda = 6.08\n"
        ;;
        "exponential")
            OUTPUT+="initialValues.bkg.lambda = 6.08\n"
        ;;
        *)
            echo "incorrect bkgType"
            exit 1
        ;;
    esac

    OUTPUT+="initialValues.dcb.mean = 9.46\n"
    OUTPUT+="initialValues.dcb.alpha = 1.722\n"
    OUTPUT+="initialValues.dcb.n = 2.405\n"
    OUTPUT+="initialValues.dcb.sigma = 0.02\n"
    OUTPUT+="initialValues.dcb.x = 0.5\n"
    OUTPUT+="initialValues.dcb.f = 0.5\n"

    FILENAME="${OUTDIR}/${BASENAME}${i}.fitconf"

    printf "$OUTPUT" > $FILENAME
    echo "generated fit configuration file : ${FILENAME}"
done

#printf "$OUTPUT" "${pt[0]}" "${pt[ $(($num+1)) ]}" > "${OUTDIR}/${BASENAME}_integrated.fitconf"