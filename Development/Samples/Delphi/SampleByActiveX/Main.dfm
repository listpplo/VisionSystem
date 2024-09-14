object Form1: TForm1
  Left = 10
  Top = 101
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'MvView'
  ClientHeight = 867
  ClientWidth = 1276
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object btnOpenAll: TButton
    Left = 246
    Top = 808
    Width = 129
    Height = 41
    Caption = 'Open All Cameras'
    TabOrder = 0
    OnClick = btnOpenAllClick
  end
  object btnDiscovery: TButton
    Left = 24
    Top = 808
    Width = 129
    Height = 41
    Caption = 'Discovery'
    TabOrder = 1
    OnClick = btnDiscoveryClick
  end
  object btnCloseAll: TButton
    Left = 468
    Top = 808
    Width = 137
    Height = 41
    Caption = 'Close All Cameras'
    TabOrder = 2
    OnClick = btnCloseAllClick
  end
  object btnOpenTrigger: TButton
    Left = 691
    Top = 808
    Width = 145
    Height = 41
    Caption = 'Open SoftTrigger'
    TabOrder = 3
    OnClick = btnOpenTriggerClick
  end
  object btnSoftwareTrigger: TButton
    Left = 1136
    Top = 808
    Width = 145
    Height = 41
    Caption = 'Execute SoftTrigger'
    TabOrder = 4
    OnClick = btnSoftwareTriggerClick
  end
  object btnCloseTrigger: TButton
    Left = 913
    Top = 808
    Width = 145
    Height = 41
    Caption = 'Close SoftTrigger'
    TabOrder = 5
    OnClick = btnCloseTriggerClick
  end
  object pbImage_1: TPanel
    Left = 24
    Top = 8
    Width = 393
    Height = 353
    Caption = 'Channel1'
    TabOrder = 6
  end
  object pbImage_2: TPanel
    Left = 432
    Top = 8
    Width = 401
    Height = 353
    Caption = 'Channel2'
    TabOrder = 7
  end
  object pbImage_3: TPanel
    Left = 848
    Top = 8
    Width = 433
    Height = 353
    Caption = 'Channel3'
    TabOrder = 8
  end
  object pbImage_4: TPanel
    Left = 24
    Top = 376
    Width = 393
    Height = 409
    Caption = 'Channel4'
    TabOrder = 9
  end
  object pbImage_5: TPanel
    Left = 432
    Top = 376
    Width = 401
    Height = 409
    Caption = 'Channel5'
    TabOrder = 10
  end
  object pbImage_6: TPanel
    Left = 848
    Top = 376
    Width = 433
    Height = 409
    Caption = 'Channel6'
    TabOrder = 11
  end
end
