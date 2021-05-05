//---------------------------------------------------------------------------

#ifndef UCDPAuthorizationH
#define UCDPAuthorizationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
#include <FileCtrl.hpp>
#include "cdiroutl.h"
#include <Grids.hpp>
#include <Outline.hpp>

#include <registry.hpp>
//---------------------------------------------------------------------------
class TFCDPAuthorization : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TButton *BauthorOK;
        TButton *BauthorCanc;
        TComboBox *CBLoginUser;
        TLabel *Label1;
        TLabel *Label2;
        TCDirectoryOutline *CDirectoryOutline1;
        TDriveComboBox *DriveComboBox1;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall BauthorCancClick(TObject *Sender);
        void __fastcall DriveComboBox1Change(TObject *Sender);
        void __fastcall BauthorOKClick(TObject *Sender);
        void __fastcall CDirectoryOutline1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TRegistry *Reg1;
        bool CanMoveFile;
        __fastcall TFCDPAuthorization(TComponent* Owner);
        void __fastcall DeleteDir(String wdir);
        void __fastcall TFCDPMoveFiles( String slSourceDir, String slTargetDir, bool toCopy );        
};
//---------------------------------------------------------------------------
extern PACKAGE TFCDPAuthorization *FCDPAuthorization;
//---------------------------------------------------------------------------
#endif

