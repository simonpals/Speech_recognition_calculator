//---------------------------------------------------------------------
#include <vcl.h>
#include <mem.h>
#include <utilcls.h>
#include <fstream.h>

#pragma hdrstop

#include "UCDPCalc.h"
#include "UCalc.h"
#include "UCDPEducation.h"
#include "UCDPSelectWord.h"
#include "UCDPAuthorization.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))

//---------------------------------------------------------------------
#pragma link "trayicon"
#pragma resource "*.dfm"
TFCDPMain *FCDPMain;

//---------------------------------------------------------------------
__fastcall TFCDPMain::TFCDPMain(TComponent *AOwner)
	: TForm(AOwner)
{
   CDPUserName = "";
   FCDPAuthorization = new TFCDPAuthorization(this);
   FCDPAuthorization->ShowModal();
   if(CDPUserName=="") Application->Terminate();
   FCDPAuthorization = NULL;

   FCalc = new TFCalc(this);

   memset(srec_buf.arr, 0, sizeof(short)*cdpparam.fftN);
   IsMinimize = false;
   recorder = NULL;
   fft = new TFFT(cdpparam.LogFrameSize);
   First_Sec_Disable = 0;

   tsbuf = new Graphics::TBitmap();
   wbcgr = new Graphics::TBitmap();
   wbuf = new Graphics::TBitmap();
}

//---------------------------------------------------------------------------

__fastcall TFCDPMain::~TFCDPMain()
{
    delete recorder;
    delete fft;
    vsig.clear();  

    delete tsbuf;
    delete wbcgr;
    delete wbuf;
}

//---------------------------------------------------------------------------

// Обробка буфера WaveRecorder
void WaveInProc(DWORD pinstance,DWORD pbufer)
{
   ((TFCDPMain*)pinstance)->ProcessBuffer((short*)pbufer);
}

void __fastcall TFCDPMain::ProcessBuffer(short *buf)
{
   if(!recorder)
      return;

   if(First_Sec_Disable == floor(cdpparam.MSampleRate/(double)cdpparam.fftN) && First_Noise_Sec)
   {
      First_Noise_Sec = false;
      First_Sec_Disable = 0;
   }
   if(First_Noise_Sec)
   {
      First_Sec_Disable++;
      return;
   }   

   Move(buf,fft->ReData,sizeof(short)*cdpparam.fftN);
   if(!IsMinimize)
      WaveUpdate();

   //////////////////////////////////////////

   copy(buf, buf+cdpparam.fftN, srec_buf.arr);
   vsig.push_back(srec_buf);

   double *keyword = NULL;

   int ret_code = CDPSelectWord(vsig);
   String curwrd = "";

   if(ret_code == 1 && vsig.size() < cdpparam.max_vec_size)
   {

      if(!SpeedButton1->Down || cdpparam.isFirstTime)
      {
         vsig.clear();
         return;
      }
      keyword = new double [vsig.size()*cdpparam.fftN];
      memset(keyword, 0, sizeof(double) * vsig.size() * cdpparam.fftN);
      for(int i=0; i<vsig.size(); i++)
         for(int j=0; j<cdpparam.fftN; j++)
            keyword[i*cdpparam.fftN + j] = vsig[i].arr[j];

      int tokm = 0;
      if(cdpparam.TokenSens)
      {
         tokm = TokenCountFunc(keyword, vsig.size());
      }

      curwrd = cdprecognize.SelectionSignal( keyword, vsig.size() * cdpparam.fftN, String(cdpparam.CDPUserPath + "\\" + FCDPMain->CDPUserName + cdpparam.dbCDPext).c_str(), tokm);
      StatusBar->Panels->Items[0]->Text = curwrd;

      call_buttons_calc(curwrd);

      vsig.clear();
      delete [] keyword; keyword = NULL;
   }
   else if(ret_code == 1 && vsig.size() >= cdpparam.max_vec_size)
   {
      vsig.clear();
   }

}

