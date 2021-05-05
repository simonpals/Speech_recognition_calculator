//----------------------------------------------------------------------------
#ifndef UCDPCalcH
#define UCDPCalcH
//----------------------------------------------------------------------------
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include "trayicon.h"

#include <vector.h>
#include "wave.h"
#include "fft.h"
#include "UCDPParam.h"
#include "UCDPRecognize.h"

//----------------------------------------------------------------------------
class TFCDPMain : public TForm
{
__published:
    TToolBar *ToolBar1;
    TStatusBar *StatusBar;
        TPopupMenu *PopupMenu1;
        TActionList *ActionList1;
        TPanel *Panel1;
        TToolButton *ToolButton1;
        TAction *Calc;
        TAction *Recog;
        TAction *Talk;
        TAction *Educat;
        TAction *Settings;
        TAction *Help;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TTrayIcon *TrayIcon1;
        TToolButton *ToolButton4;
        TSpeedButton *SpeedButton1;
        TToolButton *ToolButton5;
        TSpeedButton *SpeedButton2;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TPaintBox *PBTimeSpectr;
        TPaintBox *PBWave;
        TImageList *ImageList2;
        TTimer *Timer1;
        void __fastcall CalcExecute(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall TrayIcon1Minimize(TObject *Sender);
        void __fastcall TrayIcon1Restore(TObject *Sender);
        void __fastcall EducatExecute(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall PBPaintBoth(TObject *Sender);
        void __fastcall Timer1Timer(TObject *Sender);
private:
public:

    // Розгортка
    Graphics::TBitmap *tsbuf;
    // Waveform
    Graphics::TBitmap *wbcgr;
    Graphics::TBitmap *wbuf;

    vector <Srec_buf> vsig;
    Srec_buf srec_buf;
    TFFT *fft;
    TWaveRecorder *recorder;
    bool IsMinimize;
    bool First_Noise_Sec;
    int First_Sec_Disable;
    String CDPUserName;

    virtual __fastcall TFCDPMain(TComponent *AOwner);
    __fastcall TFCDPMain::~TFCDPMain();

    void __fastcall ProcessBuffer(short *buf);
    void __fastcall call_buttons_calc(String curwrd);
    void __fastcall UpdateBcgr();
    void __fastcall WaveUpdate();    
};
//----------------------------------------------------------------------------
extern TFCDPMain *FCDPMain;
//----------------------------------------------------------------------------
#endif
