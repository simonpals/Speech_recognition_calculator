//---------------------------------------------------------------------------

#include <vcl.h>
#include <iostream.h>
#include <windows.h>
#include <fstream.h>
#pragma hdrstop

#include "UCDPAuthorization.h"
#include "UCDPParam.h"
#include "UCDPCalc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cdiroutl"
#pragma resource "*.dfm"
TFCDPAuthorization *FCDPAuthorization;
//---------------------------------------------------------------------------
__fastcall TFCDPAuthorization::TFCDPAuthorization(TComponent* Owner)
        : TForm(Owner)
{
   Reg1 = new TRegistry;
   Reg1->RootKey = HKEY_LOCAL_MACHINE;

   if( Reg1->KeyExists(cdpparam.RegisterPath) )
   {

      HANDLE hFindFile1;
      WIN32_FIND_DATA fd1;
      Reg1->OpenKey(cdpparam.RegisterPath,true);      
      String pathf1 = Reg1->ReadString("???? ???????? ????????"), bufs="";
      int iter = 0;

      hFindFile1 = FindFirstFile(String(pathf1+"\\*").c_str(), &fd1);
      if(hFindFile1 == INVALID_HANDLE_VALUE)
      {
         ShowMessage("??????? ??????? ?? ????????");
         return;
      }
      while(FindNextFile(hFindFile1, &fd1))
      {
         iter++;
         bufs = fd1.cFileName;
         if(bufs.Length() <= cdpparam.dbCDPext.Length()) continue;
         bufs = bufs.Delete(1, bufs.LastDelimiter('.')-1);
         if(bufs != cdpparam.dbCDPext) continue;
         bufs = fd1.cFileName;
         bufs = bufs.Delete(bufs.LastDelimiter('.'), bufs.Length());
         CBLoginUser->Items->Add(bufs);
      }
      FindClose(hFindFile1);
      CBLoginUser->ItemIndex = 0;

      if(iter > 1)
         CanMoveFile = true;
      else CanMoveFile = false;

      CDirectoryOutline1->Directory = pathf1;
      DriveComboBox1->Drive = CDirectoryOutline1->Directory.operator [](1);
   }
}
//---------------------------------------------------------------------------
void __fastcall TFCDPAuthorization::FormClose(TObject *Sender,
      TCloseAction &Action)
{
   delete Reg1;
   Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TFCDPAuthorization::BauthorCancClick(TObject *Sender)
{
   FCDPMain->CDPUserName = "";
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TFCDPAuthorization::DriveComboBox1Change(TObject *Sender)
{
   CDirectoryOutline1->Drive = DriveComboBox1->Drive;
}
//---------------------------------------------------------------------------
void __fastcall TFCDPAuthorization::BauthorOKClick(TObject *Sender)
{
   Reg1->OpenKey(cdpparam.RegisterPath,true);
   if(CanMoveFile && Reg1->ReadString("???? ???????? ????????") != NULL &&
      (Reg1->ReadString("???? ???????? ????????") != CDirectoryOutline1->Directory))
      {
         int coderet = Application->MessageBox(AnsiString("? ???????? ???????? "+ Reg1->ReadString("???? ???????? ????????") +
           " ?????????? ?????-???????? ???????. ????????? ?? ? ????? ????????").c_str(), "??????????? ??????? ??????",
           MB_YESNOCANCEL+MB_ICONWARNING);
         if(coderet ==  IDNO)
         {
            DeleteDir(Reg1->ReadString("???? ???????? ????????"));
            CanMoveFile = false;
            CBLoginUser->Clear();
            CBLoginUser->ItemIndex = -1;
         }
         if(coderet ==  IDYES)
         {
            CBLoginUser->ItemIndex = -1;
            TFCDPMoveFiles(String(Reg1->ReadString("???? ???????? ????????")+"\\*"),CDirectoryOutline1->Directory, false);
            DeleteDir(Reg1->ReadString("???? ???????? ????????"));
         }
         else
         {
            CBLoginUser->ItemIndex = -1;
            CanMoveFile = false;
         }
      }
   Reg1->WriteString("???? ???????? ????????",CDirectoryOutline1->Directory);

   if(CBLoginUser->Text.Length() == 0)
   {
      Beep();
      return;
   }

   FCDPMain->CDPUserName = CBLoginUser->Text;
   cdpparam.CDPUserPath = CDirectoryOutline1->Directory;

   if( CBLoginUser->ItemIndex == -1 )
      cdpparam.isFirstTime = true;
   else
   {
      cdpparam.isFirstTime = false;

         ifstream ifs1(String(cdpparam.CDPUserPath + "\\" + FCDPMain->CDPUserName + cdpparam.dbCDPext).c_str());
         if(ifs1)
         {
            ifs1>>cdpparam.curr_realization_U;
            int zm1=0;
            ifs1>>zm1;
            cdpparam.alg_ = zm1;
            ifs1>>zm1;
            cdpparam.kind_ = zm1;
            ifs1>>cdpparam.min_energy;
            ifs1>>cdpparam.err_;
            ifs1.close();
         }
   }
   
   Close();
}
//---------------------------------------------------------------------------
/*
void __fastcall TFCDPAuthorization::TFCDPDeleteDir(String wdir)
{
   HANDLE hFindFile = NULL;
   WIN32_FIND_DATA fd;
   String pathf = wdir;
   char szFullFileName[250];

   hFindFile = FindFirstFile(String(pathf+"\\*").c_str(), &fd);
   if(hFindFile == INVALID_HANDLE_VALUE)
   {
      ShowMessage("????????? ?????? ?? ?????????...");
      return;
   }
   while(FindNextFile(hFindFile, &fd))
   {
      if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
         sprintf(szFullFileName, String(pathf+"\\%s").c_str(), fd.cFileName);
         if(!DeleteFile(szFullFileName))
         {
            ShowMessage("????????? ?????? ?? ?????????...");
            return;
         }
      }
   }
   if(!FindClose(hFindFile))
   {
      ShowMessage("????????? ?????? ?? ?????????...");
      return;
   }
   if(!RemoveDirectory(pathf.c_str()))
   {
      ShowMessage("????????? ?????? ?? ?????????...");
      return;
   }
   FindClose(hFindFile);
   //CloseHandle(hFindFile);
}
*/

void __fastcall TFCDPAuthorization::DeleteDir(String DirName)
{
TSearchRec sr;
if (DirName.Length())
 {
 if (!FindFirst(DirName+"\\*.*",faAnyFile,sr))
 do
  {
  if (!(sr.Name=="." || sr.Name==".."))// ??? ??????? ?? ????
   if (((sr.Attr & faDirectory) == faDirectory ) ||
   (sr.Attr == faDirectory))// ??????? ?????
    {
    FileSetAttr(DirName+"\\"+sr.Name, faDirectory );// ????? ?????? read-only
    DeleteDir(DirName+"\\"+sr.Name);//?????????? ??????? ??????????
    RemoveDir(DirName + "\\"+sr.Name);// ??????? ?????? ??? ?????? ?????
    }
    else// ????? ?????? ????
    {
    FileSetAttr(DirName+"\\"+sr.Name, 0);// ????? ?????? read-only
    DeleteFile(DirName+"\\"+sr.Name);// ??????? ????
    }
  }
 while (!FindNext(sr));// ???? ?????, ???? ?? ?????? ???
 FindClose(sr);
 }
RemoveDir(DirName);
//return true;
}


//---------------------------------------------------------------------------
/*
void __fastcall TFCDPAuthorization::TFCDPMoveFiles(String wdir, String newdir)
{
   HANDLE hFindFile = NULL;
   WIN32_FIND_DATA fd;
   String pathf = wdir, bbb="";
   char szFullFileName[250];

   hFindFile = FindFirstFile(String(pathf+"\\*").c_str(), &fd);
   if(hFindFile == INVALID_HANDLE_VALUE)
   {
      ShowMessage("??????????? ?????? ?? ?????????...");
      return;
   }
   while(FindNextFile(hFindFile, &fd))
   {
      if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
      {
         sprintf(szFullFileName, String(pathf+"\\%s").c_str(), fd.cFileName);
         bbb = String(szFullFileName).Delete(1,String(szFullFileName).LastDelimiter('\\'));
         if( !MoveFile(szFullFileName, String(newdir+"\\"+bbb).c_str()) )
         {
            ShowMessage("??????????? ?????? ?? ?????????...");
            return;
         }
      }
   }
   if(!FindClose(hFindFile))
   {
      ShowMessage("??????????? ?????? ?? ?????????...");
      return;
   }
   if(!RemoveDirectory(pathf.c_str()))
   {
      ShowMessage("??????????? ?????? ?? ?????????...");
      return;
   }
   FindClose(hFindFile);
   //CloseHandle(hFindFile);
}
*/
void __fastcall TFCDPAuthorization::TFCDPMoveFiles( String slSourceDir, String slTargetDir, bool toCopy = true )
{
SHFILEOPSTRUCT sh;
sh.fFlags = FOF_NOCONFIRMATION | FOF_SILENT; //| FOF_FILESONLY;
sh.wFunc = toCopy ? FO_COPY : FO_MOVE;
sh.pFrom = slSourceDir.c_str();
sh.pTo = slTargetDir.c_str();
sh.hNameMappings = NULL;
sh.lpszProgressTitle = NULL;
if( SHFileOperation ( &sh ) )
   ShowMessage("??????????? ?????? ?? ?????????...");
}


void __fastcall TFCDPAuthorization::CDirectoryOutline1Change(
      TObject *Sender)
{
      CBLoginUser->Clear();
      HANDLE hFindFile11;
      WIN32_FIND_DATA fd11;
      String pathf11 = CDirectoryOutline1->Directory, bufs="";
      int iter = 0;

      hFindFile11 = FindFirstFile(String(pathf11+"\\*").c_str(), &fd11);
      if(hFindFile11 == INVALID_HANDLE_VALUE)
      {
         return;
      }

      while(FindNextFile(hFindFile11, &fd11))
      {
         iter++;
         bufs = fd11.cFileName;
         if(bufs.Length() <= cdpparam.dbCDPext.Length()) continue;
         bufs = bufs.Delete(1, bufs.LastDelimiter('.')-1);
         if(bufs != cdpparam.dbCDPext) continue;
         bufs = fd11.cFileName;
         bufs = bufs.Delete(bufs.LastDelimiter('.'), bufs.Length());
         CBLoginUser->Items->Add(bufs);
      }
      FindClose(hFindFile11);
      CBLoginUser->ItemIndex = 0;

      DriveComboBox1->Drive = CDirectoryOutline1->Directory.operator [](1);
}
//---------------------------------------------------------------------------