//---------------------------------------------------------------------------
void __fastcall TFCDPMain::call_buttons_calc(String curwrd)
{

   String str1 = "";

   str1 = Word_lib_ua[0].SubString(Word_lib_ua[0].LastDelimiter(' ') + 1, Word_lib_ua[0].Length());
   if(curwrd == str1)
      FCalc->Btn0Click(0);
   str1 = Word_lib_ua[1].SubString(Word_lib_ua[1].LastDelimiter(' ') + 1, Word_lib_ua[1].Length());
   if(curwrd == str1)
      FCalc->Btn1Click(0);
   str1 = Word_lib_ua[2].SubString(Word_lib_ua[2].LastDelimiter(' ') + 1, Word_lib_ua[2].Length());
   if(curwrd == str1)
      FCalc->Btn2Click(0);
   str1 = Word_lib_ua[3].SubString(Word_lib_ua[3].LastDelimiter(' ') + 1, Word_lib_ua[3].Length());
   if(curwrd == str1)
      FCalc->Btn3Click(0);
   str1 = Word_lib_ua[4].SubString(Word_lib_ua[4].LastDelimiter(' ') + 1, Word_lib_ua[4].Length());
   if(curwrd == str1)
      FCalc->Btn4Click(0);
   str1 = Word_lib_ua[5].SubString(Word_lib_ua[5].LastDelimiter(' ') + 1, Word_lib_ua[5].Length());
   if(curwrd == str1)
      FCalc->Btn5Click(0);
   str1 = Word_lib_ua[6].SubString(Word_lib_ua[6].LastDelimiter(' ') + 1, Word_lib_ua[6].Length());
   if(curwrd == str1)
      FCalc->Btn6Click(0);
   str1 = Word_lib_ua[7].SubString(Word_lib_ua[7].LastDelimiter(' ') + 1, Word_lib_ua[7].Length());
   if(curwrd == str1)
      FCalc->Btn7Click(0);
   str1 = Word_lib_ua[8].SubString(Word_lib_ua[8].LastDelimiter(' ') + 1, Word_lib_ua[8].Length());
   if(curwrd == str1)
      FCalc->Btn8Click(0);
   str1 = Word_lib_ua[9].SubString(Word_lib_ua[9].LastDelimiter(' ') + 1, Word_lib_ua[9].Length());
   if(curwrd == str1)
      FCalc->Btn9Click(0);
   str1 = Word_lib_ua[10].SubString(Word_lib_ua[10].LastDelimiter(' ') + 1, Word_lib_ua[10].Length());
   if(curwrd == str1)
      FCalc->BtnEqClick(0);
   str1 = Word_lib_ua[11].SubString(Word_lib_ua[11].LastDelimiter(' ') + 1, Word_lib_ua[11].Length());
   if(curwrd == str1)
      Timer1->Enabled = true;
   str1 = Word_lib_ua[12].SubString(Word_lib_ua[12].LastDelimiter(' ') + 1, Word_lib_ua[12].Length());
   if(curwrd == str1)
      FCalc->BtnPlusClick(0);
   str1 = Word_lib_ua[13].SubString(Word_lib_ua[13].LastDelimiter(' ') + 1, Word_lib_ua[13].Length());
   if(curwrd == str1)
      FCalc->BtnMinClick(0);
   str1 = Word_lib_ua[14].SubString(Word_lib_ua[14].LastDelimiter(' ') + 1, Word_lib_ua[14].Length());
   if(curwrd == str1)
      FCalc->BtnMulClick(0);
   str1 = Word_lib_ua[15].SubString(Word_lib_ua[15].LastDelimiter(' ') + 1, Word_lib_ua[15].Length());
   if(curwrd == str1)
      FCalc->BtnDivClick(0);
   str1 = Word_lib_ua[16].SubString(Word_lib_ua[16].LastDelimiter(' ') + 1, Word_lib_ua[16].Length());
   if(curwrd == str1)
      FCalc->BtnPMClick(0);
   str1 = Word_lib_ua[17].SubString(Word_lib_ua[17].LastDelimiter(' ') + 1, Word_lib_ua[17].Length());
   if(curwrd == str1)
      FCalc->BtnBcspClick(0);
   str1 = Word_lib_ua[18].SubString(Word_lib_ua[18].LastDelimiter(' ') + 1, Word_lib_ua[18].Length());
   if(curwrd == str1)
      FCalc->BtnComaClick(0);
   str1 = Word_lib_ua[19].SubString(Word_lib_ua[19].LastDelimiter(' ') + 1, Word_lib_ua[19].Length());
   if(curwrd == str1)
      FCalc->BtnCClick(0);

   for(int i=0; i<wrd_length; i++)
   {
      str1 = Word_lib_ua[i].SubString(Word_lib_ua[i].LastDelimiter(' ') + 1, Word_lib_ua[i].Length());
      if(curwrd == str1 && SpeedButton2->Down)
         Variant::CreateObject("SAPI.SpVoice").OleFunction("Speak", Word_lib_eng[i].c_str());
   }
}

//---------------------------------------------------------------------------

void __fastcall TFCDPMain::FormCreate(TObject *Sender)
{
/*
   if(cdpparam.isFirstTime)
   {
      SpeedButton1->Enabled = false;
      SpeedButton2->Enabled = false;
   }
*/   
}

//---------------------------------------------------------------------

void __fastcall TFCDPMain::CalcExecute(TObject *Sender)
{
   if(FCalc->Visible) return;

   FCalc->Show();

   PBTimeSpectr->Visible = true;
   PBWave->Visible = true;
   FormResize(0);

   delete recorder;
   recorder = NULL;

   First_Noise_Sec = true;

   recorder = new TWaveRecorder(cdpparam.fftN, cdpparam.MSampleRate,
           	WaveInProc,(DWORD)this, true);
}

