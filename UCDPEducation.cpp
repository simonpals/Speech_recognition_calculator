//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UCDPEducation.h"
#include "UCDPParam.h"
#include "UCDPRecognize.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TCDPEducation *CDPEducation;
//---------------------------------------------------------------------------
__fastcall TCDPEducation::TCDPEducation(TComponent* Owner)
        : TForm(Owner)
{
   CSEU->Text = IntToStr(cdpparam.curr_realization_U);
   TBerr->Min = cdpparam.min_err_;
   TBerr->Max = cdpparam.max_err_;
   TBerr->Position = cdpparam.err_;
   Label2->Caption = cdpparam.max_err_;
   Label1->Caption = cdpparam.min_err_;

   CBTokenSens->Checked = cdpparam.TokenSens;

   if(cdpparam.alg_ == DURBOOL) RBbinarr->Checked = true;
   if(cdpparam.alg_ == DURCOEF) RBkdurb->Checked = true;
   if(cdpparam.alg_ == TFFTM) RBfftA->Checked = true;

   if(cdpparam.min_energy == cdpparam.MAX_AMPL * cdpparam.mnognyk) RBroomnoise->Checked = true;
   if(cdpparam.min_energy == floor( (cdpparam.MAX_AMPL * cdpparam.mnognyk)*1.2 ) ) RBstreetnoise->Checked = true;
   if(cdpparam.min_energy == floor( (cdpparam.MAX_AMPL * cdpparam.mnognyk)*1.4 ) ) RBmachinenoise->Checked = true;

   if(!cdpparam.isFirstTime)
   {
      GroupBox1->Enabled = false;
      GroupBox3->Enabled = false;
      //GroupBox4->Enabled = false;
      GroupBox5->Enabled = false;      
   }

   counter = 1;
   obcgr = new Graphics::TBitmap();
   obuf = new Graphics::TBitmap();
   wavebuf = new Graphics::TBitmap();
   records = NULL;
   First_Sec_Disable = 0;
   memset(srec_buf.arr, 0, sizeof(short)*cdpparam.fftN);

   /*
   all_realization = new short *[cdpparam.curr_realization_U];
   for(int i=0; i<cdpparam.curr_realization_U; i++)
      all_realization[i] = NULL;
   all_ogyn = new double *[cdpparam.curr_realization_U];
   for(int i=0; i<cdpparam.curr_realization_U; i++)
      all_ogyn[i] = NULL;

   len_realiz = new int [cdpparam.curr_realization_U];
   memset(len_realiz,0,sizeof(int)*cdpparam.curr_realization_U);
   len_ogyn = new int [cdpparam.curr_realization_U];
   memset(len_ogyn,0,sizeof(int)*cdpparam.curr_realization_U);
   */

   wave=0;
   if( !cdpparam.isFirstTime ) counter = wrd_length+1;
   CanCollapse = false;
}

//---------------------------------------------------------------------------
__fastcall TCDPEducation::~TCDPEducation()
{
   if(all_realization)
   for(int i=0; i<cdpparam.curr_realization_U; i++)
      if(all_realization[i] != NULL)
         delete [] all_realization[i];
   delete [] all_realization;
   all_realization = NULL;

   if(all_ogyn)
   for(int i=0; i<cdpparam.curr_realization_U; i++)
      if(all_ogyn[i] != NULL)
         delete [] all_ogyn[i];
   delete [] all_ogyn;
   all_ogyn = NULL;

   delete [] len_realiz; len_realiz = NULL;
   delete [] len_ogyn; len_ogyn = NULL;

   delete wavebuf;
   delete obcgr;
   delete obuf;
   delete records;
   vsig.clear();
   delete wave;
   wave=0;
}
//---------------------------------------------------------------------------
void __fastcall TCDPEducation::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   Action = caFree;
}
//---------------------------------------------------------------------------

// ??????? ?????? WaveRecorder
void WaveProc(DWORD pinstance,DWORD pbufer)
{
   ((TCDPEducation*)pinstance)->ProcesBuffer((short*)pbufer);
}

