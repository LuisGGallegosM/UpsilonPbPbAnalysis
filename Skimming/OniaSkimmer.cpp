

#include "OniaSkimmer.h"
#include "TLorentzVector.h"

OniaSkimmer::OniaSkimmer(OniaReader* reader , OniaCutter* cutter, OniaWriter* writer)
: oniaWriter(writer), oniaCutter(cutter) ,oniaReader(reader)
{
}

void OniaSkimmer::ProcessEvent(Long64_t entry)
{
    if (oniaCutter->prescale(entry)) return;

    Long64_t size=oniaReader->recoQQ.size;
    if(oniaWriter->getType()==QQtype::Gen)
        size=oniaReader->genQQ.size;
    
    for(Long64_t i=0;i<size;++i)
    {
        if (oniaCutter->cut(oniaReader.get(),i,entry))
        {
            oniaWriter->writeEntries(oniaReader.get(),i,entry);
        }
    }
}