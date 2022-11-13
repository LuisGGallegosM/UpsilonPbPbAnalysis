
.PHONY: AccEff
.PHONY: Drawing
.PHONY: Fitting
.PHONY: Skimming
.PHONY: Utils
.PHONY: OniaBase

all:
	$(MAKE) -C Utils
	$(MAKE) -C OniaBase
	$(MAKE) -C Drawing
	$(MAKE) -C Skimming
	$(MAKE) -C Fitting
	$(MAKE) -C AccEff

clear:
	$(MAKE) clear -C Utils
	$(MAKE) clear -C OniaBase
	$(MAKE) clear -C Drawing
	$(MAKE) clear -C Skimming
	$(MAKE) clear -C Fitting
	$(MAKE) clear -C AccEff

AccEff:
	$(MAKE) -C AccEff

Drawing:
	$(MAKE) -C Drawing

Fitting:
	$(MAKE) -C Fitting

Skimming:
	$(MAKE) -C Skimming

Utils:
	$(MAKE) -C Utils

OniaBase:
	$(MAKE) -C OniaBase

analysis: analysis_mc analysis_data

analysis_mc:
	$(MAKE) -f analysismc_make

analysis_data:
	$(MAKE) -f analysisdata_make