#include "JetSkimmer.h"

JetSkimmer::JetSkimmer(TTree* treeIn,const char* treeOutName,Onia_Aux* oniaAuxData)
: Skimmer(treeIn,treeOutName) , oniaData(oniaAuxData)
{
    TBranch* branch;

    //input branches
    addInput("evt",&dataIn.evt);
    addInput("nref",&dataIn.nRef);
    addInput("jtpt",dataIn.jtPt);
    addInput("jteta",dataIn.jtEta);
    addInput("jty",dataIn.jtY);
    addInput("jtphi",dataIn.jtPhi);
    addInput("jtpu",dataIn.jtPu);
    addInput("jtm",dataIn.jtM);
    addInput("jtarea",dataIn.jtArea);

    //output branches
    addOutput("jtM",&dataOut.jtM);
    addOutput("jtEta",&dataOut.jtEta);
    addOutput("jtPhi",&dataOut.jtPhi);
    addOutput("jtPT",&dataOut.jtPt);
    addOutput("jtY",&dataOut.jtY);
    addOutput("jtArea",&dataOut.jtArea);
    addOutput("jtPu",&dataOut.jtPu);
    addOutput("jtEvt",&dataOut.evt);
    addOutput("z", &dataOut.z);

    return;
}

void JetSkimmer::WriteData(Int_t index, Long64_t entry)
{
    dataOut.evt= dataIn.evt;
    dataOut.jtM= dataIn.jtM[index];
    dataOut.jtEta = dataIn.jtEta[index];
    dataOut.jtPhi = dataIn.jtPhi[index];
    dataOut.jtPt = dataIn.jtPt[index];
    dataOut.jtY = dataIn.jtY[index];
    dataOut.jtPu = dataIn.jtPu[index];
    dataOut.jtArea = dataIn.jtArea[index];

    Onia_Output oniaOut = oniaData->events[dataIn.evt];   
    dataOut.z = oniaOut.pT / dataIn.jtPt[index];

}

//***********************************************
//Onia structs

Int_t Jet_Input::getSize()
{
    return nRef;
}

Jet_Input::Jet_Input()
{
    jtM = new Float_t[maxBranchSize];
    jtEta = new Float_t[maxBranchSize];
    jtPhi = new Float_t[maxBranchSize];
    jtPt = new Float_t[maxBranchSize];
    jtY = new Float_t[maxBranchSize];
    rawPt = new Float_t[maxBranchSize];
    jtPu = new Float_t[maxBranchSize];
    jtArea =new Float_t[maxBranchSize];
}

Jet_Input::~Jet_Input()
{
    delete[] jtM;
    delete[] jtEta;
    delete[] jtPhi;
    delete[] jtPt;
    delete[] jtY;
    delete[] rawPt;
    delete[] jtPu;
    delete[] jtArea;
}