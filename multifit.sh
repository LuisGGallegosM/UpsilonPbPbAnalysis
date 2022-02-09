#!/bin/bash
MAXJOBS=12
DOFIT="true"
DODRAW="true"
DOCDRAW="true"

while : do
do
    case $1 in
    "-nofit")
    DOFIT="false"
    ;;
    "-nodraw")
    DODRAW="false"
    ;;
    "-nocomp")
    DOCDRAW="false"
    ;;
    *)
    break
    ;;
    esac
    shift
done

#multifit input file
MULTIFITFILE=${1:-"../rootfiles/confFiles/merged_HiForestAOD_DATA_baseline.multifit"}
INPUTFILENAME=${2:-"../rootfiles/analysis/merged_HiForestAOD_DATA_skim/merged_HiForestAOD_DATA_skim.root"}
#drawing configuration file
DRAWCONFIG=${3:-"../rootfiles/confFiles/merged_HiForestAOD.drawconf"}

SKIMFILE="${INPUTFILENAME}"
CUTFILE=${4:-"${INPUTFILENAME%.*}.cutconf"}
#directory where to save multifit results
OUTDIR=${5:-"${INPUTFILENAME%/*}/multifit_baseline_cheb1_fx"}

echo "multiple fitting"
echo "saving files in '${OUTDIR}'"
mkdir -p "${OUTDIR}"

#fitconf files to read
./HelperScripts/fitconfGen.sh "${MULTIFITFILE}" "${OUTDIR}"
CONFIGFILES=$(find ${OUTDIR} -maxdepth 1 -name "*.fitconf")

cp "$MULTIFITFILE" "${OUTDIR}"

#do the fits
if [ ${DOFIT} = "true" ]
then
    echo "fitting..."
    
    echo "reading skim file '${SKIMFILE}'"
    #do the fitting jobs
    for CONFIG in ${CONFIGFILES[@]}
    do
        FITFILE="${CONFIG}"
        FITNAME=$(basename $CONFIG)
        FITOUTPUTDIR="${OUTDIR}/${FITNAME%.*}"
        echo "fitting file: $CONFIG"
        mkdir "${FITOUTPUTDIR}"
        ./fit.sh "$SKIMFILE" "$CUTFILE" "$FITFILE" "${FITOUTPUTDIR}" &
        JOBS=( $(jobs -p) )
        JOBNUM="${#JOBS[@]}"
        if [ $MAXJOBS = $JOBNUM ]
        then
            wait -n
        fi
    done
    wait
    echo "all fits done"
fi

rm ${CONFIGFILES}

#draw graphs
if [ ${DODRAW} = "true" ]
then
    #execute drawing
    echo "generating drawings"
    echo "reading drawing configuration file: ${DRAWCONFIG}"
    for CONFIG in ${CONFIGFILES[@]}
    do
        NAME=$(basename $CONFIG)
        ROOTFILEDRAW="${OUTDIR}/${NAME%.*}"
        echo "drawing ${ROOTFILEDRAW}"
        ./draw.sh "${ROOTFILEDRAW}" "${DRAWCONFIG}" > "${OUTDIR}/draw.log"
    done
fi

#do comparation drawing
if [ ${DOCDRAW} = "true" ]
then
    echo "starting comparative drawings..."
    FITS=$(find ${OUTDIR} -maxdepth 1 -type d -name "fit*")
    ./Fitting/fit -multidraw "${MULTIFITFILE}" "${OUTDIR}" $FITS
    ./HelperScripts/reportGen.sh "${MULTIFITFILE}" "${OUTDIR}" $FITS
fi

#cp -R "${WORKDIR}/report" "${WORKDIR}/${OUTDIR}/report"
#cd "${WORKDIR}/${OUTDIR}/report"
#pdflatex -interaction nonstopmode "report.tex" >/dev/null
#cd "../../.."


echo "all done"