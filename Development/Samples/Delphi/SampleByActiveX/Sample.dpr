program Sample;

uses
  Forms,
  Main in 'Main.pas' {Form1},
  MVSDKOCX_TLB in 'C:\Program Files (x86)\Borland\Delphi7\Imports\MVSDKOCX_TLB.pas';
{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
