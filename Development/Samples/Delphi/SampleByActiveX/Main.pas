unit Main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls, MVSDKOCX_TLB, ComObj, ActiveX;

type
  TGrabHandler = class(TObject)
  private
    m_channel: Integer;
    m_hwnd: HWND;
    m_render: _HRDisplay;
    
  public
    Constructor Create(channel: Integer; pbImage: TPanel);
    Destructor Destroy; override;
    function ToBmp(pRaw: Pointer; nRaw, nWidth, nHeight: Integer): HGLOBAL; stdcall;
    procedure Pain(pRaw: Pointer; nRaw, nWidth, nHeight: Integer); stdcall;
    procedure DumpBmp(path: string; hMem: HGLOBAL); stdcall;
  end;

  TForm1 = class(TForm)
    btnOpenAll: TButton;
    btnDiscovery: TButton;
    btnCloseAll: TButton;
    btnOpenTrigger: TButton;
    btnSoftwareTrigger: TButton;
    btnCloseTrigger: TButton;
    pbImage_1: TPanel;
    pbImage_2: TPanel;
    pbImage_3: TPanel;
    pbImage_4: TPanel;
    pbImage_5: TPanel;
    pbImage_6: TPanel;

    procedure btnDiscoveryClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure btnOpenAllClick(Sender: TObject);
    procedure btnCloseAllClick(Sender: TObject);
    procedure btnOpenTriggerClick(Sender: TObject);
    procedure btnSoftwareTriggerClick(Sender: TObject);
    procedure btnCloseTriggerClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
    m_pbImage: Array[0..5] of TPanel;
    m_Handlers: Array[0..5] of TGrabHandler;
    m_Devices: Array[0..5] of _HRCamera;
  public
    { Public declarations }
    procedure OnDestroy; overload;
  end;

var
  Form1: TForm1;
  iFileName: Integer;

implementation

{$R *.dfm}

procedure OnGrabImage(param: Pointer; pGrabResult: Pointer; lBytes, width, height: Integer); stdcall
var
  obj: TGrabHandler;
  pBmp: HGLOBAL;
begin
    if param = nil Then Exit;
    obj:= TGrabHandler(param);
    obj.Pain(pGrabResult, lBytes, width, height);

    pBmp:= 0;
    {
     Inc(iFileName);
     if (iFileName mod 100) = 0 Then
     begin
      pBmp:= obj.ToBmp(pGrabResult, lBytes, width, height);
      obj.DumpBmp(ConCat(inttostr(iFileName), '.bmp'), pBmp);
     end;
    }
     if pBmp <> 0 Then GlobalFree(pBmp);
end;

Constructor TGrabHandler.Create(channel: Integer; pbImage: TPanel);
begin
  m_channel:= channel;
  m_hwnd:= pbImage.Handle;
  m_render:= CoHRDisplay.Create;
end;

Destructor TGrabHandler.Destroy;
begin
  if m_render <> nil Then m_render.Dispose;
end;

function TGrabHandler.ToBmp(pRaw: Pointer; nRaw, nWidth, nHeight: Integer): HGLOBAL;
var
  nBmp: Integer;
  pBmp: PBYTE;
begin
  nBmp:= m_render.BmpEncodeLen(nWidth, nHeight, True);
  Result:= 0;
  try
    Result:= GlobalAlloc(GHND, nBmp);
    if Result = 0 Then Exit;
    try
      pBmp:= GlobalLock(Result);
      m_render.CreateBmpFromBayerRG8(Integer(pRaw), nRaw, nWidth, nHeight, Integer(pBmp), nBmp);
    finally
      GlobalUnlock(Result);
    end;
  except
    if Result <> 0 Then GlobalFree(Result);
  end;
end;

procedure TGrabHandler.DumpBmp(path: string; hMem: HGLOBAL);
var
  pBuffer: PBYTE;
  nBuffer: Cardinal;
  hFile: THandle;
  dwWriteLen: Cardinal;
