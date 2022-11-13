
INPUTFILE=${1:-"../rootfiles/confFiles/DATA/merged_HiForestAOD_DATA_baseline_jt1020.multifit"}
OUTPUTDIR=${2:-"../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet/supermultifit_baseline_jt1020"}
VARNAME=${3:-"bkg.type"}
VARS=${4:- cheb0 cheb1 cheb2 cheb3 cheb4 cheb5 }

mkdir -p "${OUTPUTDIR}"
./Fitting/fit -expand "${INPUTFILE}" "${OUTPUTDIR}" "${VARNAME}" ${VARS} > ${OUTPUTDIR}/expandParams.log