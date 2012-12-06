#!/bin/bash
# submit_condor.sh
# AUTHOR: Alex Barbieri
# based on similar tools from Yetkin Yilmaz and Dragos Velicanu

# USAGE:
# Change the values in #Inputs#. Changes to other sections and
# exec_condor.sh should not be required in normal operation.

# The python config should respect the following ivars variables:
#   inputFiles
#   maxEvents (always forced to -1, otherwise output gets renamed)
#   outputFile

######################## Inputs ######################################

# This is the environment which gets sourced before running.
CMSSWENV=/net/hisrv0001/home/luck/CMSSW_5_3_7

# This is the python config which is run using cmsRun.
# Must be relative path from submit_condor.sh
#CONFIGFILE=runForest_44X.py
CONFIGFILE=yongsun_Dec_V45_reemul.py

# This is the file list of inputs to give to the python config. Shell
# globbing is useful here.
#DATASET=/mnt/hadoop/cms/store/user/kimy/HIMinBiasUPC/MinBias-reTracking-v1v2/*
#DATASET=/mnt/hadoop/cms/store/user/yetkin/MC_Production/HydjetDrum03/Track_v4/set[12]*
#DATASET=/mnt/hadoop/cms/store/user/yenjie/Hydjet1.8/Z2/Dijet80/tracking_v1/*
#DATASET=/mnt/hadoop/cms/store/user/davidlw/PAPhysics/PA2012_RAWSKIM_SingleTrack_v2//9f9d3879c8002f93796d3cf9442111e1/*
DATASET=/mnt/hadoop/cms/store/user/icali/AllPhysics2760/pp276_Skim_HLT_L1BscMinBiasORBptxPlusANDMinus_v2/e16e4b5a36aa9fa33062b47de2724a60/*

# This is the folder to which the output is copied after the job
# finishes.  The folder is created if it does not already exist.
# WARNING: large files should not be copied to destinations beginning
# with '/net'. Use local disks or hadoop instead.
#DESTINATION=/mnt/hadoop/cms/store/user/luck/HiMinbias_RecHitTowers_MC_v2
DESTINATION=/mnt/hadoop/cms/store/user/luck/photon_trig

# This is the name of the output files which will be placed in the
# destination folder. "_${jobNum}.root" is appended to this filename
# to avoid collisions.
OUTFILE=luck_logs

# The number of input files each job should handle. 
FILESPERJOB=1

# The total number of input files to use. If this number is larger
# than the real number of files in 'DATASET', uses all of the files in
# 'DATASET'.
MAXFILES=3000

# Location where condor logs will be placed. Logs will be named
# 'LOGNAME-$dateTime-$jobNum' appended to avoid collision.
LOGDIR=/net/hisrv0001/home/luck/CONDOR_LOGS/yongsun_Dec_V45
LOGNAME=triglog


########################## Create subfile ###############################
dateTime=$(date +%Y%m%d%H%M)
fileCounter=0
jobNum=0
inputList=()
mkdir -p $DESTINATION
mkdir -p $LOGDIR

for file in $DATASET
do
    if [ $fileCounter -ge $MAXFILES ]
    then
	break
    fi

    jobNum=$((fileCounter/FILESPERJOB))
    inputList[$jobNum]+="inputFiles=file:$file "
    fileCounter=$((fileCounter+1))
done

for (( i=0; i<=${jobNum}; i++ ))
do 
    # make the condor file
    cat > subfile <<EOF

Universe = vanilla
Initialdir = .
Executable = exec_condor.sh
+AccountingGroup = "group_cmshi.$(whoami)"
Arguments =  $CMSSWENV $CONFIGFILE $DESTINATION ${OUTFILE}_${i}.root ${inputList[$i]} maxEvents=-1
GetEnv       = True
Input = /dev/null

# log files
Output       = $LOGDIR/$LOGNAME-$dateTime-$i.out
Error        = $LOGDIR/$LOGNAME-$dateTime-$i.err
Log          = $LOGDIR/$LOGNAME-$dateTime-$i.log

should_transfer_files = YES
when_to_transfer_output = ON_EXIT
transfer_input_files = $CONFIGFILE

Queue
EOF


############################ Submit ###############################


#cat subfile
condor_submit subfile
mv subfile $LOGDIR/$LOGNAME-$dateTime-$i.subfile
done

echo "Submitted $((jobNum + 1)) jobs to Condor, $fileCounter files with $FILESPERJOB files per job."

