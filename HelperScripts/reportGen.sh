#!/bin/bash

MULTIFITFILE=${1}
DIR=${2}
FITS=${@:3}

mkdir "$DIR/report"
./Fitting/fit -report ${MULTIFITFILE} ${DIR} ${FITS}

cd "$DIR/report"
pdflatex "report.tex"