void __fastcall TCDPEducation::ProcesBuffer(short *buf)
{
   int ret_code=0;
   if(!records)
      return;

   if(First_Sec_Disable == floor(cdpparam.MSampleRate/(double)cdpparam.fftN) && First_Noise_Sec)
   {
      First_Noise_Sec = false;
      First_Sec_Disable = 0;
      Timer1->Enabled = true;
   }
   if(First_Noise_Sec)
   {
      First_Sec_Disable++;
      return;
   }

   copy(buf, buf+cdpparam.fftN, srec_buf.arr);
   vsig.push_back(srec_buf);
   if( vsig.size() > cdpparam.MAX_VECTOR_SIZE )
   {
   //   vsig.clear();
   }

   ret_code = CDPSelectWord(vsig);
   if(ret_code == 1 && vsig.size() < cdpparam.max_vec_size)
   {
      int ind=0;
      if(!TVWordList->Selected->HasChildren)
        ind = TVWordList->Selected->Parent->Index;
      else ind = TVWordList->Selected->Index;
      if(ind < 0) ind = 0;
      if(vsig.size()*cdpparam.fftN <= TemporalTranskr[ind]*cdpparam.M_)
      {
         vsig.clear();
         return;
      }

      int cur_ind = StrToInt(TVWordList->Selected->Text.SubString(String("?????????? ?").Length()+1, 1)) - 1;
      if(cur_ind < 0 && cur_ind >= cdpparam.curr_realization_U) Timer2Timer(0);
      if(all_realization[cur_ind])
      {
         delete [] all_realization[cur_ind];
         all_realization[cur_ind] = NULL;
      }
      len_realiz[cur_ind] = vsig.size()*cdpparam.fftN;
      all_realization[cur_ind] = new short [vsig.size()*cdpparam.fftN];
      memset(all_realization[cur_ind],0,sizeof(short)*vsig.size()*cdpparam.fftN);

      double *mas = new double [vsig.size() * cdpparam.fftN];
      memset(mas, 0, sizeof(double) * vsig.size() * cdpparam.fftN);
      for(int i=0; i<vsig.size(); i++)
      {
         for(int j=0; j<cdpparam.fftN; j++)
         {
            all_realization[cur_ind][i*cdpparam.fftN + j] = vsig[i].arr[j];
            mas[i*cdpparam.fftN + j] = vsig[i].arr[j];
         }
      }

      Sabs(mas,vsig.size()*cdpparam.fftN);
      Low50Hz(mas,vsig.size()*cdpparam.fftN,cdpparam.MSampleRate);
      int dt_dec = vsig.size()*cdpparam.fftN / ogyn_cnt;
      int cntm = Decimator(mas,vsig.size()*cdpparam.fftN,cdpparam.MSampleRate,dt_dec);

      if(all_ogyn[cur_ind])
      {
         delete [] all_ogyn[cur_ind];
         all_ogyn[cur_ind] = NULL;
      }
      all_ogyn[cur_ind] = new double [cntm];
      memset(all_ogyn[cur_ind],0,sizeof(double)*cntm);
      len_ogyn[cur_ind] = cntm;

      //ofstream ffff("koma.txt"); ///
      for(int j=0; j<cntm; j++)
      {
         all_ogyn[cur_ind][j] = mas[j];
         //ffff<<mas[j]<<',';  ///
      }
      //ffff.close(); ///

      vsig.clear();
      delete [] mas;
      Timer2->Interval = 20;
   }
   else if(ret_code == 1 && vsig.size() >= cdpparam.max_vec_size)
   {
      vsig.clear();
   }
}

//---------------------------------------------------------------------------


