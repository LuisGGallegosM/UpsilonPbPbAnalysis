
FILE1="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_w/multifit_baseline_jt1020/fit.root"
FILE2="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_w/multifit_baseline_jt2030/fit.root"
FILE3="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_w/multifit_baseline_jt3040/fit.root"
OUTPUT="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet_w/unfolding_baseline"


./AccEff/acceff "-yagg" ${FILE1} ${FILE2} ${FILE3} ${OUTPUT}