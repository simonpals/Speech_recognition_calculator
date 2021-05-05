//---------------------------------------------------------------------------

#ifndef UCalcH
#define UCalcH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TFCalc : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TButton *Btn1;
        TButton *Btn2;
        TButton *Btn3;
        TButton *Btn4;
        TButton *Btn5;
        TButton *Btn6;
        TButton *Btn7;
        TButton *Btn8;
        TButton *Btn9;
        TButton *Btn0;
        TButton *BtnPM;
        TEdit *Desk_num;
        TBitBtn *BtnEq;
        TBitBtn *BtnC;
        TBitBtn *BtnBcsp;
        TBitBtn *BtnPlus;
        TBitBtn *BtnMin;
        TBitBtn *BtnMul;
        TBitBtn *BtnDiv;
        TBitBtn *BtnComa;
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Btn0Click(TObject *Sender);
        void __fastcall Btn1Click(TObject *Sender);
        void __fastcall Btn2Click(TObject *Sender);
        void __fastcall Btn3Click(TObject *Sender);
        void __fastcall Btn4Click(TObject *Sender);
        void __fastcall Btn5Click(TObject *Sender);
        void __fastcall Btn6Click(TObject *Sender);
        void __fastcall Btn7Click(TObject *Sender);
        void __fastcall Btn8Click(TObject *Sender);
        void __fastcall Btn9Click(TObject *Sender);
        void __fastcall BtnPMClick(TObject *Sender);
        void __fastcall BtnComaClick(TObject *Sender);
        void __fastcall BtnDivClick(TObject *Sender);
        void __fastcall BtnMulClick(TObject *Sender);
        void __fastcall BtnMinClick(TObject *Sender);
        void __fastcall BtnPlusClick(TObject *Sender);
        void __fastcall BtnEqClick(TObject *Sender);
        void __fastcall BtnCClick(TObject *Sender);
        void __fastcall BtnBcspClick(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFCalc(TComponent* Owner);

   String expr;
   double btn;
   int btn_operator;

   void btn_operation(int);
};
//---------------------------------------------------------------------------
extern PACKAGE TFCalc *FCalc;
//---------------------------------------------------------------------------
#endif
