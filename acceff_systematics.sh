
SYSFILESDIR="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet_w/systematics"
ANDIR="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet_w/systematics/analysis"
NAME=merged_HiForestAOD
TAG=MC

UNFOLDINGFILEMC=../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/unfolding_baseline/unfold_training.root

SYSFILES=( \
"merged_HiForestAOD_${TAG}_skimjet_w_Nominal.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_GMMinus1StatErr.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_GMMinus1SystErr.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_GMPlus1StatErr.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_GMPlus1SystErr.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_HSMinus1StatErr.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_HSMinus1SystErr.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_HSPlus1StatErr.root" \
"merged_HiForestAOD_${TAG}_skimjet_w_HSPlus1SystErr.root" \
)

MULTIFITS=( "baseline_jt1020" "baseline_jt2030" "baseline_jt3040" )

for FILE in ${SYSFILES[@]}; do
    OUTD=${ANDIR}/${FILE%.*}
    SKIMWDIR=${OUTD}/${NAME}_${TAG}_skimjet_w
    SKIMW=${SKIMWDIR}/${NAME}_${TAG}_skimjet_w.root
    mkdir -p ${OUTD}
    mkdir -p ${SKIMWDIR}
    cp ${SYSFILESDIR}/${FILE} ${SKIMW}
    cp ${SYSFILESDIR}/merged_HiForestAOD_${TAG}_skimjet_w.cutconf ${SKIMWDIR}/merged_HiForestAOD_${TAG}_skimjet_w.cutconf
    for MULTIFIT in ${MULTIFITS[@]}; do 
        #./expandParams.sh ../rootfiles/confFiles/${TAG}/${NAME}_${TAG}_${MULTIFIT}.multifit ${SKIMWDIR}/supermultifit_${MULTIFIT}
        #./supermultifit.sh ${SKIMW} ${SKIMWDIR}/supermultifit_${MULTIFIT} ${SKIMWDIR}/supermultifit_${MULTIFIT} multifit_${MULTIFIT}
        #./llr.sh ${SKIMWDIR}/supermultifit_${MULTIFIT} multifit_${MULTIFIT} ${SKIMWDIR}/supermultifit_${MULTIFIT}/llr.multifit
        ./multifit.sh ../rootfiles/confFiles/${TAG}/${NAME}_${TAG}_${MULTIFIT}.multifit ${SKIMW} ${SKIMWDIR}/multifit_${MULTIFIT}
        #./multifit.sh ${SKIMWDIR}/supermultifit_${MULTIFIT}/llr.multifit ${SKIMW} ${SKIMWDIR}/multifit_${MULTIFIT}
    done
    #./unfold_data.sh ${SKIMW} ${UNFOLDINGFILEMC} ${SKIMWDIR}/unfolding_baseline/unfolding.root
done

