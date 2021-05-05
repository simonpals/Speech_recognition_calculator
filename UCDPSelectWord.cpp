//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>

#include "UCDPSelectWord.h"

#pragma hdrstop
//---------------------------------------------------------------------------
#pragma package(smart_init)

static bool prev_above_max = false;

int CDPSelectWord(vector<Srec_buf> &v)
{
  int max_vec_size = cdpparam.max_vec_size;
  int min_energy = cdpparam.min_energy;
  int segm_length = cdpparam.segm_length;

  bool curr_above_max = false;

  int i = 0, j;  
  double en = 0.0;

  if(v.size() < 3) return 0;

  vector<Srec_buf>::iterator it = v.begin();

  i = v.size() - 1;

   while(i >= 0 && ((v.size() - 4) < i) || i >= 0 && v.size() == 3)
   {
      en = 0.0;

      for(j=0; j<cdpparam.fftN; j++)
      {
         en += abs(v[i].arr[j]);
         if(en > min_energy)
         {
            curr_above_max = true;
            break;
         }
         if(j%segm_length == 0 && j != 0)
             en = 0.0;
      }

      if(curr_above_max) break;
      i--;
   }

   if(v.size() == 3)
   {
      prev_above_max = curr_above_max;
      return 0;
   }

   if(v.size() == 4)
   {
      if(!prev_above_max && !curr_above_max)
         //v.clear();
         v.erase((it+v.size()-4), (it+v.size()-1));
      if(!prev_above_max && curr_above_max)
         v.erase((it+v.size()-4), (it+v.size()-2));
      if(prev_above_max && !curr_above_max)
         //v.clear();
         v.erase((it+v.size()-4), (it+v.size()-1));
      prev_above_max = curr_above_max;
      return 0;
   }

   if(v.size() == 5)
   {
      if(prev_above_max && !curr_above_max)
         //v.clear();
         v.erase((it+v.size()-5), (it+v.size()-1));
      prev_above_max = curr_above_max;
      return 0;
   }
   
   if(v.size() == 6)
   {
      if(prev_above_max && !curr_above_max)
         //v.clear();
         v.erase((it+v.size()-6), (it+v.size()-1));
      prev_above_max = curr_above_max;
      return 0;
   }
   
   /*
   if(v.size() == 7)
   {
      if(prev_above_max && !curr_above_max)
         v.erase((it+v.size()-7), (it+v.size()-1));
      prev_above_max = curr_above_max;
      return 0;
   }
   */
   if(prev_above_max && !curr_above_max)
   {
      v.erase((it+v.size()-2), (it+v.size()));
    /*
      //for(i=0; i<2; i++)
      //{
        en = 0.0;
        for(j=0; j<cdpparam.fftN; j++)
        {
           en += abs(v[0].arr[j]);
           if(en > min_energy)
              break;
           if(j%segm_length == 0 && j != 0)
             en = 0.0;
        }
        if(j==cdpparam.fftN)
           v.erase(it, (it+1));
       // else if(i==0) break;
      //}

      if(v.size() >= 3)
    */  
         return 1;
   }

   prev_above_max = curr_above_max;
   if(v.size() > max_vec_size)
   {
      //v.clear();
      return -1;
   }

   return 0;
}

//--------------------------------------------------------
typedef double NUMERIC;

void Low50Hz(double *sig, int N, int Fs)
{
  int i, j;

  if( N < 10 ) return;
  if((Fs % 11025) != 0) return;

  int F = Fs / 11025;
  int row = (F == 1) ? 0 : F;

  NUMERIC *y = new NUMERIC[N];
  ZeroMemory(y,sizeof(NUMERIC)*N);

  y[0] = Butter3Low50[row+1][0] * sig[0];
  y[1] = Butter3Low50[row+1][0] * sig[1] + Butter3Low50[row+1][1] * sig[0] - Butter3Low50[row][1] * y[0];
  y[2] = Butter3Low50[row+1][0] * sig[2] + Butter3Low50[row+1][1] * sig[1] + Butter3Low50[row+1][2] * sig[0] - Butter3Low50[row][1] * y[1] - Butter3Low50[row][2] * y[0];

  for(i=3; i<N; i++)
     y[i] = Butter3Low50[row+1][0]*sig[i] + Butter3Low50[row+1][1]*sig[i-1] + Butter3Low50[row+1][2]*sig[i-2] + Butter3Low50[row+1][3]*sig[i-3]
      - Butter3Low50[row][1]*y[i-1] - Butter3Low50[row][2]*y[i-2] - Butter3Low50[row][3]*y[i-3];

  for(i=0; i<N; i++)
     sig[i] = y[i];

  delete [] y;
}