begin
  try
    nBuffer:= GlobalSize(hMem);
    pBuffer:= GlobalLock(hMem);
    hFile:= CreateFileA(PAnsiChar(AnsiString(path)), GENERIC_READ or GENERIC_WRITE,
      FILE_SHARE_READ or FILE_SHARE_WRITE, nil, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if hFile = 0 Then Exit;

    WriteFile(hFile, pBuffer^, nBuffer, dwWriteLen, nil);
    CloseHandle(hFile);
  finally
    GlobalUnlock(hMem)
  end;
end;

procedure TGrabHandler.Pain(pRaw: Pointer; nRaw, nWidth, nHeight: Integer);
begin
  //m_render.BayerRG8Display(Integer(pRaw), nRaw, nWidth, nHeight, m_hwnd);
  m_render.Mono8Display(Integer(pRaw), nRaw, nWidth, nHeight, m_hwnd);
end;

procedure TForm1.btnDiscoveryClick(Sender: TObject);
var
  DeviceEnumer: _HREnumerator;
  nChn: Integer;
  i: Integer;
begin
  DeviceEnumer:= CoHREnumerator.Create();
  if DeviceEnumer = nil Then Exit;

  nChn := DeviceEnumer.EnumDevice;
  if nChn > 0 then
  begin
    for i:= 0 to nChn - 1 do
    begin
      m_Devices[i]:= DeviceEnumer.GetDeviceByChannel(i);
      m_pbImage[i].Enabled:= True;
    end;

    btnDiscovery.Enabled:= False;
    btnOpenAll.Enabled:= True;
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  i: Integer;
begin
  btnOpenAll.Enabled:= False;
  btnCloseAll.Enabled:= False;
  btnOpenTrigger.Enabled:= False;
  btnCloseTrigger.Enabled:= False;
  btnSoftwareTrigger.Enabled:= False;

  iFileName:= 0;

  m_pbImage[0]:= pbImage_1;
  m_pbImage[1]:= pbImage_2;
  m_pbImage[2]:= pbImage_3;
  m_pbImage[3]:= pbImage_4;
  m_pbImage[4]:= pbImage_5;
  m_pbImage[5]:= pbImage_6;
  for i:= 0 to 5 do
  begin
    m_Handlers[i]:= TGrabHandler.Create(i, m_pbImage[i]);
  end;
end;

procedure TForm1.btnOpenAllClick(Sender: TObject);
var
  i, h, l: SmallInt;
begin
  h:= high(m_Devices);
  l:= low(m_Devices );

  for i := l to h do
    if m_Devices[i] <> nil Then
    begin
      m_Devices[i].Open(True);
      if m_Devices[i].IsOpen Then
      begin
		//m_Devices[i].Set_ImagePixelFormat('BayerRG8');
        m_Devices[i].Set_ImagePixelFormat('Mono8');
        m_Devices[i].ExposureTime := 50000.0;
        m_Devices[i].Gain := 2.0;
        m_Devices[i].CloseTrigger;
        m_Devices[i].StartGrabbing(Integer(@OnGrabImage), Integer(m_Handlers[i]), True);
      end
    end;

  btnOpenAll.Enabled:= False;
  btnCloseAll.Enabled:= True;
  btnOpenTrigger.Enabled:= True;
end;

procedure TForm1.OnDestroy;
var
  i, h, l: SmallInt;
begin
  h:= high(m_Devices);
  l:= low(m_Devices);

  for i := l to h do
  begin
    if m_Devices[i] <> nil Then
    begin
     if m_Devices[i].IsGrabbing Then m_Devices[i].StopGrabbing(True);
     if m_Devices[i].IsOpen Then m_Devices[i].Close(True);
     m_Devices[i].Dispose;
    end;
    if m_Handlers[i] <> nil Then m_Handlers[i].Destroy;
  end;
end;

procedure TForm1.btnCloseAllClick(Sender: TObject);
var
  i, h, l: SmallInt;
begin
  h:= high(m_Devices);
  l:= low(m_Devices);

  for i := l to h do
    if m_Devices[i] <> nil Then m_Devices[i].Close(True);

  btnOpenAll.Enabled:= True;
  btnCloseAll.Enabled:= False;
  btnOpenTrigger.Enabled:= False;
  btnCloseTrigger.Enabled:= False;
  btnSoftwareTrigger.Enabled:= False;
end;

procedure TForm1.btnOpenTriggerClick(Sender: TObject);
var
  i, h, l: SmallInt;

begin
  h:= high(m_Devices);
  l:= low(m_Devices);

  for i := l to h do
  begin
	  if m_Devices[i] <> nil Then
	  begin
		  if m_Devices[i].IsGrabbing Then m_Devices[i].StopGrabbing(True);
		  m_Devices[i].OpenSoftwareTrigger;
		  m_Devices[i].StartGrabbing(Integer(@OnGrabImage), Integer(m_Handlers[i]), True);
	  end;
  end;
  btnOpenTrigger.Enabled:= False;
  btnCloseTrigger.Enabled:= True;
  btnSoftwareTrigger.Enabled:= True;
end;

procedure TForm1.btnSoftwareTriggerClick(Sender: TObject);
var
  i, h, l: SmallInt;

begin
  h:= high(m_Devices);
  l:= low(m_Devices);

  for i := l to h do
    if m_Devices[i] <> nil Then m_Devices[i].ExecuteSoftwareTrigger;
end;

procedure TForm1.btnCloseTriggerClick(Sender: TObject);
var
  i, h, l: SmallInt;

begin
  h:= high(m_Devices);
  l:= low(m_Devices);

  for i := l to h do
  begin
    if m_Devices[i] <> nil Then
    begin
      m_Devices[i].StopGrabbing(True);
      m_Devices[i].CloseTrigger;
      m_Devices[i].StartGrabbing(Integer(@OnGrabImage), Integer(m_Handlers[i]), True);
    end;
  end;

  btnOpenTrigger.Enabled:= True;
  btnCloseTrigger.Enabled:= False;
  btnSoftwareTrigger.Enabled:= False;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
var
  i, h, l: SmallInt;
begin
  h:= high(m_Devices);
  l:= low(m_Devices);

  for i := l to h do
  begin
    if m_Devices[i] <> nil Then
    begin
     if m_Devices[i].IsGrabbing Then m_Devices[i].StopGrabbing(True);
     if m_Devices[i].IsOpen Then m_Devices[i].Close(True);
     m_Devices[i].Dispose;
    end;
    if m_Handlers[i] <> nil Then m_Handlers[i].Destroy;
  end;
end;

end.
