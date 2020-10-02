
#define maxBranchSize (1000)

struct Muon_Input
{
    TClonesArray* mom4;
    Int_t size;
    Int_t* Reco_QQ_mupl_idx;
    Int_t* Reco_QQ_mumi_idx;

    Muon_Input();
    ~Muon_Input();
};

struct Muon_Output
{
    Float_t mass;
    Float_t pT;
    Float_t y;
    Float_t phi;
    Float_t eta;
};

class Skimmer
{
    private:
    std::vector<TBranch*> input_branches;
    Muon_Input diMuonData;
    Muon_Output diMuonDataOut;
    TTree* tree;
    TTree* tree_output;

    void InitBranches();
    void GetEntries(int index);
    
    public:
    Skimmer(TTree* myTree);
    void Skim();
    TTree* GetTree();
};

