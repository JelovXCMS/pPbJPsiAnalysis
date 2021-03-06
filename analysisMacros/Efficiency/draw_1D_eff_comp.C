#include "../SONGKYO.h"

// no ordering in rap (just y_lab)
void draw_1D_eff_comp(TString szDir = "dir_eff_comp", bool isNoErr=false)
{
  gROOT->Macro("../Style.C");

  //// read-in file (ratio would be f01/f02 )
  /*
  int isPA =0; //0:pp, 1:Pbp, 2:pPb for drawing dashed line at low pT limit
  TFile* f01 = new TFile("./dir_eff/8rap9pt_pp_PR_newcut_Zvtx1_SF0_EffPt.root");
  TFile* f02 = new TFile("./dir_eff/8rap9pt_pp_PR_newcut_Zvtx0_SF0_EffPt.root");
  TString szF01="ppPRZvtx1";
  TString szF02="ppPRZvtx0";
  */
  /* 
  int isPA =1; //0:pp, 1:Pbp, 2:pPb for drawing dashed line at low pT limit
  TFile* f01 = new TFile("./dir_eff/8rap9pt_Pbp_PR_newcut_Zvtx1_SF0_EffPt.root");
  TFile* f02 = new TFile("./dir_eff/8rap9pt_Pbp_PR_newcut_Zvtx0_SF0_EffPt.root");
  TString szF01="PbpPRZvtx1";
  TString szF02="PbpPRZvtx0";
  */
  int isPA =1; //0:pp, 1:Pbp, 2:pPb for drawing dashed line at low pT limit
  TFile* f01 = new TFile("./dir_eff/8rap9pt_Pbp_PR_newcut_Zvtx0_SF0_EffPt.root");
  TFile* f02 = new TFile("./dir_eff/8rap9pt_Pbp_NP_newcut_Zvtx0_SF0_EffPt.root");
  TString szF01="PbpPR";
  TString szF02="PbpNP";
  
  //double ratiomin=0.98; 
  //double ratiomax=1.02; 
  double ratiomin=0.9; 
  double ratiomax=1.1; 
  //double ratiomin=0.; 
  //double ratiomax=1.2; 
  //// end of "to-be-modified" region

  const TString szFinal = Form("%s_%s",szF01.Data(),szF02.Data());
  cout << "szFinal = " << szFinal << endl;

  //// read-in tree
  TTree *tr = (TTree*)f01->Get("tRap");
  int nEdge;
  double rapEdge[531];
  TBranch *b_nEdge;
  TBranch *b_rapEdge;
  tr->SetBranchAddress("nEdge",&nEdge,&b_nEdge);
  tr->SetBranchAddress("rapEdge",&rapEdge,&b_rapEdge);
  cout << "tr entries = " << tr->GetEntries() << endl;
  for (int ev=0; ev <tr->GetEntries(); ev++){
    tr->GetEntry(ev);
  }
  cout << "nEdge = " << nEdge << endl;
  for (Int_t i=0; i< nEdge; i++) {
    cout << "rapEdge["<<i<<"] = " << rapEdge[i] << endl;
  }
  //// read-in histos
  const int nRap = nEdge -1 ; 
  cout << "nRap = " << nRap << endl;
  TH1D* h1D_01[nRap]; 
  TH1D* h1D_02[nRap]; 
  TH1D *hRatio[nRap]; //f01/f02
  for (int iy=0; iy < nRap; iy ++){
    h1D_01[iy] = (TH1D*)f01->Get(Form("h1D_EffPt_%d",iy));
    h1D_02[iy] = (TH1D*)f02->Get(Form("h1D_EffPt_%d",iy));
  cout << "h1D_01["<<iy<<"] = " << h1D_01[iy] << endl;
  cout << "h1D_02["<<iy<<"] = " << h1D_02[iy] << endl;
    hRatio[iy]=(TH1D*)h1D_01[iy]->Clone(Form("hRatio_%d",iy));
    hRatio[iy]->Divide(h1D_02[iy]);
  }
  
  //////////////////////////////////////////////////////////////////
  //// Draw histograms
  TLatex* latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextSize(0.04);

  TCanvas* c1 = new TCanvas("c1","c1",1600,800);
  c1->Divide(4,2);
  TCanvas* c2 = new TCanvas("c2","c2",1600,800);
  c2->Divide(4,2);
  TLegend *legBR = new TLegend(0.50,0.30,0.80,0.41,NULL,"brNDC");
  SetLegendStyle(legBR);

  for (Int_t iy = 0; iy < nRap; iy++) {
    //// 1)  distributions
    c1->cd(iy+1);
    SetHistStyle(h1D_01[iy],3,0);
    SetHistStyle(h1D_02[iy],4,10);
    h1D_01[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    h1D_01[iy]->GetYaxis()->SetTitle("Efficiency");
    h1D_01[iy]->SetMinimum(0.);
    h1D_01[iy]->SetMaximum(1.);
    h1D_01[iy]->GetXaxis()->SetRangeUser(0., 30.);
    h1D_01[iy]->Draw("pe");
    h1D_02[iy]->Draw("pe same");
    //// draw line at low pT limit
    if (isPA==0) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy ==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy ==2 || iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    else if (isPA==1) {
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,0.,6.5,1.,2,1);
      else if (iy==5)  dashedLine (5.,0.,5.,1.,2,1);
    }
    else if (isPA==2){
      if (iy==0 || iy==7) dashedLine (2.,0.,2.,1.,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,0.,4.,1.,2,1);
      else if (iy==2)  dashedLine (5.,0.,5.,1.,2,1);
      else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,0.,6.5,1.,2,1);
    }
    //latex->DrawLatex(0.53,0.25,szFinal.Data());
    latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));
    if (iy==0) {
      legBR->AddEntry(h1D_01[iy],szF01.Data(),"lp");
      legBR->AddEntry(h1D_02[iy],szF02.Data(),"lp");
      legBR->Draw();
    }
    
    //// 2) ratio f01/f02
    c2->cd(iy+1);
    SetHistStyle(hRatio[iy],5,0);
    hRatio[iy]->GetXaxis()->SetTitle("p_{T} (GeV)");
    hRatio[iy]->GetYaxis()->SetTitle(Form("[ %s ]/[ %s ]",szF01.Data(),szF02.Data()));
    hRatio[iy]->SetMinimum(ratiomin);
    hRatio[iy]->SetMaximum(ratiomax);
    hRatio[iy]->GetXaxis()->SetRangeUser(0.,30.);
    if (isNoErr) {
      for (int ipt=0; ipt<9; ipt++) {
        hRatio[iy]->SetBinError(ipt+1,0.);
      }
    }
    hRatio[iy]->Draw("pe");
    dashedLine(0.,1.,30.,1.,1,1);
    
    //// draw line at lot pT limit
    if (isPA==0) {
      if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,1);
      else if (iy ==1 || iy==6) dashedLine (4.,ratiomin,4.,ratiomax,2,1);
      else if (iy ==2 || iy==3 || iy==4 || iy==5)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,1);
    }
    else if (isPA==1) {
      if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,ratiomin,4.,ratiomax,2,1);
      else if (iy==2 || iy==3 || iy==4)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,1);
      else if (iy==5)  dashedLine (5.,ratiomin,5.,ratiomax,2,1);
    }
    else if (isPA==2){
      if (iy==0 || iy==7) dashedLine (2.,ratiomin,2.,ratiomax,2,1);
      else if (iy==1 || iy==6) dashedLine (4.,ratiomin,4.,ratiomax,2,1);
      else if (iy==2)  dashedLine (5.,ratiomin,5.,ratiomax,2,1);
      else if (iy==3 || iy==4 || iy==5)  dashedLine (6.5,ratiomin,6.5,ratiomax,2,1);
    }
    latex->DrawLatex(0.53,0.25,szFinal.Data());
    latex->DrawLatex(0.53,0.19,Form("%.2f < y_{lab} < %.2f",rapEdge[iy],rapEdge[iy+1]));

  }

  c1->SaveAs(Form("%s/comp_eff_%s.pdf",szDir.Data(),szFinal.Data()));
  c2->SaveAs(Form("%s/ratio_eff_%s.pdf",szDir.Data(),szFinal.Data()));

  return;

}

