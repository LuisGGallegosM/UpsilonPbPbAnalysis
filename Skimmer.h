

class Skimmer
{
    private:
    TString toKeep;
    
    public:
    Skimmer();
    TTree* Skim(TTree* tree);
};

