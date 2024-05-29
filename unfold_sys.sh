TAG="unfolding_baseline_jtpt"
RESPONSEFILE="${1:-../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/unfolding_baseline/unfold_training.root}"
OUTPUTFILE="${2:-../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_w/unfolding_baseline}"

mkdir -p "${OUTPUTFILE}/systematics"
./AccEff/acceff -unfold_syst ${RESPONSEFILE} ${OUTPUTFILE} > "${OUTPUTFILE}/systematics/log.txt"