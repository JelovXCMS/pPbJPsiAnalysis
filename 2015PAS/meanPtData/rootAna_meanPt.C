#include <../SONGKYO.h>

void formRapArr(Double_t binmin, Double_t binmax, TString arr);
void formAbsRapArr(Double_t binmin, Double_t binmax, TString arr);
void formPtArr(Double_t binmin, Double_t binmax, TString arr);
void formCentArr(Double_t binmin, Double_t binmax, TString arr);
void formEtArr(Double_t binmin, Double_t binmax, TString arr);

int rootAna_meanPt(TString szPA = "pa", TString szBinning = "8rap9pt", bool isSmall =true){

	const int nbin = 100; //arbitrary binning for each pT hist.
	const int nEntry = 10000;
		
	gROOT->Macro("../Style.C");

  //// read-in root file
  TFile *fData;
  TFile *fPRMC;
  TTree *treeData;
  TTree *treePRMC;
  if (strcmp(szPA,"pp")==0) {
    fData = new TFile("/storage/OniaTree/Onia5TeV/ppData/OniaTree_DoubleMu_Run2015E-PromptReco-v1_Run_262157_262328_noCUT_TRKMU.root");
    fPRMC = new TFile("/storage/OniaTree/Onia5TeV/ppOfficialMC/OniaTree_JpsiMM_5p02TeV_TuneCUETP8M1_Trk_HINppWinter16DR-75X_mcRun2_asymptotic_ppAt5TeV_v3-v1.root");
    treeData = (TTree*)fData->Get("hionia/myTree");
    treePRMC = (TTree*)fPRMC->Get("hionia/myTree");
  } else if (strcmp(szPA,"pa")==0) {
    fData = new TFile("/home/songkyo/kyo/pPbDataSample/Data/RD2013_pa_1st_run_210676-211256_GR_P_V43D_nocut.root");
    fPRMC = new TFile("/home/songkyo/kyo/pPbDataSample/EfficiencySample/MCJPsiWithFSR_pa_1st_run_STARTHI53_V27_ext1_nocut.root");
    treeData = (TTree*)fData->Get("myTree");
    treePRMC = (TTree*)fPRMC->Get("myTree");
  }
  else { cout << "WARNING :: Select among szPA = \"pp\" or \"pa\" "; return 0; }

	/////////////////////////////////////////////////////////////	
	////////////// binning
	Double_t ptArr[] = {2.0, 3.0, 4.0, 5.0, 6.5, 7.5, 8.5, 10.0, 14.0, 30.0}; // 8rap9pt	
	const Int_t nPt = sizeof(ptArr)/sizeof(double)-1;
	cout << "nPt=" << nPt << endl;

	// in Ycm (will be change to 1st lab and 2nd lab later)
	Double_t yArr[] = {1.93, 1.5, 0.9, 0., -0.9, -1.5, -1.93, -2.4, -2.87}; // 8rap9pt
	const Int_t nRap = sizeof(yArr)/sizeof(double)-1;
	cout << "nRap=" << nRap << endl;

	TString rapstrArr[nRap];
	for (int ir=0; ir<nRap; ir++){
		formRapArr(yArr[ir+1], yArr[ir], &rapstrArr[ir]);
		cout << "rap string arr = " << rapstrArr[ir].c_str() << endl;
	}

	Double_t etArr[] = {0.0, 120.0}; // 8rap9pt
	//Double_t etArr[] = {0.0, 20., 30., 120.0}; // ethf
	const Int_t nEt = sizeof(etArr)/sizeof(double)-1;
	cout << "nEt=" << nEt << endl;

	TString etstrArr[nEt];
	for (int in=0; in<nEt; in++){
		 formEtArr(etArr[in], etArr[in+1], &etstrArr[in]);
		 cout << "et string arr = " << etstrArr[in].c_str() << endl;
	}

	/////////////////////////////////////////////////////////////	
	////////////// cut definitions
	TCut trigCut = "( (Reco_QQ_trig&1)==1 && (HLTriggers&1)==1 )"; 
	TCut recoCut = "Reco_QQ_sign==0 && Reco_QQ_VtxProb > 0.01";	
//	TCut massRange = "Reco_QQ_4mom.M() >2.6 && Reco_QQ_4mom.M() < 3.5";
	TCut massRange = "Reco_QQ_4mom.M() >2.9 && Reco_QQ_4mom.M() < 3.3";
	TCut ctauRange = "Reco_QQ_ctau > -3.0 && Reco_QQ_ctau < 5.0 && Reco_QQ_ctauErr > 0.0 && Reco_QQ_ctauErr < 1.0"; 
//	TCut rapRange = "Reco_QQ_4mom.Rapidity() > -2.4 && Reco_QQ_4mom.Rapidity() < 2.4";
//	TCut ptRange = "Reco_QQ_4mom.Pt() > 0.0 && Reco_QQ_4mom.Pt() < 30.0";
	TCut accRecoPlus = "(TMath::Abs(Reco_QQ_mupl_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mupl_4mom.Eta())<1.3 && Reco_QQ_mupl_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && TMath::Abs(Reco_QQ_mupl_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mupl_4mom.Px()*Reco_QQ_mupl_4mom.Px()+ Reco_QQ_mupl_4mom.Py()*Reco_QQ_mupl_4mom.Py() + Reco_QQ_mupl_4mom.Pz()*Reco_QQ_mupl_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mupl_4mom.Eta()) && Reco_QQ_mupl_4mom.Pt()>=0.8) )";
	TCut accRecoMinus = "(TMath::Abs(Reco_QQ_mumi_4mom.Eta())<2.4) && ( (TMath::Abs(Reco_QQ_mumi_4mom.Eta())<1.3 && Reco_QQ_mumi_4mom.Pt()>=3.3) || ( 1.3<= TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && TMath::Abs(Reco_QQ_mumi_4mom.Eta()) < 2.2 && sqrt(Reco_QQ_mumi_4mom.Px()*Reco_QQ_mumi_4mom.Px()+ Reco_QQ_mumi_4mom.Py()*Reco_QQ_mumi_4mom.Py() + Reco_QQ_mumi_4mom.Pz()*Reco_QQ_mumi_4mom.Pz())>=2.9 ) || (2.2<=TMath::Abs(Reco_QQ_mumi_4mom.Eta()) && Reco_QQ_mumi_4mom.Pt()>=0.8) )";
	TCut ptRange[nEt][nRap][nPt];
	TCut rapRangePbp[nEt][nRap][nPt];
	TCut rapRangepPb[nEt][nRap][nPt];
	TCut totalCutPbp[nEt][nRap][nPt];
	TCut totalCutpPb[nEt][nRap][nPt];
	TCut runCut = "runNb >=210498 && runNb <= 210658"; //1st 7 run

	// forming TCut for diffential binning	
	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
			for (int ipt=0; ipt<nPt; ipt++) {
				ptRange[in][ir][ipt] = Form("Reco_QQ_4mom.Pt()>%.1f && Reco_QQ_4mom.Pt()<%.1f",ptArr[ipt] , ptArr[ipt+1]);
				//change y_CM to  y_lab
				rapRangePbp[in][ir][ipt] = Form("-0.47-1.*Reco_QQ_4mom.Rapidity()>%.2f && -0.47-1.*Reco_QQ_4mom.Rapidity()<%.2f",yArr[ir+1] , yArr[ir]);
				rapRangepPb[in][ir][ipt] = Form("-0.47+Reco_QQ_4mom.Rapidity()>%.2f && -0.47+Reco_QQ_4mom.Rapidity()<%.2f",yArr[ir+1] , yArr[ir]);
				cout << "ptRange = " <<ptRange[in][ir][ipt] << endl;
				cout << "rapRangePbp = " <<rapRangePbp[in][ir][ipt] << endl;
				cout << "rapRangepPb = " <<rapRangepPb[in][ir][ipt] << endl;
				totalCutPbp[in][ir][ipt] = trigCut && recoCut && massRange && ctauRange && accRecoPlus && accRecoMinus && rapRangePbp[in][ir][ipt] && ptRange[in][ir][ipt];
				totalCutpPb[in][ir][ipt] = trigCut && recoCut && massRange && ctauRange && accRecoPlus && accRecoMinus && rapRangepPb[in][ir][ipt] && ptRange[in][ir][ipt];
			}
		}
	}


	/////////////////////////////////////////////////////////////	
	/////////////////////////////////////////////////////////////	
	// define 1D hist
	TH1D *hMeanPt[nEt][nRap][nPt];
	double meanVal[nEt][nRap][nPt];
	double maxVal[nEt][nRap][nPt];

	//TCanvas* c1 = new TCanvas("c1","c1",600,600);
	TCanvas* cmulti[nEt][nRap];
	TLegend *legUR = new TLegend(0.58,0.68,0.90,0.90,NULL,"brNDC");
	TLegend *legUM = new TLegend(0.30,0.68,0.65,0.90,NULL,"brNDC");
	TLegend *legUL = new TLegend(0.17,0.68,0.51,0.90,NULL,"brNDC");
	TLegend *legBM = new TLegend(0.30,0.20,0.65,0.42,NULL,"brNDC");
	SetLegendStyle(legUR);
	SetLegendStyle(legUM);
	SetLegendStyle(legUL);
	SetLegendStyle(legBM);
	TLatex* latex = new TLatex();
	latex->SetNDC();
	latex->SetTextAlign(12);
	latex->SetTextSize(0.04);

	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
	 		cmulti[in][ir]= new TCanvas(Form("cmulti_%d_%d",in,ir),Form("%s",rapstrArr[ir].c_str()),1500,600);
			cmulti[in][ir]->Divide(5,2);
			for (int ipt=0; ipt<nPt; ipt++) {
				hMeanPt[in][ir][ipt] = new TH1D(Form("hMeanPt_%d_%d_%d",in, ir, ipt),";p_{T} (GeV/c);events", nbin, ptArr[ipt], ptArr[ipt+1]);
				hMeanPt[in][ir][ipt] ->Sumw2();
				SetHistStyle(hMeanPt[in][ir][ipt],4,0);
				//cout << "pT from " << ptArr[ipt] <<" , to " << ptArr[ipt+1] << endl;
				//cout << "hMeanPt["<<in<<"]["<<ir<<"]["<<ipt<<"] = " << hMeanPt[in][ir][ipt] << endl;
				//cout << "hist name = " << hMeanPt[in][ir][ipt]->GetName() << endl;
				//c1->cd();
				cmulti[in][ir]->cd(ipt+1);
				gPad->SetLogy(1);
//				treeDataPbp1->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&(!runCut)),"",nEntry);
//				treeDataPbp2->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&runCut),"",nEntry);
//				treeDatapPb->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),totalCutpPb[in][ir][ipt],"",nEntry);
				treeDataPbp1->Draw(Form("Reco_QQ_4mom.Pt()>>%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&(!runCut)),"");
				treeDataPbp2->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),(totalCutPbp[in][ir][ipt]&&runCut),"");
				treeDatapPb->Draw(Form("Reco_QQ_4mom.Pt()>>+%s",hMeanPt[in][ir][ipt]->GetName()),totalCutpPb[in][ir][ipt],"");
				meanVal[in][ir][ipt] =  hMeanPt[in][ir][ipt]->GetMean(1);
				maxVal[in][ir][ipt] = 50*hMeanPt[in][ir][ipt]->GetMaximum();
				hMeanPt[in][ir][ipt]->GetYaxis()->SetRangeUser(0.1,maxVal[in][ir][ipt]);
				std::cout << hMeanPt[in][ir][ipt]->GetName() <<" : entries = " << hMeanPt[in][ir][ipt]->GetEntries() << std::endl;
				cout << " <p_{T}> = " << meanVal[in][ir][ipt] << endl;			
				latex->DrawLatex(0.54, 0.87, rapstrArr[ir].c_str());
				latex->DrawLatex(0.54, 0.80, Form("<p_{T}> = %.2f (GeV/c)",meanVal[in][ir][ipt]));
				//c1->Update();
				//c1->SaveAs(Form("%s.png",hMeanPt[in][ir][ipt]->GetName()));
				//c1->Clear();
			}
			cmulti[in][ir]->Update();
			cmulti[in][ir]->SaveAs(Form("meanPt_%s_%d_%d.png",szBinning,in,ir));
			//cmulti[in][ir]->Clear();
		}
	}

	/// Save as a root file
	TFile *outFile = new TFile(Form("meanPt_%s.root",szBinning),"RECREATE");
	std::cout << "szBinning : " << szBinning << std::endl;
	outFile->cd();
	for (int in=0; in<nEt; in++){
		for (int ir=0; ir<nRap; ir++ ) {
			for (int ipt=0; ipt<nPt; ipt++) {
				hMeanPt[in][ir][ipt]->Write();
			}
		}
	}
	outFile->Close();

	return 0;	

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// sub-routines function 

