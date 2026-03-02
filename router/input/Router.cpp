/******************************************************************************/
//	ファイル名			Router.cpp
//	システム名			ＤＫ−Ｍｘ （共通）
//	プログラム種別		ルータ操作クラス
//	概要				ルータ操作クラス
//	作成者				林エンジニアリング
//	バージョン				02/01/30				R 1.000
/*******************************************************************************

	※このクラスは　ISDNルータ(RT103i,RTA52i,RT52pro)の読み書き用のクラスである

*/
#include "StdAfx.h"
#include "Router.h"
#include "DkTextFile.h"
#include <ATLBASE.H>

int g_line;	// デバッグ用
CString sBuf;
CString	sCr = "\r\n";

const char *szLine[]   = {"isdn","l64", "l128"};

//------ プロバイダのプライマリDNS --------------------------------
static BYTE g_bDNS1[4][4] ={					// 個別契約
							{211,129, 14,134},	// OCN関東
							{211,129, 12, 43},	// OCN関西
							{202,239, 61, 62},	// 旧東京イ
							{  8,  8,  8,  8}};	// GooglePB
//------ プロバイダのセカンダリDNS --------------------------------
static BYTE g_bDNS2[4][4] ={					// 個別契約
							{211,129, 12, 43},	// OCN関東
							{211,129, 14,134},	// OCN関西
							{202,239, 61, 51},	// 旧東京イ
							{  8,  8,  4,  4}};	// GooglePB

#define	SEARCH_CHR		" \t\n"

//	コンストラクタ
//		pDkLan : DK-LAN and OA-LAN set
//		pDkLanには ip,mask,gate をセットする
//		pLogにはログファイル出力クラス
CDkRouter::CDkRouter(SDkMcnInfo *pDkLan, CDkMxFile *pLog/*=NULL*/)
{
	m_nArray		= 0;
	m_nCurrentPP	= 0;			// 処理中のPP
	m_fReadRtType	= FALSE;
	m_fAnonymous	= FALSE;		// アノニマス処理中フラグ
	m_pLogFile		= pLog;			// ログファイルpointer
	m_fSendBin		= FALSE;		// 設定ファイル送信時に".bin"ファイルを送るか否か？
	
	m_fSisRoboVwM	= FALSE;
	m_nStockVwNo	= 0;
	m_fCantTypeRead = FALSE;
	
	ZeroMemory(m_szStockVwID,	sizeof(m_szStockVwID));
	ZeroMemory(m_szStockVwPass, sizeof(m_szStockVwPass));
	
	ZeroMemory(&m_sRouter, sizeof(m_sRouter));
	// DK-LAN backup
	memset(m_pDkLan, 0, sizeof(m_pDkLan));
	if ( pDkLan != NULL ) {
		memcpy(&m_pDkLan[0], pDkLan++, sizeof(SDkMcnInfo));	// DK LAN　Set
		memcpy(&m_pDkLan[1], pDkLan  , sizeof(SDkMcnInfo));	// OA LAN　Set
	}
	
	ZeroMemory(szRtType, sizeof(szRtType));
	int n = 0;
	szRtType[n++] =RT100i;
	szRtType[n++] =RT103i;
	szRtType[n++] =RT52p;
	szRtType[n++] =RTA54i;
	szRtType[n++] =RTA55i;
	szRtType[n++] =RTA52i;
	szRtType[n++] =RT57i;
	szRtType[n++] =RT200i;
	szRtType[n++] =RTX1000;
	szRtType[n++] =RTX1000_OA;
	szRtType[n++] =RTX1100;
	szRtType[n++] =RTX1100_OA;
	szRtType[n++] =RT58i;
	szRtType[n++] =RTX1200;
	szRtType[n++] =RTX1200_OA;
	szRtType[n++] =RTX1210;
	szRtType[n++] =RTX1210_OA;
	szRtType[n++] =RTX830;
	szRtType[n++] =RTX830_OA;
	szRtType[n++] =RTX1220;
	szRtType[n++] =RTX1220_OA;

//===== ↓クラウドIP電話対応 ============================================================
	m_nFqdnCnt = 0;
	m_strFqdnCmb = NULL;
	m_strNowSelFqdn = "";
	
	// ルータ情報ファイルから設定内容を読み込む
	GetRouterInfo();
	
	// (DK様提供)FQDN一覧リストを読み込み
	if ( !GetFqdnInfo(&m_strFqdnArray, &m_nFqdnCnt) )
	{
		DebDisp("FQDN一覧リストの取得に失敗しました");
	}
	
	// (HE作成)FQDN取得情報ファイルを読み込み(CMS0332.ini)
	char szNowFqdn[256] = {0};
	if ( !m_cRtPassEx.GetNowFqdnIp(szNowFqdn, &m_strIniGetIpArray) )
	{
		DebDisp("FQDN取得情報ファイルの取得に失敗しました");
	}
	m_strRouterSendIpArray.Copy(m_strIniGetIpArray);	// 実際に送る用
	
	// 現在選択されているFQDNの選定
	if ( m_nFqdnCnt > 0 || lstrlen(szNowFqdn) > 0 )	// 選択されていたFQDN?
	{	
		m_strNowSelFqdn = szNowFqdn;
		
		for ( int i = 0; i < m_nFqdnCnt; i++ )
		{
			if ( m_strFqdnArray.GetAt(i).Compare(szNowFqdn) == 0 )	// FQDN一覧の中にあるならそれを現在選択しているものとする
			{
				m_strNowSelFqdn = szNowFqdn;
				break;
			}
		}
		if ( i >= m_nFqdnCnt && m_nFqdnCnt > 0 )	// 過去に取得成功記憶していたFQDNが現在の一覧にないorFQDN取得情報がない
		{
			m_strNowSelFqdn = m_strFqdnArray[0];	// 先頭をFQDNを選択していることにする
		}
	}
	else
	{
		m_strNowSelFqdn = "";
	}
//===== ↑クラウドIP電話対応 ============================================================
}

CDkRouter::~CDkRouter()
{
	if ( m_strFqdnCmb == NULL )
		return;

	if ( m_nFqdnCnt > 0 )
	{
		for (int i = 0; i < 2; i++)
			delete[] m_strFqdnCmb[i];
		delete[] m_strFqdnCmb;
	}
}

//----------------------------------------------------------------------------
//	<概要>	RouterInfo.infからADMIN_PASSを取得
//	<引数>	char *szPass = 指定されてきたパスワード文字列(指定無しならNULL)
//	<戻値>	char ADMIN_PASS(取得失敗の場合はデフォルトパス、指定されてきた場合はそのまま返す)
//----------------------------------------------------------------------------
char* CDkRouter::GetAdminPass(char *szPass)
{
	static char szRouterPass[256] = {0};
	
	if ( lstrcmp(szPass, "") == 0 ) 
	{
		strncpy0(szRouterPass, (CStoS)m_strADMIN_PASS, sizeof(szRouterPass));
		
		CString str;
		if ( isMentMod() )
		{
			str.Format("passをADMIN_PASSの%sにしました", szRouterPass);
			DebDisp(str);
		}
	}
	else
	{
		strncpy0(szRouterPass, szPass, sizeof(szRouterPass));	// パスワードの指定があるならそれでいい
	}
	
	return szRouterPass;
}

//	TFTP用ファイル名を作成
char *CDkRouter::GetFileName(int no, int read_write)
{
	const char *ROUTER_FILE_NAME = "%s\\%s\\router%02d%c.dat";				//	router??
	wsprintf(m_szFileName, ROUTER_FILE_NAME, CDkMxBas::GetBaseDir(), ROUTER_FILE_DIR, no,
			(read_write == eWrite) ? 'W' : 'R');
	return m_szFileName;
}

//	ISDNの処理をする
int CDkRouter::ProcISDN(char *buf)
{
	const char 	*local		= "local",
				*remote		= "remote",
				*address	= "address",
				*call		= "call",
				*arrive		= "arrive",
				*bri1		= "bri1",
				*forced		= "forced",
				*disconnect	= "disconnect",
				*time		= "time";
	char		*p;
	
	p = strtok(NULL, SEARCH_CHR);
	
	//	local
	if ( p && strncmp(p, local, lstrlen(local)) == 0 )
	{
		//	address
		p = strtok(NULL, SEARCH_CHR);
		if (p && strncmp(p, address, lstrlen(address)) == 0)
		{
			if ( (RT57or58 || GetRtCategory(m_sRouter.kind) == eRtType1000 || GetRtCategory(m_sRouter.kind) == eRtType1000_OA ) &&
				p && strncmp(p, bri1, lstrlen(bri1)) )	// RT57iの時は"bri1"も取り除く
			{
				p = strtok(NULL, SEARCH_CHR);	//	bri1
			}
			
			p = strtok(NULL, SEARCH_CHR);		//	tel
			strncpy0(m_sRouter.port[0].Tel, p, sizeof(m_sRouter.port[0].Tel));
			
			m_sRouter.port[0].kaisen = eKaisenINS64;	// INS64が回線種別として選択されている証
		}
	}
	else if ( p && strncmp(p, remote, lstrlen(remote)) == 0 )	// remote
	{
		p = strtok(NULL, SEARCH_CHR);	// address
		if (p && strncmp(p, address, lstrlen(address)) == 0)
		{
			p = strtok(NULL, SEARCH_CHR);	//	call or arrive
			if (p && strncmp(p, call, lstrlen(call)) == 0)
			{
				p = strtok(NULL, SEARCH_CHR);
				if ( p ) {	// tel
					strncpy0(m_sRouter.con[m_nCurrentPP].Tel, p, sizeof(m_sRouter.con[m_nCurrentPP].Tel));
				}
				// SIS05通信先設定であるという判断
				if ( lstrcmp(p, SIS05_TEL) == 0 )
				{
					ZeroMemory(m_sRouter.con[m_nCurrentPP].Tel, sizeof(m_sRouter.con[m_nCurrentPP].Tel));
				}
			}
			else if (p && strncmp(p, arrive, lstrlen(arrive)) == 0)	// DialupとかPHSの場合がこれに該当する
			{	
				p = strtok(NULL, SEARCH_CHR);
				if ( p ) {	// tel
					strncpy0(m_sRouter.con[m_nCurrentPP].Tel, p, sizeof(m_sRouter.con[m_nCurrentPP].Tel));
				}
			}
		}
	}
	else if ( p && strncmp(p, forced, lstrlen(forced)) == 0 )	// forced
	{
		p = strtok(NULL, SEARCH_CHR);		// disconnect
		if (p && strncmp(p, disconnect, lstrlen(disconnect)) == 0)
		{
			p = strtok(NULL, SEARCH_CHR);	//	time
			if (p && strncmp(p, time, lstrlen(time)) == 0)
			{
				p = strtok(NULL, SEARCH_CHR);
				if ( p ) {	// mmm×60
					m_sRouter.shOverTimer = atoi(p) / 60;
				}
			}
		}
	}
	
	return TRUE;
}

//-----------------------------------------------------------------------------
//	<概要>	IPアドレスとマスクを取得する
//	<引数>	char *ip_mask = IPアドレス文字列
//          BYTE *ip	  = IPアドレス格納用
//          BYTE *mask	  = NETMASK格納用
//-----------------------------------------------------------------------------
void CDkRouter::SetIpC2B(char *ip_mask, BYTE *ip, BYTE *mask/*=NULL*/)
{
	char	*p = ip_mask,
			*pst = p;
	//	"x.x.x.x" -> byte[4]
	for (int i = 0; i < 4 && *p != '\0'; p++)	// IPアドレス格納
	{
		if (*p != '.' && *p != '/' && *p != 0)
			continue;
		*p++ = 0;
		*ip++ = (BYTE)atoi(pst);
		pst = p;
		i++;
	}
	if (i < 4 && pst) {
		*ip++ = (BYTE)atoi(pst);
		pst = "";
	}
	
	if ( mask == NULL ) // MASKを構成しない
		return;
	
	if ( pst == NULL || *pst == '\0' ) {
		if ( lstrcmp(ip_mask, "default") == 0 )  // 但しdefaultの場合はNetmaskも全て０
			memset( mask,   0, 4 );
		else
			memset( mask, 255, 4 );	// MASKの表記がないときはすべて255
		return;
	}
	
	int nMask = atoi(pst);	// NETMASK数
	for ( i = 0; i < 4; i++, mask++ )
	{
		if ( nMask >= 8 ) {
			*mask = 255;
		} else {
			if ( nMask > 0 ) {
				for ( int n = 7; nMask != 0; n--, nMask-- ) {
					(*mask) |= (1<<(n));
				}
			} else {
				*mask = 0;
			}
		}
		nMask -= 8;
	}
}

