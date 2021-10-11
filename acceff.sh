#!/bin/bash
DEBUG="YES"

CLING="NO"
#input file for acceptancy located in datasets directory
FLAGS=${1:-"-all"}
ACCINPUTFILENAME="OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
EFFINPUTFILENAME="merged_HiForestAOD_MCFix2.root"
CONFIG="../rootfiles/analysis/merged_HiForestAOD_MCFix2_skim/merged_HiForestAOD_MCFix2_skim.cutconf"
DATA_MULTIFITINPUTFILENAME="merged_HiForestAOD_DATA_skim/multifit_baseline"
MC_MULTIFITINPUTFILENAME="merged_HiForestAOD_MCFix2_skim/multifit_baseline"
SKIMFILE=

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
ACCOUTPUTFILE="${OUTPUTFOLDER}/Acc/outputAcc.root"
EFFOUTPUTFILE="${OUTPUTFOLDER}/Eff/outputEff.root"
FINALOUTPUTFILE="${OUTPUTFOLDER}/AccEff/outputAccXEff.root"
YIELDFITOUTPUTFILE="${OUTPUTFOLDER}/YieldFit/yieldFit.root"
CLOSUREOUTPUTFILE="${OUTPUTFOLDER}/ClosureTest/closureTest.root"
ACCCORROUTPUTFILE="${OUTPUTFOLDER}/AccCorr/outputAccCorr.root"
EFFCORROUTPUTFILE="${OUTPUTFOLDER}/EffCorr/outputEffCorr.root"
FINALCORROUTPUTFILE="${OUTPUTFOLDER}/AccEffCorr/outputAccXEffCorr.root"

mkdir -p "$OUTPUTFOLDER"

TIMECMD=/usr/bin/time
TIMEARGS=-v

acc () {
    mkdir -p "${OUTPUTFOLDER}/Acc"
    $TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCOUTPUTFILE}" |& tee "${ACCOUTPUTFILE%.*}.log"
    }

eff () {
    mkdir -p "${OUTPUTFOLDER}/Eff"
    $TIMECMD $TIMEARGS ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFOUTPUTFILE}" "${CONFIG}" |& tee "${EFFOUTPUTFILE%.*}.log"
    #$TIMECMD $TIMEARGS gdb --args ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFOUTPUTFILE}" "${CONFIG}" |& tee "${EFFOUTPUTFILE%.*}.log"
    }

final () { 
    mkdir -p "${OUTPUTFOLDER}/AccEff"
    $TIMECMD $TIMEARGS ./AccEff/acceff -final "${ACCOUTPUTFILE}" "${EFFOUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${FINALOUTPUTFILE}" |& tee "${FINALOUTPUTFILE%.*}.log"
    }

closure () { 
    mkdir -p "${OUTPUTFOLDER}/ClosureTest"
    $TIMECMD $TIMEARGS ./AccEff/acceff -closure "${FINALOUTPUTFILE}" "${ACCOUTPUTFILE}" "${EFFOUTPUTFILE}" "${ACCINPUTFILE}" "${EFFINPUTFILE}" "${CONFIG}" "${CLOSUREOUTPUTFILE}" |& tee "${CLOSUREOUTPUTFILE%.*}.log"
    }

addw (){
    INPUT="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet/merged_HiForestAOD_DATA_skimjet.root"
    OUTPUTDIR="../rootfiles/analysis/merged_HiForestAOD_DATA_skimjet/weighted"
    ACC="../rootfiles/analysis/daniel/ptvsrap/Acc/OutAcc2D.root"
    EFF="../rootfiles/analysis/daniel/ptvsrap/Eff/OutEff2D.root"
    OUTPUT="${OUTPUTDIR}/acceff/merged_HiForestAOD_DATA_skimjet_w.root"
    mkdir -p $( dirname ${OUTPUT})
    $TIMECMD $TIMEARGS ./AccEff/acceff -addw ${INPUT} ${ACC} ${EFF} ${OUTPUT}
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
    $TIMECMD $TIMEARGS ./AccEff/acceff -fit "${FINALOUTPUTFILE}" "${YIELDFITOUTPUTFILE}" |& tee "${YIELDFITOUTPUTFILE%.*}.log"
    }

acccorr () { 
    mkdir -p "${OUTPUTFOLDER}/AccCorr"
    $TIMECMD $TIMEARGS ./AccEff/acceff -acc "${ACCINPUTFILE}" "${ACCCORROUTPUTFILE}" "${YIELDFITOUTPUTFILE}" |& tee  "${ACCCORROUTPUTFILE%.*}.log"
    }

effcorr () {
    mkdir -p "${OUTPUTFOLDER}/EffCorr"
    $TIMECMD $TIMEARGS ./AccEff/acceff -eff "${EFFINPUTFILE}" "${EFFCORROUTPUTFILE}" "${CONFIG}" "${YIELDFITOUTPUTFILE}" |& tee  "${EFFCORROUTPUTFILE%.*}.log"
    }

finalcorr () {
    mkdir -p "${OUTPUTFOLDER}/AccEffCorr"
    $TIMECMD $TIMEARGS ./AccEff/acceff -final "${ACCCORROUTPUTFILE}" "${EFFCORROUTPUTFILE}" "${DATA_FITINPUTFILENAME}" "${MC_FITINPUTFILENAME}" "${FINALCORROUTPUTFILE}" |& tee  "${FINALCORROUTPUTFILE%.*}.log"
    }

#execute skim
if [ $CLING = "YES" ]
then
cd AccEff
root -q 'AccEff.cpp("'../${ACCINPUTFILE}'","'../${EFFINPUTFILE}'","'../${ACCOUTPUTFILE}'","'../${CONFIG}'")'
cd ..
else
case $FLAGS in
    "-acc")
    acc
    ;;
    "-eff")
    eff
    ;;
    "-final")
    final
    ;;
    "-addw")
    addw
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
    "-finalcorr")
    finalcorr
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