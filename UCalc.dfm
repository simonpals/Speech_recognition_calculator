object FCalc: TFCalc
  Left = 332
  Top = 279
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = #1050#1072#1083#1100#1082#1091#1083#1103#1090#1086#1088
  ClientHeight = 218
  ClientWidth = 272
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 272
    Height = 218
    Align = alClient
    TabOrder = 0
    object Desk_num: TEdit
      Left = 26
      Top = 12
      Width = 228
      Height = 21
      ReadOnly = True
      TabOrder = 0
      Text = '0'
    end
    object Btn1: TButton
      Left = 27
      Top = 130
      Width = 33
      Height = 33
      Caption = '1'
      TabOrder = 1
      OnClick = Btn1Click
    end
    object Btn2: TButton
      Left = 67
      Top = 130
      Width = 33
      Height = 33
      Caption = '2'
      TabOrder = 2
      OnClick = Btn2Click
    end
    object Btn3: TButton
      Left = 108
      Top = 130
      Width = 33
      Height = 33
      Caption = '3'
      TabOrder = 3
      OnClick = Btn3Click
    end
    object Btn4: TButton
      Left = 27
      Top = 90
      Width = 33
      Height = 33
      Caption = '4'
      TabOrder = 4
      OnClick = Btn4Click
    end
    object Btn5: TButton
      Left = 67
      Top = 90
      Width = 33
      Height = 33
      Caption = '5'
      TabOrder = 5
      OnClick = Btn5Click
    end
    object Btn6: TButton
      Left = 108
      Top = 90
      Width = 33
      Height = 33
      Caption = '6'
      TabOrder = 6
      OnClick = Btn6Click
    end
    object Btn7: TButton
      Left = 27
      Top = 50
      Width = 33
      Height = 33
      Caption = '7'
      TabOrder = 7
      OnClick = Btn7Click
    end
    object Btn8: TButton
      Left = 67
      Top = 50
      Width = 33
      Height = 33
      Caption = '8'
      TabOrder = 8
      OnClick = Btn8Click
    end
    object Btn9: TButton
      Left = 108
      Top = 50
      Width = 33
      Height = 33
      Caption = '9'
      TabOrder = 9
      OnClick = Btn9Click
    end
    object BtnPM: TButton
      Left = 67
      Top = 170
      Width = 33
      Height = 33
      Caption = '+/-'
      TabOrder = 10
      OnClick = BtnPMClick
    end
    object Btn0: TButton
      Left = 27
      Top = 170
      Width = 33
      Height = 33
      Caption = '0'
      TabOrder = 11
      OnClick = Btn0Click
    end
    object BtnEq: TBitBtn
      Left = 221
      Top = 170
      Width = 33
      Height = 33
      Caption = '='
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 12
      OnClick = BtnEqClick
    end
    object BtnC: TBitBtn
      Left = 221
      Top = 114
      Width = 33
      Height = 41
      Caption = 'C'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 13
      OnClick = BtnCClick
    end
    object BtnBcsp: TBitBtn
      Left = 221
      Top = 50
      Width = 33
      Height = 49
      Caption = 'Bksp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 14
      OnClick = BtnBcspClick
    end
    object BtnPlus: TBitBtn
      Left = 163
      Top = 170
      Width = 33
      Height = 33
      Caption = '+'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 15
      OnClick = BtnPlusClick
    end
    object BtnMin: TBitBtn
      Left = 163
      Top = 130
      Width = 33
      Height = 33
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 16
      OnClick = BtnMinClick
    end
    object BtnMul: TBitBtn
      Left = 163
      Top = 90
      Width = 33
      Height = 33
      Caption = '*'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 17
      OnClick = BtnMulClick
    end
    object BtnDiv: TBitBtn
      Left = 163
      Top = 50
      Width = 33
      Height = 33
      Caption = '/'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 18
      OnClick = BtnDivClick
    end
    object BtnComa: TBitBtn
      Left = 107
      Top = 170
      Width = 33
      Height = 33
      Caption = ','
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 19
      OnClick = BtnComaClick
    end
  end
end
