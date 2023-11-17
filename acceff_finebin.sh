
INPUTFILE="../rootfiles/datasets/merged_HiForestAOD_MC.root"
INPUTGENONLYFILE="../rootfiles/datasets/OniaTree_Ups1SMM_5p02TeV_TuneCUETP8M1_nofilter_pp502Fall15-MCRUN2_71_V1-v1_GENONLY.root"
OUTFILEACC="../rootfiles/analysis/acceff/mod/OutAcc2D.root"
OUTFILEEFF="../rootfiles/analysis/acceff/mod/OutEff2D.root"

#hGenUpsilonEffDen
./AccEff/acceff -accfinebin ${INPUTGENONLYFILE} ${OUTFILEACC}
#./AccEff/acceff -efffinebin ${INPUTFILE} ${OUTFILEEFF}