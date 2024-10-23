require stream
require rs_nrpxxsn
require autosave
require iocstats

epicsEnvSet("TOP",     "$(E3_CMD_TOP)/..")
epicsEnvSet("AS_TOP",  "$(E3_CMD_TOP)")
epicsEnvSet("IOCDIR",  "TS2-010:Ctrl-IOC-002")
epicsEnvSet("IOCNAME", "TS2-010RFC:SC-IOC-007")
epicsEnvSet("SETTINGS_FILES","settings")

epicsEnvSet("N1",	"1")
epicsEnvSet("R1",	"EMR-RFPM-010:")
epicsEnvSet("IP1",	"pr-01-srf.tn.esss.lu.se")

# Add extra startup scripts requirements here
iocshLoad("$(autosave_DIR)/autosave.iocsh")
iocshLoad("$(iocstats_DIR)/iocStats.iocsh")
iocshLoad("${rs_nrpxxsn_DIR}/rs_nrpxxsn.iocsh", "P=$(P), R=$(R1), N=$(N1), IP=$(IP1), ASYN_PORT=$(R1)")

# Call autosave function
afterInit("fdbrestore("$(AS_TOP)/$(IOCNAME)/save/$(SETTINGS_FILES).sav")")

# Start any sequence programs
seq("sncProcess","P=$(P), R=$(R1)")

# Call iocInit to start the IOC
iocInit()