//--------------------------------------------------------
int Decimator(double *sig,int N, int Fs, int dt_dec)
{
  int i, j;
  //int dt_dec = 200;

  if( N < 10 ) return -1;
  if((Fs % 11025) != 0) return -1;

  int F = Fs / 11025;  F *= dt_dec;
  int dn = ceil(N/F) + 1;

  NUMERIC *y = new NUMERIC[dn+1];
  ZeroMemory(y,sizeof(NUMERIC)*(dn+1));

  j = 0;
  for(i=0; i<N; i+=F)
  {
     y[j] = sig[i];
     j++;
  }

  for(i=0; i<N; i++)
  {
    if(i < j)
     sig[i] = y[i];
    else  sig[i] = 0;
  }

  delete [] y;
  return j;
}
//--------------------------------------------------------

void Sabs(double *sig,int N)
{
  int i;

  if( N < 10 ) return;

  for(i=0; i<N; i++)
     sig[i] = fabs(sig[i]);
}
//--------------------------------------------------------

void High300Hz(double *sig, int N, int Fs)
{
  int i, j;

  if( N < 10 ) return;
  if((Fs % 11025) != 0) return;

  int F = Fs / 11025;
  int row = (F == 1) ? 0 : F;

  NUMERIC *ym = new NUMERIC[N];
  ZeroMemory(ym,sizeof(NUMERIC)*N);

  ym[0] =  Butter1High300[row+1][0] * sig[0];

  for(i=1; i<N; i++)
     ym[i] = Butter1High300[row+1][0] * sig[i] + Butter1High300[row+1][1] * sig[i-1] - Butter1High300[row][1] * ym[i-1];

  for(i=0; i<N; i++)
     sig[i] = ym[i];

  delete [] ym;
}
//--------------------------------------------------------

void Low300Hz(double *sig, int N, int Fs)
{
  int i, j;

  if( N < 10 ) return;
  if((Fs % 11025) != 0) return;

  int F = Fs / 11025;
  int row = (F == 1) ? 0 : F;

  NUMERIC *ym = new NUMERIC[N];
  ZeroMemory(ym,sizeof(NUMERIC)*N);

  ym[0] =  Butter1Low300[row+1][0] * sig[0];

  for(i=1; i<N; i++)
     ym[i] = Butter1Low300[row+1][0] * sig[i] + Butter1Low300[row+1][1] * sig[i-1] - Butter1Low300[row][1] * ym[i-1];

  for(i=0; i<N; i++)
     sig[i] = ym[i];

  delete [] ym;
}
//--------------------------------------------------------

 int CountToken(short *sig, int N)
 {
     int MAX_AMPL = 32765;  // maksumalna amplituda
     double lim = MAX_AMPL * 0.008; // min dopystumuj riven` sugnaly
     int min = MAX_AMPL * 0.001; // granucya pochatkovogo vidsijyvannya
     int count = 1; // kilkist skladiv
     int i=0, j=N-1, k, c;

    if(N < 5)  return -1;
// pochatkove vidsijyvannya
    for(k=0; k<N; k++)
       if(abs(sig[k]) < min) sig[k] = 0;
// poshyk pochatky i kincya sugnaly
    while(sig[i] == 0 && i < N)
       i++;

    if(i == N) return 0;

    while(sig[j] == 0 && j > 0)
       j--;

    for(k=i; k<=j; k++)
       if(sig[k] == 0) sig[k] = 1;
// poshyk znachen` sugnaly jaki peretunajyt` nyl`
    for(k=i+1; k<=j; k++)
       if( sig[k-1] > 0 &&  sig[k] < 0  ||  sig[k-1] < 0 &&  sig[k] > 0 )
          count++;

    int *zerom = new int[count+1];
    ZeroMemory(zerom,sizeof(int)*(count+1));
// poshyk jih indeksiv
    zerom[0] = i; c=1;
    for(k=i+1; k<=j; k++)
    {
      if( sig[k-1] > 0 &&  sig[k] < 0 ) {  zerom[c] = k-1; c++;  }
      if( sig[k-1] < 0 &&  sig[k] > 0 ) {  zerom[c] = k-1; c++;  }
    }
// filtr za rivnem: lim
    for(i=1; i<c; i+=2)
    {
       for(j=zerom[i-1]; j<=zerom[i]; j++)
       {
          if(sig[j]>lim) break;
       }
       if(j>zerom[i]) { zerom[i-1] = -1; zerom[i] = -1; }
    }
//v inshomy poryadky
    for(i=2; i<c; i+=2)
    {
      if( zerom[i-1] != -1  &&  zerom[i] != -1 )
      {
        for(j=zerom[i-1]; j<=zerom[i]; j++)
        {
          if(abs(sig[j])>lim) break;
        }
        if(j>zerom[i]) { zerom[i-1] = -1; zerom[i] = -1; }
      } 
    }

    count =  c == 1 ? count : 0;
    for(i=0; i<c; i++)
       if(zerom[i] >= 0)
          count++;
// 1 sklad mistut` dva peretunu nulya
   count = ceil((double) count / 2);        

   delete [] zerom;
   return count;
 }

