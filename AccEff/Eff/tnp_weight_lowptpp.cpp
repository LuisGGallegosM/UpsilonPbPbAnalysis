
#include"tnp_weight_lowptpp.h"

std::tuple<double,double,double,double> tnp_weight_GlobalMuon_TightAcceptance_pp(double pt, double eta){
  if( (fabs(eta) > 0) && (fabs(eta) < 1.2) ){
    if (pt<3.7) return std::make_tuple(1.06147, 0.00823842, 0.00208669, 0.00849858);
    else if (pt<3.9) return std::make_tuple(1.01011, 0.00737501, 0.00252889, 0.00779654);
    else if (pt<4.2) return std::make_tuple(1.00592, 0.00570771, 0.00221069, 0.00612088);
    else if (pt<4.6) return std::make_tuple(0.991387, 0.00475945, 0.00223252, 0.00525705);
    else if (pt<5.2) return std::make_tuple(0.990916, 0.00399021, 0.0102105, 0.0109624);
    else if (pt<7) return std::make_tuple(0.993442, 0.0027033, 0.000973951, 0.0028734);
    else if (pt<10.5) return std::make_tuple(0.990363, 0.00299319, 0.00196834, 0.00358239);
    else if (pt<30) return std::make_tuple(0.985178, 0.00459295, 0.000794411, 0.00466114);

  }

  if( (fabs(eta) > 1.2) && (fabs(eta) < 1.8) ){
    if (pt<2.4) return std::make_tuple(1.01382, 0.0325946, 0.00962322, 0.0339855);
    else if (pt<2.8) return std::make_tuple(1.00242, 0.0139186, 0.00365599, 0.0143908);
    else if (pt<3.2) return std::make_tuple(0.98561, 0.00937482, 0.00377605, 0.0101067);
    else if (pt<3.9) return std::make_tuple(0.993246, 0.00628579, 0.00237889, 0.00672088);
    else if (pt<5) return std::make_tuple(0.992181, 0.00532263, 0.00180271, 0.00561962);
    else if (pt<8) return std::make_tuple(0.992916, 0.00450105, 0.0034851, 0.00569258);
    else if (pt<30) return std::make_tuple(0.979602, 0.00641576, 0.00331339, 0.00722084);

  }

  if( (fabs(eta) > 1.8) && (fabs(eta) < 2.4) ){
    if (pt<1.8) return std::make_tuple(0.962602, 0.0253389, 0.00739227, 0.0263952);
    else if (pt<2.3) return std::make_tuple(0.967941, 0.0133937, 0.00661216, 0.0149369);
    else if (pt<2.9) return std::make_tuple(0.972082, 0.01028, 0.00375234, 0.0109434);
    else if (pt<3.7) return std::make_tuple(0.982591, 0.00868358, 0.00138498, 0.00879334);
    else if (pt<6) return std::make_tuple(0.984427, 0.00594631, 0.000904974, 0.00601478);
    else if (pt<30) return std::make_tuple(0.971683, 0.00672377, 0.00560575, 0.00875406);

  }

  if( (fabs(eta) > 1.8) && (fabs(eta) < 2.4) ){
    if (pt<1.8) return std::make_tuple(0.962602, 0.0253389, 0.00739227, 0.0263952);
    else if (pt<2.3) return std::make_tuple(0.967941, 0.0133937, 0.00661216, 0.0149369);
    else if (pt<2.9) return std::make_tuple(0.972082, 0.01028, 0.00375234, 0.0109434);
    else if (pt<3.7) return std::make_tuple(0.982591, 0.00868358, 0.00138498, 0.00879334);
    else if (pt<6) return std::make_tuple(0.984427, 0.00594631, 0.000904974, 0.00601478);
    else if (pt<30) return std::make_tuple(0.971683, 0.00672377, 0.00560575, 0.00875406);

  }
  return std::make_tuple(1.0, 0.0, 0.0, 0.0);
}

//To be used for HybridSoftID efficiency, with loose muon kinematic acceptance cuts
//Central scale-factor value, statistical error, systematic error, total error

