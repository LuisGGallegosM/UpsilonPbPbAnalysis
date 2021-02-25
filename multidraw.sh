
echo "multiple drawing"
#do fit comparison drawings
INPUTDIR="${1}"
OUTDIR="${2}"
CONFIGFILES=( ${@:3} )
NAME="${OUTDIR}/fit.pdf"
mkdir ${OUTDIR}

FITFILENAMES=()
for CONFIG in ${CONFIGFILES[@]}
do
    INPUTFILE="${INPUTDIR}/${CONFIG%.*}/${CONFIG%.*}.fit"
    echo "reading fit result file '${INPUTFILE}'"

    if [ ! -f ${INPUTFILE} ]
    then
        echo "Error: ${INPUTFILE} does not exists!"
        exit 1
    fi

    FITFILENAMES=( ${FITFILENAMES[@]} $INPUTFILE)
done

./Drawing/draw -m $NAME "${FITFILENAMES[@]}" > "${NAME%.*}_Drawing.log" 2>&1

echo "output to ${OUTDIR}"
echo "multidraw completed"
