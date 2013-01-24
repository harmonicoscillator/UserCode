#include "make_comp_plots_photontree.C"
#include "make_comp_plots_rechittree.C"
#include "make_etaphi_photontree.C"
#include "make_sigmaetaeta.C"


void makeAllPlots()
{
  const bool save = true;
  
  const bool doPilot = false;
  const bool doMC = false;
  const bool doData = true;

  //MIT files
  // const TString pilotForest = "/mnt/hadoop/cms/store/user/luck/pA2013_pilot/PA2013_HiForest_Express_r0_pilot_minbias_v0.root";
  // const TString dataForest = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_Express_r210635_autoforest_v2.root";
  // const TString mcForest = "/mnt/hadoop/cms/store/user/luck/pA2013_MC/HiForest_pPb_Hijing_NEWFIX_v2.root";

  //CERN files
  const TString pilotForest = "root://eoscms//eos/cms/store/group/phys_heavyions/velicanu/forest/PA2013_HiForest_Express_r0_pilot_minbias_v0.root";
  const TString dataForest = "root://eoscms//eos/cms/store/group/phys_heavyions/velicanu/forest/PA2013_HiForest_PromptRecofirstPR_forestv51.root";
  const TString mcForest = "root://eoscms//eos/cms/store/group/phys_heavyions/azsigmon/HiForest_pPb_Hijing_NEWFIX_v2.root";
  
  make_comp_plots_photontree(save,
  			     doPilot,
  			     doMC,
  			     doData,
			     pilotForest,
  			     dataForest,
			     mcForest);
  
  make_comp_plots_rechittree(save,
  			     doPilot,
  			     doMC,
  			     doData,
  			     pilotForest,
  			     dataForest,
  			     mcForest,
  			     "eb");

  make_comp_plots_rechittree(save,
  			     doPilot,
  			     doMC,
  			     doData,
  			     pilotForest,
  			     dataForest,
  			     mcForest,
  			     "ee");
  make_etaphi_photontree(save,
  			 doPilot,
  			 doMC,
  			 doData,
  			 pilotForest,
  			 dataForest,
  			 mcForest);

  make_sigmaetaeta(save,
  		   doPilot,
  		   doMC,
  		   doData,
  		   pilotForest,
  		   dataForest,
  		   mcForest);
}
