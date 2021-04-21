#!/bin/bash

echo "generating fitconf files..."

MULTIFITFILE=${1:-../rootfiles/confFiles/merged_HiForestAOD_MC_baseline.multifit}
OUTDIR=${2:-../rootfiles/confFiles/multifitmc}
mkdir $OUTDIR
BASENAME="fit"

echo "Reading file: ${1}"
if [ ! -f "$MULTIFITFILE" ]
then
    echo "Error: ${MULTIFITFILE} file does not exist!"
    exit 1
fi

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
    OUTPUT="moreUpsilon = ${MOREUPSILON}\n"
    OUTPUT+="cut.mass.high = ${MASSHIGH}\n"
    OUTPUT+="cut.mass.low = ${MASSLOW}\n"
    OUTPUT+="cut.pt.low = ${ptLow[$i]}\n"
    OUTPUT+="cut.pt.high = ${ptHigh[$i]}\n"
    OUTPUT+="cut.y.low = 0.0\n"
    OUTPUT+="cut.y.high = 2.4\n"
    OUTPUT+="signal.nSigY1S.high = 10000000.0\n"
    OUTPUT+="signal.nSigY1S.value = ${NSIG1S}\n"
    OUTPUT+="signal.nSigY1S.low = 0.0\n"

    if [ $MOREUPSILON = "true" ]
    then
        OUTPUT+="signal.nSigY2S.high = 10000000.0\n"
        OUTPUT+="signal.nSigY2S.value = ${NSIG2S}\n"
        OUTPUT+="signal.nSigY2S.low = 0.0\n"

        OUTPUT+="signal.nSigY3S.high = 10000000.0\n"
        OUTPUT+="signal.nSigY3S.value = ${NSIG3S}\n"
        OUTPUT+="signal.nSigY3S.low = 0.0\n"
    fi

    OUTPUT+="bkg.nBkg.high = 1000000.0\n"
    OUTPUT+="bkg.nBkg.value = ${NBKG}\n"
    OUTPUT+="bkg.nBkg.low = 0.0\n"

    OUTPUT+="bkg.type = ${BKGTYPE[$i]}\n"
    case ${BKGTYPE[$i]} in
        "chev" | "expChev2")
            OUTPUT+="bkg.chk4_k1.high = 6.0\n"
            OUTPUT+="bkg.chk4_k1.value = ${CH1}\n"
            OUTPUT+="bkg.chk4_k1.low = -6.0\n"

            OUTPUT+="bkg.chk4_k2.high = 6.0\n"
            OUTPUT+="bkg.chk4_k2.value = ${CH2}\n"
            OUTPUT+="bkg.chk4_k2.low = -6.0\n"
        ;;
        "special")
            OUTPUT="bkg.mu.high = 30.0\n"
            OUTPUT="bkg.mu.value = ${MU_BKG}\n"
            OUTPUT="bkg.mu.low = 0.0\n"

            OUTPUT="bkg.sigma.high = 4.0\n"
            OUTPUT="bkg.sigma.value = ${SIGMA_BKG}\n"
            OUTPUT="bkg.sigma.low = 0.0\n"
            
            OUTPUT="bkg.lambda.high = 30.0\n"
            OUTPUT="bkg.lambda.value = ${LAMBDA_BKG}\n"
            OUTPUT="bkg.lambda.low = 0.0\n"
        ;;
        "exponential")
            OUTPUT="bkg.lambda.high = 30.0\n"
            OUTPUT="bkg.lambda.value = ${LAMBDA_BKG}\n"
            OUTPUT="bkg.lambda.low = 0.0\n"
        ;;
        "none")
        ;;
        *)
            echo "\nError : incorrect bkgType in ${OUTDIR}/${BASENAME}${i}.fitconf\n"
            echo "bkgType= ${BKGTYPE[$i]}\n"
            exit
        ;;
    esac

    OUTPUT+="signal.mean.high = 9.56 \n"
    OUTPUT+="signal.mean.value = 9.46 \n"
    OUTPUT+="signal.mean.low = 9.36 \n"
    OUTPUT+="signal.mean.fixed = false \n"

    OUTPUT+="signal.alpha.high = 5.5 \n"
    OUTPUT+="signal.alpha.value = ${initialAlpha[$i]} \n"
    OUTPUT+="signal.alpha.low = 0.5 \n"
    OUTPUT+="signal.alpha.fixed = ${FIXALPHA} \n"

    OUTPUT+="signal.sigma.high = 0.6 \n"
    OUTPUT+="signal.sigma.value = ${initialSigma[$i]} \n"
    OUTPUT+="signal.sigma.low = 0.01 \n"
    OUTPUT+="signal.sigma.fixed = false \n"

    OUTPUT+="signal.n.high = 15.0 \n"
    OUTPUT+="signal.n.value = ${initialN[$i]} \n"
    OUTPUT+="signal.n.low = 0.05 \n"
    OUTPUT+="signal.n.fixed = ${FIXN} \n"

    OUTPUT+="signal.x.high = 1.0 \n"
    OUTPUT+="signal.x.value = 0.5 \n"
    OUTPUT+="signal.x.low = 0.0 \n"
    OUTPUT+="signal.x.fixed = false \n"

    OUTPUT+="signal.f.high = 1.0 \n"
    OUTPUT+="signal.f.value = 0.5 \n"
    OUTPUT+="signal.f.low = 0.0 \n"
    OUTPUT+="signal.f.fixed = false \n"
    
    FILENAME="${OUTDIR}/${BASENAME}${i}.fitconf"

    printf "$OUTPUT" > $FILENAME
    echo "generated fit configuration file : ${FILENAME}"
done

#printf "$OUTPUT" "${pt[0]}" "${pt[ $(($num+1)) ]}" > "${OUTDIR}/${BASENAME}_integrated.fitconf"