//	Byte IP -> Char Ip/mask	(同じwsprint内で2回使わないこと。使うならGetIpB2C_Secondを使え)
char *CDkRouter::GetIpB2C(BYTE *ip, BYTE *mask/*=NULL*/)
{
	static char szWkIp[20];
	
	//	ip set
	wsprintf(szWkIp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	if (mask == NULL || *mask == 0)
		return szWkIp;
	//	mask set
	int	nMask = 0;
	//	"x.x.x.x" -> byte[4]
	for (int i = 0; i < 4; i++, mask++)
	{
		for (int n = 0; n < 8; n++)
			if ((256 - (1<<n)) == *mask)
				break;
			nMask += (8 - n);
	}
	wsprintf(szWkIp + lstrlen(szWkIp), "/%d", nMask);

	return szWkIp;
}

//	Byte IP -> Char Ip/mask
char *CDkRouter::GetIpB2C_Second(BYTE *ip, BYTE *mask/*=NULL*/)	// wsprintfでGetIpB2Cを一度に２回つかえないのでこれをつかう
{
	static char szWkIp2[20];

	//	ip set
	wsprintf(szWkIp2, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	if (mask == NULL || *mask == 0)
		return szWkIp2;
	//	mask set
	int	nMask = 0;
	//	"x.x.x.x" -> byte[4]
	for (int i = 0; i < 4; i++, mask++)
	{
		for (int n = 0; n < 8; n++)
			if ((256 - (1<<n)) == *mask)
				break;
			nMask += (8 - n);
	}
	wsprintf(szWkIp2 + lstrlen(szWkIp2), "/%d", nMask);

	return szWkIp2;
}

//	データを書き出す
BOOL CDkRouter::_Put(CString strCmd)
{
	if ( !_Put((CStoS)strCmd) )
		return FALSE;
	
	return TRUE;
}

BOOL CDkRouter::_Put(char *buf, BOOL fKaigyo/*=TRUE*/)
{
	CString strBuf;
	
	strBuf = buf;
	
	strBuf.Replace("\n", "");	// ユーザIDなどで改行コードが入っている可能性があるため事前に削除
	
	if ( fKaigyo )
	{
		strBuf += "\n";
	}
	
	lstrcpy(m_szDebug, (CStoS)strBuf);
	
	int n = fputs((CStoS)strBuf, m_Fp);
	return (n != EOF);
}

BOOL CDkRouter::_Put(const char *buf[], int n, BOOL fKaigyo/*=TRUE*/)
{
	for (int i = 0; i < n; i++)
		if (!_Put((char*)buf[i], fKaigyo))
			return FALSE;
	return TRUE;
}

//	データを書き出す
BOOL CDkRouter::_PutTit(char *buf)
{
	char	szBuf[256];
	if (!_Put(""))
		return FALSE;
	if (*buf == '#')
		strncpy0(szBuf, buf, sizeof(szBuf));
	else
		wsprintf(szBuf, "# %s", buf);
	if (!_Put(szBuf))
		return FALSE;
	if (!_Put(""))
		return FALSE;
	return TRUE;
}

//	PP の前処理
BOOL CDkRouter::PutPpHead(char *tit)
{
	if ( RT57or58 ) {
		if (!_Put("rip use on"))
			return FALSE;
	}

	const char *pCmd01[] = {
		"pp disable all",
	};

	if (!_PutTit(tit))
		return FALSE;
	
	if (!_Put(pCmd01, DK_Number(pCmd01)))
		return FALSE;
	
	return TRUE;
}

//	文字変換 '-' -> ' '  and tolower
void CDkRouter::_Conv(char *inout)
{
	for (;*inout; inout++) {
		if (*inout == '-')
			*inout = ' ';
		*inout = tolower(*inout);
	}
}

//-----------------------------------------------------------------------------
//	<概要>	プロバイダの番号取得
//	<引数>	BYTE *pDNS1 = プライマリDNSのIPアドレスポインタ
//	<戻値>	int 0.個別契約　1.OCN関東　2.OCN関西　3.旧東京
//-----------------------------------------------------------------------------
int CDkRouter::GetProviderNo(char *szDNS1, BYTE *pDNS1/*=NULL*/, BYTE *pDNS2/*=NULL*/)
{
	char szBuf[256];
	for ( int i = 0; i < 3; i++ ) {	// プライマリDNSがプロバイダの既定アドレスと同じか？
		wsprintf(szBuf, "%d.%d.%d.%d", g_bDNS1[i][0], g_bDNS1[i][1], g_bDNS1[i][2], g_bDNS1[i][3]);
		if ( strcmp(szDNS1, szBuf) == 0 ) {	// プライマリDNSがプロバイダの既定アドレスと合致
			if ( pDNS1 != NULL )
				CopyMemory(pDNS1, g_bDNS1[i], sizeof(BYTE) * IP_BYTE);    // そのプロバのセカンダリDNSのセット
			if ( pDNS2 != NULL )
				CopyMemory(pDNS2, g_bDNS2[i], sizeof(BYTE) * IP_BYTE);    // そのプロバのセカンダリDNSのセット
			return (i + 1);
		}
	}
	
	return 0;	// 見つからんかった
}

//	プログラム起動
BOOL CDkRouter::_Run(char *pg)
{
	STARTUPINFO				stinfo;
	PROCESS_INFORMATION		pinfo;
	BOOL					b;
	
	memset(&stinfo, 0, sizeof(stinfo));
	stinfo.cb			= sizeof(STARTUPINFO);
	stinfo.dwFlags		= STARTF_USESHOWWINDOW;
	stinfo.wShowWindow	= SW_HIDE;
	
	DebDisp(pg);	// コマンドをログに出力
	
	b = CreateProcess(NULL, pg, NULL, NULL, FALSE, 0, NULL, NULL, &stinfo, &pinfo);
	DWORD dwReturn;
	for ( int cnt = 0; cnt < 100; cnt++ )
	{
		DoIdle(10);
		dwReturn = WaitForSingleObject(pinfo.hProcess, 100);
		if ( dwReturn != WAIT_TIMEOUT )
			break;
	}
	
	if ( dwReturn == WAIT_TIMEOUT )
		return FALSE;
	else
		return TRUE;
}

//	文字列がファイルに含まれるかチェックする
BOOL CDkRouter::CheckResult(char *fname, CStringArray *pStrCheck/*=NULL*/)
{	
	CStringArray cDefCheckWord;	// 成功時にリダイレクトファイルに書き込まれる文字列(標準)
	if ( pStrCheck == NULL )
	{
		pStrCheck = &cDefCheckWord;
		pStrCheck->Add("successful");
		pStrCheck->Add("正常に完了し");
	}
	
	CDkFileRap f(eReportRead | eReportWrite);
	char	buf[512];
	
	BOOL b = f.Open(fname, CFile::modeRead);
	if (!b)
		return FALSE;
	
	memset(buf, 0, sizeof(buf));
	
	//	文字を読む
	f.Read(buf, sizeof(buf));
	
	for ( int i = 0; i < pStrCheck->GetSize(); i++ )
	{
		if ( strstr(buf, (CStoS)pStrCheck->GetAt(i)) != NULL )
			return TRUE;
	}
	
	return FALSE;
}

int CDkRouter::DebDisp(CString strMsg)
{
	return DebDisp((CStoS)strMsg);
}

//----------------------------------------------------------------------------
//	<概要>	読み取り専用解除
//	<引数>	CString strPath = 
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::ReadOnlyBreak(CString strPath)
{
	DWORD dwAttri;
	for ( int i = 0; i < 3; i++ )
	{
		dwAttri = GetFileAttributes(strPath);
		if ( dwAttri != 0xFFFFFFFF ) {	// 読み取り専用でなくする
			dwAttri &= 0xFFFFFFFE;
			SetFileAttributes(strPath,dwAttri);
			DebDisp("%sの読み取り専用解除", strPath);
			break;
		}
		else
		{
			Sleep(500);
			continue;
		}
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	ルータのカテゴリの取得
//	<引数>	int nRtType = ルータタイプ
//	<戻値>	int ルータのカテゴリ
//----------------------------------------------------------------------------
int CDkRouter::GetRtCategory(int nRtType)
{
	int nRtCategory = 0;
	
	switch ( m_sRouter.kind )  // ルータ種別
	{
	case eRT103i:		case eRT52pro:		case eRTA52i:
		nRtCategory = eRtTypeISDN;		// ＩＳＤＮルータ
		break;	
	case eRTA54i:		case eRTA55i:		case eRT57i:		case eRT58i:
		nRtCategory = eRtTypeBB;		// ブロードバンドルータ
		break;
	case eRTX1000:		case eRTX1100:		case eRTX1200:		case eRTX1210:	case eRTX830:	case eRTX1220:
		nRtCategory = eRtType1000;		// IPSEC対応ルータ
		break;
	case eRTX1000_OA:	case eRTX1100_OA:	case eRTX1200_OA:	case eRTX1210_OA:	case eRTX830_OA:	case eRTX1220_OA:
		nRtCategory = eRtType1000_OA;	// IPSEC対応ルータ_OA
		break;
	}
	
	return nRtCategory;
}

//----------------------------------------------------------------------------
//	<概要>	デバッグログ出力処理
//	<引数>	LPSTR lpszFmt = wsprintfの引数フォーマットと同じ
//	<戻値>	int 
//----------------------------------------------------------------------------
int CDkRouter::DebDisp(LPSTR lpszFmt, ...)
{
	CString			strMsg;
	LPSTR			*lppParam;
	char			szBuff[3000];
	
	lppParam = ((LPSTR*)&lpszFmt) + 1;
    vsprintf(szBuff, lpszFmt, (LPSTR)lppParam);
	strMsg = szBuff;
	
	CString		strDispMsg;
	char		szLog[2048] = {0};
	SYSTEMTIME	sSystem;
// 	LPVOID		szErrMsg;
	
// 	if ( nErrCode > -1 )	// エラーコードがSETされてきた
// 	{
// 		FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
// 						NULL,
// 						nErrCode,									// エラーコード
// 						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	// デフォルト言語
// 						(LPTSTR)&szErrMsg,
// 						0,
// 						NULL);
// 		
// 		strDispMsg.Format("%s エラーコード=%d(0x%X) %s", strMsg, nErrCode, nErrCode, szErrMsg);
// 	} else {
		strDispMsg = strMsg;
// 	}
	
	GetLocalTime(&sSystem);
	
	wsprintf(szLog, "%04d/%02d/%02d %02d:%02d:%02d.%03d << %.256s",
					sSystem.wYear,
					sSystem.wMonth,
					sSystem.wDay,
					sSystem.wHour,
					sSystem.wMinute,
					sSystem.wSecond,
					sSystem.wMilliseconds,
					strDispMsg);
	
	lstrcat(szLog, "\r\n");	// ファイルエラーログへ書けるなら書き込む
	
	int ret = 0;
	if ( m_pLogFile != NULL )
	{		
		if ( m_pLogFile->m_hFile != (UINT)CFile::hFileNull ) {
			m_pLogFile->Write(szLog, lstrlen(szLog));
		}
	}
	
	char szExePath[MAX_PATH], szExeName[MAX_PATH];
	GetModuleFileName(NULL, szExePath, sizeof(szExePath));
	_splitpath(szExePath, NULL, NULL, szExeName, NULL);	// 実行ファイル名のみ取得
	
// 	if ( pMxBas->isDebug() || isMentMod() )
		OutputDebugString((CString)szExeName + ">CDkRouter >> " + strDispMsg);
	
	return lstrlen(szLog);
}

//-----------------------------------------------------------------------------
//	<概要>	IPアドレス形式の文字列をBYTE型の配列に分配する
//	<引数>	char *szIp	= IPアドレスの文字列
//          BYTE *pIp	= 分配したIPアドレスを入れる配列の先頭アドレス
//	<戻値>	TRUE = 成功    FALSE = 失敗
//-----------------------------------------------------------------------------
BOOL CDkRouter::ConvertIpDelmit(char *szIp, BYTE *pIp)
{
	char szNum[5];
	ZeroMemory(pIp, sizeof(BYTE) * IP_BYTE);
	
	int i = 0, p = 0;
	while ( szIp != NULL ) {
		szNum[i] = *szIp;
		if ( szNum[i] == '.' || *szIp == '\0' ) { // フィールドの終端か文字終端
			szNum[i] = '\0';
			pIp[p] = atoi(szNum);
			if ( *szIp == NULL ) break; // 終端
			i = 0, p++, szIp++;
			continue;
		}
		szIp++;
		i++;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	(DK様提供)FQDNリスト情報取得
//	<引数>	CStringArray *pFqdnArray = FQDNリスト格納用
//          int *pFqdnCnt = 取得リスト数格納用)
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::GetFqdnInfo(CStringArray *pFqdnArray, int *pFqdnCnt)
{	
	CString strLine;
	
	// ----------------- DK様提供FFQDN一覧定義ファイル ------------------------
	// ファイルから読む
	char szFqdnInfFile[MAX_PATH];
	wsprintf(szFqdnInfFile, "%s%s", pMxBas->GetBaseDir(), FQDN_DAT_REF_PATH);
	
	pFqdnArray->RemoveAll();
	*pFqdnCnt = 0;
	
	BOOL fFqdnDefStart = FALSE;
	
	while ( TRUE )
	{
		CStdioFile fileFqdnList;
		if ( !fileFqdnList.Open(szFqdnInfFile, CFile::modeRead | CFile::typeText) )
			break;
		
		int nReadCnt = 0;
		// 1行ずつ読み込んでCStringArrayに格納する
		while ( LineRead(&fileFqdnList, &strLine) )
		{
			if ( strLine == "# FQDN_LIST_END" )
				break;
			
			if ( fFqdnDefStart )
			{
				nReadCnt++;
				if ( nReadCnt == 1 )	// 日付
					continue;
			}
			
			if ( strLine == "# FQDN_LIST" )
			{
				fFqdnDefStart = TRUE;
				continue;
			}
			
			if ( strLine.Left(1) == "#" || strLine.Left(1) == ";" || strLine.IsEmpty() || !fFqdnDefStart )
				continue;
			
			pFqdnArray->Add(strLine);
		}
		
		*pFqdnCnt = pFqdnArray->GetSize();
		if ( *pFqdnCnt > 0 )
		{
			m_strFqdnCmb = new CString*[2];	// コンボセット用配列にいれる　
			for ( int i = 0; i < 2; i++ ) 
			{
				m_strFqdnCmb[i] = new CString[*pFqdnCnt];
				for ( int n = 0; n < *pFqdnCnt; n++ )
				{
					m_strFqdnCmb[i][n] = pFqdnArray->GetAt(n);
				}
			}
		}
		
		fileFqdnList.Close();	// ファイルを閉じる
		break;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------------
//	<概要>	RouterInfo.infから情報を取得し、メンバ変数へセットする(取得できなかったらデフォルト値セット)
//	<引数>	char *szPass = 指定されてきたパスワード文字列(指定無しならNULL)
//	<戻値>	char ADMIN_PASS(取得失敗の場合はデフォルトパス、指定されてきた場合はそのまま返す)
//----------------------------------------------------------------------------------------------------------
BOOL CDkRouter::GetRouterInfo(int nIpIdx/*=0*/)
{	
	m_strPRE_SHARED_KEY.Empty();
 	m_strADMIN_PASS.Empty();
 	m_strAUTH_ID.Empty();
 	m_strAUTH_PASS.Empty();
	m_strADMIN_PASS_HALL.Empty();
	
	// ファイルから読む
	char szRtInfFile[MAX_PATH];
	wsprintf(szRtInfFile, "%s\\%s\\%s", pMxBas->GetBaseDir(), ROUTER_CONFIG_DIR, MX_RT_INFO);
	
	BOOL fGetResult = FALSE;
	
	while ( TRUE )
	{
		CString	strFile;
		//const int KEY_LEN = 64;			//!< ログインパスワード長MAX
		// デフォルトを読む(従来フォーマット分)
		CDkTextFile	cFile;
		BOOL	b;
		char	*pt;
		char	szPre[KEY_LEN] = {0}, szAdmin[KEY_LEN] = {0}, szAuth[KEY_LEN] = {0}, szAuthPass[KEY_LEN] = {0}, szAdmin_Hall[KEY_LEN] = {0};
		do {
			b = cFile.OpenFile(szRtInfFile, CDkTextFile::eFileRead);
			if (!b)
				break;
			
			b = FALSE;
			pt = cFile.ReadLine();
			if (pt == NULL)
				break;
			
			pt = strtok(pt, "',\n");
			if (pt == NULL)
				break;
			lstrcpy(szPre, pt);
			
			pt = strtok(NULL, "',\n");
			if (pt == NULL)
				break;
			lstrcpy(szAdmin, pt);
			
			pt = strtok(NULL, "',\n");
			if (pt == NULL)
				break;
			lstrcpy(szAuth, pt);
			
			pt = strtok(NULL, "',\n");
			if (pt == NULL)
				break;
			lstrcpy(szAuthPass, pt);
			
			pt = strtok(NULL, "',\n");
			if (pt == NULL)
				lstrcpy(szAdmin_Hall, "*");	// 「ADMIN_PASS_HALL」のデフォルト値は「」とする。本パラメータが存在しない場合は「」がセットされているとみなす。
			else
				lstrcpy(szAdmin_Hall, pt);
			
			b = TRUE;
			
			m_strPRE_SHARED_KEY = szPre;
			m_strADMIN_PASS		= szAdmin;
			m_strAUTH_ID		= szAuth;
			m_strAUTH_PASS		= szAuthPass;
			m_strADMIN_PASS_HALL= szAdmin_Hall;
			
			fGetResult = TRUE;
		} while (FALSE);
		
		if (b)
		{
			char *pSep = "',";
						
			// そのＩＰの最終書き込みパスワードを探す
			char *pt = cFile.ReadLine();
			while (pt != NULL)
			{
				//! 該当ルータの最新パスワード定義がある場合、それを使ってログインする
				do {			
					int nReadIdx = (pt[0] - '0');
					
					pt = strtok(pt+1, pSep);
					if (pt == NULL)
						break;
					lstrcpy(szPre, pt);
					
					pt = strtok(NULL, pSep);
					if (pt == NULL)
						break;
					lstrcpy(szAdmin, pt);
					
					pt = strtok(NULL, pSep);
					if (pt == NULL)
						break;
					lstrcpy(szAuth, pt);
					
					pt = strtok(NULL, pSep);
					if (pt == NULL)
						break;
					lstrcpy(szAuthPass, pt);
					
					pt = strtok(NULL, "',\n");
					if (pt == NULL)
						lstrcpy(szAdmin_Hall, "*");	// 「ADMIN_PASS_HALL」のデフォルト値は「」とする。本パラメータが存在しない場合は「」がセットされているとみなす。
					else
						lstrcpy(szAdmin_Hall, pt);
					
					if ( nReadIdx == nIpIdx )
					{
						m_strPRE_SHARED_KEY = szPre;
						m_strADMIN_PASS		= szAdmin;
						m_strAUTH_ID		= szAuth;
						m_strAUTH_PASS		= szAuthPass;
						m_strADMIN_PASS_HALL= szAdmin_Hall;
						fGetResult = TRUE;
					}
				} while (FALSE);
				
				pt = cFile.ReadLine();
			}
		}
		break;
	}
	
	if ( !fGetResult )
	{
		DebDisp("RouterInfo.infの読み込みに失敗しました");
		
		m_strPRE_SHARED_KEY	= "*";	// latestkey	()
		m_strADMIN_PASS		= "*";		// latest		()
		m_strAUTH_ID		= "*";		// latestid		()
		m_strAUTH_PASS		= "*";	// latestpass	()
		m_strADMIN_PASS_HALL= "*";		// latest		()
	}
	
	CString str;
	if ( isMentMod() )
	{
		str.Format("PRE_SHARED_KEY=%s ADMIN_PASS=%s AUTH_ID=%s AUTH_PASS=%s m_strADMIN_PASS_HALL=%s", m_strPRE_SHARED_KEY, m_strADMIN_PASS, m_strAUTH_ID, m_strAUTH_PASS, m_strADMIN_PASS_HALL);
		DebDisp(str);
	}
	
	return fGetResult;
}
//----------------------------------------------------------------------------
//	<概要>	ルータのタイプがこのＰＧで編集可なのかを判断する
//	<引数>	int nRtType = ルータのタイプ
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::IsRtTypeEditable(int nRtType)
{
	char szExePath[MAX_PATH], szExeName[MAX_PATH];
	GetModuleFileName(NULL, szExePath, sizeof(szExePath));
	_splitpath(szExePath, NULL, NULL, szExeName, NULL);	// 実行ファイル名のみ取得
	
	if ( nRtType == eRT100i || nRtType == eRT200i || nRtType < eRT103i || nRtType > eRTmax )	// もともと対応してない
		return FALSE;
	
	if ( lstrcmp(szExeName, "CMS0360") == 0 )		// ISDN,BBルータ専用ルータ設定
	{
		if ( GetRtCategory(m_sRouter.kind) == eRtType1000 || GetRtCategory(m_sRouter.kind) == eRtType1000_OA )
		{
			m_fCantTypeRead = TRUE;
			return FALSE;
		}
	}
	else if ( lstrcmp(szExeName, "CMS0330") == 0 )	// IPSEC対応ルータ専用ルータ設定
	{
		if ( !(GetRtCategory(m_sRouter.kind) == eRtType1000 || GetRtCategory(m_sRouter.kind) == eRtType1000_OA) )
		{
			m_fCantTypeRead = TRUE;
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	
	return TRUE;
}

// 制限文字長内の文字配列に文字列をコピー(制限文字長以上が来たら切り捨ててワトソンがでないようにしてほしい 根木様の要望 07/02/09)
int CDkRouter::strncpy0(char *pszOUT, char *pszIN, int nLen)	// ※ポインタのsizeofを渡さないように
{
	if ( nLen <= 0 )
		return 0;
	
	strncpy(pszOUT, pszIN, nLen);
	
	pszOUT[nLen-1] = '\0';	// 文字列の最後には必ず終端文字を入れる
	return lstrlen(pszOUT);
}

//----------------------------------------------------------------------------
//	<概要>	現ルータＩＰ情報ファイルにルータＩＰを書き込む
//	<引数>	BYTE *pIp = ＩＰ
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::PutRouterIpFile(BYTE *pIp)
{
	CString strIp;
	strIp.Format("%d.%d.%d.%d", pIp[0], pIp[1], pIp[2], pIp[3]);
	
	char szFilePath[MAX_PATH];
	wsprintf(szFilePath, "%s\\%s\\RouterIp.inf", pMxBas->GetBaseDir(), ROUTER_CONFIG_DIR);
	
	CStdioFile cFile;
	if ( !cFile.Open(szFilePath, CFile::modeCreate | CFile::modeWrite) ) {
		DebDisp("ルータＩＰ情報ファイルのオープンに失敗しました");
		return FALSE;
	}
	
	BOOL b = TRUE;
	
	TRY
	{
		cFile.WriteString(strIp);
	}
	CATCH( CFileException, e )
	{
		b = FALSE;
		DebDisp("ルータＩＰ情報ファイルへのＩＰの書き込みに失敗しました");
		printf("badly... %d\n", e);
	}
	END_CATCH
		
	CString str;
	str.Format("ルータＩＰ情報ファイルへのＩＰ情報を書き込みました %s", strIp);
	DebDisp(str);
	
	cFile.Close();
	
	return b;
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
// 
//↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ クラウドリニューアル対応  ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
// 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

//----------------------------------------------------------------------------
//	<概要>	各種.iniから取得
//	<引数>	char		 *szNowFQDN		= 現在取得成功しているFQDN
//          CStringArray *pIpArray		= FQDNで取得したIP一覧
//          int			 *pEnable		= クラウドIP有効無効状態(0:無効 1:LAN1 2:LAN3)
//          char		 *szLastSendIp	= 最終送信ルータIP
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::GetPutIniFile(BOOL fRead, SDkKanshiSrvInfo *pInfo)// (char *szNowFQDN, CStringArray *pIpArray, int *pEnable/*=0*/, char *szLastSendIp/*=NULL*/)
{
	char szTmp[2048] = {0};
	char szIniFileCMB0490[MAX_PATH];
	char szIniFileCMS0334[MAX_PATH];
	wsprintf(szIniFileCMB0490, "%s%s", pMxBas->GetBaseDir(), CMB0490_INI_PATH);
	wsprintf(szIniFileCMS0334, "%s%s", pMxBas->GetBaseDir(), CMS0334_INI_PATH);
	
	if ( fRead )	// GET
	{
// 		CFileStatus status;
// 		if (!CFile::GetStatus(szIniFileCMS0334, status))
// 		{
// 			CFile file;
// 			if (file.Open(szIniFileCMS0334, CFile::modeCreate | CFile::modeWrite))
// 				file.Close();
// 		}
		
		ZeroMemory(pInfo, sizeof(SDkKanshiSrvInfo));
		
		::GetPrivateProfileString("CMS0334", "LIFT_STATUS", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->nStatus = atoi(szTmp);
		//===== ↓読み取りのみ ====================================================================
		::GetPrivateProfileString("ROUTER", "TUNNEL_IP1", "", szTmp, sizeof(szTmp), szIniFileCMB0490);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szAddIp1, szTmp);
		::GetPrivateProfileString("ROUTER", "TUNNEL_IP2", "", szTmp, sizeof(szTmp), szIniFileCMB0490);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szAddIp2, szTmp);
		::GetPrivateProfileString("ROUTER", "PROXY_IP1", "", szTmp, sizeof(szTmp), szIniFileCMB0490);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szProxyIp1, szTmp);
		::GetPrivateProfileString("ROUTER", "PROXY_IP2", "", szTmp, sizeof(szTmp), szIniFileCMB0490);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szProxyIp2, szTmp);
		::GetPrivateProfileString("ROUTER", "PROXY_PORT", "", szTmp, sizeof(szTmp), szIniFileCMB0490);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szProxyPort, szTmp);
		//===== ↑読み取りのみ ====================================================================
		::GetPrivateProfileString("CMS0334", "ADD_TUNNEL_NO1", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->nAddTunnelNo = atoi(szTmp);		
		::GetPrivateProfileString("CMS0334", "ADD_TUNNEL_NO2", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->nAddSubTunnelNo = atoi(szTmp);
		::GetPrivateProfileString("CMS0334", "PRE_TUNNEL_NO1", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->nPreTunnelNo = atoi(szTmp);		
		::GetPrivateProfileString("CMS0334", "PRE_TUNNEL_NO2", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->nPreSubTunnelNo = atoi(szTmp);
		::GetPrivateProfileString("CMS0334", "WROTE_TUNNEL_IP1", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szWroteIp1, szTmp);
		::GetPrivateProfileString("CMS0334", "WROTE_TUNNEL_IP2", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szWroteIp2, szTmp);
		::GetPrivateProfileString("CMS0334", "PRE_TUNNEL_IP1", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szPreIp1, szTmp);
		::GetPrivateProfileString("CMS0334", "PRE_TUNNEL_IP2", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			lstrcpy(pInfo->szPreIp2, szTmp);
		
		// サーバーIPファイルの中身		
		::GetPrivateProfileString("ROUTER", "SERVER_IP_COUNT", "", szTmp, sizeof(szTmp), szIniFileCMB0490);
		int nCnt = atoi(szTmp);
		m_sSvIpList.RemoveAll();
		for ( int i = 1 ; i <= nCnt; i++ )
		{// 192.0.2.33,61.114.186.33,www.d-deltanet.com
			CString strRec, strKey;
			strKey.Format("SERVER_IP%d_NEW",i);
			::GetPrivateProfileString("ROUTER", strKey, "", szTmp, sizeof(szTmp), szIniFileCMB0490);
			strRec += (CString)szTmp + ",";
			
			strKey.Format("SERVER_IP%d_OLD",i);
			::GetPrivateProfileString("ROUTER", strKey, "", szTmp, sizeof(szTmp), szIniFileCMB0490);
			strRec += (CString)szTmp + ",";
			
			strKey.Format("SERVER_IP%d_FQDN",i);
			::GetPrivateProfileString("ROUTER", strKey, "", szTmp, sizeof(szTmp), szIniFileCMB0490);
			strRec += (CString)szTmp;
			
			m_sSvIpList.Add(strRec);
		}
		
		::GetPrivateProfileString("CMS0334", "LIFT_PRE_FILE1", "", pInfo->szLiftChgBackPath[0], sizeof(pInfo->szLiftChgBackPath[0]), szIniFileCMS0334);
		::GetPrivateProfileString("CMS0334", "LIFT_PRE_FILE2", "", pInfo->szLiftChgBackPath[1], sizeof(pInfo->szLiftChgBackPath[1]), szIniFileCMS0334);
		::GetPrivateProfileString("CMS0334", "JNET_MENT", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->fJNETment = atoi(szTmp);
		::GetPrivateProfileString("CMS0334", "ADD_JNET_MENT_NO", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->nAddJnetMentNo = atoi(szTmp);	
		
		::GetPrivateProfileString("CMS0334", "DEBUG_IP", "", pInfo->szDebugIp, sizeof(pInfo->szDebugIp), szIniFileCMS0334);
		::GetPrivateProfileString("CMS0334", "DEBUG_NO_CHECK_TUNNEL", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->fDebugNoCheckTunnel = atoi(szTmp);
		::GetPrivateProfileString("CMS0334", "DEBUG_CMD_TIMEOUT", "", szTmp, sizeof(szTmp), szIniFileCMS0334);
		if ( pInfo != NULL )
			pInfo->nDebugCmdRecvTimeOut = atoi(szTmp);
	}
	else	// PUT
	{
		char buffer[32]; // 数値を文字列に変換するためのバッファ
		sprintf(buffer, "%d", pInfo->nStatus);
		DebDisp("LIFT_STATUS=%s", buffer); 
		WritePrivateProfileString("CMS0334", "LIFT_STATUS", buffer, szIniFileCMS0334);
		
		sprintf(buffer, "%d", pInfo->nAddTunnelNo);
		DebDisp("ADD_TUNNEL_NO1=%s", buffer);
		WritePrivateProfileString("CMS0334", "ADD_TUNNEL_NO1", buffer, szIniFileCMS0334);
		
		sprintf(buffer, "%d", pInfo->nAddSubTunnelNo);
		DebDisp("ADD_TUNNEL_NO2=%s", buffer);
		WritePrivateProfileString("CMS0334", "ADD_TUNNEL_NO2", buffer, szIniFileCMS0334);
		
		sprintf(buffer, "%d", pInfo->nPreTunnelNo);
		DebDisp("PRE_TUNNEL_NO1=%s", buffer); 
		WritePrivateProfileString("CMS0334", "PRE_TUNNEL_NO1", buffer, szIniFileCMS0334);
		
		sprintf(buffer, "%d", pInfo->nPreSubTunnelNo);
		DebDisp("PRE_TUNNEL_NO2=%s", buffer);
		WritePrivateProfileString("CMS0334", "PRE_TUNNEL_NO2", buffer, szIniFileCMS0334);
				
// 		for ( int i = 0 ; i < DK_Number(m_cSrvIpList); i++ )
// 		{
// 			CString strTag;
// 			if ( m_cSrvIpList[i].GetSize() )	// 61.114.186.* 置換用
// 			{
// 				strTag.Format("SERVER_IP_DEF1_%d", i);
// 				DebDisp("SERVER_IP_DEF1_%d=%s", i, ConvertStrList(&m_cSrvIpList[i]));
// 				WritePrivateProfileString("CMS0334", strTag, ConvertStrList(&m_cSrvIpList[i]), szIniFileCMS0334);
// 			}
// 			
// 			if ( m_cSrvIpList2[i].GetSize() )	// 113.52.158.* 置換用
// 			{
// 				strTag.Format("SERVER_IP_DEF2_%d", i);
// 				DebDisp("SERVER_IP_DEF2_%d=%s", i, ConvertStrList(&m_cSrvIpList2[i]));
// 				WritePrivateProfileString("CMS0334", strTag, ConvertStrList(&m_cSrvIpList2[i]), szIniFileCMS0334);
// 			}
// 		}
		DebDisp("LIFT_PRE_FILE1=%s", pInfo->szLiftChgBackPath[0]);
		WritePrivateProfileString("CMS0334", "LIFT_PRE_FILE1", pInfo->szLiftChgBackPath[0], szIniFileCMS0334);
		DebDisp("LIFT_PRE_FILE2=%s", pInfo->szLiftChgBackPath[1]);
		WritePrivateProfileString("CMS0334", "LIFT_PRE_FILE2", pInfo->szLiftChgBackPath[1], szIniFileCMS0334);
		
		sprintf(buffer, "%s", pInfo->szPreIp1);
		DebDisp("PRE_TUNNEL_IP1=%s", buffer);
		WritePrivateProfileString("CMS0334", "PRE_TUNNEL_IP1", buffer, szIniFileCMS0334);		
		sprintf(buffer, "%s", pInfo->szPreIp2);
		DebDisp("PRE_TUNNEL_IP2=%s", buffer);
		WritePrivateProfileString("CMS0334", "PRE_TUNNEL_IP2", buffer, szIniFileCMS0334);
		
		sprintf(buffer, "%s", pInfo->szWroteIp1);
		DebDisp("WROTE_TUNNEL_IP1=%s", buffer);
		WritePrivateProfileString("CMS0334", "WROTE_TUNNEL_IP1", buffer, szIniFileCMS0334);		
		sprintf(buffer, "%s", pInfo->szWroteIp2);
		DebDisp("WROTE_TUNNEL_IP2=%s", buffer);
		WritePrivateProfileString("CMS0334", "WROTE_TUNNEL_IP2", buffer, szIniFileCMS0334);
		
		sprintf(buffer, "%d", pInfo->fJNETment);
		DebDisp("JNET_MENT=%s", buffer);
		WritePrivateProfileString("CMS0334", "JNET_MENT", buffer, szIniFileCMS0334);
		
		sprintf(buffer, "%d", pInfo->nAddJnetMentNo);
		DebDisp("ADD_JNET_MENT_NO=%s", buffer);
		WritePrivateProfileString("CMS0334", "ADD_JNET_MENT_NO", buffer, szIniFileCMS0334);
	}
	
	return TRUE;
}

BOOL CDkRouter::LineRead(CStdioFile *pFile, CString *pBuf)
{
	if ( !pFile->ReadString(*pBuf) )
		return FALSE;
	
	pBuf->TrimLeft();
	pBuf->TrimRight();
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	書き込んだconfigファイルにクラウドリニューアル対応の処理を加える
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterJnetMentWrite(SDkRouter *pRouter, CString strTargetFile)
{// CMS0330では書かない
/*	GetPutIniFile(TRUE, &m_sIniFileInf);
	if ( m_sIniFileInf.nAddJnetMentNo == 0 )
	{
		return TRUE;	// J-NET顧客代行対応のconfigは追加されていない
	}
	
	CStdioFile cGetFile;
	CFile cSendFile;
	CString strConvLastFile;
	
	strConvLastFile.Format("%s\\data\\cm\\router\\routerWorkJNET.dat", pMxBas->GetBaseDir());
	
	// 読込用ファイルオープン
	if ( !cGetFile.Open(strTargetFile, CFile::modeRead|CFile::shareDenyWrite|CFile::typeText) )
	{
		DebDisp("ルータ設定読込用ファイルOPEN失敗");
		return FALSE;
	}
	
	// 先行読込で必要な情報を得る
	if ( !PreConfigRead(&cGetFile, TRUE) )
		return FALSE;
	
	// 送信用(編集用)ファイルオープン
	ReadOnlyBreak(strConvLastFile);
	if ( !cSendFile.Open(strConvLastFile, CFile::modeWrite|CFile::shareDenyWrite|CFile::modeCreate) )
	{
		DebDisp("ルータ設定送信用ファイルOPEN失敗");
		return FALSE;
	}
	
	DebDisp("J-NET tunnelの追加処理開始");	
	m_fJnetMentWrote = FALSE;
	if ( !RouterCmdEdit_JnerMent(&cGetFile, &cSendFile) )
		return FALSE;
	
	cGetFile.Close();
	cSendFile.Close();
	
	// 編集したものを本物にする
	if ( !CopyFile(strConvLastFile, strTargetFile, FALSE) ) {
		DWORD n = ::GetLastError();
		return FALSE;
	}
*/	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	書き込んだconfigファイルにクラウドリニューアル対応の処理を加える
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCloudWrite(SDkRouter *pRouter, CString strTargetFile)
{
	GetPutIniFile(TRUE, &m_sIniFileInf);
	if ( m_sIniFileInf.nStatus == SDkKanshiSrvInfo::eNonMode ) {	// 従来通り
		return TRUE;
	}
	
	lstrcpy(m_szTargetRouterIp, GetIpB2C(pRouter->ip));
	
	CStdioFile cGetFile;
	CFile cSendFile;
	CString strConvLastFile;
	
	// サーバーIPファイル内容解析
	if ( !ReadServerIpFile() )
		return FALSE;
	
	int dotPos = strTargetFile.ReverseFind('.');
	CString targetFilePath[4];
	CString newFilePath[4];
	targetFilePath[0] = strTargetFile;
	targetFilePath[1].Format("%s\\data\\cm\\router\\routerWorkB.dat", pMxBas->GetBaseDir());
	targetFilePath[2].Format("%s\\data\\cm\\router\\routerWorkC.dat", pMxBas->GetBaseDir());
	targetFilePath[3].Format("%s\\data\\cm\\router\\routerWorkD.dat", pMxBas->GetBaseDir());
	
	newFilePath[0].Format("%s\\data\\cm\\router\\routerWorkB.dat", pMxBas->GetBaseDir());//strTargetFile.Left(dotPos) + _T("_work") + strTargetFile.Mid(dotPos);// エラーになるかもなので、とりあえずworkファイルに書く
	newFilePath[1].Format("%s\\data\\cm\\router\\routerWorkC.dat", pMxBas->GetBaseDir());
	newFilePath[2].Format("%s\\data\\cm\\router\\routerWorkD.dat", pMxBas->GetBaseDir());
	newFilePath[3].Format("%s\\data\\cm\\router\\routerWorkE.dat", pMxBas->GetBaseDir());
// 	if ( !CopyFile(strTargetFile, newFilePath[0], FALSE) )
// 		return FALSE;

	m_szPublicIp[0] = '\0';
	int i = 0;
	if ( pRouter->port[3].kaisen == eKaisenLTE )
	{
		BYTE *p = pRouter->sLte.ip;
		sprintf(m_szPublicIp, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	}
	else
	{
		for ( i = 0; i < SETU_MAX; i++ ) 
		{
			if ( m_sRouter.ncon[i].no == m_sRouter.m_nAdslNo ) { // 接続先設定構造体
				sprintf(m_szPublicIp, "%d.%d.%d.%d", m_sRouter.ncon[i].fixIp[0], m_sRouter.ncon[i].fixIp[1], m_sRouter.ncon[i].fixIp[2], m_sRouter.ncon[i].fixIp[3]);
				break;
			}
		}
	}
	
	if ( lstrlen(m_szPublicIp) == 0 )
	{
		DebDisp("ルーターパブリックIPの取得に失敗しました");
		return FALSE;
	}
	
	// 送信用(編集用)ファイルオープン
/*	ReadOnlyBreak(newFilePath[0]);
	if ( !cSendFile.Open(newFilePath[0], CFile::modeWrite|CFile::shareDenyWrite|CFile::modeCreate) )
	{
		DebDisp("ルータ設定送信用ファイルOPEN失敗");
		return FALSE;
	}
	cSendFile.Close();
	
	DebDisp("新tunnelの追加処理開始");		
	if ( !RouterCmdEdit_TunnelAdd(&cGetFile, &cSendFile) )
		return FALSE;
				
	cSendFile.Close();
	cGetFile.Close();
*/	
	int nProcStatus[] = {SDkKanshiSrvInfo::eAddTunnel, SDkKanshiSrvInfo::eLiftMode, SDkKanshiSrvInfo::eDelete, SDkKanshiSrvInfo::eProxyAdd};
	BOOL fRet = TRUE;
	for ( i = 0; i < DK_Number(nProcStatus); i++ )
	{
		if ( nProcStatus[i] > m_sIniFileInf.nStatus )	// 未達
			break;
		
		// 読込用ファイルオープン
		if ( !cGetFile.Open(targetFilePath[i], CFile::modeRead|CFile::shareDenyWrite|CFile::typeText) )
		{
			DebDisp("ルータ設定読込用ファイルOPEN失敗");
			return FALSE;
		}
		
		// 先行読込で必要な情報を得る
		if ( !PreConfigRead(&cGetFile, TRUE) )
			return FALSE;
		
		// 送信用(編集用)ファイルオープン
		ReadOnlyBreak(newFilePath[i]);
		if ( !cSendFile.Open(newFilePath[i], CFile::modeWrite|CFile::shareDenyWrite|CFile::modeCreate) )
		{
			DebDisp("ルータ設定送信用ファイルOPEN失敗");
			return FALSE;
		}
		
		switch ( nProcStatus[i] )// m_sIniFileInf.nStatus
		{
		case SDkKanshiSrvInfo::eAddTunnel:
			{
				DebDisp("新tunnelの追加処理開始");		
				if ( !RouterCmdEdit_TunnelAdd(&cGetFile, &cSendFile) )
					return FALSE;
				strConvLastFile = newFilePath[i];
			}
			break;
		case SDkKanshiSrvInfo::eLiftMode:
			{
				DebDisp("オンプレ→リフト切替処理開始");
				if ( !RouterCmdEdit_Change(&cGetFile, &cSendFile))
					return FALSE;
				strConvLastFile = newFilePath[i];
			}
			break;
		case SDkKanshiSrvInfo::eDelete:
			{
				DebDisp("未接続のtunnel削除");
				if ( !RouterCmdEdit_TunnelDelete(&cGetFile, &cSendFile))
					return FALSE;
				strConvLastFile = newFilePath[i];
			}
			break;
		case SDkKanshiSrvInfo::eProxyAdd:
			{
				DebDisp("proxy追加");
				if ( !RouterCmdEdit_ProxyAdd(&cGetFile, &cSendFile))
					return FALSE;
				strConvLastFile = newFilePath[i];
			}
			break;
		}
		
		cSendFile.SeekToEnd();
        cSendFile.Write("save\r\n", 6);
		cSendFile.Close();
		
		cGetFile.Close();
	}
	
// 	cGetFile.Close();
// 	cSendFile.Close();
	
	// 編集したものを本物にする
	if ( !CopyFile(strConvLastFile, strTargetFile, FALSE) ) {
		DWORD n = ::GetLastError();
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	切替パターン①：新tunnelの追加
//	<引数>	char *szIp		= 取得IPアドレス
//          char *szGetFile = 取得ファイル
//          char *szPutFile = 送信ファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCmdEdit_TunnelAdd(CStdioFile *pGetFile, CFile *pSendFile)
{
	int i = 0;
	CString msg;
	CString strPut, str;
	BOOL fAddRouteWrite = FALSE;
	BOOL fAddTunnelWrite = FALSE;
	
	int len = 0;
	
	BOOL fRet = FALSE;
	BOOL fMIRAIGATE = FALSE;
	
	int nReserveNo[50] = {0};
	int nReserveFilter[200] = {0};
	int n = 0, j = 0;
	
	CString strFilterSetIN, strFilterSetOUT;	// filter set用
// 追加2024/06/19
// 	strFilterSetIN.Format( " %d %d %d %d %d %d ", 2600 + m_sIniFileInf.nAddTunnelNo, 2600 + m_sIniFileInf.nAddSubTunnelNo, 2700 + m_sIniFileInf.nAddTunnelNo, 2700 + m_sIniFileInf.nAddSubTunnelNo, 2900 + m_sIniFileInf.nAddTunnelNo, 2900 + m_sIniFileInf.nAddSubTunnelNo);
// 	strFilterSetOUT.Format(" %d %d %d %d %d %d ", 2650 + m_sIniFileInf.nAddTunnelNo, 2650 + m_sIniFileInf.nAddSubTunnelNo, 2750 + m_sIniFileInf.nAddTunnelNo, 2750 + m_sIniFileInf.nAddSubTunnelNo, 2950 + m_sIniFileInf.nAddTunnelNo, 2950 + m_sIniFileInf.nAddSubTunnelNo);
	strFilterSetIN.Format( " %d %d %d %d ", 2600 + m_sIniFileInf.nAddTunnelNo, 2600 + m_sIniFileInf.nAddSubTunnelNo, 2700 + m_sIniFileInf.nAddTunnelNo, 2700 + m_sIniFileInf.nAddSubTunnelNo);
	strFilterSetOUT.Format(" %d %d %d %d ", 2650 + m_sIniFileInf.nAddTunnelNo, 2650 + m_sIniFileInf.nAddSubTunnelNo, 2750 + m_sIniFileInf.nAddTunnelNo, 2750 + m_sIniFileInf.nAddSubTunnelNo);
	
    // ファイルポインタを先頭に戻す
    pGetFile->SeekToBegin();
	
	// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
		{
			fRet = TRUE;
			break;
		}
		m_nLine++;
		
		int nFind = sBuf.Find("ip route ");	// ip route 句の位置に追加
		if ( nFind != -1 && !fAddRouteWrite )
		{
			strPut.Empty();
			str.Format("ip route 10.142.65.0/24 gateway tunnel %d", m_sIniFileInf.nAddTunnelNo);	strPut += str + sCr;
			str.Format("ip route 10.142.165.0/24 gateway tunnel %d", m_sIniFileInf.nAddTunnelNo);	strPut += str + sCr;
			str.Format("ip route 10.145.0.0/16 gateway tunnel %d", m_sIniFileInf.nAddTunnelNo);		strPut += str + sCr;
			str.Format("ip route 10.141.0.0/16 gateway tunnel %d", m_sIniFileInf.nAddTunnelNo);		strPut += str + sCr;	// 24/08/02追加分
/*	セコム様の仕様書にかは書かれていない		
			str.Format("ip route %s gateway pp %d", m_sIniFileInf.szAddIp1, m_sRouter.m_nAdslNo);strPut += str + sCr;
			str.Format("ip route %s gateway pp %d", m_sIniFileInf.szAddIp2, m_sRouter.m_nAdslNo);strPut += str + sCr;
			lstrcpy(m_sIniFileInf.szWroteIp1, m_sIniFileInf.szAddIp1);
			lstrcpy(m_sIniFileInf.szWroteIp2, m_sIniFileInf.szAddIp2);
*/			
			fAddRouteWrite = TRUE;
			strPut = strPut + sCr;
			pSendFile->Write(strPut, strPut.GetLength());
		}
		
		nFind = sBuf.Find("ip filter 1 ");	// ip filter 句の位置に追加
		if ( nFind != -1 && !fAddTunnelWrite )
		{	// 追加書き込み分取得
			TunnelAdd(&strPut, m_sIniFileInf.nAddTunnelNo, m_sIniFileInf.nAddSubTunnelNo, m_sIniFileInf.szAddIp1, m_sIniFileInf.szAddIp2, m_szTargetRouterIp, m_szPublicIp);
			
			fAddTunnelWrite = TRUE;
			strPut = strPut + sCr;
			pSendFile->Write(strPut, strPut.GetLength());
		}
				
		nFind = sBuf.Find("ip filter set dk_pp_lan2 in ");
		if ( nFind != -1 )
		{
			sBuf += strFilterSetIN;	//  + "12600 12700";// 12600 12700はサポート端末接続用フィルタの追加対応 サポート端末のフィルタはルータ設定画面で登録
		}
		
		nFind = sBuf.Find("ip filter set dk_pp_lan2 out ");
		if ( nFind != -1 )
		{
			int index = sBuf.Find("dynamic");
			// "dynamic"が見つかった場合、その手前に文字列を挿入
			if ( index != -1 )
			{
				sBuf.Insert(index, strFilterSetOUT.Mid(1) /*+ "12650 12750 "*/);	// dyanmic前　12650 12750はサポート端末接続用フィルタの追加対応
			}
			// 	sBuf += strFilterSet;// dyanmic後
		}
		
		sBuf = sBuf + sCr;
		pSendFile->Write(sBuf, sBuf.GetLength());
	}
	
	sBuf = "description 1 \"configversion=5.1\"" + sCr;
	pSendFile->Write(sBuf, sBuf.GetLength());
	
	return fRet;
}

//----------------------------------------------------------------------------
//	<概要>	切替パターン①：新tunnelの修正(tunnel番号は変えられない)
//	<引数>	char *szIp		= 取得IPアドレス
//          char *szGetFile = 取得ファイル
//          char *szPutFile = 送信ファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCmdEdit_TunnelRepair(CStdioFile *pGetFile, CFile *pSendFile)
{
	int i = 0;
	CString msg;
	
	BOOL fMIRAIGATE = FALSE;
	BOOL fDengerCmd = FALSE;
	
	int nReserveNo[50] = {0};
	int n = 0;
	
	int nNowTunnel = 0;
	
	// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
			break;
		
		m_nLine++;
		
		int nFind = sBuf.Find("tunnel select");
		if ( nFind != -1 )
		{
			CString numberStr = sBuf.Mid(nFind + strlen("tunnel select ")); // 数値部分を抽出
			nNowTunnel = _ttoi(numberStr);
		}
		nFind = sBuf.Find("tunnel enable");
		if ( nFind != -1 )
		{
			CString numberStr = sBuf.Mid(nFind + strlen("tunnel enable ")); // 数値部分を抽出
			nNowTunnel = 0;
		}
		
		nFind = sBuf.Find("ipsec ike remote address");
		if ( nFind != -1 )
		{
			if ( nNowTunnel == m_sIniFileInf.nAddTunnelNo )	// 修正対象のトンネル番号内の定義
				sBuf.Format("ipsec ike remote address %d %s", m_sIniFileInf.nAddTunnelNo, m_sIniFileInf.szAddIp1);
			else if ( nNowTunnel == m_sIniFileInf.nAddSubTunnelNo )	// 修正対象のトンネル番号内の定義
				sBuf.Format("ipsec ike remote address %d %s", m_sIniFileInf.nAddSubTunnelNo, m_sIniFileInf.szAddIp2);
		}
		
		CString strPut, str;
		str.Format("ip filter %d ", 2600+m_sIniFileInf.nAddTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s udp * 500", 2600+m_sIniFileInf.nAddTunnelNo, m_sIniFileInf.szAddIp1, m_szTargetRouterIp);
		str.Format("ip filter %d ", 2600+m_sIniFileInf.nAddSubTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s udp * 500", 2600+m_sIniFileInf.nAddSubTunnelNo, m_sIniFileInf.szAddIp2, m_szTargetRouterIp);
		
		str.Format("ip filter %d ", 2650+m_sIniFileInf.nAddTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s udp * 500", 2650+m_sIniFileInf.nAddTunnelNo,	m_szTargetRouterIp, m_sIniFileInf.szAddIp1);
		
		str.Format("ip filter %d ", 2650+m_sIniFileInf.nAddSubTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s udp * 500", 2650+m_sIniFileInf.nAddSubTunnelNo, m_szTargetRouterIp, m_sIniFileInf.szAddIp2);
		
		str.Format("ip filter %d ", 2700+m_sIniFileInf.nAddTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s esp * *", 2700+m_sIniFileInf.nAddTunnelNo, m_sIniFileInf.szAddIp1, m_szTargetRouterIp);
		str.Format("ip filter %d ", 2700+m_sIniFileInf.nAddSubTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s esp * *", 2700+m_sIniFileInf.nAddSubTunnelNo, m_sIniFileInf.szAddIp2, m_szTargetRouterIp);
		
		str.Format("ip filter %d ", 2750+m_sIniFileInf.nAddTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s esp * *", 2750+m_sIniFileInf.nAddTunnelNo, m_szTargetRouterIp, m_sIniFileInf.szAddIp1);
		str.Format("ip filter %d ", 2750+m_sIniFileInf.nAddSubTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s %s esp * *", 2750+m_sIniFileInf.nAddSubTunnelNo, m_szTargetRouterIp, m_sIniFileInf.szAddIp2);

/*	セコム様の仕様書には書かれていない
		str.Format("ip filter %d ", 2900+m_sIniFileInf.nAddTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s 172.16.0.0/16 icmp * *", 2900+m_sIniFileInf.nAddTunnelNo, m_sIniFileInf.szAddIp1);
		str.Format("ip filter %d ", 2900+m_sIniFileInf.nAddSubTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass %s 172.16.0.0/16 icmp * *", 2900+m_sIniFileInf.nAddSubTunnelNo, m_sIniFileInf.szAddIp2);

		str.Format("ip filter %d ", 2950+m_sIniFileInf.nAddTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass 172.16.0.0/16 %s icmp * *", 2950+m_sIniFileInf.nAddTunnelNo, m_sIniFileInf.szAddIp1);
		str.Format("ip filter %d ", 2950+m_sIniFileInf.nAddSubTunnelNo);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip filter %d pass 172.16.0.0/16 %s icmp * *", 2950+m_sIniFileInf.nAddSubTunnelNo, m_sIniFileInf.szAddIp2);
		str.Format("ip route %s ", m_sIniFileInf.szWroteIp1);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip route %s gateway pp %d", m_sIniFileInf.szAddIp1, m_sRouter.m_nAdslNo);
		str.Format("ip route %s ", m_sIniFileInf.szWroteIp2);
		if ( sBuf.Find(str) != -1 )
			sBuf.Format("ip route %s gateway pp %d", m_sIniFileInf.szAddIp2, m_sRouter.m_nAdslNo);
*/		
		sBuf = sBuf + sCr;
		pSendFile->Write(sBuf, sBuf.GetLength());
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	ip filter setから定義番号を探す
//	<引数>	int nFilterNo = 対象番号
//          CString strBuf = 対象文字列
//	<戻値>	1:Nomalに定義あり 2:Dynamicに定義あり 3:両方に定義あり
//----------------------------------------------------------------------------
BYTE CDkRouter::SearchFilterSetNo(int nFilterNo, CString strBuf)
{
	int startPos = strBuf.Find("ip filter set ");
	if (startPos == -1) 
		return FALSE;
	
    char* charArray = new char[strBuf.GetLength() + 1];
    strcpy(charArray, strBuf.GetBuffer(0));
	
    char *token = strtok(charArray, " ");
	
	BYTE byRet = 0;
	int nRetPos = 0;
    while (token != NULL) 
	{
		if ( lstrcmp(token, "dynamic") == 0 )
			nRetPos++;
		
        int num;
        if ( sscanf(token, "%d", &num) == 1 )
		{
            if ( nFilterNo == num ) {
				DK_BitSet(byRet, nRetPos);
			}
        }
        token = strtok(NULL, " ");
    }
	
    delete[] charArray;
	
    return byRet;
}

//----------------------------------------------------------------------------
//	<概要>	切替パターン②：オンプレ→リフト切替処理
//	<引数>	char *szIp		= 取得IPアドレス
//          char *szGetFile = 取得ファイル
//          char *szPutFile = 送信ファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCmdEdit_Change(CStdioFile *pGetFile, CFile *pSendFile)
{
	BOOL fMIRAIGATE = FALSE;
	BOOL fDengerCmd = FALSE;
	
	int nReserveNo[50] = {0};
	int n = 0, nIdx = 0;
	int nPtn = 0;
	
	CString str, str1;
	
	// === 該当Filter set 定義 ======================================================================
	int nFilSet[4][30] = {
		{50,51,52,53,54, 55,56,57,58,59, 2050,2051,2052,2053,2054, 2055,2056,2057,2058,2059, 3050,3051,3052,3053,3054, 3055,3056,3057,3058,3059},
		{82,87,2082,2087,3082,3087},
		{2450,2451,3450,3451},
		{5050,5055,8050,8055},
	};
	
	//  === 該当Filter Dynamic定義 =================================================================
	CStringArray strOldFilterDynamic[4];
	CStringArray strNewFilterDynamic[4];
	n = 0, nIdx = 0;
	strOldFilterDynamic[n].Add("ip filter dynamic 55 172.16.0.0/16 61.114.186.0/25 filter 55 in 50");
	strOldFilterDynamic[n].Add("ip filter dynamic 56 172.16.5.0/24 61.114.186.0/25 filter 56 in 51");
	strOldFilterDynamic[n].Add("ip filter dynamic 57 172.16.15.0/24 61.114.186.0/25 filter 57 in 52");
	strOldFilterDynamic[n].Add("ip filter dynamic 58 172.16.60.0/24 61.114.186.0/25 filter 58 in 53");
	strOldFilterDynamic[n].Add("ip filter dynamic 59 172.16.80.0/22 61.114.186.0/25 filter 59 in 54");
	strOldFilterDynamic[n].Add("ip filter dynamic 2055 172.16.0.0/16 61.114.186.0/25 filter 2055 in 2050");
	strOldFilterDynamic[n].Add("ip filter dynamic 2056 172.16.5.0/24 61.114.186.0/25 filter 2056 in 2051");
	strOldFilterDynamic[n].Add("ip filter dynamic 2057 172.16.15.0/24 61.114.186.0/25 filter 2057 in 2052");
	strOldFilterDynamic[n].Add("ip filter dynamic 2058 172.16.60.0/24 61.114.186.0/25 filter 2058 in 2053");
	strOldFilterDynamic[n].Add("ip filter dynamic 2059 172.16.80.0/22 61.114.186.0/25 filter 2059 in 2054");
	strOldFilterDynamic[n].Add("ip filter dynamic 3055 172.16.0.0/16 61.114.186.0/25 filter 3055 in 3050");
	strOldFilterDynamic[n].Add("ip filter dynamic 3056 172.16.5.0/24 61.114.186.0/25 filter 3056 in 3051");
	strOldFilterDynamic[n].Add("ip filter dynamic 3057 172.16.15.0/24 61.114.186.0/25 filter 3057 in 3052");
	strOldFilterDynamic[n].Add("ip filter dynamic 3058 172.16.60.0/24 61.114.186.0/25 filter 3058 in 3053");
	strOldFilterDynamic[n].Add("ip filter dynamic 3059 172.16.80.0/22 61.114.186.0/25 filter 3059 in 3054");
	
	strNewFilterDynamic[n].Add("ip filter dynamic 20055 172.16.0.0/16 %s filter 20055 in 20050");
	strNewFilterDynamic[n].Add("ip filter dynamic 20056 172.16.5.0/24 %s filter 20056 in 20056");
	strNewFilterDynamic[n].Add("ip filter dynamic 20057 172.16.15.0/24 %s filter 20057 in 20052");
	strNewFilterDynamic[n].Add("ip filter dynamic 20058 172.16.60.0/24 %s filter 20058 in 20053");
	strNewFilterDynamic[n].Add("ip filter dynamic 20059 172.16.80.0/22 %s filter 20059 in 20054");
	strNewFilterDynamic[n].Add("ip filter dynamic 22055 172.16.0.0/16 %s filter 22055 in 22050");
	strNewFilterDynamic[n].Add("ip filter dynamic 22056 172.16.5.0/24 %s filter 22056 in 22051");
	strNewFilterDynamic[n].Add("ip filter dynamic 22057 172.16.15.0/24 %s filter 22057 in 22052");
	strNewFilterDynamic[n].Add("ip filter dynamic 22058 172.16.60.0/24 %s filter 22058 in 22053");
	strNewFilterDynamic[n].Add("ip filter dynamic 22059 172.16.80.0/22 %s filter 22059 in 22054");
	strNewFilterDynamic[n].Add("ip filter dynamic 23055 172.16.0.0/16 %s filter 23055 in 23050");
	strNewFilterDynamic[n].Add("ip filter dynamic 23056 172.16.5.0/24 %s filter 23056 in 23051");
	strNewFilterDynamic[n].Add("ip filter dynamic 23057 172.16.15.0/24 %s filter 23057 in 23052");
	strNewFilterDynamic[n].Add("ip filter dynamic 23058 172.16.60.0/24 %s filter 23058 in 23053");
	strNewFilterDynamic[n].Add("ip filter dynamic 23059 172.16.80.0/22 %s filter 23059 in 23054");
	
	//------------- 
	n++, nIdx = 0;
	strOldFilterDynamic[n].Add("ip filter dynamic 87 172.16.15.0/24 * filter 87 in 82");			// 検索値
	strOldFilterDynamic[n].Add("ip filter dynamic 2087 172.16.15.0/24 * filter 2087 in 2082");		// 検索値
	strOldFilterDynamic[n].Add("ip filter dynamic 3087 172.16.15.0/24 * filter 3087 in 3082");		// 検索値
	
	strNewFilterDynamic[n].Add("ip filter dynamic 20087 172.16.0.0/16 %s filter 20087 in 20082");	// 基本　終盤の番号も変える 
	strNewFilterDynamic[n].Add("ip filter dynamic 22087 172.16.0.0/16 %s filter 22087 in 22082");	// 基本　終盤の番号も変える
	strNewFilterDynamic[n].Add("ip filter dynamic 23087 172.16.0.0/16 %s filter 23087 in 23082");	// 基本　終盤の番号も変える
	
	//------------- 
	n++, nIdx = 0;
	strOldFilterDynamic[n].Add("ip filter dynamic 2450 ");	// %s %s filter 2450 in 2451	検索値(2450が定義されていればその後に2451,3450,3451も必ず定義されている)
	strOldFilterDynamic[n].Add("ip filter dynamic 3450 ");	// %s %s filter 3450 in 3451	検索値
	
	strNewFilterDynamic[n].Add("ip filter dynamic 22450 [L2_IP] %s filter 22450 in 22451");		// 基本　終盤の番号も変える 	// 172.16.0.0/16に統一
	strNewFilterDynamic[n].Add("ip filter dynamic 23450 [L2_IP] %s filter 23450 out 23451");
	
	char szLan3Mask[20] = {0};
	// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
			break;
		
		m_nLine++;
		
		// -- MIRAIGATE接続先をリフト側に切替 -------------------------------------------------------------------
		// ip route 172.31.191.0/24 gateway tunnel [n1]
		int startPos = sBuf.Find("ip route 172.31.191.0/24 gateway tunnel ");
		if (startPos != -1)
		{
			GetToken(sBuf, 5, &m_sIniFileInf.nPreTunnelNo);	// 変換前MIRAIGATE接続Noを取得
			sBuf.Format("ip route 172.31.191.0/24 gateway tunnel %d", m_sIniFileInf.nAddTunnelNo);
			goto WRITE_PROC;
		}
		// ip route 172.31.65.0/24 gateway tunnel [n1]
		startPos = sBuf.Find("ip route 172.31.65.0/24 gateway tunnel ");
		if (startPos != -1)
		{
			GetToken(sBuf, 5, &m_sIniFileInf.nPreTunnelNo);	// 変換前MIRAIGATE接続Noを取得
			sBuf.Format("ip route 172.31.65.0/24 gateway tunnel %d", m_sIniFileInf.nAddTunnelNo);
			goto WRITE_PROC;
		}
		
		// == ip filterの書き込み =================================================================================
		for ( nPtn = 0; nPtn < DK_Number(m_fFilterCheck); nPtn++ )
		{
			startPos = sBuf.Find("ip filter dynamic ");
			if (startPos != -1) 
				continue;
			startPos = sBuf.Find("ip filter set ");
			if (startPos != -1) 
				continue;
			
			if ( !m_fFilterCheck[nPtn] || !m_fFilterSetCheck[nPtn] || !m_fSrvIpFileCheck[nPtn] )	// フィルタ追加条件が揃っている
				continue;
			
			for ( int n = 0; n < DK_Number(nFilSet[nPtn]); n++ )
			{
				if ( nFilSet[nPtn][n] == 0 )
					continue;
				str.Format("ip filter %d ", nFilSet[nPtn][n]);
				startPos = sBuf.Find(str);
				if (startPos != -1) 
				{
					int no = 0;
					GetToken(sBuf, 2, &no);	// 元の番号
// 					if ( no == 2450 )
// 					{
// 						GetToken(sBuf, 4, NULL, szLan3Mask);	// LAN3maskIPの取得
// 					}
					
					// サーバーIP書き込み対象IPリスト
					CStringArray *pIpList = (nPtn != 1) ? m_cSrvIpList : m_cSrvIpList2;
					
					// 基本
					// str.Format(strNewFilter[nPtn].GetAt(n), szLan3Mask);	// %sがついていれば変換される
					str = sBuf;
					
					CString str1,str2;
					str1.Format(" %d ", no);
					str2.Format(" %d ", 20000 + no);
					str.Replace(str1, str2);	// Noを置き換える
					
					str.Replace((nPtn != 1) ? "61.114.186.0/25" : "113.52.158.128/25", ConvertStrList(&pIpList[0]));	// 61.114.186.*　をサーバーIP一覧に置換
					
					sBuf += sCr + str;		// 増殖
					
					goto WRITE_PROC;
				}// if (startPos != -1)
			}
		}
		
		// -- ip filter dynamicの書き込み -------------------------------------------------------------------------------
		for ( nPtn = 0; nPtn < DK_Number(m_fFilterCheck); nPtn++ )
		{
			if ( !m_fFilterCheck[nPtn] || !m_fFilterSetCheck[nPtn] || !m_fSrvIpFileCheck[nPtn] )	// フィルタ追加条件が揃っている
				continue;
			
			for ( int n = 0; n < strOldFilterDynamic[nPtn].GetSize(); n++ )
			{
				str = strOldFilterDynamic[nPtn].GetAt(n);
				startPos = sBuf.Find(strOldFilterDynamic[nPtn].GetAt(n));
				if (startPos != -1) 
				{
					int no = 0;
					GetToken(sBuf, 3, &no);	// 元の番号
					if ( no == 2450 )
					{
						GetToken(sBuf, 4, NULL, szLan3Mask);	// LAN3maskIPの取得
					}
					
					// サーバーIP書き込み対象IPリスト
					CStringArray *pIpList = (nPtn != 1) ? m_cSrvIpList : m_cSrvIpList2;
					
					str1 = strNewFilterDynamic[nPtn][n];
					str1.Replace("[L2_IP]", szLan3Mask);	// L2のIPアドレスがついていれば変換される
					
					str.Format(str1, ConvertStrList(&pIpList[0]));	// %sがついていれば変換される
					// str.Replace((nPtn != 1) ? "61.114.186.0/25" : "113.52.158.128/25", );	// <- ＊のみの記述もあるので使えない
					
					sBuf += sCr + str;		// 増殖
					
					goto WRITE_PROC;
				}// if (startPos != -1)
			}
		}
		
		// *** ip filter setの書き込み *************************************************************************************		
		startPos = sBuf.Find("ip filter set ");
		if (startPos != -1)
		{
			for ( nPtn = 0; nPtn < DK_Number(m_fFilterCheck); nPtn++ )
			{
				if ( !m_fFilterCheck[nPtn] || !m_fFilterSetCheck[nPtn] || !m_fSrvIpFileCheck[nPtn] )	// フィルタ追加条件が揃っている
					continue;
				
				for ( int i = 0; i < DK_Number(nFilSet[nPtn]); i++ )
				{
					if ( nFilSet[nPtn][i] == 0 )
						continue;
					
					CString strOld, strWk, strNewNo;
					
					BYTE byRet = SearchFilterSetNo(nFilSet[nPtn][i], sBuf);
					if ( byRet ) 
					{
						int nFilNo = nFilSet[nPtn][i];
						strWk.Format("%d", 20000 + nFilNo);
						
						strNewNo += " " + strWk;
						if ( DK_BitChk(byRet, 0) )	// ノーマル部に定義あり
						{
							int pos = sBuf.Find(" dynamic");	// "dynamic"の位置を検索して手前に挿入
							if (pos != -1) { // dynamicが見つかった場合
								sBuf.Insert(pos, strNewNo); // posの位置に挿入
							} else {
								sBuf += strNewNo;
							}
						}
						
						if ( DK_BitChk(byRet, 1) )	// dynamic部に定義あり
							sBuf += strNewNo;
					}
				}
			}
		}
		
WRITE_PROC:
		sBuf = sBuf + sCr;
		pSendFile->Write(sBuf, sBuf.GetLength());
	}
	
	sBuf = "description 1 \"configversion=5.2\"" + sCr;
	pSendFile->Write(sBuf, sBuf.GetLength());
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	切替パターン②：リフト→オンプレ切替処理またはリカバリー
//	<引数>	char *szIp		= 取得IPアドレス
//          char *szGetFile = 取得ファイル
//          char *szPutFile = 送信ファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCmdEdit_Recovery(CStdioFile *pGetFile, CFile *pSendFile)
{
	CString strMsg;
	strMsg = "clear configuration" + sCr;
	pSendFile->Write(strMsg, strMsg.GetLength());	// 設定クリアコマンド
	
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
			break;
		
		m_nLine++;
		
		sBuf = sBuf + sCr;
		pSendFile->Write(sBuf, sBuf.GetLength());
	}
	
	strMsg = "save" + sCr;
	pSendFile->Write(strMsg, strMsg.GetLength());	// saveコマンド

	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	切替パターン③：未接続のtunnel削除
//	<引数>	char *szIp		= 取得IPアドレス
//          char *szGetFile = 取得ファイル
//          char *szPutFile = 送信ファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCmdEdit_TunnelDelete(CStdioFile *pGetFile, CFile *pSendFile)
{
	int i = 0, n = 0;
	BOOL fDelHit = FALSE;
	CString sBuf;
	int nNowTunnel = 0;
	CString str;
	
	int nTagetFil[] = {50,51,52,53,54,55,56,57,58,59, 2450,2451, 3050,3051,3052,3053,3054,3055,3056,3057,3058,3059, 3450,3451};
	int nTagetFilDynamic[] = {55,56,57,58,59, 2055,2056,2057,2058,2059, 2450, 3055,3056,3057,3058,3059, 3450};
	
	// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
			break;
		
		m_nLine++;
		
		BOOL fWrite = FALSE;
		while ( TRUE )
		{	//===== ↓ip filter set===============================================================
/* 24/08/02仕様変更で削除不要になった
			CString strFilteSet[] = {
				"ip filter set dk_pp_lan2 in",
				"ip filter set dk_pp_lan2 out",
				"ip filter set dk_lan3 in",
				"ip filter set dk_lan3 out",
				"ip filter set dk_pp_lab out"
			};
			for ( int nSetIdx = 0; nSetIdx < DK_Number(strFilteSet); nSetIdx++ )
			{
				int nFind = sBuf.Find(strFilteSet[nSetIdx]);	// ip filter set ????
				if ( nFind != -1 )
				{
					str = sBuf;
					char *input = str.GetBuffer(sBuf.GetLength());
					
					// "in" 以降の文字列を抽出
					nFind = str.Find("in");	// ip filter set ????
					char* pos = strstr(input, (nFind != -1) ? "in" : "out");
					char* numbersStr = pos + ((nFind != -1) ? 3 : 4); // "in "の後ろから数字部分のみ抽出
					
					// 文字列ストリームを使って数値を抽出
					int numbers[1000] = {0};
					int numbersDyna[1000] = {0};
					BOOL fDynamic = FALSE;
					int idx = 0, idxDyna = 0;
					char* pch = strtok(numbersStr, " ");
					while ( pch != NULL )
					{
						if ( lstrcmp(pch, "dynamic") == 0 )
						{
							fDynamic = TRUE;
						}
						else
						{
							int number = atoi(pch);
							if ( number != 0 )
							{
								if ( !fDynamic )
									numbers[idx++] = number;
								else
									numbersDyna[idxDyna++] = number;
							}
						}						
						pch = strtok(NULL, " ");
					}
					
					// 消す処理
					sBuf = strFilteSet[nSetIdx];
					for ( int j = 0; j < DK_Number(numbers); j++ )
					{
						if ( numbers[j] == 0 )
							break;
						
						fDelHit = FALSE;
						if ( numbers[j] < 20000 )// +20000は追加分だから消さない
						{
							for ( n = 0; n < DK_Number(nTagetFil); n++ )
							{
								if ( numbers[j] == nTagetFil[n] ) {
									fDelHit = TRUE;
									break;	// 削除
								}
							}
						}
						if ( !fDelHit )
						{
							str.Format(" %d", numbers[j]);
							sBuf += str;
						}
					}
					
					if ( numbersDyna[0] > 0 )	// dynamicあり
					{
						sBuf += " dynamic";
						for ( int j = 0; j < DK_Number(numbersDyna); j++ )
						{
							if ( numbersDyna[j] == 0 )
								break;
							
							fDelHit = FALSE;
							if ( numbersDyna[j] < 20000 )// +20000は追加分だから消さない
							{
								for ( n = 0; n < DK_Number(nTagetFilDynamic); n++ )
								{
									if ( numbersDyna[j] == nTagetFilDynamic[n] ) {
										fDelHit = TRUE;
										break;	// 削除
									}
								}
							}
							if ( !fDelHit )
							{
								str.Format(" %d", numbersDyna[j]);
								sBuf += str;
							}
						}
					}
				}
			}
*/			
			//===== ↓tunnel ====================================================================
			int nFind = sBuf.Find("tunnel select");
			if ( nFind != -1 )
			{
				CString numberStr = sBuf.Mid(nFind + strlen("tunnel select ")); // 数値部分を抽出
				nNowTunnel = _ttoi(numberStr);
			}
			
			nFind = sBuf.Find("tunnel enable");
			if ( nFind != -1 )
			{
				CString numberStr = sBuf.Mid(nFind + strlen("tunnel enable ")); // 数値部分を抽出
				int nTarminal = nNowTunnel;
				nNowTunnel = 0;
				
				if ( nTarminal == m_sIniFileInf.nPreTunnelNo && m_sIniFileInf.nPreTunnelNo > 0 )		// 削除対象は読み飛ばして書かない
					break;
				if ( nTarminal == m_sIniFileInf.nPreSubTunnelNo && m_sIniFileInf.nPreSubTunnelNo > 0 )	// 削除対象は読み飛ばして書かない
					break;
			}
			
			if ( nNowTunnel == m_sIniFileInf.nPreTunnelNo && m_sIniFileInf.nPreTunnelNo > 0 )		// 削除対象は読み飛ばして書かない
				break;
			if ( nNowTunnel == m_sIniFileInf.nPreSubTunnelNo && m_sIniFileInf.nPreSubTunnelNo > 0 )	// 削除対象は読み飛ばして書かない
				break;
			
			//===== ↓ip route ================================================================
			if ( lstrlen(m_sIniFileInf.szPreIp1) > 0 ) 
			{
				str.Format("ip route %s", m_sIniFileInf.szPreIp1);	// オンプレのメイン用グローバルIPアドレス
				nFind = sBuf.Find(str);
				if ( nFind != -1 )
					break;
			}
			if ( lstrlen(m_sIniFileInf.szPreIp2) > 0 ) 
			{
				str.Format("ip route %s", m_sIniFileInf.szPreIp2);	// オンプレのバックアップ用グローバルIPアドレス
				nFind = sBuf.Find(str);
				if ( nFind != -1 )
					break;
			}
			nFind = sBuf.Find("ip route 61.114.186.0/25");
			if ( nFind != -1 )
				break;
/* 24/08/02仕様変更で削除不要になった			
			//===== ↓ip filter ================================================================
			for ( i = 0 ; i < DK_Number(nTagetFil); i++ )
			{
				str.Format("ip filter %d ", nTagetFil[i]);
				nFind = sBuf.Find(str);
				if ( nFind != -1 )
					break;
			}
			if ( nFind != -1 )
				break;
			
			int nTnlOrg[] = {2600,2650,2700,2750,2900,2950};
			for ( int i = 0; i < DK_Number(nTnlOrg); i++ )
			{
				str.Format("ip filter %d", nTnlOrg[i] + m_sIniFileInf.nPreTunnelNo);
				nFind = sBuf.Find(str);
				if ( nFind != -1 )
					break;
				str.Format("ip filter %d", nTnlOrg[i] + m_sIniFileInf.nPreSubTunnelNo);
				nFind = sBuf.Find(str);
				if ( nFind != -1 )
					break;
			}
			if ( nFind != -1 )
				break;
			
			//===== ↓ip filter dynamic =========================================================
			for ( i = 0 ; i < DK_Number(nTagetFilDynamic); i++ )
			{
				str.Format("ip filter dynamic %d ", nTagetFilDynamic[i]);
				nFind = sBuf.Find(str);
				if ( nFind != -1 )
					break;
			}
			if ( nFind != -1 )
				break;
*/			
			fWrite = TRUE;
			break;
		}
		
		if ( fWrite )
		{
			sBuf = sBuf + sCr;
			pSendFile->Write(sBuf, sBuf.GetLength());
		}
	}
	
	sBuf = sBuf + sCr;
	pSendFile->Write(sBuf, sBuf.GetLength());
	
	sBuf = "description 1 \"configversion=5.3\"" + sCr;
	pSendFile->Write(sBuf, sBuf.GetLength());
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	切替パターン④：proxy追加
//	<引数>	char *szIp		= 取得IPアドレス
//          char *szGetFile = 取得ファイル
//          char *szPutFile = 送信ファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCmdEdit_ProxyAdd(CStdioFile *pGetFile, CFile *pSendFile)
{
	int i = 0;
	CString msg;
	CString strPut, str;
	BOOL fAddRouteWrite = FALSE;
	BOOL fAddTunnelWrite = FALSE;
	
	int len = 0;
	
	BOOL fRet = FALSE;
	BOOL fMIRAIGATE = FALSE;
    // ファイルポインタを先頭に戻す
    pGetFile->SeekToBegin();
	
	// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
		{
			fRet = TRUE;
			break;
		}
		m_nLine++;
		
		int nFind = sBuf.Find("ip filter 1 ");	// ip filter 句の位置に追加
		if ( nFind != -1 && !fAddTunnelWrite )
		{	// 追加書き込み分取得
			strPut.Empty();
			str.Format("ip filter 20001 pass %s,%s 172.16.0.0/16 tcp %s,www,https *", m_sIniFileInf.szProxyIp1, m_sIniFileInf.szProxyIp2, m_sIniFileInf.szProxyPort);
			strPut += str + sCr;
			str.Format("ip filter 20002 pass 172.16.0.0/16 %s,%s tcp * %s,www,https", m_sIniFileInf.szProxyIp1, m_sIniFileInf.szProxyIp2, m_sIniFileInf.szProxyPort);
			
			strPut += str + sCr;
			
			fAddTunnelWrite = TRUE;
			pSendFile->Write(strPut, strPut.GetLength());
		}
		
		nFind = sBuf.Find("ip filter set dk_pp_lan2 in ");
		if ( nFind != -1 )
		{
			sBuf += " 20001";	// 12600 12700はサポート端末接続用フィルタの追加対応
		}
		
		nFind = sBuf.Find("ip filter set dk_pp_lan2 out ");
		if ( nFind != -1 )
		{
			int index = sBuf.Find("dynamic");
			// "dynamic"が見つかった場合、その手前に文字列を挿入
			if(index != -1)
			{
				sBuf.Insert(index, "20002 ");	// dyanmic前　12650 12750はサポート端末接続用フィルタの追加対応
			}
			// 	sBuf += strFilterSet;// dyanmic後
		}
		
		sBuf = sBuf + sCr;
		pSendFile->Write(sBuf, sBuf.GetLength());
	}
	
	sBuf = sBuf + sCr;
	pSendFile->Write(sBuf, sBuf.GetLength());
	
	sBuf = "description 1 \"configversion=5.4\"" + sCr;
	pSendFile->Write(sBuf, sBuf.GetLength());
	
	return fRet;
}

//----------------------------------------------------------------------------
//	<概要>	リフト移行用追加tunnelコマンドの取得
//	<引数>	CString *strPut		= 格納用
//          int nAddTunnelNo1	= 追加TunnelNo1
//          int nAddTunnelNo2	= 追加TunnelNo2
//          char *szTunnelIP1	= 追加TunnelのIP1
//          char *szTunnelIP2	= 追加TunnelのIP2
//          char *szRouterIP	= ルータのIP
//          char *szFixIP		= ADSL接続先IP
//----------------------------------------------------------------------------
void CDkRouter::TunnelAdd(CString *strPut, int nAddTunnelNo1, int nAddTunnelNo2, char *szTunnelIP1, char *szTunnelIP2, char *szRouterIP, char *szFixIP)
{
	CString str;
	strPut->Empty();
	
	str.Format("tunnel select %d", nAddTunnelNo1);		*strPut += str + sCr;
	str.Format("tunnel encapsulation ipsec");			*strPut += str + sCr;
	str.Format("ipsec tunnel %d", 100+nAddTunnelNo1);	*strPut += str + sCr;
	str.Format("ipsec sa policy %d %d esp 3des-cbc sha-hmac", 100+nAddTunnelNo1, nAddTunnelNo1);	*strPut += str + sCr;
	str.Format("ipsec ike duration ipsec-sa %d 28800", nAddTunnelNo1);			*strPut += str + sCr;
	str.Format("ipsec ike encryption %d 3des-cbc", nAddTunnelNo1);				*strPut += str + sCr;
	str.Format("ipsec ike group %d modp1024", nAddTunnelNo1);					*strPut += str + sCr;
	str.Format("ipsec ike hash %d sha", nAddTunnelNo1);							*strPut += str + sCr;
	str.Format("ipsec ike keepalive use %d on heartbeat 60 10", nAddTunnelNo1);	*strPut += str + sCr;
	str.Format("ipsec ike local address %d %s", nAddTunnelNo1, szRouterIP);		*strPut += str + sCr;
	str.Format("ipsec ike pfs %d on", nAddTunnelNo1);							*strPut += str + sCr;
	str.Format("ipsec ike pre-shared-key %d text %s", nAddTunnelNo1, szFixIP);	*strPut += str + sCr;
	str.Format("ipsec ike remote address %d %s", nAddTunnelNo1, szTunnelIP1);	*strPut += str + sCr;
	str.Format("tunnel backup tunnel %d switch-interface=on", nAddTunnelNo2);	*strPut += str + sCr;
	str.Format("ip tunnel nat descriptor 2012");		*strPut += str + sCr;
	str.Format("ip tunnel rip send off");				*strPut += str + sCr;
	str.Format("ip tunnel rip receive off");			*strPut += str + sCr;
	str.Format("tunnel enable %d", nAddTunnelNo1);		*strPut += str + sCr;
	
	str.Format("tunnel select %d", nAddTunnelNo2);		*strPut += str + sCr;
	str.Format("tunnel encapsulation ipsec");			*strPut += str + sCr;
	str.Format("ipsec tunnel %d", 100+nAddTunnelNo2);	*strPut += str + sCr;
	str.Format("ipsec sa policy %d %d esp 3des-cbc sha-hmac", 100+nAddTunnelNo2, nAddTunnelNo2);	*strPut += str + sCr;
	str.Format("ipsec ike duration ipsec-sa %d 28800", nAddTunnelNo2);			*strPut += str + sCr;
	str.Format("ipsec ike encryption %d 3des-cbc", nAddTunnelNo2);				*strPut += str + sCr;
	str.Format("ipsec ike group %d modp1024", nAddTunnelNo2);					*strPut += str + sCr;
	str.Format("ipsec ike hash %d sha", nAddTunnelNo2);							*strPut += str + sCr;
	str.Format("ipsec ike keepalive use %d on heartbeat 60 10", nAddTunnelNo2);	*strPut += str + sCr;
	str.Format("ipsec ike local address %d %s", nAddTunnelNo2, szRouterIP);		*strPut += str + sCr;
	str.Format("ipsec ike pfs %d on", nAddTunnelNo2);							*strPut += str + sCr;
	str.Format("ipsec ike pre-shared-key %d text %s", nAddTunnelNo2, szFixIP);	*strPut += str + sCr;
	str.Format("ipsec ike remote address %d %s", nAddTunnelNo2, szTunnelIP2);	*strPut += str + sCr;	
	str.Format("ip tunnel nat descriptor 2012");	*strPut += str + sCr;
	str.Format("ip tunnel rip send off");			*strPut += str + sCr;
	str.Format("ip tunnel rip receive off");		*strPut += str + sCr;
	str.Format("tunnel enable %d", nAddTunnelNo2);	*strPut += str + sCr;
	
	str.Format("ip filter %d pass %s %s udp * 500", 2600+nAddTunnelNo1, szTunnelIP1, szRouterIP);	*strPut += str + sCr;
	str.Format("ip filter %d pass %s %s udp * 500", 2600+nAddTunnelNo2, szTunnelIP2, szRouterIP);	*strPut += str + sCr;
	str.Format("ip filter %d pass %s %s udp * 500", 2650+nAddTunnelNo1, szRouterIP, szTunnelIP1);	*strPut += str + sCr;
	str.Format("ip filter %d pass %s %s udp * 500", 2650+nAddTunnelNo2, szRouterIP, szTunnelIP2);	*strPut += str + sCr;
	
	str.Format("ip filter %d pass %s %s esp * *",	2700+nAddTunnelNo1, szTunnelIP1, szRouterIP);	*strPut += str + sCr;
	str.Format("ip filter %d pass %s %s esp * *",	2700+nAddTunnelNo2, szTunnelIP2, szRouterIP);	*strPut += str + sCr;
	str.Format("ip filter %d pass %s %s esp * *",	2750+nAddTunnelNo1, szRouterIP, szTunnelIP1);	*strPut += str + sCr;
	str.Format("ip filter %d pass %s %s esp * *",	2750+nAddTunnelNo2, szRouterIP, szTunnelIP2);	*strPut += str + sCr;
/*	// 追加2024/06/19
	str.Format("ip filter %d pass %s 172.16.0.0/16 icmp * *",	2900+nAddTunnelNo1, szTunnelIP1);	*strPut += str + sCr;
	str.Format("ip filter %d pass %s 172.16.0.0/16 icmp * *",	2900+nAddTunnelNo2, szTunnelIP2);	*strPut += str + sCr;
	str.Format("ip filter %d pass 172.16.0.0/16 %s icmp * *",	2950+nAddTunnelNo1, szTunnelIP1);	*strPut += str + sCr;
	str.Format("ip filter %d pass 172.16.0.0/16 %s icmp * *",	2950+nAddTunnelNo2, szTunnelIP2);	*strPut += str + sCr;
	// サポート端末接続用フィルタの追加
	CString strSptFilter = "222.151.227.83,221.186.163.27,221.186.163.28,220.110.177.59,220.110.177.60,220.110.177.61,122.1.9.170,122.1.9.171,122.1.9.172,122.1.9.173,122.1.9.174,125.206.196.139,125.206.210.19,60.32.159.235,60.32.143.116,60.32.143.117,202.41.213.170,202.41.213.171,202.41.213.172,202.41.213.173,221.186.110.83,60.32.158.219,114.156.254.18,222.151.236.148,222.151.236.149,222.151.236.146,153.231.254.66,153.231.254.67,60.32.188.115,60.32.188.116,60.32.123.123,60.32.123.124,125.206.255.44,125.206.255.45,60.32.122.123,60.32.122.124,60.32.191.51,180.42.70.100,180.42.70.102,180.42.70.103,180.42.70.104,180.42.70.105,118.22.14.108,118.22.14.109,153.150.21.247";
	str.Format("ip filter 12600 pass %s 172.16.5.0/24 udp * 500", strSptFilter);	*strPut += str + sCr;
	str.Format("ip filter 12650 pass 172.16.5.0/24 %s udp * 500", strSptFilter);	*strPut += str + sCr;
	str.Format("ip filter 12700 pass %s 172.16.5.0/24 esp * *", strSptFilter);		*strPut += str + sCr;
	str.Format("ip filter 12750 pass 172.16.5.0/24 %s esp * *", strSptFilter);		*strPut += str + sCr;
*/
}

//----------------------------------------------------------------------------
//	<概要>	JNET顧客代行対応
//	<引数>	char *szIp		= 取得IPアドレス
//          char *szGetFile = 取得ファイル
//          char *szPutFile = 送信ファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::RouterCmdEdit_JnerMent(CStdioFile *pGetFile, CFile *pSendFile)
{
    CStdioFile file;
    CFileException fileException;

	int i = 0;
	CString strPut, str;
	BOOL fAddRouteWrite = FALSE;
	BOOL fAddTunnelWrite = FALSE;
// 	---------- VPNIPADDRESSINF2.DAT--------------	
// 		グローバルアドレス,（JNETFlag）,'端末名称’	
// 		114.156.30.183',3,'ＪＮＥＴ１'	
// 		219.160.163.186',3,'ＪＮＥＴ２'	
// 		***.***.***.***',3,'ＪＮＥＴ＊'	
// 　　　　（最大１０行）	
	
	CString strNewJnetIp;
	char szFqdnInfFile[MAX_PATH];
	wsprintf(szFqdnInfFile, "%s%s", pMxBas->GetBaseDir(), FQDN_DAT_REF_PATH);
	if ( !file.Open(szFqdnInfFile, CFile::modeRead | CFile::shareDenyWrite | CFile::typeText, &fileException) )
	{
		DebDisp("VpnIpAddressInf2.datのオープンに失敗しました", m_szTargetRouterIp);
		return FALSE;
	}
	
	CString line;
	while (file.ReadString(line))
	{
		char* token = strtok((CStoS)line, ",");
		
		while (token != NULL) 
		{
			CString str = token;
			str.Remove('\'');
			token = strtok(NULL, ",");
			if ( token != NULL && atoi(token) == 3)	// フラグ=3のIPのアドレスのみ抽出
			{
				if ( strNewJnetIp.IsEmpty() )
					strNewJnetIp = str;
				else
					strNewJnetIp = strNewJnetIp + "," + str;
			}
		}
	}
	file.Close();
	
	if ( strNewJnetIp.IsEmpty() )
	{
		DebDisp("VpnIpAddressInf2.datからJ-NETグローバルアドレスを取得できませんでした", m_szTargetRouterIp);
		return FALSE;
	}
	DebDisp("J-NETグローバルアドレス=%s", strNewJnetIp);
	
    // ファイルポインタを先頭に戻す
    pGetFile->SeekToBegin();
	
	if ( !m_fJnetMentWrote )
		DebDisp("J-NET顧客代行対応 config追加処理開始(追加tunnelNo=%d)", m_sIniFileInf.nAddJnetMentNo);
	else		
		DebDisp("J-NET顧客代行対応 config更新処理開始");
	
	// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
			break;
		
		m_nLine++;		
		
		if ( !m_fJnetMentWrote )
		{
			int nFind = sBuf.Find("ip route ");	// ip route 句の位置に追加
			if ( nFind != -1 && !fAddRouteWrite )
			{
				strPut.Empty();
				
				str.Format("ip route 10.0.241.0/24 gateway tunnel %d", m_sIniFileInf.nAddJnetMentNo);	strPut += str + sCr;
				
				fAddRouteWrite = TRUE;
				strPut = strPut + sCr;
				pSendFile->Write(strPut, strPut.GetLength());
			}
			
			nFind = sBuf.Find("ip filter 1 ");	// ip filter 句の位置に追加
			if ( nFind != -1 && !fAddTunnelWrite )
			{	// 追加書き込み分取得
				CString str;
				CString strPut;
				int nAddTunnelNo1 = m_sIniFileInf.nAddJnetMentNo;
				
				sBuf = sBuf + sCr;
				str.Format("tunnel select %d", nAddTunnelNo1);						sBuf += str + sCr;
				str.Format("tunnel encapsulation ipsec");							sBuf += str + sCr;
				str.Format("ipsec tunnel %d", 100+nAddTunnelNo1);					sBuf += str + sCr;
				str.Format("ipsec sa policy %d %d esp aes-cbc sha-hmac", 100+nAddTunnelNo1, nAddTunnelNo1);	sBuf += str + sCr;
				str.Format("ipsec ike duration ipsec-sa %d 28800", nAddTunnelNo1);	sBuf += str + sCr;
				str.Format("ipsec ike encryption %d aes-cbc", nAddTunnelNo1);		sBuf += str + sCr;
				str.Format("ipsec ike group %d modp1024", nAddTunnelNo1);			sBuf += str + sCr;
				str.Format("ipsec ike hash %d sha", nAddTunnelNo1);					sBuf += str + sCr;
				str.Format("ipsec ike keepalive use %d on heartbeat 10 2", nAddTunnelNo1);		sBuf += str + sCr;
				str.Format("ipsec ike local address %d 172.16.5.1", nAddTunnelNo1);	sBuf += str + sCr;
				str.Format("ipsec ike pfs %d on", nAddTunnelNo1);					sBuf += str + sCr;
				str.Format("ipsec ike pre-shared-key %d text jnetremotepsk", nAddTunnelNo1);	sBuf += str + sCr;
				str.Format("ipsec ike remote address %d any", nAddTunnelNo1);		sBuf += str + sCr;
				str.Format("ipsec ike remote name %d jnetremotename key-id", nAddTunnelNo1);	sBuf += str + sCr;
				str.Format("ip tunnel rip send off", nAddTunnelNo1);				sBuf += str + sCr;
				str.Format("ip tunnel rip receive off", nAddTunnelNo1);				sBuf += str + sCr;
				str.Format("tunnel enable %d", nAddTunnelNo1);						sBuf += str + sCr;
				
				str.Format("ip filter 20010 pass %s 172.16.5.1 esp * *",	strNewJnetIp);	sBuf += str + sCr;
				str.Format("ip filter 20015 pass 172.16.5.1 %s esp * *",	strNewJnetIp);	sBuf += str + sCr;
				str.Format("ip filter 20020 pass %s 172.16.5.1 udp * 500",	strNewJnetIp);	sBuf += str + sCr;
				str.Format("ip filter 20025 pass 172.16.5.1 %s udp * 500",	strNewJnetIp);	sBuf += str + sCr;
				str.Format("ip filter 20030 pass %s 172.16.5.1 icmp * *",	strNewJnetIp);	sBuf += str + sCr;
				str.Format("ip filter 20035 pass 172.16.5.1 %s icmp * *",	strNewJnetIp);	sBuf += str + sCr;
				
				fAddTunnelWrite = TRUE;
			}
			
			nFind = sBuf.Find("ip filter set dk_pp_lan2 in ");
			if ( nFind != -1 )
			{
				sBuf += " 20010 20020 20030";
			}
			
			nFind = sBuf.Find("ip filter set dk_pp_lan2 out ");
			if ( nFind != -1 )
			{
				int index = sBuf.Find("dynamic");
				// "dynamic"が見つかった場合、その手前に文字列を挿入
				if(index != -1)
				{
					sBuf.Insert(index, "20015 20025 20035 ");	// dyanmic前　12650 12750はサポート端末接続用フィルタの追加対応
				}
			}		
		}
		else
		{	// すでに追加されているので、IPアドレスの変更のみ
			CString strPut, str;
			str.Format("ip filter %d ", 20010);
			if ( sBuf.Find(str) != -1 )
				sBuf.Format("ip filter 20010 pass %s 172.16.5.1 esp * *", strNewJnetIp);
			
			str.Format("ip filter %d ", 20015);
			if ( sBuf.Find(str) != -1 )
				sBuf.Format("ip filter 20015 pass 172.16.5.1 %s esp * *", strNewJnetIp);
			
			str.Format("ip filter %d ", 20020);
			if ( sBuf.Find(str) != -1 )
				sBuf.Format("ip filter 20020 pass %s 172.16.5.1 udp * 500",	strNewJnetIp);
			
			str.Format("ip filter %d ", 20025);
			if ( sBuf.Find(str) != -1 )
				sBuf.Format("ip filter 20025 pass 172.16.5.1 %s udp * 500",	strNewJnetIp);
			
			str.Format("ip filter %d ", 20030);
			if ( sBuf.Find(str) != -1 )
				sBuf.Format("ip filter 20030 pass %s 172.16.5.1 icmp * *", strNewJnetIp);
			
			str.Format("ip filter %d ", 20035);
			if ( sBuf.Find(str) != -1 )
				sBuf.Format("ip filter 20035 pass 172.16.5.1 %s icmp * *", strNewJnetIp);
		}
		
		sBuf = sBuf + sCr;
		pSendFile->Write(sBuf, sBuf.GetLength());
	}
	
	return TRUE;
}

/*-----------------------------------------------------------------------------
	関数名  オンプレからクラウドに変更する前にクラウド接続用新トンネルが開通しているかをチェックする
-----------------------------------------------------------------------------*/
BOOL CDkRouter::CheckTunnelConnect(int nTunnelNo, char *szIP1)
{
	class CRouterTelnet : public CDkMxTelnet 
	{
	public:
		CDkRouter *pParent;
		void Disp(LPCTSTR msg) {
			pParent->DebDisp("●Telnet:%s", msg);    
		}
	};
	
	BOOL fDone = FALSE, fAnoni= FALSE;
	const char	*pRepMoji[] = {"", "" };	
	CString	strCmd, strMsg;
	
	//	ルータにテルネット接続する
	//CDkMxTelnet	cTel;
	CRouterTelnet 	cTel;
	cTel.pParent = this;
	
	int			b;
	do {
		b = cTel.Open(m_szTargetRouterIp);
		if (!b) {
			DebDisp("ルータへのログインに失敗しました(%s)", m_szTargetRouterIp);
			return FALSE;
		}
		b = cTel.Login("*", (m_nNowGetRouterKind >= eRTX1200) ? "*" : NULL);
	} while (FALSE);
	
	if (!b)
	{
		DebDisp("ルータへのログインに失敗しました(ログイン認証)");
		return FALSE;
	}
	
	do {
		CString strCmd;
		strCmd.Format("show ipsec sa gateway %d", nTunnelNo);
		//	 	strCmd="show config";
		DebDisp("送信コマンド=%s", strCmd);
		
		b = cTel.Command(strCmd + "\r\n", NULL, (m_sIniFileInf.nDebugCmdRecvTimeOut <= 0) ? 60 : m_sIniFileInf.nDebugCmdRecvTimeOut);
		if (!b) {
			DebDisp("コマンド送信失敗しました timeout=%d", m_sIniFileInf.nDebugCmdRecvTimeOut);
			break;
		}
		
		char *result = strstr(cTel.m_ReadBuf, szIP1);
		
		DebDisp("cTel.m_ReadBuf=\n%s", cTel.m_ReadBuf);
		DebDisp("szIP1=%s コマンド結果=%s", szIP1, (result != NULL) ? "成功" : "失敗");
		
		// 検索結果の判定
		b = (result != NULL) ? TRUE : FALSE;
		
	} while (FALSE);
	
	return b;
}

//----------------------------------------------------------------------------
//	<概要>	ルータ機種を判別する
//	<引数>	char *buf = ルータ機種が記載されていると思われる注釈文
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
int CDkRouter::AnalizeRouterType(char *buf, int *pSetuzokuMax)
{
	char	*p;
	BOOL	fFind = FALSE;
	char	*szRtType[] = {"RTX1000","RTX1100","RTX1200","RTX1210","RTX830","RTX1220"};
	int		nKind[] = {eRTX1000, eRTX1100, eRTX1200, eRTX1210, eRTX830, eRTX1220};
	int		nSetuzokuMax[] = {30,30,49,49,15,49};
	
	if ( m_fReadRtType )	// 処理済みなら何もせん
		return -1;	
	
	if ( *buf != '#' )		// 注釈以外
		return -1;
	
	p = strtok(buf+1, SEARCH_CHR);
	if ( p == NULL )
		return -1;
	
	//	RTxxx
	for ( int type = 0; type < DK_Number(szRtType); type++ )
	{
		if (p && strncmp(p, szRtType[type], lstrlen(szRtType[type])) == 0)
		{
			m_fReadRtType = TRUE;	// ルータタイプ判明済みフラグON
			*pSetuzokuMax = nSetuzokuMax[type];
			return nKind[type];
		}
	}
	
	return -1;	// ただの注釈文の場合はeNon
}

//----------------------------------------------------------------------------
//	<概要>	サーバーIPファイルから一覧を取得
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::ReadServerIpFile()
{
	CString filePath;
	CStdioFile file;
	CString line;
	BYTE ipSt[IP_BYTE], ipEd[IP_BYTE], ipSvFile[IP_BYTE];
	//例
// 	123.456.48.58,99.98.99.98,samplefqdn1.domain
// 	1.2.3.4,5.6.7.8,samplefqdn2.domain
// 	111.222.333.1,61.114.186.33,samplefqdn1.domain
// 	227.226.225.9,61.114.186.35,samplefqdn9.domain
// 	123.4.5.1,113.52.158.153,samplefqdn1.domain
// 	45.5.1.9,113.52.158.155,samplefqdn9.domain
	
	CString strCheckIpSt[4] = {"61.114.186.0",	"113.52.158.128", "61.114.186.0",	"61.114.186.0"};
	CString strCheckIpEd[4] = {"61.114.186.127","113.52.158.255", "61.114.186.127", "61.114.186.127"};
	
	for ( int i = 0; i < 4; i++ )
	{
		m_cSrvIpList[i].RemoveAll();
		m_cSrvIpList2[i].RemoveAll();
		m_fSrvIpFileCheck[i] = FALSE;
	}

	// ファイルから一行ずつ読み込む
	for ( m_nLine = 0; m_nLine < m_sSvIpList.GetSize(); m_nLine++ )
	{
		line = m_sSvIpList.GetAt(m_nLine);
		
		CString ipAddress1;	// 新サーバIP
		CString ipAddress2;	// 現サーバIP
		int commaIndex = 0;
		
		// 新IPアドレスを抽出
		int firstCommaIndex = line.Find(_T(','));	// 最初のカンマの位置を見つける
		ipAddress1 = line.Left(firstCommaIndex);	// 最初のカンマまでの文字列を抽出
		
		// 現IPアドレスを抽出
		CString restOfString = line.Mid(firstCommaIndex + 1);	// 最初のカンマ以降の文字列
		int secondCommaIndex = restOfString.Find(_T(','));		// 次のカンマの位置を見つける
		ipAddress2 = restOfString.Left(secondCommaIndex);		// 次のカンマまでの文字列を抽出
		
		if ( ipAddress2.Find("61.114.186.") != -1 )
		{
			ConvertIP(ipSt, "61.114.186.0");
			ConvertIP(ipEd, "61.114.186.127");
			ConvertIP(ipSvFile, (CStoS)ipAddress2);	// 113.52.158.*
			
			if ( ipSt[0] == ipSvFile[0] && ipSt[1] == ipSvFile[1] && ipSt[2] == ipSvFile[2] && ipSt[3] <= ipSvFile[3] && ipSvFile[3] <= ipEd[3] )
			{
				m_fSrvIpFileCheck[0] = TRUE;
				m_fSrvIpFileCheck[2] = TRUE;
				m_fSrvIpFileCheck[3] = TRUE;
			}
			
			for ( int i = 0; i < 4; i++ )	// 200個ずつ格納
			{
				if ( m_cSrvIpList[i].GetSize() < 200 ) {
					m_cSrvIpList[i].Add(ipAddress1);	// 61.114.186.*
					break;
				}
			}
		}
		if ( ipAddress2.Find("113.52.158.") != -1 )
		{
			ConvertIP(ipSt, "113.52.158.128");
			ConvertIP(ipEd, "113.52.158.255");
			ConvertIP(ipSvFile, (CStoS)ipAddress2);	// 113.52.158.*
			
			if ( ipSt[0] == ipSvFile[0] && ipSt[1] == ipSvFile[1] && ipSt[2] == ipSvFile[2] && ipSt[3] <= ipSvFile[3] && ipSvFile[3] <= ipEd[3] )
			{
				m_fSrvIpFileCheck[1] = TRUE;
			}
			
			for ( int i = 0; i < 4; i++ )	// 200個ずつ格納
			{
				if ( m_cSrvIpList2[i].GetSize() < 200 ) {
					m_cSrvIpList2[i].Add(ipAddress1);	// 113.52.158.*
					break;
				}
			}
		}
	}
	
// 	for (  i = 0; i < 4; i++ )	// 拡張分をカウント
// 	{
// 		if ( m_cSrvIpList[i].GetSize() ) 
// 			m_nSrvIp200CntMax1++;
// 		if ( m_cSrvIpList2[i].GetSize() ) 
// 			m_nSrvIp200CntMax2++;
// 	}
	
#if 0	// TEST用
	for ( int f = 0; f < 200; f++ )
	{
		m_cSrvIpList[1].Add("123.123.132.132");
		m_cSrvIpList[2].Add("201.202.203.204");
		m_cSrvIpList2[1].Add("323.323.332.332");
		m_cSrvIpList2[2].Add("401.402.403.404");
	}
#endif
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	CStringArrayをカンマつきCStringに
//----------------------------------------------------------------------------
CString CDkRouter::ConvertStrList(CStringArray *pArray)
{
	CString str;
	
	str = pArray->GetAt(0);
	
	// 要素をカンマ区切りで追加
    for (int i = 1; i < pArray->GetSize(); ++i)
    {
		str += "," + pArray->GetAt(i);
    }
	
	return str;
}

//----------------------------------------------------------------------------
//	<概要>	スペース区切りトークンの取得
//	<引数>	CString strText = 対象文字列
//          int nPosCnt = トークン目
//          int *pData=NULL = 格納用
//          char *pszData=NULL = 格納用
//----------------------------------------------------------------------------
void CDkRouter::GetToken(CString strText, int nPosCnt, int *pData/*=NULL*/, char *pszData/*=NULL*/)
{
	char str[4056] = {0};
	wsprintf(str, "%s", (CStoS)strText);	// 	char str[] = "ipsec ike pre-shared-key [接続先番号] text [接続先の固定IPアドレス]";
	
	char *token;
	int count = 0;
	
	token = strtok(str, " ");
	while (token != NULL) 
	{
		if (count == nPosCnt) {
			if ( pszData )
				lstrcpy(pszData, token);
			if ( pData )
				*pData = atoi(token);
			break;
		}
		token = strtok(NULL, " ");
		count++;
	}
}

//----------------------------------------------------------------------------
//	<概要>	新フィルタ適用する必要があるのか先読みチェック
//	<引数>	CString sBuf  = 対象文字列
//          int		nKind = 
//----------------------------------------------------------------------------
void CDkRouter::CheckCmd(CString sBuf, int nKind)
{
	CString str;
	int nNo;
	
	int nConvFilter[LIFT_CHG_PTN_MAX][30] = {	
		{50,51,52,53,54, 55,56,57,58,59, 2050,2051,2052,2053,2054, 2055,2056,2057,2058,2059, 3050,3051,3052,3053,3054, 3055,3056,3057,3058,3059},
		{82,87,2082,2087,3082,3087},
		{2450,2451,3450,3451},
		{5050,5055,8050,8055},
	};
	int nConvFilterDynamic[LIFT_CHG_PTN_MAX][30] = {	
		{55,56,57,58,59, 2055,2056,2057,2058,2059, 3055,3056,3057,3058,3059},
		{87,2087,3087},
		{2450,3450},
		{5055,8055},
	};
/*
	int nConvFilter[LIFT_CHG_PTN_MAX][10] = {
		{50,55,2050,2055,3050,3055},
		{82,87,2082,2087,3082,3087},
		{2450,2451,3450,3451},
		{5050,5055,8050,8055},
	};
	int nConvFilterDynamic[LIFT_CHG_PTN_MAX][10] = {	
		{55,2055,3055},
		{87,2087,3087},
		{2450,3450},
		{5055,8055},
	};
*/	

//===== ↓J-NET顧客代行対応 ==========================================================
	str.Format("ip filter 20010 ");
	if ( sBuf.Find(str) != -1 ) {
		m_fJnetMentWrote = TRUE;	// すでに設定済み
		return ;
	}
//===== ↑J-NET顧客代行対応 ==========================================================
	
	for ( int nPtn = 0; nPtn < LIFT_CHG_PTN_MAX; nPtn++ )
	{
		if ( m_fSrvIpFileCheck[nPtn] != TRUE )	// サーバーIPの条件はクリアしているか
			continue;
		//-- ip filter -----------------------------------------------------------------------------
		BOOL fHit = FALSE, fDynamic = FALSE;
		for ( int nChkFilIdx = 0; nChkFilIdx < DK_Number(nConvFilter[nPtn]); nChkFilIdx++ )
		{
			if ( nConvFilter[nPtn][nChkFilIdx] == 0 )
				continue;
			str.Format("ip filter %d ", nConvFilter[nPtn][nChkFilIdx]);
			if ( sBuf.Find(str) != -1 ) {
				nNo = nConvFilter[nPtn][nChkFilIdx];
				fHit = TRUE;	// 存在する
				break;
			}
		}
		for ( nChkFilIdx = 0; nChkFilIdx < DK_Number(nConvFilterDynamic[nPtn]); nChkFilIdx++ )
		{
			if ( nConvFilterDynamic[nPtn][nChkFilIdx] == 0 )
				continue;
			str.Format("ip filter dynamic %d ", nConvFilterDynamic[nPtn][nChkFilIdx]);
			if ( sBuf.Find(str) != -1 ) {
				nNo = nConvFilterDynamic[nPtn][nChkFilIdx];
				fHit = TRUE, fDynamic = TRUE;	// 存在する
				break;
			}
		}
		
		if ( fHit )
		{
			m_fFilterCheck[nPtn] = TRUE;	// 存在する
		}
		//-----------------------------------------------------------------------------------------
		
		//-- ip filter set -------------------------------------------------------------------------
		fHit = FALSE;
		for ( nChkFilIdx = 0; nChkFilIdx < DK_Number(nConvFilter[nPtn]); nChkFilIdx++ )
		{
			str.Format("ip filter set ");
			if ( sBuf.Find(str) != -1 )
			{
				if ( SearchFilterSetNo(nConvFilter[nPtn][nChkFilIdx], sBuf) ) 
				{
					fHit = TRUE;
					break;
				}
			}
		}
		
		if ( fHit )
		{
			m_fFilterSetCheck[nPtn] = TRUE;	// 存在する
		}
		//-----------------------------------------------------------------------------------------
	}

#if 0 // TEST用
	for ( nPtn = 0; nPtn < LIFT_CHG_PTN_MAX; nPtn++ )
	{
 		m_fFilterCheck[nPtn]  = TRUE;
 		m_fFilterSetCheck[nPtn] = TRUE;
 		m_fSrvIpFileCheck[nPtn] = TRUE;	// フィルタ追加条件が揃っている
	}
// 		m_fFilterCheck[2] = FALSE; 
#endif
	return;	
}

//----------------------------------------------------------------------------
//	<概要>	処理前先読み処理
//	<引数>	CStdioFile *pGetFile = ルータ本体設定configファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::PreConfigRead(CStdioFile *pGetFile, BOOL fRtSet/*=FALSE*/, BOOL fJnetMent/*=FALSE*/)
{
	int i = 0;
	CString msg;
	CString strPut;
	CString str;
	
	CString	sBuf, sCr = "\r\n";
	int len = 0;
	
	CString strDenger[] = {	// 既にconfigこの設定がある場合は上書きになるためエラー
		"ip route 10.142.65.0/24",
		"ip route 10.142.165.0/24",
		"ip route 10.145.0.0/16"
	};
	
	BOOL fRet = FALSE;
	BOOL fMIRAIGATE = FALSE;
	

	int nReserveNo[TUNNEL_DEF_MAX] = {0};
	int n = 0;
	
	m_fReadRtType = FALSE;
	
	// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
	for (m_nLine = 0; ;)
	{
		if ( !LineRead(pGetFile, &sBuf) )
		{
			fRet = TRUE;
			break;
		}
		m_nLine++;
		
		int s = 0;
		m_nNowGetRouterKind = AnalizeRouterType((CStoS)sBuf, &s);
		if ( s != -1 && m_nRtSetuzokuMax == 0 )	
		{
			m_nRtSetuzokuMax = s;	// ルータ毎の番号設定最大
		}
		
		// 注釈はスキップ
		if (sBuf.GetLength() < 2 || sBuf.GetAt(0) == '#')
			continue;
		
		// すでに追加コマンドと重複するコマンドが入ってる場合はエラー
		if  ( m_sIniFileInf.nStatus == SDkKanshiSrvInfo::eNonMode )
		{
			for ( i = 0; i < DK_Number(strDenger); i++ )
			{
				if ( sBuf.Find(strDenger[i]) != -1 )	// 汎用ルータ設定によるコマンド追加かぶり対策
				{
					DebDisp("ルータ本体の設定内容がクラウドリニューアルの追加コマンドと重複しています");
					DebDisp(sBuf);
					return FALSE;
				}
			}
		}
		
		// tuunel selectのリザーブを取得
		int startPos = sBuf.Find("tunnel select ");
		if (startPos != -1)
		{
			int endPos = sBuf.Find(" ", startPos + lstrlen("tunnel select"));
			if (endPos != -1)
			{
				CString result = sBuf.Mid(startPos + lstrlen("tunnel select"));
				
				int valueToAdd = atoi(result);
				BOOL isDuplicate = FALSE;
				
				// 配列内の重複確認
				for ( i = 0; i < n; i++ )
				{
					if (nReserveNo[i] == valueToAdd)
					{
						isDuplicate = TRUE;
						break;
					}
				}				
				// 重複がなければ値を追加
				if ( !isDuplicate )
				{
					if ( n > (TUNNEL_DEF_MAX-1) )
					{
						DebDisp("ルータ本体のトンネル設定数の上限を超えています");
						DebDisp(sBuf);
						return FALSE;
					}
					nReserveNo[n++] = valueToAdd;
				}
			}
		}
		// pp selectのリザーブを取得
		startPos = sBuf.Find("pp select ");
		if (startPos != -1)
		{
			int endPos = sBuf.Find(" ", startPos + lstrlen("pp select"));
			if (endPos != -1)
			{
				CString result = sBuf.Mid(startPos + lstrlen("pp select"));
				
				int valueToAdd = atoi(result);
				BOOL isDuplicate = FALSE;
				
				// 配列内の重複確認
				for ( i = 0; i < n; i++ )
				{
					if (nReserveNo[i] == valueToAdd)
					{
						isDuplicate = TRUE;
						break;
					}
				}				
				// 重複がなければ値を追加
				if ( !isDuplicate )
				{
					if ( n > (TUNNEL_DEF_MAX-1) )
					{
						DebDisp("ルータ本体のトンネル設定数の上限を超えています");
						DebDisp(sBuf);
						return FALSE;
					}
					nReserveNo[n++] = valueToAdd;
				}
			}
		}
// MIRAIGATE接続先が設定されているルータ＝"ipsec ike pre-shared-key [接続先番号] text [接続先の固定IPアドレス]"が必ず存在するので、[接続先の固定IPアドレス]＝パブリックIPとして取得する
// 		startPos = sBuf.Find("ipsec ike pre-shared-key ");	// この語句の後ろにIPアドレスがあるのはMIRAIGATE接続先のみ
// 		if (startPos != -1)
// 		{
// 			GetToken(sBuf, 5, NULL, m_szPublicIp);	// 他のtunnelの設定も取れてしまうので使えない
// 		}
		
		startPos = sBuf.Find("ip filter ");
		if (startPos != -1)
		{
			CheckCmd(sBuf, eFilter);
		}
	}// for (m_nLine = 0; ;) 先行読み終了
	
	if ( !fRtSet )	// ルータ設定で登録の際は既にiniに書かれているのでそちらを参照する
	{
		if ( fJnetMent == FALSE )	// CMS0334よりJ-NETメンテ番号の追加で呼び出しではない
		{
			if  ( m_sIniFileInf.nStatus == SDkKanshiSrvInfo::eNonMode )// g_nHikisu == eHikisuTunnelAdd
			{
				// 複数台のルータがある環境であっても、MIRAIGATE用ルータ設定が登録されているルータは1台のみという前提
				// 	追加トンネル用番号を検索
				m_sIniFileInf.nAddTunnelNo = 1;
				while ( TRUE )
				{	
					BOOL fHit = FALSE;
					for (i = 0; i < DK_Number(nReserveNo); i++) 
					{
						if ( nReserveNo[i] == m_sIniFileInf.nAddTunnelNo ) {
							fHit = TRUE;
							break;
						}
					}
					if ( fHit )
						m_sIniFileInf.nAddTunnelNo++;
					else
						break;
				}
				
				m_sIniFileInf.nAddSubTunnelNo = 1;
				while ( TRUE )
				{	
					BOOL fHit = FALSE;
					for (i = 0; i < DK_Number(nReserveNo); i++) 
					{
						if (nReserveNo[i] == m_sIniFileInf.nAddSubTunnelNo || m_sIniFileInf.nAddSubTunnelNo == m_sIniFileInf.nAddTunnelNo) {
							fHit = TRUE;
							break;
						}
					}
					if ( fHit )
						m_sIniFileInf.nAddSubTunnelNo++;
					else
						break;
				}
				if ( m_sIniFileInf.nAddTunnelNo > m_nRtSetuzokuMax || m_sIniFileInf.nAddSubTunnelNo > m_nRtSetuzokuMax )
				{
					DebDisp("設定可能なTunnel番号がありません。");
					return FALSE;
				}
			}
			DebDisp("nAddSubTunnelNo=%d  nAddSubTunnelNo=%d", m_sIniFileInf.nAddTunnelNo, m_sIniFileInf.nAddSubTunnelNo);
		}
		else
		{
			if ( !m_fJnetMentWrote )
			{
				m_sIniFileInf.nAddJnetMentNo = 1;
				while ( TRUE )
				{	
					BOOL fHit = FALSE;
					for (i = 0; i < DK_Number(nReserveNo); i++) 
					{
						if ( nReserveNo[i] == m_sIniFileInf.nAddJnetMentNo ||
							(m_sIniFileInf.nStatus == SDkKanshiSrvInfo::eDelete && (m_sIniFileInf.nAddJnetMentNo == m_sIniFileInf.nPreTunnelNo || m_sIniFileInf.nAddJnetMentNo == m_sIniFileInf.nPreSubTunnelNo)) ) {	// D状態で消したPRE_TUNNELも適用できない
							fHit = TRUE;
							break;
						}
					}
					if ( fHit )
						m_sIniFileInf.nAddJnetMentNo++;
					else
						break;
				}
				if ( m_sIniFileInf.nAddJnetMentNo > m_nRtSetuzokuMax )
				{
					DebDisp("設定可能なJ-NETメンテ用Tunnel番号がありません。");
					// 専用診断レポート送信
					SDkSindanReport report;
					ZeroMemory(&report, sizeof(SDkSindanReport));
					sprintf(report.szDetail, "日次更新時でルータの自動設定変更を実行しましたが、正常にJ-NET用のトンネルを追加できませんでした。");
					report.szDetail[sizeof(report.szDetail)-1] = '\0';
					report.nGouki = 1;
					report.nSubSystem = eDkGyoumuCommon;
					report.nKind = eC2SndRptHsKdE;
					report.nCode = 90/*eC2SndRptHsCdApp*/;
					report.isRestore = 1;
					report.nEigyoState = 0;
					report.nDetailCode = 9001;
					sprintf(report.szContent, "J-NETルータ設定自動更新失敗");
					
					if ( !CDkM200Common2::WriteSindanReport(report) )
						DebDisp("診断レポート書き込み失敗");
					
					return FALSE;
				}				
				DebDisp("nAddJnetMentNo=%d", m_sIniFileInf.nAddJnetMentNo);
			}	
		}
	}	
	
	pGetFile->SeekToBegin();	// 読込初期位置に戻す
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	専用ローカルファイルと同期コピー
//	<引数>	CString strSendFilePath = 送信済みファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter::SyncLocalFile(CString strSendFilePath)
{
	// router00W.dat、set00W.datと同期
	char szSyncFile[MAX_PATH];
	char szSyncFile2[MAX_PATH];
	
	wsprintf(szSyncFile, "%s\\data\\CM\\router\\router00W.dat", pMxBas->GetBaseDir());
	wsprintf(szSyncFile2, "%s\\data\\CM\\router\\set00W.dat", pMxBas->GetBaseDir());
	ReadOnlyBreak(szSyncFile);
	ReadOnlyBreak(szSyncFile2);
	
	GetPutIniFile(TRUE, &m_sIniFileInf);
	
	if ( m_sIniFileInf.nStatus == SDkKanshiSrvInfo::eNonMode )	// A状態は同期ファイルは消す
	{
		if ( m_cRtPassEx.isExist(szSyncFile) )
		{
			if ( !DeleteFile(szSyncFile) ) {
				OutputDebugString("削除失敗：" + (CString)szSyncFile);
				return FALSE;
			}
		}
		if ( m_cRtPassEx.isExist(szSyncFile2) )
		{
			if ( !DeleteFile(szSyncFile2) ) {
				OutputDebugString("削除失敗：" + (CString)szSyncFile2);
				return FALSE;
			}
		}
	}
	else
	{	
		if ( !CopyFile(strSendFilePath, szSyncFile, FALSE) ) {
			OutputDebugString("削除失敗：" + (CString)szSyncFile2);
			return FALSE;
		}
		if ( !CopyFile(strSendFilePath, szSyncFile2, FALSE) ) {
			OutputDebugString("削除失敗：" + (CString)szSyncFile2);
			return FALSE;
		}
	}
	
	return TRUE;
}

//-------------------------------------------------------------------------------------------
//	<目的>	IPアドレス文字列(例:"123.155.212.15")をBYTE型配列に入れる
//	<引数>	BYTE *ip	= 変換したＩＰアドレスを入れるBYTE型配列(４Byte)のポインタ
//			char *szIP	= 変換するＩＰアドレスの文字列
//--------------------------------------------------------------------------------------------
BOOL CDkRouter::ConvertIP(BYTE *ip, char *szIP)
{
	int i = 0;
	char sep[10] = ".", szCopy[50];
	lstrcpy(szCopy, szIP);
	char *szTmp = strtok(szCopy, sep);
	
	while ( szTmp != NULL ) {
		*(ip+i) = atoi(szTmp);
		szTmp = strtok(NULL, sep);
		i++;
	}
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	Xai環境かどうか
//	<戻値>	TRUE = Xai環境    FALSE Xai環境以外
//----------------------------------------------------------------------------
BOOL CDkRouter::IsXai()
{
	CRegKey regKey;
	if (regKey.Open(HKEY_LOCAL_MACHINE, _T("Software\\Daikoku\\Mx\\CM"), KEY_READ) == ERROR_SUCCESS)
	{
		DWORD dwValue;
		if (regKey.QueryValue(dwValue, "KaiType") == ERROR_SUCCESS)
		{
			if (dwValue == 2018)
				return TRUE;
		}
	}
	return FALSE;
}
