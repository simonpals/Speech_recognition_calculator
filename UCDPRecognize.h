#ifndef UCDPRecognizeH
#define UCDPRecognizeH

#include <stdio.h>
#include <SysUtils.hpp>
#include <vcl.h>
#include <windows.h>
#include <fstream.h>
#include <iostream.h>
#include <mem.h>
#include <math.h>
#include <Math.hpp>
#include <vector>
#include <numeric>
#include <algorithm.h>

#include "UCDPEducation.h"

#include "fft.h"
#include "UCDPParam.h"

//typ porivnyjvanyh elementiv
#define types double

//dlya decryptoriv. Rivnuy max qk
#define max_qk_ 15

//typ miry podibnosti g(x,e)
typedef double retval;

String ExtendToFile = "_sawe";

//max znachennya za modulem
template<class T> T abmax(T a, T b) {

        return  (fabs(a) > fabs(b)) ? a : b;
};

class Criterion;
class CDPRecognize
{
public:

int counter; // lichyl`nyk zapysanyh failiv
//rozshyrennya faylu nalashtuvan` dlya dyktora
char sysext [10];
//chu znachenna < 1 ?
bool less1_;
//max znachennya dyskrety
int MAX_;
//maksymal`na kilkist` iteraciy
int MAX_ITER_;
//chastota dyskretyzaciyi
int Fs_;
//zvaguval`na funkciya
bool win_;
int channels_;
//kilkist kanaliv sygnalu
int bits_;
//rozryadnist`
double dT_;
//dT - interval vybirky
double Tmin_;
//Tmin- min znachennya T osnovnogo tonu
double dt_;
// dt - vidstan` mig dyskretamy
int vgr_;
// vgr - granychna chastota = 1/(2*dt)
//bool isParalel_ = true;

   __fastcall CDPRecognize();

   //predstavlyaye dani u sproshchenomu vyglyadi, komponuye
   //vsi realizaciyi slova i podaye na analiz
   String EducationSignal(types const *in, long long int N, short U, int u, String name, String slovo, int qk);
   //in - povnyu movnyu sygnal
   //N - dovgyna in
   //kilkist realizaciy slova
   //u - realizaciya slova
   //name - nazva korystuvacha
   //slovo - nazva slova rozpiznavannya

   //predstavlyaye dani u sproshchenomu vyglyadi, porivnyuye z etalonamy
   //znahodyt` vidpovidnyu
   String SelectionSignal(types const *in, long long int N, String pth_word, int tokm);
   //in - povnyu movnyu sygnal
   //N - dovgyna in
   //en_word - faily z etalonnymy danymy
   //n - kilkist` etaloniv v en_word

   //formuye etalon i temporal`nu
   //transkrypciyu z realizacij odnogo slova
   String HeadCycle(String path, String name, String slovo, int qk);
   //path - byfer z vidibranym vhidnym masyvom
   //name - nazva korystuvacha
   //slovo - nazva slova rozpiznavannya

   //porivnyuye masyvy
   bool equalE(types const *E, types const *E_pre, int qk);
   //prysvoyuye masyvy
   void assign(types *E, types *E_pre, int qk);
   //poshuk odnakovyh sliv 
   int CheckSameWord(String pathw, String slovo);

   //segmentaciya vectora
   Criterion Segmentat(const types *x, const types *ek, int xn, int qk);
   //x - vhidnyu vector
   //ek - etalon
   //xn - rozmir x

   //samosegmentaciya vectora
   Criterion SelfSegmentat(const types *x, int xn, int qk);
   //x - vhidnyu vector
   //xn - rozmir x

   //segmentaciya vectora z urahuvannyam temporal`noyi transkrypciyi
   retval TempSegmentat(const types *x, const types *ek, const int *tau, int xn, int qk);
   //x - vhidnyu vector
   //ek - etalon
   //xn - rozmir x
   //tau - temporal`na transkrypciya

   //porivnyannya etalona z samym soboyu
   //dya vyznachennya max ocinky shozhosti
   types SelfCompareEtalon(types const *E, int qk, Criterion C);
   //E - etalon
   //qk - elementy etalonu
   //Vybirka chyslovyh danyh za pevnym algorytmom
   types *Palgorithm(types const *inm);
   //inm - segment movnogo sygnalu

   //Vybir etalonu Ekr po odniy realizaciyi
   void Pargmax(types const *x, Criterion C, types *ek, int qk);
   //x - vubirka po oniy realizaciyi
   //C - klas iz granycyamy segmentiv: x
   //ek - rezyltujuchyj etalon


   //Vuznachennya podibnosti vectoriv
   types g(const types *x, const types *e, int v, int s);
   //x - vybirka r-ji realizaciyi
   //e - etalon
   //v - element x
   //s - element e

   //Avtokorelyaciynyj analiz
   types *PAutoCorr(types const *inm);
   //win = 1 - nakladayetsya wikno Hamminga
   //win = 0 - wikno vidsutnye
   // inm - masyv movnogo sygnalu
   // m - Tmin/(2*Fs)
   // M - Kilkist` vidlikiv u vybirci: dT/Fs
   // Tmin - min znachennya T osnovnogo tonu, f.e. 2 ms
   // dT - interval vybirky
   // dt - vidstan` mig dyskretamy

   //Algorytm Durbina
   double *PDurbin(types const *B);
   // m - Tmin/(2*Fs)
   // M - Kilkist` vidlikiv u vybirci: dT/Fs
   // Tmin - min znachennya T osnovnogo tonu, f.e. 2 ms
   // dT - interval vybirky
   // dt - vidstan` mig dyskretamy
   // B(s) - masyv-element avtokorelyaciyi
   // G(p) - autoregresyvnyj spektr
   // vgr - granychna chastota = 1/(2*dt)
   // k - masyv koeficientiv vidobrazhennya
   // Q - dovzhyna masyvu G(p)
   // sigma - ocinka dyspersiyi shumu
   // E - masyv pohybok prognozuvannya
   // b - avtokorelyaciya impulsnogo vidguku filtru z harakterystykoyu A
   // xiu - masyv dviykovyh oznak
   // teta - porogove znachennya na G(p), dlya vrahuvannya pobutovogo shumu

   //Spectral`nyj analiz (iz userednennyam)
   types *Pfftm(types const *inm);
   //inm - masyv movnogo sygnalu


};

extern CDPRecognize cdprecognize;


//klas dlya elementiv rozpiznavannya
class Criterion
{
public:
  int in, qk;
  //in - kilkist` elemntiv movnogo sygnalu
  //qk - kilkist` elemntiv etalonu
  int *v, *w;
  //v - kilkist` elementiv segmentu: s
  //w - masyv granyc` segmentu: s, r-yi realizaciyi

  int i;
  retval **F;

  //konstuktor
  Criterion();
  //konstuktor dlya zapovnennya
  Criterion(int _in, int _qk);
  //vyklykaye konstuktor dlya zapovnennya
  void Creation(int _in, int _qk);
  //destryktor
  ~Criterion();
  //konstuktor kopiyi
  Criterion(const Criterion &op2);
  //pereventagenyj operator prysvoyennya
  Criterion operator=(const Criterion &op2);
};


#endif
