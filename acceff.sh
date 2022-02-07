#!/bin/bash
DEBUG="YES"

CLING="NO"
#input file for acceptancy located in datasets directory
FLAGS=${1:-"-all"}
ACCINPUTFILENAME="OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
EFFINPUTFILENAME="merged_HiForestAOD_MC.root"
CONFIG="../rootfiles/analysis/merged_HiForestAOD_MC_skim/merged_HiForestAOD_MC_skim.cutconf"
DATA_MULTIFITINPUTFILENAME="merged_HiForestAOD_DATA_skim/multifit_baseline"
MC_MULTIFITINPUTFILENAME="merged_HiForestAOD_MC_skim/multifit_baseline"


#input file with relative path for Acceptancy
ACCINPUTFILE="../rootfiles/datasets/${ACCINPUTFILENAME}"
#input file with relative path for Effiency
EFFINPUTFILE="../rootfiles/datasets/${EFFINPUTFILENAME}"
#files with multifit results
DATA_FITINPUTFILENAME="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/fit.root"
MC_FITINPUTFILENAME="../rootfiles/analysis/${MC_MULTIFITINPUTFILENAME}/fit.root"
#folder to output all files
OUTPUTFOLDER="../rootfiles/analysis/${DATA_MULTIFITINPUTFILENAME}/acceff"

#output file, put in OUTPUTFOLDER and same as output folder but with .root extension
ACC_OUTPUTFILE="${OUTPUTFOLDER}/Acc/outputAcc.root"
EFF_OUTPUTFILE="${OUTPUTFOLDER}/Eff/outputEff.root"
ACCEFF_OUTPUTFILE="${OUTPUTFOLDER}/AccEff/outputAccXEff.root"
CROSSSECTION_FILE="${OUTPUTFOLDER}/AccEff/outputCrossSection.root"
YIELDFIT_OUTPUTFILE="${OUTPUTFOLDER}/YieldFit/yieldFit.root"
CLOSURE_OUTPUTFILE="${OUTPUTFOLDER}/ClosureTest/closureTest.root"
ACC_CORR_OUTPUTFILE="${OUTPUTFOLDER}/AccCorr/outputAccCorr.root"
EFF_CORR_OUTPUTFILE="${OUTPUTFOLDER}/EffCorr/outputEffCorr.root"
ACCEFF_CORR_OUTPUTFILE="${OUTPUTFOLDER}/AccEffCorr/outputAccXEffCorr.root"
CROSSSECTION_CORR_FILE="${OUTPUTFOLDER}/AccEff/outputCrossSectionCorr.root"

mkdir -p "$OUTPUTFOLDER"

TIMECMD=/usr/bin/time
TIMEARGS=-v

acc () {
    mkdir -p "${OUTPUTFOLDER}/Acc"
    $TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACC_OUTPUTFILE}" |& tee "${ACC_OUTPUTFILE%.*}.log"
    }

eff () {
    mkdir -p "${OUTPUTFOLDER}/Eff"
    $TIMECMD $TIMEARGS ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFF_OUTPUTFILE}" "${CONFIG}" |& tee "${EFF_OUTPUTFILE%.*}.log"
    #$TIMECMD $TIMEARGS gdb --args ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFF_OUTPUTFILE}" "${CONFIG}" |& tee "${EFF_OUTPUTFILE%.*}.log"
    }

acceff () { 
    mkdir -p "${OUTPUTFOLDER}/AccEff"
    $TIMECMD $TIMEARGS ./AccEff/acceff -acceff "${ACC_OUTPUTFILE}" "${EFF_OUTPUTFILE}" "${ACCEFF_OUTPUTFILE}" |& tee "${ACCEFF_OUTPUTFILE%.*}.log"
    }

