//---------------------------------------------------------------------------

#ifndef UCDPEducationH
#define UCDPEducationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include "CSPIN.h"
#include <Buttons.hpp>

#include "UCDPCalc.h"
#include "UCDPSelectWord.h"

//---------------------------------------------------------------------------
class TCDPEducation : public TForm
{
__published:	// IDE-managed Components
        TRadioButton *RBroomnoise;
        TRadioButton *RBstreetnoise;
        TRadioButton *RBmachinenoise;
        TGroupBox *GBEdParam;
        TButton *BtnCont;
        TTrackBar *TBerr;
        TGroupBox *GroupBox1;
        TGroupBox *GroupBox3;
        TRadioButton *RBbinarr;
        TRadioButton *RBfftA;
        TRadioButton *RBkdurb;
        TGroupBox *GroupBox4;
        TGroupBox *GroupBox5;
        TCSpinEdit *CSEU;
        TLabel *Label1;
        TLabel *Label2;
        TLabel *Label3;
        TGroupBox *GBEducRecog;
        TGroupBox *GBIndicate;
        TPaintBox *PBOgynEtal;
        TPaintBox *PBOgynUser;
        TLabel *LInform1;
        TLabel *LInform2;
        TProgressBar *PBIndicateEduc;
        TRichEdit *REUserInform;
        TLabel *Recording;
        TSpeedButton *BRecord;
        TSpeedButton *BPlay;
        TButton *BNextWrd1;
        TLabel *LReplay1;
        TLabel *LPlay1;
        TTimer *Timer1;
        TLabel *Label4;
        TSpeedButton *SBsaveEduc;
        TLabel *LFinishProcess;
        TTreeView *TVWordList;
        TPaintBox *PBWaveForm;
        TTimer *Timer2;
        TCheckBox *CBTokenSens;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall CSEUKeyPress(TObject *Sender, char &Key);
        void __fastcall CSEUChange(TObject *Sender);
        void __fastcall TBerrChange(TObject *Sender);
        void __fastcall BtnContClick(TObject *Sender);
        void __fastcall TVWordListExpanding(TObject *Sender,
          TTreeNode *Node, bool &AllowExpansion);
        void __fastcall PBOgynPaint(TObject *Sender);
        void __fastcall TVWordListChange(TObject *Sender, TTreeNode *Node);
        void __fastcall BRecordClick(TObject *Sender);
        void __fastcall SBsaveEducClick(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
        void __fastcall Timer2Timer(TObject *Sender);
        void __fastcall BPlayClick(TObject *Sender);
        void __fastcall TVWordListCollapsing(TObject *Sender,
          TTreeNode *Node, bool &AllowCollapse);
        void __fastcall BNextWrd1Click(TObject *Sender);
        void __fastcall TVWordListChanging(TObject *Sender,
          TTreeNode *Node, bool &AllowChange);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:	// User declarations
public:		// User declarations

        int counter;
        bool First_Noise_Sec;
        int First_Sec_Disable;
        Graphics::TBitmap *obcgr;
        Graphics::TBitmap *obuf;
        Graphics::TBitmap *wavebuf;

        TWaveRecorder *records;
        vector <Srec_buf> vsig;
        Srec_buf srec_buf;
        short **all_realization;
        double **all_ogyn;
        int *len_realiz;
        int *len_ogyn;
        TWave *wave;
        int cur_ind;
        bool CanCollapse;

        void __fastcall ProcesBuffer(short *buf);
                
        __fastcall TCDPEducation(TComponent* Owner);
        __fastcall ~TCDPEducation();
        void __fastcall TCDPEducationMain();
        void __fastcall TCDPUserInfo();
        void __fastcall TCDPDrawOgyn();
        void __fastcall TCDPDrawWave();
        void __fastcall TCDPRefreshWordBuf(int par);
        void __fastcall EnterNewEtalon(String somewrd, String allwrd);

       // void __fastcall OnProgressBarStemM(TMessage& Msg);
       // BEGIN_MESSAGE_MAP
        //   MESSAGE_HANDLER(WM_ProgressBarStemM, TMessage, OnProgressBarStemM)
       // END_MESSAGE_MAP(TComponent)
};
//---------------------------------------------------------------------------
extern PACKAGE TCDPEducation *CDPEducation;
//---------------------------------------------------------------------------
#endif
