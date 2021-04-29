#ifndef tnp_weight_h
#define tnp_weight_h

#include "TMath.h"
#include <tuple>

// IN THIS FILE YOU WILL FIND:
// ++++++++++++++

//BINNED SCALE FACTORS for pp 2017

//To extract the scale factor for a muon of PT=pt and ETA=eta, do:

//#include "tnp_weight.h"
//void testtnpweight(){
// auto SFwithError = tnp_weight_theEfficiencyYouWant_pp( pt, eta );
// double central_value = std::get<0>(SFwithError);
// double stat_error = std::get<1>(SFwithError);
// double syst_error = std::get<2>(SFwithError);
// double tot_error = std::get<3>(SFwithError);
//}

// ++++++++++++++

///////////////////////////////////////////////////
//                 G l o b a l    p p               //
///////////////////////////////////////////////////

//To be used for global muon efficiency, with tight muon kinematic acceptance cuts
//Central scale-factor value, statistical error, systematic error, total error

std::tuple<double,double,double,double> tnp_weight_GlobalMuon_TightAcceptance_pp(double pt, double eta);

//To be used for HybridSoftID efficiency, with loose muon kinematic acceptance cuts
//Central scale-factor value, statistical error, systematic error, total error

std::tuple<double,double,double,double> tnp_weight_HybridSoftID_LooseAcceptance_pp(double pt, double eta);


//To be used for HybridSoftID+(J/psi trigger) efficiency, with tight muon kinematic acceptance cuts (the loose cuts cannot be used with trigger efficiency)
//Central scale-factor value, statistical error, systematic error, total error

std::tuple<double,double,double,double> tnp_weight_HybridSoftIDTrigger_TightAcceptance_pp(double pt, double eta);

#endif