crossect () { 
    mkdir -p "${OUTPUTFOLDER}/CrossSect"
    $TIMECMD $TIMEARGS ./AccEff/acceff -crosssection "${ACCEFF_OUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${CROSSSECTION_FILE}" |& tee "${CROSSSECTION_FILE%.*}.log"
    }

closure () { 
    mkdir -p "${OUTPUTFOLDER}/ClosureTest"
    $TIMECMD $TIMEARGS ./AccEff/acceff -closure "${ACCEFF_OUTPUTFILE}" "${ACC_OUTPUTFILE}" "${EFF_OUTPUTFILE}" "${ACCINPUTFILE}" "${EFFINPUTFILE}" "${CONFIG}" "${CLOSURE_OUTPUTFILE}" |& tee "${CLOSURE_OUTPUTFILE%.*}.log"
    }

addw (){
    DATA_INPUT="../rootfiles/analysis/merged_HiForestAOD_DATA_skim/merged_HiForestAOD_DATA_skim.root"
    OUTPUTDIR="../rootfiles/analysis/merged_HiForestAOD_DATA_skim/weighted"
    ACC="../rootfiles/analysis/daniel/ptvsrap/Acc/OutAcc2D.root"
    EFF="../rootfiles/analysis/daniel/ptvsrap/systematic_errors/OutEff2DSyst.root"
    OUTPUT="${OUTPUTDIR}/acceff/merged_HiForestAOD_DATA_skimjet_w.root"
    mkdir -p $( dirname ${OUTPUT})
    $TIMECMD $TIMEARGS ./AccEff/acceff -addw ${DATA_INPUT} ${ACC} ${EFF} ${OUTPUT}
}

addw2 (){
    DATA_INPUT="../rootfiles/analysis/merged_HiForestAOD_DATA_skim/merged_HiForestAOD_DATA_skim.root"
    OUTPUTDIR="../rootfiles/analysis/merged_HiForestAOD_DATA_skim/weighted"
    ACC="../rootfiles/analysis/daniel/ptvsrap/Acc/OutAcc2D.root"
    EFF="../rootfiles/analysis/daniel/ptvsrap/systematic_errors/OutEff2DSyst.root"
    OUTPUT="${OUTPUTDIR}/acceff/merged_HiForestAOD_DATA_skim.root"
    mkdir -p $( dirname ${OUTPUT})
    $TIMECMD $TIMEARGS ./AccEff/acceff -addw2 ${DATA_INPUT} ${ACC} ${EFF} ${OUTPUT}
}

closurejet () { 
    INPUT="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/merged_HiForestAOD_MC_skimjet.root"
    OUTPUTDIR="../rootfiles/analysis/merged_HiForestAOD_MC_skimjet/closurejet/ptvsrap"
    ACCUNWEIGHTED="../rootfiles/analysis/daniel/ptvsrap/Acc/OutAcc2D.root"
    EFFUNWEIGHTED="../rootfiles/analysis/daniel/ptvsrap/Eff/OutEff2D.root"
    ACCWEIGHTED=$ACCUNWEIGHTED
    EFFWEIGHTED=$EFFUNWEIGHTED
    OUTPUT="${OUTPUTDIR}/acceff/output.root"
    mkdir -p $( dirname ${OUTPUT})
    $TIMECMD $TIMEARGS ./AccEff/acceff -closurejet ${INPUT} ${ACCUNWEIGHTED} ${EFFUNWEIGHTED} ${ACCWEIGHTED} ${EFFWEIGHTED} ${OUTPUT}

    ACCUNWEIGHTED="../rootfiles/analysis/daniel/ptvsrap/Acc/OutAcc2Dfactor2.root"
    EFFUNWEIGHTED="../rootfiles/analysis/daniel/ptvsrap/Eff/OutEff2Dfactor2.root"
    ACCWEIGHTED=$ACCUNWEIGHTED
    EFFWEIGHTED=$EFFUNWEIGHTED
    OUTPUT="${OUTPUTDIR}/factor2/output.root"
    mkdir -p $( dirname ${OUTPUT})
    $TIMECMD $TIMEARGS ./AccEff/acceff -closurejet ${INPUT} ${ACCUNWEIGHTED} ${EFFUNWEIGHTED} ${ACCWEIGHTED} ${EFFWEIGHTED} ${OUTPUT} 

    ACCUNWEIGHTED="../rootfiles/analysis/daniel/ptvsrap/Acc/OutAcc2Dfactor1over2.root"
    EFFUNWEIGHTED="../rootfiles/analysis/daniel/ptvsrap/Eff/OutEff2Dfactor1over2.root"
    ACCWEIGHTED=$ACCUNWEIGHTED
    EFFWEIGHTED=$EFFUNWEIGHTED
    OUTPUT="${OUTPUTDIR}/factor1over2/output.root"
    mkdir -p $( dirname ${OUTPUT})
    $TIMECMD $TIMEARGS ./AccEff/acceff -closurejet ${INPUT} ${ACCUNWEIGHTED} ${EFFUNWEIGHTED} ${ACCWEIGHTED} ${EFFWEIGHTED} ${OUTPUT} 
    }

