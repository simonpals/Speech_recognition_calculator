//---------------------------------------------------------------------
#include <vcl.h>
#include <registry.hpp>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("UCDPCalc.cpp", FCDPMain);
USEFORM("UCalc.cpp", FCalc);
USEFORM("UCDPEducation.cpp", CDPEducation);
USEFORM("UCDPAuthorization.cpp", FCDPAuthorization);
//---------------------------------------------------------------------------
TRegistry *Reg = new TRegistry;
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
        Reg->RootKey = HKEY_LOCAL_MACHINE;
        if( Reg->KeyExists("\\Software\\CDPCalc") )
        {
           SetForegroundWindow(FindWindow(0, "Мовний калькулятор"));
           return 0;
        }
        else
        {
        Reg->OpenKey("\\Software\\CDPCalc",true);

	Application->Initialize();
	Application->Title = "CDPCalc";
        Application->CreateForm(__classid(TFCDPMain), &FCDPMain);
                 Application->Run();
        Reg->DeleteKey("\\Software\\CDPCalc");
        }
        delete Reg;
        }
        catch (Exception &exception)
        {
                 if( Reg->KeyExists("\\Software\\CDPCalc") )
                    Reg->DeleteKey("\\Software\\CDPCalc");
                 delete Reg;

                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        
        return 0;
}
//---------------------------------------------------------------------
