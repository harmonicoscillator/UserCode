#ifndef alexUtils_h
#define alexUtils_h

#include "TString.h"
#include "TCanvas.h"

void saveCanvas(TCanvas *c, TString name){
  TString date = "_201302013_";
  TString run = "_211792_";
  
  c->SaveAs(name+date+run+".eps");
  // c->SaveAs(name+date+run+".pdf");
  c->SaveAs(name+date+run+ ".C");
}

#endif
