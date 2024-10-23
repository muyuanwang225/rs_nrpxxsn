TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = rs_nrpxxsn
# rs_nrpxxsn.dbd will be created and installed
DBD += rs_nrpxxsn.dbd

# rs_nrpxxsn.dbd will be made up from these files:
rs_nrpxxsn_DBD += base.dbd

# Include dbd files from all support applications:
#rs_nrpxxsn_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#rs_nrpxxsn_LIBS += xxx

# rs_nrpxxsn_registerRecordDeviceDriver.cpp derives from rs_nrpxxsn.dbd
rs_nrpxxsn_SRCS += rs_nrpxxsn_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
rs_nrpxxsn_SRCS_DEFAULT += rs_nrpxxsnMain.cpp
rs_nrpxxsn_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#rs_nrpxxsn_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
rs_nrpxxsn_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
