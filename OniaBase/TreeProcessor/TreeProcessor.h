#ifndef TREEPROCESSOR
#define TREEPROCESSOR

#include "TTree.h"
#include "TreeReader.h"
#include "TreeWriter.h"

class DataReader
{
    public:
    virtual void registerReader(TreeReader* reader) = 0;
    virtual ~DataReader() = default;
};

class DataWriter
{
    public:
    virtual void registerWriter(TreeWriter* writer) = 0;
    virtual ~DataWriter() = default;
};

class TreeProcessor
{
    TreeReader reader;
    TreeWriter writer;

    protected:
    virtual void ProcessEvent(Long64_t entry) = 0;

    void Process();

    public:
    TreeProcessor(TTree* inputTree, const char* outTreeName) : 
        reader(inputTree), writer(outTreeName)
    {
    }

    void registerInputs( DataReader* in)
    {
        in->registerReader(&reader);
    }

    void registerOutputs( DataWriter* out)
    {
        out->registerWriter(&writer);
    }

    void FillEntries() { writer.FillEntries();}

    void write() { writer.Write();}
    
    virtual ~TreeProcessor() =default;
};

#endif