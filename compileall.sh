#!/bin/bash

cd Skimming
./skimcompile.sh
cd ..

cd Fitting
./fitcompile.sh
cd ..

cd Drawing
./drawcompile.sh
cd ..