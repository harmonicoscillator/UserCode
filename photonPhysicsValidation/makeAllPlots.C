#include "make_comp_plots_photontree.C"
#include "make_comp_plots_rechittree.C"
#include "make_etaphi_photontree.C"
#include "make_sigmaetaeta.C"
#include "TString.h"

void makeAllPlots()
{
  const bool save = false;
  const bool doPilot = true;
  const bool doMC = false;
  const bool doData = true;
  const TString dataForest = "/mnt/hadoop/cms/store/user/luck/pA2013_forests/pPb_hiForest2_r210354_notracker_v1.root";
  
  make_comp_plots_photontree(save,
  			     doPilot,
  			     doMC,
  			     doData,
  			     dataForest);
  
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
  make_etaphi_photontree(save,
  			 doPilot,
  			 doMC,
  			 doData,
  			 dataForest);

  make_sigmaetaeta(save,
  		   doPilot,
  		   doMC,
  		   doData,
  		   dataForest);
}
