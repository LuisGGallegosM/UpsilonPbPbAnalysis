## UpsilonPbPbAnalysis

# Packages:
- Utils
	- Helper: Some helper functions for opening files and managing strings.
	- Serializer: Class used in serialization of some other classes, like fitParams.
- OniaBase
	- Common : common functionality within this package
	- JetCorrection : class JetCorrector and JetUncertainty.
	- OniaIO : manages input and output data formating for TTree files.
	- Params : defines common classes for defining and using serializable parameter configuration files, cut parameters, etc.
	- TreeProccessor : base functionality for reading and writing TTrees and looping inside.
- Drawing
	- Basic functions for drawing, used in Fitting package.
- Skimming
	- Common : common functionality
	- OniaJetSkim : Upsilon and jet skimming.
	- OniaSkim : Upsilon skimming.
- Fitting
	- Drawcomp : draws plots for parameter comparison in pT bins ( parameters vs pT )
	- DrawPlot : draws plots for a single fit result.
	- Fit: Upsilon Fitting.
- AccEff
	- Acc : Acceptancy test
	- AccEffResults : Final steps after Acceptancy and Efficiency test (e.g. yield corrections).
	- Common : common functionality
	- Eff : Efficiency test
- Helper Scripts
    - cutconfGen.sh : script for generating a simple cut config file.
    - drawconfGen.sh : script for generating a simple draw config file.
    - fitconfGen.sh : script to generate a set of fit configuration files from a multifit file.
    - multifitGen.sh : example of a multifit file.
- acceff.sh : script for acceptancy and efficiency tests.
- draw.sh : script for drawing fit results.
- fit.sh : script for fitting.
- multifit.sh : script for running multiple fits and generating parameter vs pT plots.
- skim.sh : script for skimming.

## Dependecy graph:
![Dependecy graph](https://github.com/LuisGGallegosM/UpsilonPbPbAnalysis/blob/master/Doc/DependecyGraph.png)
