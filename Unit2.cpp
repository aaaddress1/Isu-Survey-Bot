
#include <fmx.h>
#pragma hdrstop
#include <System.IOUtils.hpp>
#include "System.RegularExpressions.hpp"
#include "NetObject\NetClient.h"
String RegIsuOnlineName = L"登出</font>";
#include "Unit2.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
TNetClient *nClient 	 = new TNetClient(NULL);
String		IDArray[100] = {};
String		SendOpt[100] = {};
String		NameArray[100] = {};
String		TeacherArray[100]={};
String SurveyMenuURL = L"http://netreg.isu.edu.tw/wapp/wap_13/wap_130100.asp";


String LoginIsuOnlineApp(AnsiString UserName,AnsiString PassWord)
{
	String Source = "";
	TStringList *Arg = new TStringList;
	Arg->Add("language=zh_TW");
	Arg->Add("lange_sel=zh_TW");
	Arg->Add("logon_id=" + UserName);
	Arg->Add("txtpasswd=" + PassWord);
	Arg->Add("submit1=µn¤J");
	nClient->IdHTTP->Request->Referer = "http://netreg.isu.edu.tw/Wapp/wap_indexmain.asp?call_from=logout";
	Source = nClient->IdHTTP->Post("http://netreg.isu.edu.tw/Wapp/wap_check.asp", Arg);
	Source = nClient->GetHtml("http://netreg.isu.edu.tw/Wapp/left.asp");
	delete Arg;
	if (TRegEx::IsMatch(Source,RegIsuOnlineName)) return Source;
	return NULL;
}

String StrRemove(String Source,String RemovedStr)
{
	return StringReplace(Source,RemovedStr,"",TReplaceFlags()<<rfReplaceAll);
}

void __fastcall TForm2::Button1Click(TObject *Sender)
{
	if (LoginIsuOnlineApp(this->UserNameTB->Text ,this->PasswordTB->Text) == NULL)
	{
		ShowMessage(L"登入義守帳號時發生問題！");
		return;
	}

	this->Panel1->Enabled = false;
	String SurveyMenuSource = nClient->GetHtml(SurveyMenuURL);
	if (SurveyMenuSource.Pos(L"填寫教學意見調查問卷") < 1)
	{
		ShowMessage(L"載入教學意願調查表時發生錯誤！");
		return;
	}

	SurveyMenuSource = StrRemove(SurveyMenuSource, "\n");
	SurveyMenuSource = StrRemove(SurveyMenuSource, "\t");
	SurveyMenuSource = StrRemove(SurveyMenuSource, "\r");
	SurveyMenuSource = StrRemove(SurveyMenuSource, " ");
	TMatch match ;
	TMatchCollection matches;
	matches = TRegEx::Matches(SurveyMenuSource, L"value=\"[^>]+>([^\<]+)</td><td>&nbsp;([^<]+)</td><td>&nbsp;([^<]+)</td><td>&nbsp;([^<]+)</td><INPUTid.*?><INPUTid=command.{1,50}hiddenvalue=\"([^\"]+)\">");
	for (int i = 0; i < matches.Count ; i++)
	{
		match = matches[i] ;
		NameArray[i] = match.Groups[2].Value;
		TeacherArray[i] = match.Groups[3].Value;
		IDArray[i] = match.Groups[1].Value;
		SendOpt[i] = match.Groups[5].Value;
		int Pos = this->ListBox->Items->Add(TeacherArray[i]+ "\t"+ match.Groups[4].Value +"\t"+ NameArray[i]) ;
		bool Finished = (SendOpt[i] == "Redo_SURVEY");
		this->ListBox->ItemByIndex(Pos)->IsChecked = !Finished;
	}
}

