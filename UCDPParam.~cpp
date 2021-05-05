//---------------------------------------------------------------------------
#pragma hdrstop
#include "UCDPParam.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

 TCDPParam cdpparam;

 TCDPParam::TCDPParam()
 {
    TokenSens = true;
    CDPUserPath = "";
    dbCDPext = ".cdp";
    RegisterPath = "\\Software\\CDPWorkDir";
    //parametry poshuku slova
    MAX_AMPL = 32767;
    max_vec_size = 43;
    mnognyk = 6;
    min_energy = MAX_AMPL * mnognyk;
    segm_length = 64;

    //vizualizaciya ta fft
    MAX_VECTOR_SIZE = 50;
    minfreq = 1;
    maxfreq = 2048;
    fftN = 1024;
    MSampleRate = 11025;
    LogFrameSize = 10;

    //parametry CDP
    max_realization_U = 9;
    curr_realization_U = 5;
    min_realization_U = 3;
    wait_for_threads = 20000;   
    m_ = 20;                     // ?
    M_ = 256;                   // ?
    err_ = 26;
    min_err_ = 10;
    max_err_ = 40;

    less1_ = false;
    MAX_ITER_ = 30;

    //algotytmy CDP
    alg_ = DURBOOL; //  TFFTM;  DURCOEF;
    kind_ = DIMHEMIN;
    if (alg_ != DURBOOL)
       kind_ = DIMEUCLID;

    //parametry modulya navchannya   
    isFirstTime = true;
    msTimeToEndRec = 15000;
 }
