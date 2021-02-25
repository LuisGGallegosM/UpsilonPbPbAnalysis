
.PHONY: AccEff
.PHONY: Drawing
.PHONY: Fitting
.PHONY: Skimming
.PHONY: Utils
.PHONY: TreeProcessor
.PHONY: OniaBase

all:
	$(MAKE) -C Utils
	$(MAKE) -C TreeProcessor
	$(MAKE) -C OniaBase
	$(MAKE) -C Skimming
	$(MAKE) -C Fitting
	$(MAKE) -C Drawing
	$(MAKE) -C AccEff

clear:
	$(MAKE) clear -C Utils
	$(MAKE) clear -C TreeProcessor
	$(MAKE) clear -C OniaBase
	$(MAKE) clear -C Skimming
	$(MAKE) clear -C Fitting
	$(MAKE) clear -C Drawing
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

TreeProcessor:
	$(MAKE) -C TreeProcessor

OniaBase:
	$(MAKE) -C OniaBase