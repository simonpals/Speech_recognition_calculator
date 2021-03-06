//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UCalc.h"
#include "UCDPCalc.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFCalc *FCalc;

   const int max_mantis = 15;
//---------------------------------------------------------------------------
__fastcall TFCalc::TFCalc(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFCalc::FormClose(TObject *Sender, TCloseAction &Action)
{
//   Action = caFree;
   FCalc->Visible = false;
   FCDPMain->SpeedButton1->Down = false;
   FCDPMain->SpeedButton2->Down = false;
   FCDPMain->PBTimeSpectr->Visible = false;
   FCDPMain->PBWave->Visible = false;
   if(FCDPMain->recorder) delete FCDPMain->recorder;
   FCDPMain->recorder = NULL;
   FCDPMain->vsig.clear();
   FCDPMain->First_Sec_Disable = 0;
   FCDPMain->StatusBar->Panels->Items[0]->Text = "";     
}
//---------------------------------------------------------------------------

void TFCalc::btn_operation(int name_btn)
{
   if(Desk_num->Text == "0") expr = "";
   else expr = Desk_num->Text;
   if(name_btn >= 0 && name_btn <= 11 && btn_operator == 16)
   {
      btn_operator = 17;
      expr = "";
   }

   //if( Canvas->TextWidth(expr+"  ") > Desk_num->ClientWidth && name_btn < 16 )
   if( expr.Length() > max_mantis &&  name_btn < 16 )
   {
      MessageBeep(0xFFFFFFFF);
      return;
   }

   if(expr == "-" && name_btn >= 10 && name_btn <= 16)
   {
      MessageBeep(0xFFFFFFFF);
      return;
   }

   switch ( name_btn )
   {
      case 0:
         if(expr == "-") expr = expr + "0,";
         else if( expr != "") expr = expr + "0";
         else expr = expr + "0,";
      break;
      case 1:
         expr = expr + "1";
      break;
      case 2:
         expr = expr + "2";
      break;
      case 3:
         expr = expr + "3";
      break;
      case 4:
         expr = expr + "4";
      break;
      case 5:
         expr = expr + "5";
      break;
      case 6:
         expr = expr + "6";
      break;
      case 7:
         expr = expr + "7";
      break;
      case 8:
         expr = expr + "8";
      break;
      case 9:
         expr = expr + "9";
      break;
      case 10: // +/-
         if(expr == "")
            expr = expr + "-";
         else return;
      break;
      case 11: // ,
         if(expr == "-" || (expr.LastDelimiter(',') > 0) ) return;
         if(expr == "")
            expr = expr + "0,";
         else expr = expr + ",";
      break;
      case 12: // /
         if(expr == "") return;
         btn = StrToFloat(expr);
         btn_operator = 12;
         expr = "";
      break;
      case 13: // *
         if(expr == "") return;
         btn = StrToFloat(expr);
         btn_operator = 13;
         expr = "";
      break;
      case 14: // -
         if(expr == "") return;
         btn = StrToFloat(expr);
         btn_operator = 14;
         expr = "";
      break;
      case 15: // +
         if(expr == "") return;
         btn = StrToFloat(expr);
         btn_operator = 15;
         expr = "";
      break;
      case 16: // =
         if(expr == "") return;
         if( StrToFloat(expr) == 0.0 && btn_operator == 12)
         {
            MessageBeep(0xFFFFFFFF);
            return;
         }
         switch(btn_operator)
         {
            case 12:
               btn = btn / StrToFloat(expr);
            break;
            case 13:
               btn = btn * StrToFloat(expr);
            break;
            case 14:
               btn = btn - StrToFloat(expr);
            break;
            case 15:
               btn = btn + StrToFloat(expr);
            break;
         }
         expr = FloatToStr(btn);
         btn_operator = 16;
         btn = 0.0;
      break;
      case 17: // C
         expr = "0";
         btn_operator = 17;
         btn = 0.0;
      break;
      case 18: // Backspace
         if(btn_operator > 11 && btn_operator < 16 && expr == "")
         {
            MessageBeep(0xFFFFFFFF);
            return;
         }
         if(expr == "") return;
         if(expr.Length() == 1)
         {
            expr = "";
            return;
         }
         expr.SetLength(expr.Length()-1);
         if(expr.operator [](expr.Length()) == ',')
            expr.SetLength(expr.Length()-1);
      break;
   }

   Desk_num->Text = expr;
   if(expr == "") Desk_num->Text = "0";
   expr = "";
}

void __fastcall TFCalc::Btn0Click(TObject *Sender)
{
   btn_operation(0);
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn1Click(TObject *Sender)
{
   btn_operation(1);
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn2Click(TObject *Sender)
{
   btn_operation(2);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn3Click(TObject *Sender)
{
   btn_operation(3);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn4Click(TObject *Sender)
{
   btn_operation(4);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn5Click(TObject *Sender)
{
   btn_operation(5);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn6Click(TObject *Sender)
{
   btn_operation(6);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn7Click(TObject *Sender)
{
   btn_operation(7);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn8Click(TObject *Sender)
{
   btn_operation(8);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::Btn9Click(TObject *Sender)
{
   btn_operation(9);
}
//---------------------------------------------------------------------------


void __fastcall TFCalc::BtnPMClick(TObject *Sender)
{
   btn_operation(10);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnComaClick(TObject *Sender)
{
   btn_operation(11);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnDivClick(TObject *Sender)
{
   btn_operation(12);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnMulClick(TObject *Sender)
{
   btn_operation(13);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnMinClick(TObject *Sender)
{
   btn_operation(14);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnPlusClick(TObject *Sender)
{
   btn_operation(15);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnEqClick(TObject *Sender)
{
   btn_operation(16);
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnCClick(TObject *Sender)
{
   btn_operation(17);
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::BtnBcspClick(TObject *Sender)
{
   btn_operation(18);        
}
//---------------------------------------------------------------------------

void __fastcall TFCalc::FormShow(TObject *Sender)
{
   Desk_num->Text = "0";
   expr = "";
   btn = 0.0;
   btn_operator = 17;
}
//---------------------------------------------------------------------------