void __fastcall TCDPEducation::CSEUKeyPress(TObject *Sender, char &Key)
{
   String st = "123456789";
   if( st.LastDelimiter(Key) <= 0 ) Key = (char)0;
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::CSEUChange(TObject *Sender)
{
   if( StrToInt(CSEU->Text) < cdpparam.min_realization_U ) CSEU->Text = IntToStr( cdpparam.min_realization_U );
   if( StrToInt(CSEU->Text) > cdpparam.max_realization_U ) CSEU->Text = IntToStr( cdpparam.max_realization_U );
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::TBerrChange(TObject *Sender)
{
   Label3->Caption = TBerr->Position;
   Label3->Caption = Label3->Caption + String("\%");        
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::BtnContClick(TObject *Sender)
{
   cdpparam.curr_realization_U = StrToInt( CSEU->Text );
   cdpparam.err_ = TBerr->Position;
   cdpparam.TokenSens = CBTokenSens->Checked;

   if( RBbinarr->Checked )
   {
      cdpparam.alg_ = DURBOOL;
      cdpparam.kind_ = DIMHEMIN;
   }
   else if( RBkdurb->Checked )
   {
      cdpparam.alg_ = DURCOEF;
      cdpparam.kind_ = DIMEUCLID;
   }
   else if( RBfftA->Checked )
   {
      cdpparam.alg_ = TFFTM;
      cdpparam.kind_ = DIMEUCLID;
   }

   if( RBroomnoise->Checked )
      cdpparam.min_energy = cdpparam.MAX_AMPL * cdpparam.mnognyk;
   else if( RBstreetnoise->Checked )
      cdpparam.min_energy = (cdpparam.MAX_AMPL * cdpparam.mnognyk) * 1.2;
   else if( RBmachinenoise->Checked )
      cdpparam.min_energy = (cdpparam.MAX_AMPL * cdpparam.mnognyk) * 1.4;

   GBEdParam->Visible = false;
   GBEdParam->Enabled = false;

   GBEducRecog->Visible = true;
   GBEducRecog->Enabled = true;

   TCDPRefreshWordBuf(2);
   TCDPEducationMain();
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::TCDPEducationMain()
{
   TCDPUserInfo();
   TCDPDrawOgyn();
}

void __fastcall TCDPEducation::TCDPUserInfo()
{
   REUserInform->Clear();
   REUserInform->Paragraph->Alignment = taCenter;
   REUserInform->SelAttributes->Style =
   REUserInform->SelAttributes->Style << fsBold;
   REUserInform->Lines->Add("?????????????:");
   REUserInform->SelAttributes->Assign(REUserInform->DefAttributes);
   REUserInform->Paragraph->Alignment = taLeftJustify;
   REUserInform->Paragraph->Numbering = nsBullet;
   REUserInform->Lines->Add(String("   ??????? ?? ???????????? ????? ?????????."
   " ????? ?? ???????????? ? ??????????? ????? ??????? ????????."));
   REUserInform->Lines->Add(String("   ?????, ?? ?? ?????? ????? ? ?????????? ?????????,"
   " ????? ?? ??????? ???????? ?????????????. ?? ??????????? ??????????? ? ????? ?????."));
   REUserInform->Lines->Add(String("   ??????? ???????? ?????? ?????? - ???????? "
   " ??????? ????????? ???????? ??? ????????? ????????????? ?? ????????? ????. ??????, ??? ???????? ???????? "
     "?????? ??? ????, ?? ??????????? ? ??????, ??????? ??????????? ?? ?? " + IntToStr(cdpparam.curr_realization_U) + " ???. "
     "???????? ???????????? ??? ??????? ?????? ???????????."));
   REUserInform->Lines->Add(String("   ???? ?? ?????? ??? ?????? ? ??? ??????, ???????? ?????? ????????? "
   " ?????????? ??? ???? ???? ?? ????????? ?????? ?????? (???. ?????). ? ?????? ??????? ? ??????????"
    " ??????????? ??????? ?? ?????, ?? ?????? ?????????????. ??? ????? ??????? ?? ????? ? ?????? ? ??????? ???????: ????????."));
   REUserInform->Lines->Add(String("   ??? ???????? ?????? ???????? ??????? ???? ??????: ????????, ? ????? ?????? ??????: ????."
    " ??? ?????, ?? ???????????, ????? ???????????? ?????, ?????? ???????? ???? ? ?????? ?, ???????? ??????????: ????????."));
   REUserInform->Lines->Add(String("   ???? ?? ?????????: ???????? - ??????? ???????? ? ???????? ???? ? ??????, ?? ?????, ????? [?????]."
    " ????? ?????????? ?????, ????????? ????? ? ????????? ??? 0.3 ?? 4 ??????. ??????, ??? ?? ? ???? ???????????"
    " ????????? ???? ?????? ?????."));
   REUserInform->Lines->Add(String("   ??? ????????? ???????? ????? ????????????? ????? ????????: ?????????? ???????."
    " ???? ????? ????? ????? - ???????? ???? ????????????. ??? ?? ?????????? ??? ????????? ???????? ??????"
    " ??????????? ????? " +FloatToStr(cdpparam.wait_for_threads/(double)1000)+ " ??????."));
   REUserInform->Lines->Add(String("   ????? ?????? ???? ???? ?? ?? ??????????, ?? ?????? ???????? ?????????, ??????: ????????."));
   REUserInform->Lines->Add(String("   ???????? ??????? ??????????? ?????? ?? ??????? ????????. ??? ????? ???????? "
    "????????????? ????."));
   REUserInform->Paragraph->Numbering = nsNone;

   TVWordList->Items->Clear();
   TVWordList->Items->Add(NULL,Word_lib_ua[0]);

      for(int j=0; j<cdpparam.curr_realization_U; j++)
         TVWordList->Items->AddChild(TVWordList->Items->Item[0],
            String("?????????? ?"+IntToStr(j+1)+" ?????: " + Word_lib_ua[0]) );

   int len = sizeof(Word_lib_ua)/sizeof(*Word_lib_ua);

   for(int i=1; i<len; i++)
   {
      TVWordList->Items->Add(TVWordList->Items->Item[0], Word_lib_ua[i]);
      for(int j=0; j<cdpparam.curr_realization_U; j++)
         TVWordList->Items->AddChild(TVWordList->Items->Item[(cdpparam.curr_realization_U+1)*i],
            String("?????????? ?"+IntToStr(j+1)+" ?????: " + Word_lib_ua[i]) );
   }

   TVWordList->Items->Item[0]->Expand(true);
   TVWordList->Items->Item[0]->Selected = true;
}

//---------------------------------------------------------------------------
void __fastcall TCDPEducation::TVWordListExpanding(TObject *Sender,
      TTreeNode *Node, bool &AllowExpansion)
{
   if(counter < (Node->Index+1))
   {
      AllowExpansion = false;
      return;
   }

   for(int i=0; i<cdpparam.curr_realization_U; i++)
      if(len_realiz[i]>0)
      {

         if( Application->MessageBox(AnsiString("???? ?? ????????? ?? ?????, ?? ?????? (???????) ?"
         " ??????????? ????? ?????? ???????. ?????????").c_str(), "???????? ?? ???????????",
         MB_YESNO+MB_ICONWARNING) ==  IDNO)
         {
            AllowExpansion = false;
            return;
         }
         else
         {
            TCDPRefreshWordBuf(0);
            if(BNextWrd1->Enabled)   BNextWrd1->Enabled = false;
            TCDPDrawOgyn();
            TCDPDrawWave();
            break;
         }
      }

   if(Node->HasChildren)
   {
      CanCollapse = true;
      for(int i=0; i<TVWordList->Items->Count; i++)
         if(i != Node->Index && TVWordList->Items->Item[i]->HasChildren)
            TVWordList->Items->Item[i]->Collapse(true);
   }

   CanCollapse = false;
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::TCDPDrawOgyn()
{
    TCanvas *c;
    int width, height, x;
    int ind=0;

    if(!TVWordList->Selected->HasChildren)
       ind = TVWordList->Selected->Parent->Index;
    else ind = TVWordList->Selected->Index;

    if(ind < 0) ind = 0;
    int len = sizeof(wrd_array[ind])/sizeof(wrd_array[ind][0]);
    double *mas = new double [len];
    memset(mas, 0, sizeof(double) * len);

    obcgr->Width = PBOgynEtal->ClientWidth;
    obcgr->Height = PBOgynEtal->ClientHeight;
    obuf->Width = PBOgynEtal->ClientWidth;
    obuf->Height = PBOgynEtal->ClientHeight;

    c = obcgr->Canvas;
    width = obcgr->Width;
    height = obcgr->Height;
    c->Pen->Color =  clBlack;
    c->Brush->Color = clMoneyGreen;
    c->FillRect(c->ClipRect);
    c->Rectangle(c->ClipRect.left,
    c->ClipRect.Bottom,
    c->ClipRect.right,
    c->ClipRect.Top);
    obuf->Canvas->Draw(0,0,obcgr);

   for(x=0;x<len;++x)
   {
      mas[x] = wrd_array[ind][x];
   }

   c->MoveTo(0,height);
   for(x=0;x<len;++x)
   {
      c->LineTo(x*width/len, height - mas[x] * height / cdpparam.MAX_AMPL * 2);
   }

   c = obuf->Canvas;
   width = obuf->Width;
   height = obuf->Height;
   c->Pen->Color = clBlack;
   c->Brush->Color = clMoneyGreen;

   if(all_ogyn[cur_ind] && cur_ind > -1)
   {
      for(x=0;x<len_ogyn[cur_ind];++x)
      {
         mas[x] = all_ogyn[cur_ind][x];
      }
      c->MoveTo(0,height);

      for(x=0;x<len;++x)
      {
         c->LineTo(x*width/len, height - mas[x] * height / cdpparam.MAX_AMPL * 2);
      }
   }

   delete [] mas;
   PBOgynEtal->Canvas->Draw(0,0,obcgr);
   PBOgynUser->Canvas->Draw(0,0,obuf);
}

void __fastcall TCDPEducation::PBOgynPaint(TObject *Sender)
{
   PBOgynEtal->Canvas->Draw(0,0,obcgr);
   PBOgynUser->Canvas->Draw(0,0,obuf);
   PBWaveForm->Canvas->Draw(0,0,wavebuf);   
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::TVWordListChange(TObject *Sender,
      TTreeNode *Node)
{
   String str1 = Node->Text;
   str1 = str1.SetLength(String("??????????").Length());
   if(str1 == "??????????")
   {
      BRecord->Enabled = true;
      cur_ind = StrToInt(TVWordList->Selected->Text.SubString(String("?????????? ?").Length()+1, 1)) - 1;
      if(cur_ind < 0 && cur_ind >= cdpparam.curr_realization_U)
      {
         cur_ind = -1;
         return;
      }
      if(len_realiz[cur_ind]>0)
      {
         LInform1->Visible = true;
         LInform2->Visible = true;
         LReplay1->Visible = true;
         LPlay1->Visible = true;
         BPlay->Enabled = true;

      }
      else
      {
         LInform1->Visible = false;
         LInform2->Visible = false;
         LReplay1->Visible = false;
         LPlay1->Visible = false;
         BPlay->Enabled = false;
      }
   }
   else
   {
      LInform1->Visible = false;
      LInform2->Visible = false;
      BRecord->Enabled = false;
      LReplay1->Visible = false;
      LPlay1->Visible = false;
      BPlay->Enabled = false;
   }

   TCDPDrawOgyn();
   TCDPDrawWave();
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::BRecordClick(TObject *Sender)
{
   int cnt1=0, ndi=0;
   for(int i=0; i<TVWordList->Items->Count; i++)
      if(TVWordList->Items->Item[i]->Selected)
      {
         ndi = i;
         cnt1++;
      }

   if(cnt1 != 1 || cnt1==1 && TVWordList->Items->Item[ndi]->HasChildren)
   {
      ShowMessage("???????? ?????? ???? ??????????");
      return;
   }

   if(FileExists(cdpparam.CDPUserPath + "\\" + ExtendToFile + cdpparam.dbCDPext))
   {
      String StrTV = TVWordList->Items->Item[ndi]->Text.SubString(TVWordList->Items->Item[ndi]->Text.LastDelimiter(' ') + 1,
      TVWordList->Items->Item[ndi]->Text.Length() - TVWordList->Items->Item[ndi]->Text.LastDelimiter(' ') );
      if(cdprecognize.CheckSameWord(cdpparam.CDPUserPath + "\\" + ExtendToFile + cdpparam.dbCDPext, StrTV))
      {
         ShowMessage("?? ????? ??? ????????????? ? ????? ?????. ??? ??????????? ???? ?? ??? - ???????? ?????????.");
         return;
      }
   }

   BRecord->Enabled = false;
   First_Noise_Sec = true;

   records = new TWaveRecorder(cdpparam.fftN, cdpparam.MSampleRate,
           	WaveProc,(DWORD)this, true);

   Timer2->Interval = cdpparam.msTimeToEndRec;
   Timer2->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::SBsaveEducClick(TObject *Sender)
{
   //FCDPMain->SpeedButton1->Enabled = true;
   //FCDPMain->SpeedButton2->Enabled = true;
   SBsaveEduc->Enabled = false;
   if(cdpparam.isFirstTime)
   {
      cdpparam.isFirstTime = false;
      //Close();
   }

   if(FileExists(cdpparam.CDPUserPath + "\\" + ExtendToFile + cdpparam.dbCDPext))
   {
      EnterNewEtalon(cdpparam.CDPUserPath + "\\" + ExtendToFile + cdpparam.dbCDPext, cdpparam.CDPUserPath + "\\" + FCDPMain->CDPUserName + cdpparam.dbCDPext);
      DeleteFile(cdpparam.CDPUserPath + "\\" + FCDPMain->CDPUserName + cdpparam.dbCDPext);
      RenameFile(cdpparam.CDPUserPath + "\\" + ExtendToFile + cdpparam.dbCDPext, cdpparam.CDPUserPath + "\\" + FCDPMain->CDPUserName + cdpparam.dbCDPext);
   }

}

//---------------------------------------------------------------------------

void __fastcall TCDPEducation::EnterNewEtalon(String somewrd, String allwrd)
{
   int i, fqk=0, j, s, u;
   String str1 = "", nextb = "";
   char nextch [80] = {0};
   types *ek = NULL;
   int *tau = NULL;

   vector <String> vst;

   ofstream out(somewrd.c_str(), ios_base::app);

   ifstream fin(allwrd.c_str());

   if( out == NULL || fin == NULL)
      throw Exception("??????? ??? ?????? ? ??????");

   for(i=0; i<wrd_length; i++)
   {
      if(cdprecognize.CheckSameWord(somewrd, Word_lib_ua[i].SubString(Word_lib_ua[i].LastDelimiter(' ')+1, Word_lib_ua[i].Length() - Word_lib_ua[i].LastDelimiter(' ') )   ))
         vst.push_back(Word_lib_ua[i].SubString(Word_lib_ua[i].LastDelimiter(' ')+1, Word_lib_ua[i].Length() - Word_lib_ua[i].LastDelimiter(' ') ));
   }

   for(i=0; i<5; i++)
      fin>>nextch;

   for(i=0; i<wrd_length; i++)
   {
      fin>>nextch;
      nextb = nextch;
      fin>>fqk;
      ek = new types [fqk*cdpparam.m_+1];
      memset(ek,0,sizeof(types)*(fqk*cdpparam.m_+1));
      tau = new int [fqk*2+1];
      memset(tau,0,sizeof(int)*(fqk*2+1));

         for(s=0; s<fqk; s++)
         {
            fin>>tau[s*2];
            fin>>tau[s*2+1];
         }
         for(u=0; u<fqk*cdpparam.m_; u++)
         {
            fin>>ek[u];
         }

      for(j=0; j<vst.size(); j++)
         if( nextb == vst[j] )
            break;

      if(j == vst.size())
      {
         out<<nextb.c_str()<<'\t';
         out<<fqk<<'\t';
         out<<'\n';
         for(s=0; s<fqk; s++)
         {
            out<<tau[s*2]<<'\t';
            out<<tau[s*2+1]<<'\n';
         }
         out<<'\n';
         for(u=0; u<fqk*cdpparam.m_; u++)
         {
            out<<ek[u]<<'\t';
         }
         out<<'\n';
      }

      delete [] ek; ek = NULL;
      delete [] tau; tau = NULL;
   }

   fin.close();
   out.close();
}

//---------------------------------------------------------------------------

void __fastcall TCDPEducation::Timer1Timer(TObject *Sender)
{
   Recording->Visible=!Recording->Visible;

}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::Timer2Timer(TObject *Sender)
{
   Timer1->Enabled = false;
   delete records;
   records = NULL;
   BRecord->Enabled = true;
   Timer2->Enabled = false;
   BRecord->Enabled = true;
   REUserInform->Visible = false;
   Recording->Visible = false;
   LReplay1->Visible = true;
   int i;
   for(i=0;i<cdpparam.curr_realization_U;i++)
      if(len_realiz[i] == 0) break;
   if(i == cdpparam.curr_realization_U)
      BNextWrd1->Enabled = true;
   TVWordListChange(0,TVWordList->Selected);
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::TCDPDrawWave()
{
    TCanvas *c;
    int width, height, x;
    int ind=0;

    wavebuf->Width = PBWaveForm->ClientWidth;
    wavebuf->Height = PBWaveForm->ClientHeight;

    c = wavebuf->Canvas;
    width = wavebuf->Width;
    height = wavebuf->Height;
    c->Pen->Color =  clBlack;
    c->Brush->Color = clMoneyGreen;
    c->FillRect(c->ClipRect);
    c->Rectangle(c->ClipRect.left,
    c->ClipRect.Bottom,
    c->ClipRect.right,
    c->ClipRect.Top);    

    if(all_realization[cur_ind] && cur_ind > -1)
    {
       c->MoveTo(0,height);
       for(x=0;x<len_realiz[cur_ind];++x)
       {
          c->LineTo(x*width/(double)len_realiz[cur_ind], all_realization[cur_ind][x] *
          height / (double) (cdpparam.MAX_AMPL * 2.5) + height/2.);
       }
    }

    PBWaveForm->Canvas->Draw(0,0,wavebuf);
}

//---------------------------------------------------------------------------
void __fastcall TCDPEducation::BPlayClick(TObject *Sender)
{
      BPlay->Enabled = false;
      if(all_realization[cur_ind] && cur_ind > -1)
      {
         wave = new TWave("buf.wav",1,16,cdpparam.MSampleRate);
         wave->WriteData(all_realization[cur_ind],len_realiz[cur_ind]);
         sndPlaySound("buf.wav",SND_ASYNC);
         delete wave;
         wave = 0;
         DeleteFile("buf.wav");
      }
      BPlay->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TCDPEducation::TVWordListCollapsing(TObject *Sender,
      TTreeNode *Node, bool &AllowCollapse)
{
   AllowCollapse = CanCollapse;
}
//---------------------------------------------------------------------------


void __fastcall TCDPEducation::BNextWrd1Click(TObject *Sender)
{
    BNextWrd1->Enabled = false;

    int ind=0;
    if(!TVWordList->Selected->HasChildren)
       ind = TVWordList->Selected->Parent->Index;
    else ind = TVWordList->Selected->Index;
    if(ind < 0) ind = 0;

    LInform1->Visible = false;
    LInform2->Visible = false;    
    PBIndicateEduc->Visible = true;
    PBIndicateEduc->Position = 0;

    double *masw = NULL;
    String path = Word_lib_ua[ind].SubString(Word_lib_ua[ind].LastDelimiter(' ')+1,
    Word_lib_ua[ind].Length() - Word_lib_ua[ind].LastDelimiter(' '));
    //Delete(1, Word_lib_ua[ind].LastDelimiter(' ') );
    String answers = "";

    for(int i=0; i<cdpparam.curr_realization_U; i++)
    {
       masw = new double [len_realiz[i]];
       memset(masw,0,sizeof(double)*len_realiz[i]);
       copy(all_realization[i],all_realization[i]+len_realiz[i],masw);

       answers = cdprecognize.EducationSignal(masw, len_realiz[i],
       cdpparam.curr_realization_U, i, FCDPMain->CDPUserName, path, TemporalTranskr[ind]);

       delete [] masw; masw = NULL;
    }

    TCDPRefreshWordBuf(0);
    counter++;
    TCDPDrawOgyn();
    TCDPDrawWave();
    BRecord->Enabled = false;
    BPlay->Enabled = false;
    LReplay1->Visible = false;
    LPlay1->Visible = false;
    PBIndicateEduc->Visible = false;

   if(counter == wrd_length+1 || counter > wrd_length+1 && FileExists(cdpparam.CDPUserPath + "\\" + ExtendToFile + cdpparam.dbCDPext))
      SBsaveEduc->Enabled = true;

   ind++;
   if(ind == wrd_length) return;
   TVWordList->Items->Item[ind*cdpparam.curr_realization_U + ind]->Text;
   TVWordListExpanding(0, TVWordList->Items->Item[ind*cdpparam.curr_realization_U + ind], true);
   TVWordList->Items->Item[ind*cdpparam.curr_realization_U + ind]->Expanded = true;
}
//---------------------------------------------------------------------------


void __fastcall TCDPEducation::TVWordListChanging(TObject *Sender,
      TTreeNode *Node, bool &AllowChange)
{
   if( Node->Expanded || Node->Parent && Node->Parent->Expanded ) AllowChange = true;
   else AllowChange = false;
}
//---------------------------------------------------------------------------

void __fastcall TCDPEducation::TCDPRefreshWordBuf(int par)
{
         if(par == 1 || par == 0)
         {
            for(int i=0; i<cdpparam.curr_realization_U; i++)
               if(all_realization[i] != NULL)
                  delete [] all_realization[i];
            delete [] all_realization;
            all_realization = NULL;

            for(int i=0; i<cdpparam.curr_realization_U; i++)
               if(all_ogyn[i] != NULL)
                  delete [] all_ogyn[i];
            delete [] all_ogyn;
            all_ogyn = NULL;

            delete [] len_realiz; len_realiz = NULL;
            delete [] len_ogyn; len_ogyn = NULL;
        }
        if(par == 2 || par == 0)
        {
            all_realization = new short *[cdpparam.curr_realization_U];
            for(int i=0; i<cdpparam.curr_realization_U; i++)
            all_realization[i] = NULL;
            all_ogyn = new double *[cdpparam.curr_realization_U];
            for(int i=0; i<cdpparam.curr_realization_U; i++)
            all_ogyn[i] = NULL;

            len_realiz = new int [cdpparam.curr_realization_U];
            memset(len_realiz,0,sizeof(int)*cdpparam.curr_realization_U);
            len_ogyn = new int [cdpparam.curr_realization_U];
            memset(len_ogyn,0,sizeof(int)*cdpparam.curr_realization_U);
        }    
}

/*
void __fastcall TCDPEducation::OnProgressBarStemM(TMessage& Msg)
{
}
*/

void __fastcall TCDPEducation::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
   if(!all_realization) return;
   for(int i=0; i<cdpparam.curr_realization_U; i++)
      if(all_realization[i] || counter <= wrd_length && counter > 1 || SBsaveEduc->Enabled)
      {
         String pth = "???? ?? ?? ????????? ? ?????? ????????. ";
         if(cdpparam.isFirstTime) pth = pth + "??? ?????????? ???????? ???????? ????????. ";
         pth = pth + " ??????? ?????? ";
         if( Application->MessageBox( pth.c_str(), "???? ?? ?????????",
         MB_YESNO+MB_ICONWARNING) ==  IDNO)
         {
            CanClose = false;
         }
         else
         {
            pth = cdpparam.CDPUserPath+"\\"+FCDPMain->CDPUserName+cdpparam.dbCDPext;
            if( cdpparam.isFirstTime && FileExists(pth) )
               DeleteFile(pth);
            pth = cdpparam.CDPUserPath + "\\" + ExtendToFile + cdpparam.dbCDPext;               
            if(FileExists(pth))
               DeleteFile(pth);            
         }
         break;
      }    
}
//---------------------------------------------------------------------------




