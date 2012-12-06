#!/bin/sh
# exec_condor.sh
# AUTHOR: Alex Barbieri
# based on similar tools from Yetkin Yilmaz and Dragos Velicanu


# USAGE:
# $1 is the CMSSW environment to source before cmsRun.
# $2 is the python config to run using cmsRun.
# $3 should be the destination folder which must already exist.
# $4 is the complete name of the output file which will appear 
#    in the destination folder.

# All other arguments are passed to the python config as-is.
# The python config should respect the following ivars variables:
#   inputFiles
#   maxEvents
#   outputFile


CMSSWENV=$1
CONFIGFILE=$2
DESTINATION=$3
OUTFILE=$4

source /osg/app/cmssoft/cms/cmsset_default.sh
export SCRAM_ARCH=slc5_amd64_gcc462
cd $CMSSWENV
eval `scramv1 runtime -sh` 
cd -

#pass all but the first four arguments on to the python
cmsRun $CONFIGFILE ${@:5} outputFile=${CONFIGFILE}_output.root

#after the output has finished move it to Hadoop
#note: never move large files over anything in /net on cgate
#mv ${CONFIGFILE}_output.root ${DESTINATION}/${OUTFILE}
