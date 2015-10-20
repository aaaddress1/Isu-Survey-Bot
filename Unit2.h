//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListBox;
	TPanel *Panel1;
	TButton *Button1;
	TEdit *UserNameTB;
	TLabel *Label1;
	TEdit *PasswordTB;
	TLabel *Label2;
	TButton *Button2;
	TStyleBook *StyleBook1;
	TComboBox *ComboBox;
	TListBox *ListBox1;
	TLabel *Label3;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall AddInfo(String n);
	bool __fastcall SendSurvey(String ClassID,String CmdOpt,String ChoiseStr);
	void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm2 *Form2;
//---------------------------------------------------------------------------
#endif
