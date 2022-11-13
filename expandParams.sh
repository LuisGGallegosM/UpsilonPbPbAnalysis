
INPUTFILE=${1:-"/media/gabriel/STORAGE0/rootfiles/confFiles/MC/merged_HiForestAOD_MC_baseline_jt1020.multifit"}
OUTPUTDIR=${2:-"/media/gabriel/STORAGE0/rootfiles/confFiles/MC/s"}
VARNAME=${3:-"bkg.type"}
VARS=${4:- cheb0 cheb1 cheb2 cheb3 cheb4 cheb5 }

./Fitting/fit -expand "${INPUTFILE}" "${OUTPUTDIR}" "${VARNAME}" ${VARS} > ${OUTPUTDIR}/expandParams.log