bool __fastcall TForm2:: SendSurvey(String ClassID,String CmdOpt,String ChoiseStr)
{
	TStringList *Arg = new TStringList;
	Arg->Add("crcode=" + ClassID);
	Arg->Add("surtype=0");
	Arg->Add("command=" + CmdOpt);
	Arg->Add(L"submit1=修改問卷");
	String Source = nClient->IdHTTP->Post(SurveyMenuURL, Arg);
	TStringList *Arg1 = new TStringList;
	Source = StrRemove(Source, "\n");
	Source = StrRemove(Source, "\t");
	Source = StrRemove(Source, "\r");
	Source = StrRemove(Source, " ");

	Arg1->Add("cr_code=" + ClassID);
	Arg1->Add("stucrcode=" + ClassID);
	Arg1->Add("X01X06M2/Y=Y");
	Arg1->Add("X01X02/Y=Y");
	Arg1->Add("X01X04M6/Y=Y");
	Arg1->Add("X08X10M17/Y=Y");
	Arg1->Add(L"submit1=填好送出");
	Arg1->Add("item_no=");
	Arg1->Add("Group_no=");
	Arg1->Add("surtype=0");
	Arg1->Add("X08X10T/Y=");
	Arg1->Add("X01X04T/Y=");
	Arg1->Add("X01X06T/Y=");

	Arg1->Add("X01X03/Y=L1");
	Arg1->Add("X08X05/Y=L1");
	Arg1->Add("X08X06/Y=L1");
	Arg1->Add("X08X07/Y=L1");
	Arg1->Add("X08X08/Y=L1");
	Arg1->Add("X01X07M6/Y=Y");

	Arg1->Add("command=" + TRegEx::Match(Source,"name=commandtype=hiddenvalue=\"([^\"]+)\"").Groups[1].Value);
	Arg1->Add("test="+ TRegEx::Match(Source,"name=testtype=hiddenvalue=\"([^\"]+)\"").Groups[1].Value);
	Arg1->Add("GroupNoItemNo="+ TRegEx::Match(Source,"name=GroupNoItemNotype=hiddenvalue=\"([^\"]+)\"").Groups[1].Value);

	TMatch match ;
	TMatchCollection matches;
	matches = TRegEx::Matches(Source, L"name=\"([^\"]+)\"type=radiovalue=\"([^\"]+)\">" + ChoiseStr);
	for (int i = 0; i < matches.Count ; i++)
	{
		match = matches[i] ;
		Arg1->Add(match.Groups[1].Value + "=" + match.Groups[2].Value);
	}
	delete Arg;
	nClient->IdHTTP->Request->Referer = SurveyMenuURL;
	Source = nClient->IdHTTP->Post(SurveyMenuURL, Arg1);
	return true;
}

void __fastcall TForm2::AddInfo(String n)
{
	try {
		this->ListBox1->Items->Add(n);
	} catch (...) {
	}
}


class CoreModuFunc : public TCppInterfacedObject<TProc>
{
public:
	CoreModuFunc(TForm2* Form) : FForm(Form) {}
	virtual void __fastcall Invoke(void)
	{
		FForm->ComboBox->Enabled = false;
		FForm->Button2->Enabled = false;
		for (int i = 0; i < (FForm->ListBox->Count); i++)
		{
			if (!FForm->ListBox->ItemByIndex(i)->IsChecked) continue;
			if (FForm->SendSurvey(IDArray[i],SendOpt[i],FForm->ComboBox->Selected->Text))
				DirectNewItem(L"已成功對" + TeacherArray[i] + L"的" + NameArray[i] + L"填寫：" + FForm->ComboBox->Selected->Text);
			FForm->ListBox->ItemByIndex(i)->IsChecked = false;
			TThread::Sleep(100);

		}
		FForm->ComboBox->Enabled = true;
		FForm->Button2->Enabled = true;
	}

	void DirectNewItem(String Str)
	{
		TempStr = Str;
		TThread::Synchronize(TThread::CurrentThread, AddNewItem);
	}
	void __fastcall AddNewItem(void){FForm->AddInfo(TempStr);}

private:
	TForm2 * FForm;
	String TempStr ;
};
void __fastcall TForm2::Button2Click(TObject *Sender)
{
		TThread::CreateAnonymousThread(new CoreModuFunc(this))->Start();
}






void __fastcall TForm2::FormCreate(TObject *Sender)
{
	this->AddInfo("Hi,你好! 我是作者 馬聖豪.");
	this->AddInfo("如果想聯繫我,歡迎至 www.facebook.com/MaHauo");
	this->AddInfo("此程式為開源專案! Link: github.com/aaaddress1/Isu-Survey-Bot");
}
//---------------------------------------------------------------------------

