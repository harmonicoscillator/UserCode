source /osg/grid/setup.sh
grid-proxy-init
source /osg/app/crab/crab.sh # CRAB
source /osg/app/glite/etc/profile.d/grid_env.sh # gLite-UI
source /osg/app/cmssoft/cms/cmsset_default.sh #CMSSW

cd /net/hisrv0001/home/luck/CMSSW_5_3_8_HI_patch1/src
eval `scram runtime -sh`
cd -
