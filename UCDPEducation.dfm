object CDPEducation: TCDPEducation
  Left = 625
  Top = 121
  BorderStyle = bsToolWindow
  Caption = #1053#1072#1074#1095#1072#1085#1085#1103' '#1089#1080#1089#1090#1077#1084#1080
  ClientHeight = 503
  ClientWidth = 429
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  object GBEdParam: TGroupBox
    Left = 0
    Top = 0
    Width = 429
    Height = 503
    Align = alClient
    Caption = #1055#1072#1088#1072#1084#1077#1090#1088#1080' '#1088#1086#1079#1087#1110#1079#1085#1072#1074#1072#1085#1085#1103
    TabOrder = 0
    object BtnCont: TButton
      Left = 169
      Top = 462
      Width = 83
      Height = 25
      Caption = #1055#1088#1086#1076#1086#1074#1078#1080#1090#1080
      TabOrder = 0
      OnClick = BtnContClick
    end
    object GroupBox1: TGroupBox
      Left = 3
      Top = 346
      Width = 425
      Height = 100
      Caption = #1058#1080#1087' '#1096#1091#1084#1086#1074#1086#1111' '#1079#1072#1074#1072#1076#1080
      TabOrder = 1
      object RBstreetnoise: TRadioButton
        Left = 253
        Top = 33
        Width = 134
        Height = 17
        Caption = #1042#1110#1076#1082#1088#1080#1090#1077' '#1089#1077#1088#1077#1076#1086#1074#1080#1097#1077
        TabOrder = 0
      end
      object RBmachinenoise: TRadioButton
        Left = 106
        Top = 64
        Width = 163
        Height = 17
        Caption = #1052#1072#1096#1080#1085#1085#1080#1081' ('#1087#1086#1089#1090#1110#1081#1085#1080#1081') '#1096#1091#1084
        TabOrder = 1
      end
      object RBroomnoise: TRadioButton
        Left = 14
        Top = 33
        Width = 97
        Height = 17
        Caption = #1050#1110#1084#1085#1072#1090#1085#1080#1081' '#1096#1091#1084
        Checked = True
        TabOrder = 2
        TabStop = True
      end
    end
    object GroupBox3: TGroupBox
      Left = 3
      Top = 247
      Width = 425
      Height = 100
      Caption = #1040#1083#1075#1086#1088#1080#1090#1084' '#1087#1088#1077#1076#1089#1090#1072#1074#1083#1077#1085#1085#1103' '#1084#1086#1074#1085#1080#1093' '#1076#1072#1085#1080#1093
      TabOrder = 2
      object RBbinarr: TRadioButton
        Left = 11
        Top = 32
        Width = 246
        Height = 17
        Caption = #1044#1074#1110#1081#1082#1086#1074#1077' '#1087#1088#1077#1076#1089#1090#1072#1074#1083#1077#1085#1085#1103' ('#1088#1077#1082#1086#1084#1077#1085#1076#1091#1108#1090#1100#1089#1103')'
        Checked = True
        TabOrder = 0
        TabStop = True
      end
      object RBfftA: TRadioButton
        Left = 104
        Top = 64
        Width = 185
        Height = 17
        Caption = #1045#1092#1077#1082#1090#1080#1074#1085#1077' '#1079#1085#1072#1095#1077#1085#1085#1103' '#1072#1084#1087#1083#1110#1090#1091#1076#1080
        TabOrder = 1
      end
      object RBkdurb: TRadioButton
        Left = 253
        Top = 32
        Width = 156
        Height = 17
        Caption = #1050#1086#1077#1092#1110#1094#1110#1108#1085#1090#1080' '#1074#1110#1076#1086#1073#1088#1072#1078#1077#1085#1085#1103
        TabOrder = 2
      end
    end
    object GroupBox4: TGroupBox
      Left = 3
      Top = 147
      Width = 425
      Height = 100
      Caption = #1044#1086#1087#1091#1089#1090#1080#1084#1080#1081' '#1088#1110#1074#1077#1085#1100' '#1087#1086#1093#1080#1073#1082#1080' (%)'
      TabOrder = 3
      object Label1: TLabel
        Left = 20
        Top = 44
        Width = 3
        Height = 13
      end
      object Label2: TLabel
        Left = 390
        Top = 44
        Width = 3
        Height = 13
      end
      object Label3: TLabel
        Left = 202
        Top = 24
        Width = 3
        Height = 13
      end
      object TBerr: TTrackBar
        Left = 40
        Top = 40
        Width = 337
        Height = 41
        Orientation = trHorizontal
        Frequency = 1
        Position = 0
        SelEnd = 0
        SelStart = 0
        TabOrder = 0
        TickMarks = tmBottomRight
        TickStyle = tsAuto
        OnChange = TBerrChange
      end
    end
    object GroupBox5: TGroupBox
      Left = 2
      Top = 47
      Width = 425
      Height = 100
      Caption = #1050#1110#1083#1100#1082#1110#1089#1090#1100' '#1087#1086#1074#1090#1086#1088#1102#1074#1072#1085#1085#1103' '#1089#1083#1086#1074#1072
      TabOrder = 4
      object CSEU: TCSpinEdit
        Left = 125
        Top = 43
        Width = 169
        Height = 22
        TabOrder = 0
        OnChange = CSEUChange
        OnKeyPress = CSEUKeyPress
      end
    end
    object CBTokenSens: TCheckBox
      Left = 81
      Top = 20
      Width = 274
      Height = 17
      Caption = #1042#1088#1072#1093#1091#1074#1072#1085#1085#1103' '#1082#1110#1083#1100#1082#1086#1089#1090#1110' '#1089#1082#1083#1072#1076#1110#1074' '#1087#1088#1080' '#1088#1086#1079#1087#1110#1079#1085#1072#1074#1072#1085#1085#1110
      TabOrder = 5
    end
  end
  object GBEducRecog: TGroupBox
    Left = 0
    Top = 0
    Width = 429
    Height = 503
    Hint = #1047#1072#1087#1080#1089
    Align = alClient
    Caption = #1055#1072#1085#1077#1083#1100' '#1085#1072#1074#1095#1072#1085#1085#1103' '#1088#1086#1079#1087#1110#1079#1085#1072#1074#1072#1085#1085#1102
    Enabled = False
    TabOrder = 1
    Visible = False
    object Recording: TLabel
      Left = 186
      Top = 260
      Width = 58
      Height = 15
      Alignment = taCenter
      AutoSize = False
      Caption = '[ '#1047#1072#1087#1080#1089'  ]'
      Color = clRed
      ParentColor = False
      Visible = False
    end
    object BRecord: TSpeedButton
      Left = 81
      Top = 240
      Width = 25
      Height = 25
      Hint = #1047#1072#1087#1080#1089#1072#1090#1080
      Enabled = False
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333888883333333333800008833333333009999008833333099999999
        0833333099999999088333099999999990833309999999999083330999999999
        9083330999999999908333309999999908333330999999990333333300999900
        3333333333000033333333333333333333333333333333333333}
      ParentShowHint = False
      ShowHint = True
      OnClick = BRecordClick
    end
    object BPlay: TSpeedButton
      Left = 113
      Top = 240
      Width = 25
      Height = 25
      Hint = #1042#1110#1076#1090#1074#1086#1088#1080#1090#1080
      Enabled = False
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333333333333333333333333333333333333333333333333333033333
        3333333333003333333333333300033333333333330000333333333333000003
        3333333333000033333333333300033333333333330033333333333333033333
        3333333333333333333333333333333333333333333333333333}
      ParentShowHint = False
      ShowHint = True
      OnClick = BPlayClick
    end
    object LReplay1: TLabel
      Left = 13
      Top = 245
      Width = 66
      Height = 13
      Caption = #1055#1086#1074#1090#1086#1088#1080#1090#1080' ->'
      Color = clBackground
      ParentColor = False
      Visible = False
    end
    object LPlay1: TLabel
      Left = 140
      Top = 245
      Width = 71
      Height = 13
      Caption = '<- '#1055#1088#1086#1089#1083#1091#1093#1072#1090#1080
      Color = clBackground
      ParentColor = False
      Visible = False
    end
    object Label4: TLabel
      Left = 305
      Top = 270
      Width = 32
      Height = 13
      Caption = 'Label4'
      Visible = False
    end
    object SBsaveEduc: TSpeedButton
      Left = 367
      Top = 240
      Width = 25
      Height = 25
      Hint = #1047#1073#1077#1088#1077#1075#1090#1080
      Enabled = False
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        333333FFFFFFFFFFFFF33000077777770033377777777777773F000007888888
        00037F3337F3FF37F37F00000780088800037F3337F77F37F37F000007800888
        00037F3337F77FF7F37F00000788888800037F3337777777337F000000000000
        00037F3FFFFFFFFFFF7F00000000000000037F77777777777F7F000FFFFFFFFF
        00037F7F333333337F7F000FFFFFFFFF00037F7F333333337F7F000FFFFFFFFF
        00037F7F333333337F7F000FFFFFFFFF00037F7F333333337F7F000FFFFFFFFF
        00037F7F333333337F7F000FFFFFFFFF07037F7F33333333777F000FFFFFFFFF
        0003737FFFFFFFFF7F7330099999999900333777777777777733}
      NumGlyphs = 2
      ParentShowHint = False
      ShowHint = True
      OnClick = SBsaveEducClick
    end
    object PBWaveForm: TPaintBox
      Left = 220
      Top = 32
      Width = 201
      Height = 200
      Color = clMoneyGreen
      ParentColor = False
      OnPaint = PBOgynPaint
    end
    object GBIndicate: TGroupBox
      Left = 6
      Top = 288
      Width = 419
      Height = 209
      Caption = #1030#1085#1076#1080#1082#1072#1082#1094#1110#1103' '#1087#1088#1086#1094#1077#1089#1091
      TabOrder = 0
      object PBOgynEtal: TPaintBox
        Left = 8
        Top = 16
        Width = 190
        Height = 145
        Color = clMoneyGreen
        ParentColor = False
        OnPaint = PBOgynPaint
      end
      object PBOgynUser: TPaintBox
        Left = 220
        Top = 16
        Width = 190
        Height = 145
        Color = clMoneyGreen
        ParentColor = False
        OnPaint = PBOgynPaint
      end
      object LInform1: TLabel
        Left = 21
        Top = 172
        Width = 363
        Height = 13
        Caption = 
          #1055#1086#1088#1110#1074#1085#1103#1081#1090#1077' '#1094#1110' '#1076#1074#1110' '#1086#1075#1080#1085#1072#1102#1095#1110': '#1077#1090#1072#1083#1086#1085#1085#1086#1075#1086' '#1089#1083#1086#1074#1072' ('#1079#1083#1110#1074#1072') '#1090#1072' '#1074#1072#1096#1086#1075#1086' (' +
          #1089#1087#1088#1072#1074#1072')'
        Visible = False
      end
      object LInform2: TLabel
        Left = 8
        Top = 184
        Width = 394
        Height = 13
        Caption = 
          #1071#1082#1097#1086' '#1074#1080' '#1073#1072#1095#1080#1090#1077' '#1089#1091#1090#1090#1108#1074#1091' '#1088#1110#1079#1085#1080#1094#1102' ('#1091' '#1092#1086#1088#1084#1110') - '#1087#1086#1074#1090#1086#1088#1110#1090#1100' '#1073#1091#1076#1100'-'#1083#1072#1089#1082#1072' ' +
          #1079#1072#1087#1080#1089' '#1089#1083#1086#1074#1072
        Visible = False
      end
      object LFinishProcess: TLabel
        Left = 157
        Top = 167
        Width = 107
        Height = 13
        Caption = #1047#1072#1074#1077#1088#1096#1077#1085#1085#1103' '#1087#1088#1086#1094#1077#1089#1091
        Visible = False
      end
      object PBIndicateEduc: TProgressBar
        Left = 44
        Top = 184
        Width = 337
        Height = 17
        Min = 0
        Max = 100
        TabOrder = 0
        Visible = False
      end
    end
    object REUserInform: TRichEdit
      Left = 220
      Top = 32
      Width = 201
      Height = 200
      Color = clMoneyGreen
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 1
    end
    object BNextWrd1: TButton
      Left = 297
      Top = 240
      Width = 57
      Height = 25
      Caption = #1044#1072#1083#1110' >>'
      Enabled = False
      TabOrder = 2
      OnClick = BNextWrd1Click
    end
    object TVWordList: TTreeView
      Left = 8
      Top = 32
      Width = 201
      Height = 200
      Color = clMoneyGreen
      Indent = 19
      ReadOnly = True
      TabOrder = 3
      OnChange = TVWordListChange
      OnChanging = TVWordListChanging
      OnCollapsing = TVWordListCollapsing
      OnExpanding = TVWordListExpanding
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 200
    OnTimer = Timer1Timer
    Left = 24
    Top = 262
  end
  object Timer2: TTimer
    Enabled = False
    OnTimer = Timer2Timer
    Left = 264
    Top = 256
  end
end