void formRapArr(Double_t binmin, Double_t binmax, TString arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < y_{CM} < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < y_{CM} < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < y_{CM} < %.2f", binmin, binmax);
	}
}

void formAbsRapArr(Double_t binmin, Double_t binmax, TString arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.2f < |y_{CM}| < %.0f", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < |y_{CM}| < %.2f", binmin, binmax);
	} else {
		*arr = Form("%.2f < |y_{CM}| < %.2f", binmin, binmax);
	}
}

void formPtArr(Double_t binmin, Double_t binmax, TString arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < p_{T} < %.0f GeV/c", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < p_{T} < %.1f GeV/c", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}

void formCentArr(Double_t binmin, Double_t binmax, TString arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("Cent. %.0f-%.0f %%", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("Cent. %.1f-%.0f %%", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("Cent. %.0f-%.1f %%", binmin, binmax);
	} else {
		*arr = Form("Cent. %.1f-%.1f %%", binmin, binmax);
	}
}

void formEtArr(Double_t binmin, Double_t binmax, TString arr) {
	Double_t intMin, intMax; 
	Double_t fracMin = modf(binmin, &intMin);
	Double_t fracMax = modf(binmax, &intMax);
	if ( fracMin == 0 && fracMax == 0 ) {
		*arr = Form("%.0f < E_{T}^{HF} < %.0f GeV", binmin, binmax);
	} else if ( fracMin != 0 && fracMax == 0 ) {
		*arr = Form("%.1f < E_{T}^{HF} < %.0f GeV", binmin, binmax);
	} else if ( fracMin == 0 && fracMax != 0 ) {
		*arr = Form("%.0f < E_{T}^{HF} < %.1f GeV", binmin, binmax);
	} else {
		*arr = Form("%.1f < p_{T} < %.1f GeV/c", binmin, binmax);
	}
}

