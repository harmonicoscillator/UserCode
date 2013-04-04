source /afs/cern.ch/cms/LCG/LCG-2/UI/cms_ui_env.sh
voms-proxy-init --valid 168:00 -voms cms 
#-pwstdin < $HOME/.grid-cert-passphrase
voms-proxy-info --all
source /afs/cern.ch/cms/cmsset_default.sh
#todo to put on afs
cd /afs/cern.ch/user/r/richard/CMSSW_5_3_8_HI_patch2
eval `scram runtime -sh`
source /afs/cern.ch/cms/ccs/wm/scripts/Crab/crab.sh
cd -
#export STAGE_SVCCLASS=cmscaf 

