#include "make_comp_plots_photontree.C"
#include "make_comp_plots_rechittree.C"
#include "make_etaphi_photontree.C"
#include "make_sigmaetaeta.C"


void makeAllPlots()
{
  const bool save = true;
  
  const bool doPilot = false;
  const bool doMC = true;
  const bool doData = true;
  const TString dataForest = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/PA2013_HiForest_Express_r210534_stablebeams_72bunch.root";
  
  // make_comp_plots_photontree(save,
  // 			     doPilot,
  // 			     doMC,
  // 			     doData,
  // 			     dataForest);
  
  make_comp_plots_rechittree(save,
  			     doPilot,
  			     doMC,
  			     doData,
  			     dataForest,
  			     "eb");

  make_comp_plots_rechittree(save,
  			     doPilot,
  			     doMC,
  			     doData,
  			     dataForest,
  			     "ee");
  // make_etaphi_photontree(save,
  // 			 doPilot,
  // 			 doMC,
  // 			 doData,
  // 			 dataForest);

  // make_sigmaetaeta(save,
  // 		   doPilot,
  // 		   doMC,
  // 		   doData,
  // 		   dataForest);
}
