#ifndef alexUtils_h
#define alexUtils_h

#include "TString.h"
#include "TCanvas.h"

void saveCanvas(TCanvas *c, TString name){
  TString date = "_20130124_";
  TString run = "PromptRecoFirstPr";
  
  c->SaveAs(name+date+run+".eps");
  // c->SaveAs(name+date+run+".pdf");
  c->SaveAs(name+date+run+ ".C");
}

#endif