std::tuple<double,double,double,double> tnp_weight_HybridSoftID_LooseAcceptance_pp(double pt, double eta){

  if( (fabs(eta) > 0) && (fabs(eta) < 1.2) ){
    if (pt<3.7) return std::make_tuple(0.991509, 0.00132469, 0.000169579, 0.0013355);
    else if (pt<4) return std::make_tuple(0.994812, 0.00098049, 9.3781e-05, 0.000984965);
    else if (pt<4.5) return std::make_tuple(0.996624, 0.000652936, 5.94331e-05, 0.000655635);
    else if (pt<5) return std::make_tuple(0.997641, 0.000608668, 0.000281891, 0.000670775);
    else if (pt<5.5) return std::make_tuple(0.998693, 0.000581153, 4.7311e-05, 0.000583075);
    else if (pt<6) return std::make_tuple(0.997692, 0.000592181, 4.28475e-05, 0.000593729);
    else if (pt<7) return std::make_tuple(0.99823, 0.000488995, 0.000133913, 0.000507);
    else if (pt<8) return std::make_tuple(0.998932, 0.000570471, 6.84542e-05, 0.000574564);
    else if (pt<10.5) return std::make_tuple(0.999942, 0.000461622, 0.000246741, 0.000523427);
    else if (pt<14) return std::make_tuple(1.00025, 0.000588468, 0.000412485, 0.000718636);
    else if (pt<18) return std::make_tuple(0.998795, 0.00103439, 0.000343453, 0.00108992);
    else if (pt<30) return std::make_tuple(1.00013, 0.00119241, 0.000258227, 0.00122005);
  }

  if( (fabs(eta) > 1.2) && (fabs(eta) < 1.8) ){
    if (pt<2.5) return std::make_tuple(0.999008, 0.00101804, 0.00022889, 0.00104346);
    else if (pt<3) return std::make_tuple(0.997385, 0.000670367, 0.000166048, 0.000690626);
    else if (pt<3.5) return std::make_tuple(0.99795, 0.000566551, 7.41528e-05, 0.000571383);
    else if (pt<4) return std::make_tuple(0.997328, 0.000565301, 0.000106841, 0.000575308);
    else if (pt<4.5) return std::make_tuple(0.998049, 0.000586026, 0.000166683, 0.00060927);
    else if (pt<5) return std::make_tuple(0.99893, 0.000624649, 0.000139086, 0.000639946);
    else if (pt<6) return std::make_tuple(0.99775, 0.000524755, 0.000129245, 0.000540437);
    else if (pt<7) return std::make_tuple(0.997207, 0.000631782, 0.000158721, 0.000651415);
    else if (pt<9) return std::make_tuple(0.997041, 0.000592354, 0.000262767, 0.00064802);
    else if (pt<14) return std::make_tuple(0.997027, 0.000643335, 0.000101755, 0.000651333);
    else if (pt<18) return std::make_tuple(0.998987, 0.00145434, 0.000105944, 0.00145819);
    else if (pt<30) return std::make_tuple(0.998832, 0.00176589, 0.000185321, 0.00177559);
  }

  if( (fabs(eta) > 1.8) && (fabs(eta) < 2.1) ){
    if (pt<1.75) return std::make_tuple(0.99501, 0.00190285, 0.000614269, 0.00199954);
    else if (pt<2.1) return std::make_tuple(0.995534, 0.000958298, 0.000938682, 0.00134144);
    else if (pt<2.5) return std::make_tuple(0.998697, 0.000606511, 0.000452902, 0.000756951);
    else if (pt<3) return std::make_tuple(0.998797, 0.000516932, 0.000241382, 0.000570512);
    else if (pt<3.5) return std::make_tuple(0.999229, 0.000521865, 1.82422e-05, 0.000522184);
    else if (pt<4) return std::make_tuple(0.998185, 0.000596435, 0.000199466, 0.000628905);
    else if (pt<4.5) return std::make_tuple(0.997687, 0.000654825, 0.000242263, 0.000698203);
    else if (pt<5.25) return std::make_tuple(0.998599, 0.000576324, 0.000257014, 0.000631036);
    else if (pt<6.5) return std::make_tuple(0.999244, 0.000485306, 0.000321732, 0.000582266);
    else if (pt<9) return std::make_tuple(0.998085, 0.000535479, 0.000314486, 0.000620998);
    else if (pt<13) return std::make_tuple(0.998346, 0.000716329, 0.000608368, 0.000939807);
    else if (pt<30) return std::make_tuple(0.998398, 0.00112506, 0.000231803, 0.00114869);
  }

  if( (fabs(eta) > 2.1) && (fabs(eta) < 2.4) ){
    if (pt<1.9) return std::make_tuple(0.99387, 0.00114168, 0.000306098, 0.001182);
    else if (pt<2.5) return std::make_tuple(0.996848, 0.00781191, 0.000367591, 0.00782055);
    else if (pt<3) return std::make_tuple(0.998103, 0.000651977, 0.000306688, 0.000720508);
    else if (pt<3.5) return std::make_tuple(1.00042, 0.000618082, 0.000608206, 0.000867145);
    else if (pt<4) return std::make_tuple(0.998227, 0.000779589, 0.000550001, 0.000954076);
    else if (pt<4.75) return std::make_tuple(0.999744, 0.000565002, 0.000220046, 0.000606339);
    else if (pt<5.5) return std::make_tuple(0.998818, 0.000747367, 0.000685803, 0.00101434);
    else if (pt<8) return std::make_tuple(0.997726, 0.000576727, 0.000259674, 0.000632491);
    else if (pt<12) return std::make_tuple(0.999967, 0.000812264, 0.000550096, 0.000981008);
    else if (pt<30) return std::make_tuple(1.00071, 1.49564e-12, 0.000545007, 0.000545007);
  }
  return std::make_tuple(1.0, 0.0, 0.0, 0.0);
}