//---------------------------------------------------------------------------

void __fastcall TFCDPMain::FormClose(TObject *Sender, TCloseAction &Action)
{
   if(FCalc) delete FCalc;
}
//---------------------------------------------------------------------------


void __fastcall TFCDPMain::TrayIcon1Minimize(TObject *Sender)
{
   if(FCDPAuthorization)
   {
      if(FCDPAuthorization->Visible)
      {
         FCDPAuthorization->Close();
         Application->Terminate();
         if(FCalc) delete FCalc;
         return;
      }
   }
   if(FCalc->Visible)
      ShowWindow(FCalc->Handle, SW_RESTORE);
   IsMinimize = true;
}
//---------------------------------------------------------------------------

void __fastcall TFCDPMain::TrayIcon1Restore(TObject *Sender)
{
   if(!FCalc->Visible)
      ShowWindow(FCalc->Handle, SW_HIDE);
   IsMinimize = false;      
}
//---------------------------------------------------------------------------

void __fastcall TFCDPMain::EducatExecute(TObject *Sender)
{
   FCalc->Close();
   CDPEducation = new TCDPEducation(this);
   CDPEducation->ShowModal();
}
//---------------------------------------------------------------------------


// Зміна разміру вікна
void __fastcall TFCDPMain::FormResize(TObject *Sender)
{
    // Розгортка
    // Буфер
    tsbuf->Width = PBTimeSpectr->ClientWidth;
    tsbuf->Height = PBTimeSpectr->ClientHeight;
    tsbuf->Canvas->Brush->Color = clBlack;
    tsbuf->Canvas->FillRect(tsbuf->Canvas->ClipRect);
    // Waveform
    // Фон
    wbcgr->Width = PBWave->ClientWidth;
    wbcgr->Height = PBWave->ClientHeight;
    // Буфер
    wbuf->Width = PBWave->ClientWidth;
    wbuf->Height = PBWave->ClientHeight;

    UpdateBcgr();
    PBTimeSpectr->Invalidate();
    PBWave->Invalidate();
    WaveUpdate();
}
//---------------------------------------------------------------------------

// Фон
void __fastcall TFCDPMain::UpdateBcgr()
{
    //int x,y,dx,dy,h;
    int width, height;
    TCanvas *c;

    // Waveform
    // Фон
    c = wbcgr->Canvas;
    width = wbcgr->Width;
    height = wbcgr->Height;
    c->Pen->Color = clGreen;
    c->Brush->Color = clBlack;
    c->FillRect(c->ClipRect);
    c->MoveTo(0,height/2);
    c->LineTo(width,height/2);
    // Буфер
    wbuf->Canvas->Draw(0,0,wbcgr);
}
//----------------------------------------------------------------------

// Малюєм спектр
void __fastcall TFCDPMain::WaveUpdate()
{
    int x,y,width,height;
    int i,j,imin,imax,bandWidth;
    TCanvas *c;
    TColor color;

    if(!recorder) return;

    c = wbuf->Canvas;
    width = wbuf->Width;
    height = wbuf->Height;

    c->Draw(0,0,wbcgr);

    c->MoveTo(0,height/2);
    c->Pen->Color = clLime;
    
    for(x=0;x<width;++x)
    {
    	c->LineTo(x,fft->ReData[x*cdpparam.fftN/width] *
        height / (cdpparam.MAX_AMPL*2) + height/2);
    }

    fft->ApplySmoothWindow(stHamming);
    fft->GetE();
    imin = cdpparam.minfreq*cdpparam.fftN/cdpparam.MSampleRate;
    imax = cdpparam.maxfreq*cdpparam.fftN/cdpparam.MSampleRate;

    width = tsbuf->Width;
    height = tsbuf->Height;
    bandWidth = width /(imax-imin)+1;
    tsbuf->Canvas->Draw(0,1,tsbuf);

    for(i = imin; i < imax; ++i)
    {
           y = height*ToLog((float)fft->E[i]*10/cdpparam.MAX_AMPL);
           //y = fft->E[i]*height*10/cdpparam.MAX_AMPL;
           x=(i-imin)*width/(imax-imin);

           color = ToRGB(min(255,255*y/height));
           for(j=0;j<bandWidth;++j)
	      tsbuf->Canvas->Pixels[x+j][0] = color;
    }

    PBTimeSpectr->Canvas->Draw(0,0,tsbuf);
    PBWave->Canvas->Draw(0,0,wbuf);
}
//---------------------------------------------------------------------------

void __fastcall TFCDPMain::PBPaintBoth(TObject *Sender)
{
   PBTimeSpectr->Canvas->Draw(0,0,tsbuf);
   PBWave->Canvas->Draw(0,0,wbuf);
}
//---------------------------------------------------------------------------

void __fastcall TFCDPMain::Timer1Timer(TObject *Sender)
{
   FCalc->Close();
   Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

