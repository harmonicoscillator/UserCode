#!/bin/sh
#Author Alex Barbieri, based on material from Yongsun Kim

SAMPLE="/net/hisrv0001/home/luck/CONDOR_LOGS/hlt_master_8E33v2_L1reEmul/*err"

TRIGGERS="
HLT_PAPhoton10_CaloIdVL_v1 \
HLT_PAPhoton10_LooseCaloIdVL_v1 \
HLT_PAPhoton10_TightCaloIdVL_v1 \
HLT_PAPhoton10_CaloIdVL_Iso50_v1 \
HLT_PAPhoton10_TightCaloIdVL_Iso50_v1 \
HLT_PAPhoton15_CaloIdVL_v1 \
HLT_PAPhoton20_CaloIdVL_v1 \
HLT_PAPhoton20_LooseCaloIdVL_v1 \
HLT_PAPhoton20_TightCaloIdVL_v1 \
HLT_PAPhoton20_CaloIdVL_Iso50_v1 \
HLT_PAPhoton20_TightCaloIdVL_Iso50_v1 \
HLT_PAPhoton30_CaloIdVL_v1 \
HLT_PAPhoton30_LooseCaloIdVL_v1 \
HLT_PAPhoton30_TightCaloIdVL_v1 \
HLT_PAPhoton30_CaloIdVL_Iso50_v1 \
HLT_PAPhoton30_TightCaloIdVL_Iso50_v1 \
HLT_PAPhoton40_CaloIdVL_v1 \
HLT_PAPhoton40_LooseCaloIdVL_v1 \
HLT_PAPhoton40_TightCaloIdVL_v1 \
HLT_PAPhoton40_CaloIdVL_Iso50_v1 \
HLT_PAPhoton10_NoCaloIdVL_v1 \
HLT_PAPhoton20_NoCaloIdVL_v1 \
HLT_PAPhoton30_NoCaloIdVL_v1 \
HLT_PAPhoton40_NoCaloIdVL_v1 \
HLT_PAPhoton60_NoCaloIdVL_v1 \
HLT_PAPhoton10_Photon10_NoCaloIdVL_v1 \
HLT_PAPhoton10_Photon10_CaloIdVL_v1 \
HLT_PAPhoton10_Photon10_TightCaloIdVL_v1 \
HLT_PAPhoton10_Photon10_CaloIdVL_Iso50_v1 \
HLT_PAPhoton10_Photon10_TightCaloIdVL_Iso50_v1 \
HLT_PAPhoton15_Photon10_NoCaloIdVL_v1 \
HLT_PAPhoton15_Photon10_CaloIdVL_v1 \
HLT_PAPhoton15_Photon10_TightCaloIdVL_v1 \
HLT_PAPhoton15_Photon10_CaloIdVL_Iso50_v1 \
HLT_PAPhoton20_Photon15_NoCaloIdVL_v1 \
HLT_PAPhoton20_Photon15_CaloIdVL_v1 \
HLT_PAPhoton20_Photon15_TightCaloIdVL_v1 \
HLT_PAPhoton20_Photon15_CaloIdVL_Iso50_v1 \
HLT_PAPhoton20_Photon20_NoCaloIdVL_v1 \
HLT_PAPhoton20_Photon20_CaloIdVL_v1 \
HLT_PAPhoton20_Photon20_TightCaloIdVL_v1 \
HLT_PAPhoton20_Photon20_CaloIdVL_Iso50_v1 \
HLT_PAPhoton30_Photon30_NoCaloIdVL_v1"

echo "Reading trigger logs."
echo "#trigger #total #name"

for triggername in $TRIGGERS
do
    tot=0
    trg=0

    grep "TrigReport ---------- Path   Summary ------------" $SAMPLE -A 120 | grep $triggername  >& tempLog.txt

    while read inputline
    do
	nTot="$(echo $inputline | awk '{print $4}')"
	tot=`expr $tot + $nTot`

	nTrig="$(echo $inputline | awk '{print $5}')"
	trg=`expr $trg + $nTrig`


    done < tempLog.txt

    echo "${trg} ${tot} ${triggername}"

done