//To be used for HybridSoftID+(J/psi trigger) efficiency, with tight muon kinematic acceptance cuts (the loose cuts cannot be used with trigger efficiency)
//Central scale-factor value, statistical error, systematic error, total error

std::tuple<double,double,double,double> tnp_weight_HybridSoftIDTrigger_TightAcceptance_pp(double pt, double eta){

  if( (fabs(eta) > 0) && (fabs(eta) < 1.2) ){
    if (pt<3.75) return std::make_tuple(1.04685, 0.0030737, 0.000900168, 0.0032028);
    else if (pt<4) return std::make_tuple(1.03785, 0.00231229, 0.000865001, 0.00246879);
    else if (pt<4.25) return std::make_tuple(1.02122, 0.00196689, 0.000129247, 0.00197113);
    else if (pt<4.5) return std::make_tuple(1.01242, 0.00182759, 0.000452042, 0.00188266);
    else if (pt<4.75) return std::make_tuple(1.01333, 0.00177537, 0.000501881, 0.00184495);
    else if (pt<5) return std::make_tuple(1.01311, 0.00179201, 0.00038986, 0.00183393);
    else if (pt<5.25) return std::make_tuple(1.01071, 0.00177936, 0.000558043, 0.00186481);
    else if (pt<5.5) return std::make_tuple(1.01492, 0.00177077, 0.000621342, 0.00187662);
    else if (pt<5.75) return std::make_tuple(1.01162, 0.00189087, 0.000708795, 0.00201935);
    else if (pt<6) return std::make_tuple(1.00824, 0.00199524, 0.00144451, 0.00246325);
    else if (pt<6.5) return std::make_tuple(0.99932, 0.00149522, 0.000387211, 0.00154454);
    else if (pt<7) return std::make_tuple(0.990015, 0.00167288, 0.000334128, 0.00170592);
    else if (pt<7.5) return std::make_tuple(0.989662, 0.00183027, 0.000377601, 0.00186881);
    else if (pt<8) return std::make_tuple(0.991271, 0.00197681, 0.000397838, 0.00201644);
    else if (pt<9.25) return std::make_tuple(0.993863, 0.00144008, 0.000219424, 0.0014567);
    else if (pt<10.5) return std::make_tuple(0.98847, 0.00179481, 0.00046839, 0.00185492);
    else if (pt<12.25) return std::make_tuple(0.988261, 0.00198848, 0.000342797, 0.00201781);
    else if (pt<14) return std::make_tuple(0.986502, 0.00266287, 0.000312191, 0.00268111);
    else if (pt<16) return std::make_tuple(0.967524, 0.00355877, 0.000816031, 0.00365113);
    else if (pt<18) return std::make_tuple(0.974107, 0.00471465, 0.00173171, 0.00502262);
    else if (pt<24) return std::make_tuple(0.971851, 0.0042844, 0.000750224, 0.00434959);
    else if (pt<30) return std::make_tuple(0.95726, 0.0076637, 0.00260996, 0.00809594);
  }

  if( (fabs(eta) > 1.2) && (fabs(eta) < 1.8) ){
    if (pt<2.5) return std::make_tuple(1.05392, 0.00740653, 0.0086659, 0.0113998);
    else if (pt<2.75) return std::make_tuple(1.05418, 0.00509847, 0.000493877, 0.00512233);
    else if (pt<3) return std::make_tuple(1.03165, 0.00361682, 0.00051552, 0.00365338);
    else if (pt<3.25) return std::make_tuple(1.00137, 0.00323329, 0.000650532, 0.00329808);
    else if (pt<3.5) return std::make_tuple(0.985531, 0.00324821, 0.00101988, 0.00340456);
    else if (pt<3.75) return std::make_tuple(0.979026, 0.00332663, 0.00080565, 0.00342279);
    else if (pt<4) return std::make_tuple(0.978533, 0.00348141, 0.00128796, 0.00371201);
    else if (pt<4.5) return std::make_tuple(0.981693, 0.00261395, 0.00138638, 0.00295885);
    else if (pt<5) return std::make_tuple(0.988393, 0.00281823, 0.000574534, 0.0028762);
    else if (pt<5.5) return std::make_tuple(0.988535, 0.00295307, 0.0015193, 0.00332098);
    else if (pt<6) return std::make_tuple(0.995795, 0.0030352, 0.000849741, 0.0031519);
    else if (pt<7) return std::make_tuple(0.986439, 0.00238387, 0.00132843, 0.00272902);
    else if (pt<8.5) return std::make_tuple(0.991989, 0.00234303, 0.00141202, 0.00273562);
    else if (pt<10) return std::make_tuple(0.993719, 0.0029372, 0.00125018, 0.00319219);
    else if (pt<12) return std::make_tuple(0.990475, 0.00312347, 0.00082619, 0.00323089);
    else if (pt<14.5) return std::make_tuple(0.99984, 0.00384704, 0.00226697, 0.0044653);
    else if (pt<19) return std::make_tuple(0.999167, 0.0673357, 0.00146068, 0.0673515);
    else if (pt<30) return std::make_tuple(0.998484, 0.00703573, 0.000958602, 0.00710074);
  }

  if( (fabs(eta) > 1.8) && (fabs(eta) < 2.1) ){
    if (pt<1.9) return std::make_tuple(1.16258, 0.00781262, 0.00518218, 0.00937507);
    else if (pt<2.2) return std::make_tuple(1.08194, 0.00433687, 0.00165702, 0.00464265);
    else if (pt<2.5) return std::make_tuple(1.04721, 0.00304661, 0.00198365, 0.00363548);
    else if (pt<2.8) return std::make_tuple(1.0117, 0.00248692, 0.00103727, 0.00269457);
    else if (pt<3.1) return std::make_tuple(1.0045, 0.00239878, 0.000301947, 0.0024177);
    else if (pt<3.4) return std::make_tuple(0.998153, 0.00253776, 0.00107157, 0.00275472);
    else if (pt<4) return std::make_tuple(0.991499, 0.00205249, 0.000162552, 0.00205891);
    else if (pt<5) return std::make_tuple(0.982154, 0.00201585, 0.00109106, 0.00229217);
    else if (pt<6) return std::make_tuple(0.979281, 0.0026728, 0.00139839, 0.00301651);
    else if (pt<8) return std::make_tuple(0.978722, 0.0028223, 0.00105435, 0.00301282);
    else if (pt<11) return std::make_tuple(0.982305, 0.00411488, 0.000812637, 0.00419435);
    else if (pt<15) return std::make_tuple(0.98089, 0.0072865, 0.000877749, 0.00733918);
    else if (pt<30) return std::make_tuple(0.985428, 0.0130435, 0.012101, 0.0177923);
  }

  if( (fabs(eta) > 2.1) && (fabs(eta) < 2.4) ){
    if (pt<1.9) return std::make_tuple(1.10953, 0.00325122, 0.000888127, 0.00337034);
    else if (pt<2.3) return std::make_tuple(1.04759, 0.00205232, 0.00049553, 0.00211129);
    else if (pt<2.7) return std::make_tuple(1.02649, 0.00197078, 0.00156344, 0.00251562);
    else if (pt<3) return std::make_tuple(1.01133, 0.00246973, 0.000958998, 0.00264939);
    else if (pt<3.5) return std::make_tuple(1.01379, 0.00206648, 0.00104966, 0.00231778);
    else if (pt<4.25) return std::make_tuple(1.00341, 0.00200472, 0.000761361, 0.00214443);
    else if (pt<5) return std::make_tuple(0.997733, 0.00251532, 0.00299566, 0.00391162);
    else if (pt<6) return std::make_tuple(0.991922, 0.00271899, 0.000262191, 0.0027316);
    else if (pt<7) return std::make_tuple(0.9968, 0.00362768, 0.00239638, 0.00434772);
    else if (pt<8.5) return std::make_tuple(1.00011, 0.0035347, 0.00168078, 0.00391397);
    else if (pt<13) return std::make_tuple(1.00663, 0.00403812, 0.00220707, 0.00460191);
    else if (pt<30) return std::make_tuple(0.992803, 0.0110441, 0.0109342, 0.0155412);
  }
  return std::make_tuple(1.0, 0.0, 0.0, 0.0);
}
