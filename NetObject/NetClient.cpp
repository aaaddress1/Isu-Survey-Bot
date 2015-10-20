
#include "NetClient.h"


__fastcall TNetClient::TNetClient(TComponent* Owner)
{
	this->IdHTTP = new TIdHTTP(NULL);
	this->IdHTTP->CookieManager = new TIdCookieManager(0);
	myStream = new TMemoryStream();
	this->IdHTTP->HandleRedirects = true;
/*	this->IdSSLIOHandlerSocketOpenSSL =  new TIdSSLIOHandlerSocketOpenSSL(NULL);
	this->IdHTTP->IOHandler = this->IdSSLIOHandlerSocketOpenSSL;*/
}

String TNetClient::GetCookieValue(String CookieName)
{
	int Index = this->IdHTTP->CookieManager->CookieCollection->GetCookieIndex(CookieName);
	if (Index == -1)
	{
		return NULL;
	}else
	{
		return this->IdHTTP->CookieManager->CookieCollection->operator [](Index)->Value;
	}
}

String TNetClient::GetPhoneHtml(String URL)
{
	try
	{
		this->IdHTTP->Request->UserAgent = "Mozilla/5.0 (Linux; U; Android 4.0.3; ko-kr; LG-L160L Build/IML74K) AppleWebkit/534.30 (KHTML, like Gecko) Version/4.0 Mobile Safari/534.30";
		String ret = this->IdHTTP->Get(URL);
		ret = StringReplace(ret,"&amp;","&",TReplaceFlags()<<rfReplaceAll);
		return ret;
	}
	catch(...)
	{
		//ShowMessage("服务器连接失败，请稍后重试");
		return NULL;
	}
}

String TNetClient::GetHtml(String URL)
{
	try
	{
		this->IdHTTP->Request->UserAgent = "Mozilla/5.0 (Windows NT 6.1; rv:29.0) Gecko/20100101 Firefox/29.0";
		String ret = this->IdHTTP->Get(URL);
		ret = StringReplace(ret,"&amp;","&",TReplaceFlags()<<rfReplaceAll);
		return ret;
	}
	catch(...)
	{
		//ShowMessage("服务器连接失败，请稍后重试");
		return NULL;
	}
}

void TNetClient::SetCookies(String nDomain,String InputCookieRaw)
{
	TStringList *sList = new TStringList();
	sList->Delimiter = ';'; // 設定要分割的字元
	sList->DelimitedText = InputCookieRaw;

	for (int i = 0; i < sList->Count; i++)
	{
	if (!sList->Strings[i].Length()) continue;

	TIdCookie *NewCookie = this->IdHTTP->CookieManager->CookieCollection
							->AddClientCookie(sList->Strings[i]);
	 NewCookie->Domain = nDomain;
	}
	delete sList;
}

String TNetClient::GetCookiesRaw()
{
	String nReturn = "";
	for (int i = 0; i < this->IdHTTP->CookieManager->CookieCollection->Count; i++) {
		TIdCookie *CurrentCookie = this->IdHTTP->CookieManager->CookieCollection->operator [](i);
		nReturn += ( CurrentCookie->CookieName + "=" + CurrentCookie->Value +";");
	}
	return nReturn;
}