fit () { 
    mkdir -p "${OUTPUTFOLDER}/YieldFit"
    $TIMECMD $TIMEARGS ./AccEff/acceff -fit "${CROSSSECTION_FILE}" "${YIELDFIT_OUTPUTFILE}" |& tee "${YIELDFIT_OUTPUTFILE%.*}.log"
    }

acccorr () { 
    mkdir -p "${OUTPUTFOLDER}/AccCorr"
    $TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACC_CORR_OUTPUTFILE}" "${YIELDFIT_OUTPUTFILE}" |& tee  "${ACC_CORR_OUTPUTFILE%.*}.log"
    }

effcorr () {
    mkdir -p "${OUTPUTFOLDER}/EffCorr"
    $TIMECMD $TIMEARGS ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFF_CORR_OUTPUTFILE}" "${CONFIG}" "${YIELDFIT_OUTPUTFILE}" |& tee  "${EFF_CORR_OUTPUTFILE%.*}.log"
    }

acceffcorr () {
    mkdir -p "${OUTPUTFOLDER}/AccEffCorr"
    $TIMECMD $TIMEARGS ./AccEff/acceff -acceff "${ACC_CORR_OUTPUTFILE}" "${EFF_CORR_OUTPUTFILE}" "${ACCEFF_CORR_OUTPUTFILE}" |& tee  "${ACCEFF_CORR_OUTPUTFILE%.*}.log"
    }

crossectcorr () { 
    mkdir -p "${OUTPUTFOLDER}/CrossSectCor"
    $TIMECMD $TIMEARGS ./AccEff/acceff -crossection "${ACCEFF_CORR_OUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${CROSSSECTION_CORR_FILE}" |& tee "${CROSSSECTION_FILE%.*}.log"
    }

#execute skim
if [ $CLING = "YES" ]
then
cd AccEff
root -q 'AccEff.cpp("'../${ACCINPUTFILE}'","'../${EFFINPUTFILE}'","'../${ACC_OUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
case $FLAGS in
    "-acc")
    acc
    ;;
    "-eff")
    eff
    ;;
    "-acceff")
    acceff
    ;;
    "-crosssection")
    crossect
    ;;
    "-addw")
    addw
    ;;
    "-addw2")
    addw2
    ;;
    "-closure")
    closure
    ;;
    "-closurejet")
    closurejet
    ;;
    "-fit")
    fit
    ;;
    "-acccorr")
    acccorr
    ;;
    "-effcorr")
    effcorr
    ;;
    "-acceffcorr")
    acceffcorr
    ;;
    "-crossectioncorr")
    crossectcorr
    ;;
    "-all")
    acc
    eff
    final
    closure
    fit
    acccorr
    effcorr
    finalcorr
    ;;
esac
fi