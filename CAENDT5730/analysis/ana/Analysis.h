//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Apr 30 19:54:18 2016 by ROOT version 6.06/02
// from TChain t/
//////////////////////////////////////////////////////////

#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "TGraph.h"
#include "TCanvas.h"


// Header file for the classes stored in the TTree if any.

class Analysis : public TSelector {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           b;
   Int_t           ch;
   Int_t           size;
   UShort_t        e;
   ULong64_t       t;
   Short_t         wave[1500];   //[size]

   // List of branches
   TBranch        *b_b;   //!
   TBranch        *b_ch;   //!
   TBranch        *b_size;   //!
   TBranch        *b_e;   //!
   TBranch        *b_t;   //!
   TBranch        *b_wave;   //!

   Analysis(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~Analysis() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   // wuhongyi
   int count;
   TGraph *g;
   TCanvas *c1;
   
   ClassDef(Analysis,0);
};

#endif

#ifdef Analysis_cxx
void Analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("b", &b, &b_b);
   fChain->SetBranchAddress("ch", &ch, &b_ch);
   fChain->SetBranchAddress("size", &size, &b_size);
   fChain->SetBranchAddress("e", &e, &b_e);
   fChain->SetBranchAddress("t", &t, &b_t);
   fChain->SetBranchAddress("wave", wave, &b_wave);
}

Bool_t Analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef Analysis_cxx
