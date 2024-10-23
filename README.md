
rs_nrpxxsn
======
European Spallation Source ERIC Site-specific EPICS module : rs_nrpxxsn


This module should compile as a regular EPICS module.

* Run `make` from the command line.

This module should also compile as an E3 module:

* Install `conda` (https://docs.conda.io/en/latest/miniconda.html)
* Create and activate a conda environment that minimally includes the following packages: `make perl tclx compilers epics-base require`
* Run `make -f Makefile.E3 target` to build `target` using the E3 build process