//--------------------------------------------------------

int TokenCountFunc(double *keyword, int vecsize)
{
   int tokm = 0;
   double *kwcopy = NULL;   
   short *tokbuf = NULL;

         kwcopy = new double [vecsize*cdpparam.fftN];
         memset(kwcopy, 0, sizeof(double) * vecsize * cdpparam.fftN);
         copy(keyword, keyword+vecsize*cdpparam.fftN, kwcopy);

         for(int j=0; j<cdpparam.fftN * vecsize; j++)
            kwcopy[j] = kwcopy[j] /(double) cdpparam.MAX_AMPL;
         //Vurivnjyjem i otrumyjem ogunajychy
         Sabs(kwcopy, cdpparam.fftN * vecsize);
         Low50Hz(kwcopy, cdpparam.fftN * vecsize, cdpparam.MSampleRate);
         //Piddajemo masuv decumaciji
        int lengthd = Decimator(kwcopy, cdpparam.fftN * vecsize, cdpparam.MSampleRate, 160);

        Low300Hz(kwcopy,lengthd,cdpparam.MSampleRate);
        High300Hz(kwcopy,lengthd,cdpparam.MSampleRate);

        tokbuf = new short [lengthd];
        memset(tokbuf, 0, sizeof(short) * lengthd);
        for(int j=0; j<lengthd; j++)
           tokbuf[j] = kwcopy[j] * cdpparam.MAX_AMPL;
        //Znahodgennya kilkosti skladiv po kognomy slovy
        tokm = CountToken(tokbuf, lengthd);

   delete [] tokbuf; tokbuf = NULL;
   delete [] kwcopy; kwcopy = NULL;

   return tokm;
}

//--------------------------------------------------------

/*
void High50Hz(double *sig, int N, int Fs);

void High50Hz(double *sig, int N, int Fs)
{
  int i, j;

  int F = Fs / 11025;
  int row = 0;//(F == 1) ? 0 : F;

  double *ym = new double[N];
  ZeroMemory(ym,sizeof(double)*N);

double Butter2High50[2][3];

Butter2High50[0][0] = 1.000;
Butter2High50[0][1] = -1.95970337973902;
Butter2High50[0][2] = 0.960499408325787;
Butter2High50[1][0] = 0.980050697016202;
Butter2High50[1][1] = -1.9601013940324;
Butter2High50[1][2] = 0.980050697016202;

try
{
  ym[0] = Butter2High50[row+1][0] * sig[0];
  ym[1] = Butter2High50[row+1][0] * sig[1] + Butter2High50[row+1][1] * sig[0] - Butter2High50[row][1]* sig[0];

  for(i=2; i<N; i++)
     ym[i] = Butter2High50[row+1][0] * sig[i] + Butter2High50[row+1][1] * sig[i-1] + Butter2High50[row+1][2] * sig[i-2] - Butter2High50[row][1] * ym[i-1] - Butter2High50[row][2] * ym[i-2];

  for(i=0; i<N; i++)
     sig[i] = ym[i];

  delete [] ym;
}
catch(...)
{
}
}
*/
/*
      FILE* stream = fopen("vuv.txt","a+b");
      for(int i=0; i<vsig.size(); i++)
      fwrite(vsig[i].arr,sizeof(short),cdpparam.fftN,stream);
      fclose(stream);

   double *ym;
   ym = new double[cdpparam.fftN];
   ZeroMemory(ym,sizeof(double)*cdpparam.fftN);

   for(int i=0; i<cdpparam.fftN; i++)
   {
      ym[i] = buf[i] / (double) cdpparam.MAX_AMPL;
   }

 //  High50Hz(ym, cdpparam.fftN, cdpparam.MSampleRate);

   for(int i=0; i<cdpparam.fftN; i++)
      buf[i] = ym[i] * (double)cdpparam.MAX_AMPL;
*/
/*
      ofstream stream("1.txt");
      for(int i=0; i<cntm; i++)
         stream<<mas[i]<<',';
      //fwrite(mas,sizeof(double),cntm,stream);
      stream.close();
*/
