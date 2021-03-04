

#include "OniaSkimmer.h"
#include "TLorentzVector.h"

OniaSkimmer::OniaSkimmer(OniaReader* reader , OniaCutter* cutter, OniaWriterFull* writer)
: oniaWriter(writer), oniaCutter(cutter) ,oniaReader(reader)
{
}

void OniaSkimmer::ProcessEvent(Long64_t entry)
{
    if (oniaCutter->prescale(entry)) return;

    Long64_t size=oniaReader->recoQQ_size;
    
    for(Long64_t i=0;i<size;++i)
    {
        if (oniaCutter->cut(oniaReader.get(),i,entry))
        {
            oniaWriter->writeRecoQQ(oniaReader.get(),i,entry);
        }
    }
}