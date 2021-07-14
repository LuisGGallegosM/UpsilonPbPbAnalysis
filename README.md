## UpsilonPbPbAnalysis

# Getting started
This repo uses a make build system for compilation in Linux using GCC.
ROOT library should be present.

Run "make" in the main directory to build/update automatically all packages, should be run if any change to c++ source code is made. Run in root command line will be done in one or two days.
Repo is divided into Packages and each package into one or more modules.
The bash scripts:
- acceff.sh
- draw.sh
- fit.sh
- multifit.sh
- skim.sh

are the most top level entities in the repo, analyses can be run more easily with these.

Packages:
- Skimming
- Fitting
- AccEff

Are executable ones and can be run directly in terminal if desired, the associated script serves as a example of its usage.

# Dependecy graph:
Most packages are not standalone and depends on another as follows to reuse code (arrow indicates "depends on"):
![Dependecy graph](https://github.com/LuisGGallegosM/UpsilonPbPbAnalysis/blob/master/Doc/DependecyGraph.png)


# Packages explanation:
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
- multifit.sh : script for running multiple fits in parallel and generating parameter vs pT plots.
- skim.sh : script for skimming.

# HOW TO FIT

Code for Fitting is in the Fitting subdirectory.
All commands must be executed in the main repository directory.

First make sure run make command build the framework.

```
make
```

For fitting there is fit.sh and multifit.sh scripts.

For a single fit use fit.sh.
For a full range fit use multifit.sh

multifit.sh
- configure inside the multifit.sh bash script the paths to the files using the variables:
	- MULTIFITFILE : path to the multifit configuration file that describes the how fits will be done.
	- INPUTFILENAME : path to the file previously skimmed using the Skimming part of the framework. the skimming framework placed a .cutconf file in the same directory as the skim output and must be present.
	- DRAWCONFIG : path to a draw configuration file ( under changes, use default draw config file )
	- MAXJOBS : max number of cores/threads to use at the same time.

use bash script multifitGen.sh (or get them from the ./HelperScripts/examplefiles subdirectory)
```
./HelperScripts/multifitGen.sh -DATA outputfile.multifit
```
to generate a example multifit configuration file, second argument is the output file path.

Similarly use 
```
./HelperScripts/drawconfGen.sh outputfile.drawconf
```
to get a drawconf example file.

edit the multifit configuration file to set the fitting parameters.
in parameters like:
signal.nSigY1S.value
-	value indicates the initial value
-	high indicates the maximum value
-	low indicates the minimum value
-	fixed indicates if is fixed or not, valid values are "true" or "false"
keep the whitespace between the equals signs in the parameters definitions.

-	"bkg.type" indicates the type of the background function, valid values are: "none", "chev", "special", "exponential"
-	"moreUpsilon" indicates if there is just Upsilon 1S or 1S+2S+3S
-	"diffVar.name" is the name of the differential variable, example "pt"
-	"diffVar.values.0" indicates the binning of the differential variable.

if a single value is put in initial value of a parameter, all fits will have the same initial value. To set different values for each fit, enclose the values in brackets separated with whitespaces, for example for 4 bins:
```
signal.alpha.value = [ 3.0 2.0 1.0 0.5 ]
```
Code about Fitting functions is in : Fitting/Fit/FitFunctions.cpp



parameters for chev background are like:
	"bkg.chk4_k1.value", "bkg.chk4_k2.value", etc.
place or remove them for incresing or decreacing the chebychev poly order.

parameters for exponential are like:
	"bkg.lambda.value"

parameters for special background are:
	"bkg.mu.value"
	"bkg.sigma.value"
	"bkg.lambda.value"

To run the fitting use the previously configurated bash script multifit.sh
```
./multifit.sh
```

Fitting for all bins will be done in up to MAXJOBS simultaneusly.
All output will be placed in the same directory as the skimmed file used, in a directory named multifit, including a copy of the configurations files used and a directory named "report" where a summary pdf file with all fit plots drawn.