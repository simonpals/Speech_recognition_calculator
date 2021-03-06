//--------------------------------------------------------------------------
#pragma hdrstop
#include "UCDPRecognize.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

CDPRecognize cdprecognize;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

__fastcall CDPRecognize::CDPRecognize()
{
   sprintf(sysext, "%s", cdpparam.dbCDPext.c_str());
   less1_ = cdpparam.less1_;
   MAX_ = cdpparam.MAX_AMPL;
   MAX_ITER_ = cdpparam.MAX_ITER_;
   Fs_ = cdpparam.MSampleRate;
   win_ = 1;
   channels_ = 1;
   bits_ = 16;
   dT_ = 0.04;
   Tmin_ = 0.002;
   dt_ = 1. / Fs_;
   vgr_ = 1/(2*dt_);
   counter = 0;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String CDPRecognize::EducationSignal(types const *in, long long int N, short U, int u, String name, String slovo, int qk)
{
try
{
   int i, end=0;
   String path = "", etalon = "";
   long double nrm = 0;

   int m_ = cdpparam.m_;
   int M_ = cdpparam.M_;
   int dim = ((int)N/M_)*m_+1;
   types *sig = NULL;
   types *buf = NULL;
   sig = new types[dim];
   memset(sig,0,sizeof(types)*(dim));

//---------------------------------------------------------------------------
   //vydilennya movnuh oznak
   for(i=0; i<N-M_+1; i++)
   {
      if(i%M_==0)
      {
         buf = Palgorithm(in+i);
         copy(buf,buf+m_,sig+end*m_);
         nrm = abmax( nrm, Norm(buf,m_-1) );
         end++;
         delete [] buf; buf = NULL;
      }
   }
   //normuvannua
   for(i=0; i<dim; i++)
      if(cdpparam.alg_ != DURBOOL)
         sig[i] /= nrm;
   else break;

   //formuvannya failu   
   path = cdpparam.CDPUserPath + "\\" + name + "_" + slovo + "_x" + sysext;
   if(u==0)
   {
      ofstream fout(path.c_str());
      fout<<U<<'\n';
      fout.close();
   }
   ofstream fout(path.c_str(),ios_base::app);

//   fout<<nrm<<'\t';
   fout<<dim<<'\t';
   for(i=0; i<dim; i++)
   {
      fout<<sig[i]<<"  ";
   }
   fout<<'\n';
   fout<<'\n';
   fout.close();

   if((u+1) == U)
   {
      //analiz
      etalon = HeadCycle(path, name, slovo, qk);
      DeleteFile(path);
   }

//---------------------------------------------------------------------------
   delete [] sig; sig = NULL;
   return etalon;
}
catch(...)
{
   ShowMessage("??????? ??? ????????? ?????? ?????");
   return "";
}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

String CDPRecognize::SelectionSignal(types const *in, long long int N, String pth_word, int tokm)
{
try
{
   register int i;
   int c=0;
   int end=0;
   String wrd = "";
   String nextb = "";
   char nextch [80] = {0};
   int fqk = 0, wrdlen=0;
   long double nrm = 0;

   int m_ = cdpparam.m_;
   int M_ = cdpparam.M_;
   int dim = ((int)N/M_)*m_+1;
   types *sig = NULL;
   types *buf = NULL;
   sig = new types[dim];
   memset(sig,0,sizeof(types)*(dim));
   types *ek = NULL;
   int *tau = NULL;
   retval WRD = 0, OLDW = 0;

//---------------------------------------------------------------------------

   //vydilennya movnuh oznak
   for(i=0; i<N-M_+1; i++)
   {
      if(i%M_==0)
      {
         buf = Palgorithm(in+i);
         copy(buf,buf+m_,sig+end*m_);
         nrm = abmax( nrm, Norm(buf,m_-1) );
         end++;
         delete [] buf; buf = NULL;
      }
   }

   //normuvannua
   for(i=0; i<dim; i++)
      if(cdpparam.alg_ != DURBOOL)
         sig[i] /= nrm;
   else break;

   ifstream fin(pth_word.c_str());
   if(!fin) throw Exception("??????? ??? ????????? ????? ");
   for(i=0; i<5; i++)
      fin>>nextch;
   /*
      out1<<cdpparam.curr_realization_U<<'\t';
      out1<<cdpparam.alg_<<'\t';
      out1<<cdpparam.kind_<<'\t';
      out1<<cdpparam.min_energy<<'\t';
      out1<<cdpparam.err_<<'\n';
      ...
      out1<<slovo<<'\n';
   */

   while(wrdlen < wrd_length)
   {
      fin>>nextch;
      nextb = nextch;
      fin>>fqk;
      
      ek = new types [fqk*m_+1];
      memset(ek,0,sizeof(types)*(fqk*m_+1));
      tau = new int [fqk*2+1];
      memset(tau,0,sizeof(int)*(fqk*2+1));

         for(i=0; i<fqk; i++)
         {
            fin>>tau[i*2];
            fin>>tau[i*2+1];
         }
         for(i=0; i<fqk*m_; i++)
         {
            fin>>ek[i];
         }

        for(i=0; i<wrd_length; i++)
           if( Word_lib_ua[i].SubString(Word_lib_ua[i].LastDelimiter(' ')+1,
              Word_lib_ua[i].Length() - Word_lib_ua[i].LastDelimiter(' ')) == nextb )
                 break;

      //porivnyannya skazanogo slova z etalonamu
      if(Word_NumTok[i].LastDelimiter( IntToStr(tokm) ) > 0 || !cdpparam.TokenSens)
         WRD = TempSegmentat(sig, ek, tau, dim, fqk);
      else WRD = 0;

      if( OLDW < WRD )
      {
         OLDW = WRD;
         wrd = nextb;
      }

      nextb = "";

      delete [] ek; ek = NULL;
      delete [] tau; tau = NULL;
      wrdlen++;
   }
   fin.close();

//---------------------------------------------------------------------------
      delete [] sig; sig = NULL;

   return wrd;
}
catch(...)
{
   ShowMessage("??????? ??? ????????? ?????? ?????");
   return "";
}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//global`ni masyvy
//Xval - masyvy U-realizaciy
//dimen - rozmiry Xval masyviv
types **Xval=NULL;
int *dimen=NULL;
int qkk=0;

DWORD ids[max_qk_];        //?????????????? ???????
HANDLE hThreads[max_qk_];    //??????
CRITICAL_SECTION csect;        //??? ????????? ??????: ???????. ????????    

//---------------------------------------------------------------------------

//funkciya paralel`nogo vykonannya
DWORD WINAPI StartCond(void *lParam)
{
   Criterion cp = *((Criterion *)lParam);
   //samosegmentaciya po kogniy r-realizacyi
   cp = cdprecognize.SelfSegmentat(Xval[cp.i], dimen[cp.i], qkk);
   *((Criterion *)lParam) = cp;

   Sleep(10);    //????? ??? ??????????? ?????????
}

//---------------------------------------------------------------------------

String CDPRecognize::HeadCycle(String path, String name, String slovo, int qk)
{
   int U, iter=0;
   int r, i;
   int s, v, u;
   int m_ = cdpparam.m_;

   //rozmiry X masyvu po r-realizaciyi
   int *xl = NULL;
   //temporal`na transkrypciya
   int *tau = NULL;
   //etalony U-realizaciy
   types **ek = NULL;
   //naykrashchyu element r-realizaciyi
   int *re = NULL;

   //masyvy U-realizaciy
   types **X = NULL;
   //masyv harakterystyk rozpiznavannya
   //dlya U-realizaciy
   Criterion *C = NULL;
   //kincivyu etalon
   types *E = NULL;
   types *E_pre = NULL;
   types maxe=0, last=0;

   ifstream in(path.c_str());

   if(in)
   {
      //kilkist` realizaciy
      in>>U;
      xl = new int [U+1];
      memset(xl,0,sizeof(int)*(U+1));
      X = new types *[U];

      for(r=0; r<U; r++)
      {
         //rozmiry realizaciy
         in>>xl[r];
         X[r] = new types [xl[r]+1];
         memset(X[r],0,sizeof(types)*(xl[r]+1));

         for(i=0; i<xl[r]; i++)
         {
            //vvedennya realizaciyi
            in >> X[r][i];
         }
      } // end r
   } // end if
   else
   {
      in.close();
      throw Exception("??????? ??? ????????? ????? "+path);
   }
   in.close();
//---------------------------------------------------------------------------

   E = new types [qk*m_+1];
   memset(E,0,sizeof(types)*(qk*m_+1));
   E_pre = new types [qk*m_+1];
   memset(E_pre,0,sizeof(types)*(qk*m_+1));
   C = new Criterion [U];
   ek = new types *[U];
   re = new int [qk+1];
   memset(re,0,sizeof(int)*(qk+1));
   tau = new int [2*qk+2];
   memset(tau,0,sizeof(int)*(2*qk+2));

//---------------------------------------------------------------------------

   for(r=0; r<U; r++)
   {
      C[r].Creation((int)xl[r]/m_, qk);
      C[r].i = r;
      ek[r] = new types [qk*m_+1];
      memset(ek[r],0,sizeof(types)*(qk*m_+1));
     // if(!isParalel_) C[r] = SelfSegmentat(X[r], xl[r]);
   }

   dimen = xl;
   Xval = X;
   qkk = qk;
   InitializeCriticalSection( &csect );     

   try
   {
      for(i=0; i<U; i++)  //????????? ??????
      {
         hThreads[i] = CreateThread(NULL, NULL, &StartCond, &C[i], NULL, &ids[i]);
         if (hThreads[i] == NULL) throw Exception("??????? ? ????????????????");
      }

      EnterCriticalSection(&csect);
         //SendMessage(Application->Handle, WM_ProgressBarStemM, 0, 0);
         CDPEducation->PBIndicateEduc->StepBy( (int)(100-40)/(double)cdpparam.curr_realization_U );
      LeaveCriticalSection(&csect);

      //chekayemo na zavershennya potokiv
      if(WaitForMultipleObjects(U, hThreads, true, cdpparam.wait_for_threads ) == WAIT_TIMEOUT) //INFINITE
      {
         delete [] E_pre;
         delete [] C;
         delete [] xl;
         delete [] re;
         delete [] tau;
         delete [] E;
         if(ek)
            for (v = 0; v < U; v++)
               delete[] ek[v];
         delete[] ek;  ek = NULL;
         if(X)
            for (v = 0; v < U; v++)
               delete[] X[v];
         delete[] X;  X = NULL;
         ShowMessage("?????? ?? ????????? ?? " + IntToStr(cdpparam.wait_for_threads) + " ??. ??????? ???????? ? (?-??? ??-?? ???????) ??? ?????????? ?????? ?????");
         return "";
      }
      //zakryvayemo deskryptory
      for(i = 0; i < U; i++)
         CloseHandle(hThreads[i]);
      DeleteCriticalSection(&csect); 

   }
   catch(...)
   {
      return "";
   }
//---------------------------------------------------------------------------

   //Pochatkovi umovy
   for(r=0; r<U; r++)
   {
      Pargmax(X[r], C[r], ek[r], qk);
    //  C[r] = Segmentat(X[r], ek[r], xl[r], qk);
    //  Pargmax(X[r], C[r], ek[r], qk);
   }

   //Pochatkovyj etalon
   for(s=1; s<=qk; s++)
   {
      last = 0;
      for(u=0; u<U; u++)
      {
         maxe = 0;
         for(r=0; r<U; r++)
         {
            for(v=C[r].w[s-1]+1; v<=C[r].w[s]; v++)
            {
               maxe += g(X[r],ek[u],v,s); //DIMHEM
            }
         } // end r
         if(maxe >= last)
         {
            //shukayem naykrashchyu element
            //z usih U-realizaciy dlya
            //zapovnennya etalonu 1:qk
            last = maxe;
            re[s-1] = u;
         }
      }  // end u
   } // end s

   for(u=0; u<qk; u++)
   {
      for(v=0; v<m_; v++)
      {
         E[v+u*m_] = ek[re[u]][v+u*m_];
      }
   }

   //SendMessage(Application->Handle, WM_ProgressBarStemM, 0, 0);
   CDPEducation->PBIndicateEduc->StepBy( 20 );
//---------------------------------------------------------------------------

   //Golovnuy cykl
   while( !equalE(E,E_pre,qk) )
   {
      //maksymal`no MAX_ITER_ - iteraciy
      if(iter < MAX_ITER_) iter++;
      else break;
      assign(E,E_pre,qk);

      //Pershyj etap
      for(r=0; r<U; r++)
      {
         //shukayem granuci segmentiv C.w po
         //etalonu E
         C[r] = Segmentat(X[r], E, xl[r], qk);
         //C.w shukayem etalon dlya r-realizaciyi
         //po granucyah segmentiv C.w
         Pargmax(X[r], C[r], ek[r], qk);         
      }

      //Drugyj etap
      for(s=1; s<=qk; s++)
      {
         last = 0;
         for(u=0; u<U; u++)
         {
            maxe = 0;
            for(r=0; r<U; r++)
            {
               for(v=C[r].w[s-1]+1; v<=C[r].w[s]; v++)
               {
                  maxe += g(X[r],ek[u],v,s);  //DIMHEM
               }
            } // end r
            if(maxe >= last)
            {
               //shukayem naykrashchyu element
               //z usih U-realizaciy dlya
               //zapovnennya etalonu 1:qk
               last = maxe;
               re[s-1] = u;
            }
         }  // end u
      } // end s

      for(u=0; u<qk; u++)
      {
         for(v=0; v<m_; v++)
         {
            E[v+u*m_] = ek[re[u]][v+u*m_];
         }
      }
   } // end while

   //Temporal`na transkrypciya
   tau[0] = 0;
   tau[1] = -1;

   for(s=2; s<qk; s++)
   {
      for(r=0; r<U; r++)
      {
         if(C[r].w[s]-C[r].w[s-1] <= tau[s*2-2] || tau[s*2-2]==0 ) tau[s*2-2] = C[r].w[s]-C[r].w[s-1];
         if(C[r].w[s]-C[r].w[s-1] >= tau[s*2-1]) tau[s*2-1] = C[r].w[s]-C[r].w[s-1];
      }
   }
   tau[2*qk-2] = 0;
   tau[2*qk-1] = -1;

//---------------------------------------------------------------------------

   //SendMessage(Application->Handle, WM_ProgressBarStemM, 0, 0);
   CDPEducation->PBIndicateEduc->Position = 100;

   name = cdpparam.CDPUserPath + "\\" + name + sysext;

   if(!FileExists(name))
   {
      ofstream out1(name.c_str());
      out1<<cdpparam.curr_realization_U<<'\t';
      out1<<cdpparam.alg_<<'\t';
      out1<<cdpparam.kind_<<'\t';
      out1<<cdpparam.min_energy<<'\t';
      out1<<cdpparam.err_<<'\n';
      out1.close();
   }

   ofstream out;

  if(!CheckSameWord(name, slovo))
     out.open(name.c_str(),ios_base::app);
  else
  {
     if( !FileExists(cdpparam.CDPUserPath + "\\" + ExtendToFile + sysext) )
     {
        ofstream out3(String(cdpparam.CDPUserPath + "\\" + ExtendToFile + sysext).c_str());
        out3<<cdpparam.curr_realization_U<<'\t';
        out3<<cdpparam.alg_<<'\t';
        out3<<cdpparam.kind_<<'\t';
        out3<<cdpparam.min_energy<<'\t';
        out3<<cdpparam.err_<<'\n';
        out3.close();
     }
     out.open(String(cdpparam.CDPUserPath + "\\" + ExtendToFile + sysext).c_str(), ios_base::app);
  }

   //stvoryuyem fail dlya zapysu
   //etalonnyh danyh
   if(out)
   {
      out<<slovo.c_str()<<'\t';
      out<<qk<<'\t';
      out<<'\n';
      for(s=0; s<qk; s++)
      {
         out<<tau[s*2]<<'\t';
         out<<tau[s*2+1]<<'\n';
      }
      out<<'\n';
      for(u=0; u<qk*m_; u++)
      {
         out<<E[u]<<'\t';
      }
      out<<'\n';
   }
   else
   {
      //out.close();
      throw Exception("??????? ??? ????????? ????? ");
   }
   out.close();

//---------------------------------------------------------------------------
   delete [] E_pre;
   delete [] C;
   delete [] xl;
   delete [] re;
   delete [] tau;
   delete [] E;

   if(ek)
      for (v = 0; v < U; v++)
         delete[] ek[v];
   delete[] ek;  ek = NULL;

   if(X)
      for (v = 0; v < U; v++)
         delete[] X[v];
   delete[] X;  X = NULL;

   counter++;
   return name;
}

//---------------------------------------------------------------------------

bool CDPRecognize::equalE(types const *E, types const *E_pre, int qk)
{
   for(int i=0; i<qk*cdpparam.m_; i++)
      if(E[i] != E_pre[i]) return false;
   return true;
}

//---------------------------------------------------------------------------

void CDPRecognize::assign(types *E, types *E_pre, int qk)
{
   for(int i=0; i<qk*cdpparam.m_; i++)
      E_pre[i] = E[i];
}

//---------------------------------------------------------------------------

int CDPRecognize::CheckSameWord(String pathw, String slovo)
{
   char findst[50] = {0};
   int cnt=0;

   fstream out(pathw.c_str(),ios_base::in|ios_base::out);

   while(!out.eof())
   {
      out>>findst;
      cnt++;
      if(String(findst) == slovo)
         break;
   }

   out.close();
   if(String(findst) == slovo) return cnt;
   else return 0;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


Criterion CDPRecognize::Segmentat(const types *x, const types *ek, int xn, int qk)
{
try
{
   int m_ = cdpparam.m_;
   int in = (int) xn / m_;
   Criterion C(in, qk);
   register int i, s, u, v;
   retval suma = 0.0, fmax;
   retval **MG = new retval *[in+1];
   int **uk = new int* [in+1];
      for (v = 0; v < in+1; v++)
      {
         uk[v] = new int[qk+1];
         memset(uk[v],0,sizeof(int)*(qk+1));
         MG[v] = new retval [qk+1];
         memset(MG[v],0,sizeof(retval)*(qk+1));
      }
      for(s=1; s<=qk; s++)
         for (v=1; v<=in; v++)
            MG[v][s] = g(x,ek,v,s);  //DIMHEM

//---------------------------------------------------------------------------

   //Pershyj element etalonu
   for(i=1; i<=in-qk+1; i++)
   {
      C.F[i][1] = C.F[i-1][1] + MG[i][1];  // g(x,ek,i,1);   //DIMHEM
      uk[i][1] = uk[i-1][1] + 1;
   }

   //2:qk-1 - elementy etalonu
   for(s=2; s<=qk-1; s++)
   {
      for(i=s; i<=in-qk+s; i++)
      {
         fmax = 0;
         for(u=1; u<=in-qk+1 && u<i; u++)
         {
            suma = 0;
            for(v=i-u+1; v<=i; v++)
               suma += MG[v][s];  //g(x,ek,v,s);   //DIMHEM

            if( fmax <= C.F[i-u][s-1] + suma || u == 1)
            {
               uk[i][s] = u;
               fmax = C.F[i-u][s-1] + suma;
            }
         } //end u;
         C.F[i][s] = fmax;
      } //end i;
   } //end s;


//-------------------------------????
   fmax = 0;
   for(u=1; u<=in-qk+1; u++)
   {
      suma = 0;
      for(v=in-u+1; v<=in; v++)
         suma += MG[v][qk];  //g(x,ek,v,qk);   //DIMHEM

      if( fmax <= C.F[in-u][qk-1] + suma || u == 1)
      {
         uk[in][qk] = u;
         fmax = C.F[in-u][qk-1] + suma;
      }
   } //end u;
   C.F[in][qk] = fmax;
//-------------------------------????

/*
   //Ostanniy element etalonu
   for(i=qk; i<=in; i++)
   {
      if( C.F[i][qk-1] < C.F[i-1][qk] + MG[i][qk] )  //DIMHEM    g(x,ek,i,qk)
      {
         C.F[i][qk] = C.F[i-1][qk] + MG[i][qk];  //DIMHEM   g(x,ek,i,qk)
         uk[i][qk] = uk[i-1][qk] + 1;
      }
      else
      {
        C.F[i][qk] = C.F[i][qk-1];
        uk[i][qk] = 0;
      }
   }
*/   

   //Granyci segmetiw
   C.w[qk] = in;
   for(s=qk; s>=1; s--)
   {
      C.v[s] = uk[C.w[s]][s];
      C.w[s-1] = C.w[s] - C.v[s];
   }
   C.w[0] = 0;

//---------------------------------------------------------------------------
   if(uk)
      for (v = 0; v < in+1; v++)
         delete[] uk[v];
   delete[] uk;  uk = NULL;

   if(MG)
      for (v = 0; v < in+1; v++)
         delete[] MG[v];
   delete[] MG;  MG = NULL;
//---------------------------------------------------------------------------

   return C;
}
catch(...)
{
   throw Exception("??????? ??? ???????????");
}
} // end Segmentat


Criterion CDPRecognize::SelfSegmentat(const types *x, int xn, int qk)
{
try
{
   int m_ = cdpparam.m_;
   int in = (int) xn / m_;
   Criterion C(in, qk);
   register int i, s, u, v, e;
   retval suma = 0.0, fmax, maxe;
   retval **MG = new retval *[in+1];
   int **uk = new int* [in+1];
      for (v = 0; v < in+1; v++)
      {
         uk[v] = new int[qk+1];
         memset(uk[v],0,sizeof(int)*(qk+1));
         MG[v] = new retval [in+1];
         memset(MG[v],0,sizeof(retval)*(in+1));
      }
      for(e=1; e<=in; e++)
         for (v=1; v<=in; v++)
            MG[v][e] = g(x,x,v,e);  //DIMHEM

//---------------------------------------------------------------------------

   //Pershyj element etalonu
   for(i=1; i<=in-qk+1; i++)
   {
      suma = 0;
      for(u=1; u<=i; u++)
      {
         maxe = 0;
         for(v=1; v<=i; v++)
            maxe += MG[v][u];
         if(suma <= maxe) suma = maxe;
      }

      C.F[i][1] = suma;
      uk[i][1] = uk[i-1][1] + 1;
   }

   //2:qk_ - elementy etalonu
   for(s=2; s<=qk; s++)
   {
      for(i=s; i<=in-qk+s; i++)
      {
         fmax = 0;
         for(u=1; u<=in-qk+1 && u<i; u++)
         {
            suma = 0;
            for(e=i-u+1; e<=i; e++)
            {
               maxe = 0;
               for(v=i-u+1; v<=i; v++)
               {
                  maxe += MG[v][e];
               }
               if(maxe >= suma) suma = maxe;
            } //end e;

            if( fmax <= C.F[i-u][s-1] + suma || u == 1)
            {
               uk[i][s] = u;
               fmax = C.F[i-u][s-1] + suma;
            }
         } //end u;
         C.F[i][s] = fmax;
      } //end i;
   } //end s;

   //Granyci segmetiw
   C.w[qk] = in;
   for(s=qk; s>=1; s--)
   {
      C.v[s] = uk[C.w[s]][s];
      C.w[s-1] = C.w[s] - C.v[s];
   }
  C.w[0] = 0;
//  C.v[1] = C.v[1] + 1; //???

//---------------------------------------------------------------------------
   if(uk)
      for (v = 0; v < in+1; v++)
         delete[] uk[v];
   delete[] uk;  uk = NULL;

   if(MG)
      for (v = 0; v < in+1; v++)
         delete[] MG[v];
   delete[] MG;  MG = NULL;
//---------------------------------------------------------------------------

   return C;
}
catch(...)
{
   throw Exception("??????? ??? ???????????????");
}
} // end SelfSegmentat

//---------------------------------------------------------------------------

retval CDPRecognize::TempSegmentat(const types *x, const types *ek, const int *tau, int xn, int qk)
{
try
{
   int m_ = cdpparam.m_;
   int in = (int) xn / m_, err = 0;
   Criterion C(in, qk);
   register int i, s, u, v;
   retval suma = 0.0, fmax;
   retval **MG = new retval *[in+1];
   int **uk = new int* [in+1];

      for (v = 0; v < in+1; v++)
      {
         uk[v] = new int[qk+1];
         memset(uk[v],0,sizeof(int)*(qk+1));
         MG[v] = new retval [qk+1];
         memset(MG[v],0,sizeof(retval)*(qk+1));
      }

      for(s=1; s<=qk; s++)
         for (v=1; v<=in; v++)
            MG[v][s] = g(x,ek,v,s);  //DIMHEM

   //if(tau[1] == -1) tau[1] = in-qk+1;
   //if(tau[2*qk-1] == -1) tau[2*qk-1] = in-qk+1;
//---------------------------------------------------------------------------
   //Pershyj element etalonu
   for(i=1; i<=in-qk+1; i++)
   {
      C.F[i][1] = C.F[i-1][1] + MG[i][1]; //  g(x,ek,i,1);  //DIMHEM
      uk[i][1] = uk[i-1][1] + 1;      
   }

   //2:qk_-1 - elementy etalonu
   for(s=2; s<=qk-1; s++)
   {
      for(i=s; i<=in-qk+s; i++)
      {
         fmax = 0;
         for(u=tau[s*2-2]; u<=tau[s*2-1] && u<i; u++)
         {
            suma = 0;
            for(v=i-u+1; v<=i; v++)
               suma += MG[v][s]; //g(x,ek,v,s);  //DIMHEM

            if( fmax <= C.F[i-u][s-1] + suma || u == tau[s*2-2])
            {
               fmax = C.F[i-u][s-1] + suma;
               uk[i][s] = u;               
            }
         } //end u;
         C.F[i][s] = fmax;
      } //end i;
   } //end s;

//-------------------------------????
   //Ostanniy element etalonu
   fmax = 0;
   for(u=1; u<=in-qk+1; u++)
   {
      suma = 0;
      for(v=in-u+1; v<=in; v++)
         suma += MG[v][qk];  //g(x,ek,v,qk);   //DIMHEM

      if( fmax <= C.F[in-u][qk-1] + suma || u == 1)
      {
         fmax = C.F[in-u][qk-1] + suma;
         uk[in][qk] = u;         
      }
   } //end u;
   C.F[in][qk] = fmax;

   //Granyci segmetiw
   C.w[qk] = in;
   for(s=qk; s>=1; s--)
   {
      C.v[s] = uk[C.w[s]][s];
      C.w[s-1] = C.w[s] - C.v[s];
   }

   err = SelfCompareEtalon(ek, qk, C);   
//-------------------------------????
/*
   //Ostanniy element etalonu
   for(i=qk; i<=in; i++)
   {
      if( C.F[i][qk-1] < C.F[i-1][qk] + MG[i][qk] )  //DIMHEM   g(x,ek,i,qk)
      {
         C.F[i][qk] = C.F[i-1][qk] + MG[i][qk];   //DIMHEM  g(x,ek,i,qk);
      }
      else
      {
        C.F[i][qk] = C.F[i][qk-1];
      }
   }
*/   
//---------------------------------------------------------------------------
   if(uk)
      for (v = 0; v < in+1; v++)
         delete[] uk[v];
   delete[] uk;  uk = NULL;

   if(MG)
      for (v = 0; v < in+1; v++)
         delete[] MG[v];
   delete[] MG;  MG = NULL;


   if( (err - C.F[in][qk])/err*100 > cdpparam.err_ )  C.F[in][qk] = 0;

   return C.F[in][qk];
}
catch(...)
{
   throw Exception("??????? ??? ??????????? ? ????. ???????.");
}
} // end TempSegmentat

//---------------------------------------------------------------------------

types CDPRecognize::SelfCompareEtalon(types const *E, int qk, Criterion C)
{
   types maxe=0, temp=0;

   for(int u=1; u<=qk; u++)
   {
      maxe = g(E,E,u,u);
      temp += maxe;
      for(int v=1; v<C.v[u]; v++)
      {
         temp += maxe;
      }
      //temp += maxe;
      //maxe=0;
   }
   
   return temp;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

types * CDPRecognize::Palgorithm(types const *inm)
{
   switch(cdpparam.alg_)
   {
      case TFFTM:  //  Spectral`nyj analiz (seredni znachennya)
         return Pfftm(inm);
      break;
      case DURCOEF:
      case DURBOOL:  // Koeficienty algorytmu liniynogo peredbachennya (bulevi)
         types *B = PAutoCorr(inm);  // Spochatku vykonujem korelyaciynyj analiz
         types *arr = PDurbin(B);
         delete [] B; B = NULL;
         return arr;
      break;
   };
   return NULL;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void CDPRecognize::Pargmax(types const *x, Criterion C, types *ek, int qk)
{
   int m_ = cdpparam.m_;
   int *e = new int[qk+1];
   memset(e,0,sizeof(int)*(qk+1));

   types maxe=0, last=0;
   int s, v, u;

   //kilkist granyc` = qk
   for(s=1; s<=qk; s++)
   {
      last = 0;
      //cykl po granycyah segmentiv: x
      for(u=C.w[s-1]+1; u<=C.w[s]; u++)
      {
         maxe = 0;
         for(v=C.w[s-1]+1; v<=C.w[s]; v++)
         {
            //vuznachayemo podibnist` elementiv
            maxe += g(x,x,v,u);  //DIMHEM
         }
         if(maxe >= last)
         {
            //naykrashchyj element v segmenti
            last = maxe;
            e[s-1] = u;
         }
      } // end u
   } // end s

   for(u=0; u<qk; u++)
   {
      for(v=0; v<m_; v++)
      {
         //zapysuyem naykrashchyj elementy 
         ek[v+u*m_] = x[(e[u]-1)*m_+v];
      }
   }
   
   delete [] e;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

types CDPRecognize::g(const types *x, const types *e, int v, int s)
{
   int i, j;
   int m_ = cdpparam.m_;   

   v <= 0 ? v=0 : v--;
   s <= 0 ? s=0 : s--;
   types c=0;

   double disp = 0.0;
//---------------------------------------------------------------------------
try
{
   switch(cdpparam.kind_)
   {
      case DIMEUCLID: //evklidova vidstan`
         for(i=0; i<m_; i++)
            c = c + pow((x[i+v*m_] - e[i+s*m_]),2);
         //  c = c * MAX_DIST_C_;
         c = c + 1;
         c = 1./c;
         return c;
      break;

      case CROSS: //Vzayemna corelyaciya
         disp = sqrt((SumOfSquares(e+s*m_, m_-1) / m_) * (SumOfSquares(x+v*m_, m_-1) / m_));
         for(i=0; i < (int)m_/2; i++)
         {
            c = abmax(c, (std::inner_product(e+s*m_,e+s*m_+m_-i,x+v*m_+i,0) / (double)m_));
            c = abmax(c, (std::inner_product(x+v*m_,x+v*m_+m_-i,e+s*m_+i,0) / (double)m_));
         }
         return  fabs( c / abmax(disp,1.0) );
      break;

      case DIMHEMIN: //hemingova vidstan`
         for(i=0; i<m_; i++)
            if(x[i+v*m_] == e[i+s*m_]) c++;
         return c;
         /*
         for(i=0; i<m_; i++)
            c = c + fabs(x[i+v*m_]) - fabs(e[i+s*m_]);
         c++;
         c = 1./c;
         return c;
         */
      break;

      case SCALAR: //scalyarnyj dobutok (x,e)
         return 1./( std::inner_product(e+s*m_,e+s*m_+m_,x+v*m_,0) + 1);
      break;

      case OTHERDISP: //dlya sygnaliv riznoyi pryrody, z dyspersiyeyu: disp
         disp = Variance(e+s*m_, m_-1);
         //if(disp<=0.001) disp = 0.01;

         for(i=0; i<m_; i++)
            c = c + pow((x[i+v*m_] - e[i+s*m_])/disp, 2);
         c++;
         c = 1./c;
         return c;
      break;
   }
   return 0;
}
catch(...)
{
   return 0;
}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

types* CDPRecognize::PAutoCorr(types const *inm)
{
try
{
  int m_ = cdpparam.m_;
  int M_ = cdpparam.M_;
  types *B = new types [m_+1];
  memset(B, 0, sizeof(types)*(m_+1));
  types *in = new types [M_+1];
  memset(in, 0, sizeof(types)*(M_+1));

  int s, n;

       if(!less1_)
          for (s=0;  s<M_; s++)
             in[s] = inm[s] / (double)MAX_;
       else copy(inm,inm+M_,in);

         if(win_)
         {
            for (s=0;  s<M_; s++)
            {
                //zvaguval`nyj masyv
                in[s] = in[s] * ( 0.54 - 0.46 * cos(2*M_PI*(s/(M_-1))));
            }
         }

       // Rozrahunok masyvu korelyaciyi B(s)
       for (s=0;  s<m_; s++)
       {
         B[s] = 0.0;
         for (n=0;  n<M_-s; n++)
            B[s] = B[s] + in[n] * in[n+s];  //SUM( f(n)*f(n+s) )
       }

  delete [] in;
  return B;
}
catch(...)
{
    return 0;   
}
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

double* CDPRecognize::PDurbin(types const *B)
{
try
 {
  int m_ = cdpparam.m_;
  int M_ = cdpparam.M_;
  double sigma=0.0;
  int Q = m_, teta = 0;  // teta = ?  Q = floor((M_ - m_) / 2)
  int m = m_;   m--;

  double *xiu = new double [Q+1];
  memset(xiu, 0, sizeof(double)*(Q+1));

  double *E = new double [m+1];
  memset(E, 0, sizeof(double)*(m+1));
  double *k = new double [m+1];
  memset(k, 0, sizeof(double)*(m+1));
  double *copy = new double [m+1];
  memset(copy, 0, sizeof(double)*(m+1));
  double *b = new double [m+1];
  memset(b, 0, sizeof(double)*(m+1));
  double *G = new double [Q+1];
  memset(G, 0, sizeof(double)*(Q+1));

  B[0] <= 0 ? E[0] = 0.00001 : E[0] = B[0];

  double **a;
  a = new double *[m+1];
     for (int j = 0; j <= m; j++)
     {
        a[j] = new double[m+1];
        memset(a[j], 0, sizeof(double)*(m+1));
     }

   double suma=0.0, p=0.0;
   int s=0, v=0;
   
//---------------------------------------------------------------------------
     // Znahodymo masyv a:
     for (s = 1; s<=m; s++)
     {
         suma = 0.0;
         for (v = 1; v<=s-1; v++)
            suma = suma + a[s-1][v] * B[s-v];

         k[s] = - (B[s] + suma) / E[s-1];
         a[s][s] = k[s];

         for (v = 1; v<=s-1; v++)
            a[s][v] = a[s-1][v] + k[s] * a[s-1][s-v];

         E[s] = (1 - k[s]*k[s]) * E[s-1];
     }
    a[m][0] = 1;  suma = 0;

    // Znahodymo kincevyj masyv am:    
    for (s = 0; s<=m; s++)
    {
       copy[s] = a[m][s];
       suma = suma + copy[s] * B[s];  // ? - s:[1;m]
    }
    // Znahodymo sigma:
    sigma = sqrt((1./M_) * (B[0]+suma));


    // Znahodymo masyv b:
    for (s = 0; s<=m; s++)
       for (v = 0; v<=m-s; v++)
       {
          if(s==0) b[0] = b[0] + copy[v] * copy[v];
          else
          {
             v == m-s ? b[s] = 2 * (b[s] + copy[v] * copy[v+s]):
             b[s] = b[s] + copy[v] * copy[v+s];
          }
       }

    // Znahodymo masyv G:
    for (v = 0; v<Q; v++)
    {
       p = (v * ( floor(vgr_/Q) - 1 ) ) / vgr_; 
       suma = 0;
       for (s = 0; s<=m; s++)
          suma = suma + b[s] * cos(p*s*M_PI);

       G[v] = (M_ * sigma * sigma) / suma;

       if(v!=0 && G[v]>=G[v-1] && G[v]>= teta)
          xiu[v-1] = 1;
       else if(v!=0)
          xiu[v-1] = 0;
    }

//---------------------------------------------------------------------------

    delete [] copy;
    delete [] G; G = NULL;
    delete [] b; b = NULL;
    delete [] E; E = NULL;
    if(a)
       for (int i = 0; i <= m;  i++)
          delete[] a[i];
    delete[] a;  a = NULL;
    
    if(cdpparam.alg_ != DURBOOL)
    {
       delete [] xiu; xiu = NULL;    
       return k;
    }
    else
    {
       delete [] k; k = NULL;
       return xiu;
    }   
 }
 catch(...)
 {
    return 0;
//    throw Exception("??????? ? ????????? ???????");
 }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

types* CDPRecognize::Pfftm(types const *inm)
{
   try
   {
   int m_ = cdpparam.m_;
   int M_ = cdpparam.M_;
   int i, j, ost= M_/2/m_, inc=0;
   types suma=0;
   short *arr = new short [M_+1];
   memset(arr, 0, sizeof(short)*(M_+1));
   types *ans = new types [m_+1];
   memset(ans, 0, sizeof(types)*(m_+1));

   if(less1_) //yakshcho znachennya <1
   {
      for(i=0; i<M_; i++)
         arr[i] = inm[i] * MAX_;
   }
   else
   {
      for(i=0; i<M_; i++)
         arr[i] = inm[i];
   }

   //Stvoryuyemo obyekt dlya
   //shvydkogo peretvorennya fur'ye
   TFFT fftm(log(M_)/log(2));
   copy(arr, arr+M_, fftm.ReData);
   //Energetychyj spectr
   fftm.GetE();

   for(i=0; i<m_; i++)
   {
      suma = 0;
      for(j=i*ost; j<(i+1)*ost; j++)
         suma = suma + fftm.E[j]; // * fftm.E[i];
      //zapysyem useredneni znachennya
      ans[i] = suma;
   }

  // for(i=0; i<m_; i++)
  //    ans[i]=fftm.E[i];

   delete [] arr;
   return ans;
   }
   catch(...)
   {
      return 0; 
//      throw Exception("??????? ??? ????????????? ???????");
   }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------


//------------------------------------------------------------------------------

  Criterion::Criterion()
  {
     i = 0;
     in = 0;
     qk = 0;
     v = NULL;
     w = NULL;
     F = NULL;
  }

//------------------------------------------------------------------------------

  void Criterion::Creation(int _in, int _qk)
  {
  try
  {
     i = 0;
     in = _in;
     qk = _qk;
     v = new int [qk+1];
     memset(v,0,sizeof(int)*(qk+1));
     w = new int [qk+1];
     memset(w,0,sizeof(int)*(qk+1));

     F = new retval* [in+1];
     for (int i = 0; i < in+1; i++)
     {
        F[i] = new retval[qk+1];
        memset(F[i],0,sizeof(retval)*(qk+1));
     }
  }
  catch(...)
  {
     throw Exception("??????? ??? ????????? ???'??? ??'???? Criterion");
  }
  }

//------------------------------------------------------------------------------

  Criterion::Criterion(int _in, int _qk)
  {
     Creation(_in, _qk);
  }

//------------------------------------------------------------------------------

  Criterion::~Criterion()
  {
  try
  {
     delete[] v;  v = NULL;
     delete[] w;  w = NULL;
     if(F)
        for (int i = 0; i < in+1; i++)
           delete[] F[i];
     delete[] F;  F = NULL;
  }
  catch(...)
  {
     throw Exception("??????? ??? ?????????? ???'??? ??'???? Criterion");
  }
  }

//------------------------------------------------------------------------------

Criterion Criterion::operator=(const Criterion &op2)    
{
   if(in != op2.in || qk != op2.qk) throw Exception("??????? ? ???????????? ??? ???????? ??????????");
   if(F)
      for (int i = 0; i < op2.in+1; i++)
         delete[] F[i];
   delete[] F;  F = NULL;
   delete[] v;  v = NULL;
   delete[] w;  w = NULL;

   try
   {
   F = new retval* [op2.in+1];
   for (int i = 0; i < op2.in+1; i++)
   {
      F[i] = new retval[op2.qk+1];
      memset(F[i],0,sizeof(retval)*(op2.qk+1));
   }
   for(int i=0; i<op2.in+1; i++)
      for(int j=0; j<op2.qk+1; j++)
         F[i][j] = op2.F[i][j];

    v = new int [op2.qk+1];
    memset(v,0,sizeof(int)*(op2.qk+1));
    w = new int [op2.qk+1];
    memset(w,0,sizeof(int)*(op2.qk+1));

   for(int i=0; i<op2.qk+1; i++)
   {
      v[i] = op2.v[i];
      w[i] = op2.w[i];
   }

    i = op2.i;
    qk = op2.qk;
    in = op2.in;   

   return *this;
   }
  catch(...)
  {
     delete[] v;  v = NULL;
     delete[] w;  w = NULL;
     if(F)
        for (int i = 0; i < in+1; i++)
           delete[] F[i];
     delete[] F;  F = NULL;
     throw Exception("??????? ??????????????? ????????? = Criterion");
  }
}

//------------------------------------------------------------------------------
Criterion::Criterion(const Criterion &op2)
{
    try
    {
    //delete[] v;  v = NULL;
    //delete[] w;  w = NULL;
    qk = op2.qk;
    in = op2.in;
    i = op2.i;
    v = new int [op2.qk+1];
    memset(v,0,sizeof(int)*(op2.qk+1));
    w = new int [op2.qk+1];
    memset(w,0,sizeof(int)*(op2.qk+1));

   for(int i=0; i<op2.qk+1; i++)
   {
      v[i] = op2.v[i];
      w[i] = op2.w[i];
   }

   F = new retval* [op2.in+1];
   for (int i = 0; i < op2.in+1; i++)
   {
      F[i] = new retval[op2.qk+1];
      memset(F[i],0,sizeof(retval)*(op2.qk+1));
   }
   for(int i=0; i<op2.in+1; i++)
      for(int j=0; j<op2.qk+1; j++)
         F[i][j] = op2.F[i][j];
   }
  catch(...)
  {
     delete[] v;  v = NULL;
     delete[] w;  w = NULL;
     if(F)
        for (int i = 0; i < in+1; i++)
           delete[] F[i];
     delete[] F;  F = NULL;
     throw Exception("??????? ???????????? ????? Criterion");
  }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
