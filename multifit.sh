#!/bin/bash
DOFIT="true"
DODRAW="true"
DOCDRAW="true"
for FLAG in "$@"
do
    case $FLAG in
    "-nofit")
    DOFIT="false"
    ;;
    "-nodraw")
    DODRAW="false"
    ;;
    "-nocomp")
    DOCDRAW="false"
    ;;
    esac
done

#directory where files are located
WORKDIR="../rootfiles/merged_HiForestAOD_MCFix_skimmed0"
#directory where to save multifit results, inside WORKDIR
OUTDIR="output"

#skimmed file is inside WORKDIR
SKIMFILE="${WORKDIR}/$( basename $WORKDIR ).root"
#drawing configuration file
DRAWCONFIG="${SKIMFILE%.*}.drawconf"

echo "multiple fitting"
echo "saving files in '${OUTDIR}' directory"
mkdir "${WORKDIR}/${OUTDIR}"

#fitconf files to read
CONFIGFILES=$(find ${WORKDIR} -maxdepth 1 -name "*.fitconf")
MULTIFITFILE=$(find ${WORKDIR} -maxdepth 1 -name "*.multifit")
cp "$MULTIFITFILE" "${WORKDIR}/${OUTDIR}"

#do the fits
if [ ${DOFIT} = "true" ]
then
    echo "fitting..."
    ./fitconfGen.sh "$WORKDIR"
    echo "reading skim file '${SKIMFILE}'"
    #do the fitting jobs
    MAXJOBS=6
    for CONFIG in ${CONFIGFILES[@]}
    do
        FITFILE="${WORKDIR}/${CONFIG}"
        echo "fitting file: $CONFIG"
        ./fit.sh "$SKIMFILE" "$FITFILE" "${WORKDIR}/${OUTDIR}/${CONFIG%.*}" &
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

#draw graphs
if [ ${DODRAW} = "true" ]
then
    #execute drawing
    echo "generating drawings"
    echo "reading drawing configuration file: ${DRAWCONFIG}"
    for CONFIG in ${CONFIGFILES[@]}
    do
    NAME=${CONFIG%.*}
    ROOTFILEDRAW="${WORKDIR}/${OUTDIR}/${NAME}/${NAME}.root"
    echo "drawing ${ROOTFILEDRAW}"
    ./draw.sh "${ROOTFILEDRAW}" "${DRAWCONFIG}"
    done
fi

#do comparation drawing
if [ ${DOCDRAW} = "true" ]
then
    echo "starting comparative drawings..."
    COMPCONFFILES=$(find ${WORKDIR} -maxdepth 1 -name "*.drawcomp")
    for COMP in $COMPCONFFILES
    do
        COMPFILES=$( cat ${COMP} )
        COMPNAME=$( basename ${COMP} )
        echo "reading file: $COMPNAME"
        cp "$COMP" "${WORKDIR}/${OUTDIR}"
        ./multidraw.sh "${WORKDIR}/${OUTDIR}" "${WORKDIR}/${OUTDIR}/${COMPNAME%.*}" "${COMPFILES[@]}"
    done
fi

cp -R "${WORKDIR}/report" "${WORKDIR}/${OUTDIR}/report"
cd "${WORKDIR}/${OUTDIR}/report"
pdflatex -interaction nonstopmode "report.tex" >/dev/null
#cd "../../.."

echo "all done"