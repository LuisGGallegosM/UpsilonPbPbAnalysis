#include "JetSkimmer.h"

JetSkimmer::JetSkimmer(TTree* myTree) : Skimmer(myTree) 
{
    TBranch* branch;

    //input branches
    branch = tree->GetBranch("evt");
    branch->SetAddress(&dataIn.evt);
    input_branches.push_back(branch);

    branch = tree->GetBranch("nref");
    branch->SetAddress(&dataIn.nRef);
    input_branches.push_back(branch);

    branch = tree->GetBranch("jtpt");
    branch->SetAddress(dataIn.jtPt);
    input_branches.push_back(branch);

    branch = tree->GetBranch("jteta");
    branch->SetAddress(dataIn.jtEta);
    input_branches.push_back(branch);

    branch = tree->GetBranch("jty");
    branch->SetAddress(dataIn.jtY);
    input_branches.push_back(branch);

    branch = tree->GetBranch("jtphi");
    branch->SetAddress(dataIn.jtPhi);
    input_branches.push_back(branch);

    branch = tree->GetBranch("jtpu");
    branch->SetAddress(dataIn.jtPu);
    input_branches.push_back(branch);

    branch = tree->GetBranch("jtm");
    branch->SetAddress(dataIn.jtM);
    input_branches.push_back(branch);

    branch = tree->GetBranch("jtarea");
    branch->SetAddress(dataIn.jtArea);
    input_branches.push_back(branch);

    branch = tree->GetBranch("rawpt");
    branch->SetAddress(dataIn.rawPt);
    input_branches.push_back(branch);

    //output branches
    tree_output->Branch("mass", &dataOut.jtM);
    tree_output->Branch("eta", &dataOut.jtEta);
    tree_output->Branch("phi", &dataOut.jtPhi);
    tree_output->Branch("pT", &dataOut.jtPt);
    tree_output->Branch("y", &dataOut.jtY);
    tree_output->Branch("area",&dataOut.jtArea);
    tree_output->Branch("raw pT",&dataOut.rawPt);
    tree_output->Branch("Pu",&dataOut.jtPu);
    tree_output->Branch("evt",&dataOut.evt);

    return;
}

void JetSkimmer::WriteData(Int_t index)
{
    dataOut.evt= dataIn.evt;
    dataOut.jtM= dataIn.jtM[index];
    dataOut.jtEta = dataIn.jtEta[index];
    dataOut.jtPhi = dataIn.jtPhi[index];
    dataOut.jtPt = dataIn.jtPt[index];
    dataOut.jtY = dataIn.jtY[index];
    dataOut.rawPt = dataIn.rawPt[index];
    dataOut.jtPu = dataIn.jtPu[index];
    dataOut.jtArea = dataIn.jtArea[index];
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