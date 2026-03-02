/******************************************************************************/
//	ファイル名			Router1200.cpp
//	システム名			ＤＫ−Ｍｘ （共通）
//	プログラム種別		ルータ操作クラス
//	概要				ルータ操作クラス
//	作成者				林エンジニアリング
//	バージョン				2019/02/18
/*******************************************************************************

  ※このクラスは　IPSEC対応ルータ(RTX1200系)の読み書き用のクラスである
  
	☆大前提として、ルータ設定ファイルは下記の順番で書き込まれているものとする
	ip route …
	ip lan …
	pp select …
	tunnel select … 〜 ipsec …
	ip filter …
	nat descritpor …
	dns server …
*/

#include "StdAfx.h"
#include "Router.h"
#include "DkTextFile.h"
#include "MxTelnet.h"

static const char *szLine[]   = {"isdn","l64", "l128"};
							
#define	SEARCH_CHR		" \t\n"

#define WAN_API_INI		"\\config\\CM\\router\\wan_apn.ini"
// BOOL g_fBackUpCmd = FALSE;
// BOOL g_fKeepAlive = FALSE;

//	コンストラクタ
//		pDkLan : DK-LAN and OA-LAN set
//		pDkLanには ip,mask,gate をセットする
//		pLogにはログファイル出力クラス
CDkRouter1200::CDkRouter1200(SDkMcnInfo *pDkLan, CFile *pLog/*=NULL*/)
{
	m_nArray		= 0;
	m_nCurrentPP	= 0;			// 処理中のPP
	m_fReadRtType	= FALSE;
	m_fAnonymous	= FALSE;		// アノニマス処理中フラグ
	m_pLogFile		= pLog;			// ログファイルpointer
	m_fSendBin		= FALSE;		// 設定ファイル送信時に".bin"ファイルを送るか否か？
	m_fMS55IpBrank	= TRUE;
	m_bL3GateWay	= FALSE;
	m_fSisRoboVwM	= FALSE;
	m_nStockVwNo	= 0;
	ZeroMemory(m_szStockVwID,	sizeof(m_szStockVwID));
	ZeroMemory(m_szStockVwPass, sizeof(m_szStockVwPass));
	
	ZeroMemory(&m_sRouter, sizeof(m_sRouter));
	// DK-LAN backup
	memset(m_pDkLan, 0, sizeof(m_pDkLan));
	if ( pDkLan != NULL ) {
		memcpy(&m_pDkLan[0], pDkLan++, sizeof(SDkMcnInfo));	// DK LAN　Set
		memcpy(&m_pDkLan[1], pDkLan  , sizeof(SDkMcnInfo));	// OA LAN　Set
	}
	
	m_fOA_LAN = FALSE;

	ZeroMemory(m_szSendLuaFile, sizeof(m_szSendLuaFile));
	ZeroMemory(m_szSendPrmFile, sizeof(m_szSendPrmFile));

	noAngoPass = CDkMxBas::GetMxCommIniInt("DEBUG", "NO_ROUTER_ANGO", 1);
#if NO_ANGOU_PASS
	noAngoPass = 1;
#endif
	ReadRegstryData();	// レジストリデータ読み込み
	
	ZeroMemory(&m_sPrm, sizeof(m_sPrm));
	m_sPrm.nLuaTunnelMin = 80;
	m_sPrm.nLuaTunnelMax = 95;

//===== ↓LTE通信対応 ==================================================================
	char szIniFile[MAX_PATH];
	wsprintf(szIniFile, "%s%s", pMxBas->GetBaseDir(), WAN_API_INI);
	::GetPrivateProfileString("WAN_CONFIG", "NAME", "mobiledata.ntt.com", m_szWanApiName, sizeof(m_szWanApiName), szIniFile);
//===== ↑LTE通信対応 ==================================================================
}

//----------------------------------------------------------------------------
//	<概要>	Descriptionの読みとり
//	<引数>	const char *input = コマンド
//          char output[][INPUT_USER_MAX] = 項目ごとに格納
//----------------------------------------------------------------------------
void CDkRouter1200::GetDescriptionParam(const char *input, CString output[])
{
    const char *start = input;
    int nCount = 0;
	
    while ( (start = strchr(start, '=')) != NULL )
	{
        start++;  // '=' の次の文字に移動
		
        const char *end = strchr(start, ' ');
        if (end == NULL) {
            end = input + strlen(input); // 最後のスペースがない場合、文字列の終わりまでを対象とする
        }
		
        // 抽出した文字列をCStringに格納
        int len = end - start;
        CString extractedStr(start, len);
        extractedStr.Remove('"');	// '"'を除去
		extractedStr.Remove('\n');

        output[nCount] = extractedStr;
        nCount++;
        start = end;
    }
}

//----------------------------------------------------------------------------
//	<概要>	先行的にファイルを精査する(設定をカンニングしておく)
//	<引数>  char* szFile = 読み込みファイル
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter1200::PreReadFile(char* szFile)
{
	FILE *fp = fopen(szFile, "r");// FILEオープン
	if ( fp == NULL )
	{
// 		CDkMxBas::WriteSindanReportFileIO(eDkGyoumuCommon, eFileOpen, szFile, 0, 0);
		return FALSE;
	}
	
	char szBuf[RT_CMD_LINE_MAX];
	char *pIpRoute	= "ip route";
	char *pSel		= "pp select";
	char *ppp		= "pppoe use lan2";
	char *pTunnel	= "tunnel";
	char *dns		= "dns server pp";
	char *pIpFilter	= "ip filter";
	char *pSnmp		= "snmp";
	char *pDescription = "description";
	m_nAdslProc = 0;	// ADSL接続先と思われる接続先番号
	int	nSetuzokuNum = 0;
	int nSIS05L2 = 0;
	
	int nRtType = eNon;
	while ( fgets(szBuf, sizeof(szBuf), fp) )
	{
//===== ↓追加 ============================================================================
		AnalizeRouterType(szBuf);	// ルータが判明するまで先行よみさせない
		if ( m_nRtTypeKind < eRtType1200 )
			continue;
//===== ↑追加 ============================================================================
		
		CString strAnalize = szBuf;
		strAnalize.TrimLeft();		// 先頭の空白を取り除く
//===== ↓LTE通信対応 ==================================================================
		if ( strncmp(strAnalize, pDescription, lstrlen(pDescription)) == 0 )  // ip filterがきた
		{
			char *p = strtok(szBuf, SEARCH_CHR);	// 定義値の取得
			p = strtok(NULL, SEARCH_CHR);
			
			CString strItem[10];
			GetDescriptionParam((CStoS)strAnalize, strItem);// パラメータ取得
			
			if ( atoi(p) >= 10 ) {	// Descriptor10以上がある時点でLTEが設定してあることは確実
				// 	m_sRouter.fBbUse = eLaboBB_FixAdsl;
				m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenLTE;
				m_sRouter.fBbUse = eLaboBB_FixAdsl;
			}
			
			switch ( atoi(p) )
			{
			case 10:
				{
					int nKind[] = {eKaisenADSL, eKaisenFixADSL, eKaisenCableHikari};		// ケーブル光はADSL固定扱い
					m_sRouter.sTmp.nKaisenKind = nKind[atoi((CStoS)strItem[1]) - 1];		// type
					SetIpC2B((CStoS)strItem[2], m_sRouter.sTmp.ip, m_sRouter.sTmp.netmask);	// 固定IP
					m_sRouter.sTmp.nMTU = atoi((CStoS)strItem[3]);							// MTU
					SetIpC2B((CStoS)strItem[4], m_sRouter.sTmp.gateway);					// gateway
				}
				break;
			case 11:
				break;
			case 12:
				m_sRouter.sTmp.nLuaPort = atoi((CStoS)strItem[0]);			// LUAport
				lstrcpy(m_sRouter.sTmp.szPH_CodeA2910, (CStoS)strItem[1]);	// PHCODE
				break;
			case 13:
				lstrcpy(m_sRouter.sTmp.szID, (CStoS)strItem[0]);
				break;
			case 14:
				lstrcpy(m_sRouter.sTmp.szPass, (CStoS)strItem[0]);
				break;
			}
		}
//===== ↑LTE通信対応 ==================================================================	
		if ( strncmp(strAnalize, pSel, lstrlen(pSel)) == 0 )  // pp selectがきた
		{
			char *p = strtok(szBuf, SEARCH_CHR);	// プライマリDNSのIP取得
			p = strtok(NULL, SEARCH_CHR);
			p = strtok(NULL, SEARCH_CHR);
			
			nSetuzokuNum = atoi(p) - 1;			// 1〜
			if ( nSetuzokuNum < 0 ) continue;	// anonymousは何もしない
			
			m_sRouter.con[nSetuzokuNum].no	= nSetuzokuNum + 1;	// 接続先番号SET
			m_sRouter.con[nSetuzokuNum].port= ePortB1;
			m_nArray++;
		}
		if ( strncmp(strAnalize, ppp, lstrlen(ppp)) == 0 )  // pppoe use lan2は回線種別が「ADSL,ADSL固定」選択時のみ存在する設定句
		{
			m_nAdslProc = nSetuzokuNum + 1;	 // ADSL,ADSL固定についての設定がある
		}
		if ( strncmp(strAnalize, "nat descriptor static 2012 2", lstrlen("nat descriptor static 2012 2")) == 0 )  // "nat descriptor static 2012 2"があるということはSIS05がLAN2だ
		{
			if ( m_sRouter.ncon[nSIS05L2].type < 0 )
			{
				m_sRouter.ncon[nSIS05L2].type -= 10; 
			}
		}
		
		if ( strncmp(strAnalize, pIpFilter, lstrlen(pIpFilter)) == 0 )  // ip filterがきた
		{
			char *p = strtok(szBuf, SEARCH_CHR);	// プライマリDNSのIP取得
			p = strtok(NULL, SEARCH_CHR);
			p = strtok(NULL, SEARCH_CHR);
			
			int nFilNo = atoi(p);
			if ( nFilNo >= 2601 && nFilNo <= 2649 )	
			{
				p = strtok(NULL, SEARCH_CHR);
				if ( strncmp(p, "pass", lstrlen("pass")) == 0 )
				{
					p = strtok(NULL, SEARCH_CHR);
					
					if ( strncmp(p, IP_JNET_MAIN_IP, lstrlen(IP_JNET_MAIN_IP)) == 0 )
					{
						m_sRouter.m_nJnetNo = nFilNo - 2600;
					}
					if ( strncmp(p, IP_JNET_SUB_IP, lstrlen(IP_JNET_SUB_IP)) == 0 )
					{
						m_sRouter.m_nJnetNo2 = nFilNo - 2600;
					}// この接続先番号の判断の仕方がまずいなら"aes-cbc md5-hmac"でみる予定
				}
			}
		}
		
		if ( strncmp(strAnalize, pIpRoute, lstrlen(pIpRoute)) == 0 )  // ip routeがきた
		{
			char *p = strtok(szBuf, SEARCH_CHR);	// プライマリDNSのIP取得
			p = strtok(NULL, SEARCH_CHR);
			p = strtok(NULL, SEARCH_CHR);
			
			if ( p && strncmp(p, SIS05_IP, lstrlen(p)) == 0 )	// SIS05である
			{
				p = strtok(NULL, SEARCH_CHR);	// gateway
				p = strtok(NULL, SEARCH_CHR);	// tunnel or pp
				p = strtok(NULL, SEARCH_CHR);
				if ( p )
				{
					nSIS05L2 = atoi(p)-1;
				}
			}
			if ( p && strncmp(p, IP_CAR_MENT_IP, lstrlen(p)) == 0 )	// 車両管理システムメンテナンス用設定のＩＰ
			{
				p = strtok(NULL, SEARCH_CHR);	// gateway
				p = strtok(NULL, SEARCH_CHR);	// tunnel or pp
				p = strtok(NULL, SEARCH_CHR);
				if ( p )
				{
					m_sRouter.m_nCarMentNo = atoi(p);
				}
			}
		}
		if ( strncmp(strAnalize, pTunnel, lstrlen(pTunnel)) == 0 )  // tunnel selectがきた
		{
			char *p = strtok(szBuf, SEARCH_CHR);// tunnel
			p = strtok(NULL, SEARCH_CHR);		// Select or backup
			
			if ( strncmp(p, "select", lstrlen("select")) == 0 )
			{
				p = strtok(NULL,  SEARCH_CHR);	// No
				
				int nNo = atoi(p);
				if ( nNo >= m_sPrm.nLuaTunnelMin && nNo <= m_sPrm.nLuaTunnelMax )	// インターネットからのフリーPC接続用設定
					continue;
				
				nSetuzokuNum = atoi(p) - 1;			// 1〜
				if ( nSetuzokuNum < 0 ) 
					continue;	// anonymous or IP電話は何もしない
				
				if ( m_sRouter.con[nSetuzokuNum].no == 0 ) {	// pp selectなかった
					m_nArray++;
				}
				
				m_sRouter.con[nSetuzokuNum].no = nSetuzokuNum + 1;	// 接続先番号SET
			} else 
			if ( strncmp(p, "backup", lstrlen("backup")) == 0 )
			{
				p = strtok(NULL, SEARCH_CHR);
				if ( strncmp(p, "pp", lstrlen("pp")) == 0 )	// ****** (DKVPNか個別LAN)+Dialup ****** 
				{
					p = strtok(NULL, SEARCH_CHR);
					nSetuzokuNum = atoi(p) - 1;				// 1〜
					if ( nSetuzokuNum < 0 ) continue;		// anonymousは何もしない
					m_sRouter.ncon[nSetuzokuNum].type = -1;
				} else {									// ****** (DKVPNか個別LAN) *************
					m_sRouter.ncon[nSetuzokuNum].type = -2;
				}
			} 
		}
		if ( strncmp(strAnalize, dns, lstrlen(dns)) == 0 )  //dns server pp ADSL番号？ 
		{
			char *p = strtok(szBuf, SEARCH_CHR);	// プライマリDNSのIP取得
			p = strtok(NULL, SEARCH_CHR);
			p = strtok(NULL, SEARCH_CHR);
			p = strtok(NULL, SEARCH_CHR);
			if ( m_nAdslProc == atoi(p) ) {	// MIRAIGATE閲覧-BB接続にADSLが使用されているということがわかる
				m_sRouter.fBbUse = eLaboBB_Adsl;
			}
		}
		
		if ( strncmp(strAnalize, "ip lan1 address", lstrlen("ip lan1 address")) == 0 )  // OA_LANか見極めるために
		{
			char	*p= strtok(szBuf, SEARCH_CHR);	// ip
			p = strtok(NULL, SEARCH_CHR);	// lan1
			p = strtok(NULL, SEARCH_CHR);	// address
			p = strtok(NULL, SEARCH_CHR);	// このルータのIPアドレス
			
			ZeroMemory(m_thisIp, sizeof(m_thisIp));
			ConvertIpDelmit(p, m_thisIp);
		}
		
		if ( strncmp(strAnalize, "ip tunnel address", lstrlen("ip tunnel address")) == 0 )
		{
			char	*p= strtok(szBuf, SEARCH_CHR);	// ip
			p = strtok(NULL, SEARCH_CHR);	// tunnel
			p = strtok(NULL, SEARCH_CHR);	// address
			p = strtok(NULL, SEARCH_CHR);	// セカンダリアドレス
			
			m_sRouter.m_nIpTelNo = nSetuzokuNum+1;
			
			// ZeroMemory(m_thisIp, sizeof(m_thisIp));
			// ConvertIpDelmit(p, m_thisIp);
		}
		// MT-01R、MT-55Rの場合や、何らかの理由でPH会員番号が取得できなかった場合は、以下の設定句に記載された値を取得する。
		if ( strncmp(strAnalize, "snmp", lstrlen("snmp")) == 0 && m_cRtPassEx.m_nPH_Code == 0 )	// snmp △ syslocation △ "Hall △ 12345678[③PH会員番号].0123[⑥ヒント] "
		{
			char	*p= strtok(szBuf, " \t\n");	// snmp
			p = strtok(NULL, " \t\n");	// syslocation
			p = strtok(NULL, " \t\n");	// Hall	
			p = strtok(NULL, " \t\n.");	// PH会員番号
			m_cRtPassEx.m_nPH_Code = atoi(p);
		}
//===== ↓ケーブル光対応 ==================================================================
		if ( strncmp(strAnalize, "no dns server pp", lstrlen("no dns server pp")) == 0 )
		{
			m_fBBsetLAN3 = TRUE;
		}
		if ( strncmp(strAnalize, "dns private address spoof on", lstrlen("dns private address spoof on")) == 0 )
		{
			m_fBBsetLAN3 = TRUE;
		}
//===== ↑ケーブル光対応 ==================================================================
//===== ↓クラウドIP電話対応 =====================================================================
		if ( strncmp(strAnalize, "dhcp scope option 2 dns", lstrlen("dhcp scope option 2 dns")) == 0 )
		{
			m_sRouter.m_nFqdnState = eFqdnLAN3;
		}
//===== ↑クラウドIP電話対応 =====================================================================
//===== ↓クラウドリニューアル対応 ========================================================
		int nFind = strAnalize.Find("# LIFT_PHASE=");
		if ( nFind != -1 )
		{
			CString numberStr = strAnalize.Mid(nFind + strlen("# LIFT_PHASE=")); // 数値部分を抽出
			m_nFileReadStatus = atoi(numberStr);
		}
//===== ↑クラウドリニューアル対応 ========================================================
	}
	fclose(fp);
	
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	TFTPファイルを読む(TFTPコマンドによってできたファイル)
//	<引数>	int  no			= ルータ番号
//          char *szFile	= ファイル名を指定して読み込む場合のファイル名
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter1200::ReadTftpFile(int no, char* szFile)
{	
//===== ↓クラウドリニューアル対応 ========================================================
	GetPutIniFile(TRUE, &m_sIniFileInf);
	
	m_sRouter.m_nNonConnect	 = (m_sIniFileInf.nStatus < SDkKanshiSrvInfo::eLiftMode) ? m_sIniFileInf.nAddTunnelNo : m_sIniFileInf.nPreTunnelNo;			// 未接続定義
	m_sRouter.m_nNonConnectBk= (m_sIniFileInf.nStatus < SDkKanshiSrvInfo::eLiftMode) ? m_sIniFileInf.nAddSubTunnelNo : m_sIniFileInf.nPreSubTunnelNo;	// 未接続定義BK
	
	if ( m_sIniFileInf.nStatus >= SDkKanshiSrvInfo::eDelete )
	{
		m_sRouter.m_nMemNonConnect  = m_sRouter.m_nNonConnect;
		m_sRouter.m_nMemNonConnectBk= m_sRouter.m_nNonConnectBk;
		m_sRouter.m_nNonConnect		= 0;
		m_sRouter.m_nNonConnectBk	= 0;
	}
	
	m_nFileReadStatus = 0;
//===== ↑クラウドリニューアル対応 ========================================================

	g_line = 0;	// デバッグ用行数カウンタリセット
	
	CString strMsg;

	m_nRtTypeKind	= -1;
	m_fOA_LAN		= FALSE;
	m_fMS55IpBrank	= TRUE;	// ＭＳ−５５ＩＰアドレスはブランクだ
	
	FILE	*fp;
	char	szBuf[RT_CMD_LINE_MAX];
	BOOL	fMsg = FALSE;	// 不正構文の警告メッセージを出したか？

//===== ↓ケーブル光対応 ==================================================================
	ZeroMemory(&m_sWkHikari, sizeof(m_sWkHikari));
	m_fBBsetLAN3 = FALSE;
//===== ↑ケーブル光対応 ==================================================================
	
	m_sRouter.port[0].kaisen = 1;	// 回線１の回線種類のデフォルトは"INS64"
	m_nArray	 = 0;
	m_fReadRtType= FALSE;
	m_sRouter.m_nFqdnState = eFqdnNon;
	// 	m_fAnalog	 = FALSE;
	// 	m_sRouter.shOverTimer = 3600;	// 接続超過タイマ　デフォルト値 = 60 × 60 = 3600
	
	m_bL3GateWay = FALSE;	// LAN3のGateWayアドレスがセットされてない状態にする
	
	if ( szFile == NULL )	// ファイル名を指定して読み込まないとき
		szFile = GetFileName(no, eWrite);	// ルータ番号にしたがってファイル名を作成
	
//===== ↓追加 ============================================================================
	if ( !PreReadFile(szFile) )
		return FALSE;
	if ( m_nRtTypeKind < eRtType1200 )
		return CDkRouter1000::ReadTftpFile(no, szFile);
//===== ↑追加 ============================================================================

	fp = fopen(szFile, "r");// FILEオープン
	if (!fp)
	{
		return FALSE;
	}

	// データを一行づつよんでいく
	while ( fgets(szBuf, sizeof(szBuf), fp) )
	{
		g_line++;
#if _DEBUG
  		if ( g_line == 165 )
  		{
  			strMsg.Format("%d行目→%s", g_line, szBuf);
  			DebDisp(strMsg);
  		}
#endif
		if ( m_fAnonymous ) {
			if ( strstr(szBuf,"pp enable anonymous") != NULL ) { // anonymous終了
				m_fAnonymous = FALSE;
			}
			// continue;
		}
		
		CString strAnalize = szBuf;
		strAnalize.TrimLeft();		// 先頭の空白を取り除く
		
// 		char szLine[2048];
// 		lstrcpy(szLine, szBuf);
		m_strReadLine = szBuf;	// 判断を簡略化させるために追加 07/08/31
		m_strReadLine.TrimRight();
		m_strReadLine.TrimLeft();
		
		if ( Analize((CStoS)strAnalize) == -1 && !fMsg )
		{
			if ( !IsRtTypeEditable(m_sRouter.kind) )	// 規定外云々の前に、このＰＧでは読み込めない設定ファイル
			{
				fclose(fp);
				return TRUE;	// 構文エラーではないためTRUEを返す
			}

			fMsg = TRUE;// 解析不可能な構文がきてしまった場合
			
			if ( !m_fNoMsgbox )
			{
				::MessageBox(m_hwndView, "読み込んだ設定ファイルには、規定外の設定内容が含まれています。\r\n確認する場合は、「汎用ルータ設定」で確認して下さい。", CDkMxBas::m_sProgJName, MB_OK | MB_ICONEXCLAMATION);
			}
			strMsg.Format("%d行目で解析不可能な構文を検出しました", g_line);
			DebDisp(strMsg);
#if _DEBUG
			::MessageBox(m_hwndView, strMsg, "ルータ設定(DEBUG)", MB_OK | MB_ICONEXCLAMATION);
#endif
		}
	}
	
	if ( m_sRouter.uip[0] == 0 )
	{
		CopyMemory(m_sRouter.uip, m_sRouter.ip, sizeof(BYTE) * IP_BYTE);
		for ( int i = 0; i < 4; i++ )
			m_sRouter.uip[i] &= m_sRouter.mask[i];
	}
	
	for ( int i = 0; i < DK_Number(m_sRouter.con); i++ )  // 従来の構造体変数にデータが入ってるので新規対応構造体にも反映させる
	{
		memcpy((void*)&m_sRouter.ncon[i], (void*)&m_sRouter.con[i], sizeof(SDkConnectInfo));
	}
	
	if ( m_sRouter.fBbUse < 0 )  // ＢＢ接続は選択されていない
	{
		m_sRouter.fBbUse = eLaboBB_NoUse;
	}
	
	if ( GetRtCategory(m_sRouter.kind) >= eRtType1000 )	// RTX1000系の個別ＶＰＮは○固定
	{
		m_sRouter.sLAN3.sConNaiyo[eL3KobetuVPN].fNeed = TRUE;
	}
	
	// Lan2接続があるはずなので、Bri1接続のメンテナンス用設定は無効にして良い。
	// ※メンテナンス用設定の接続先番号「anonymous」は選択不可とし、
	// 設定されていた場合は無効にする。
	if ( m_sRouter.nLuaPort > 0 )	// 設定されたらメンテナンスのB1ポート選択は強制的にブランクにする
	{
		if ( m_sRouter.fMent == eMentAnonymous || (m_sRouter.fMent == eMentAuto && m_sRouter.port[1].kaisen == eKaisenADSL) )
			m_sRouter.fMent = eMentNoUse;
	}
	
//===== ↓LTE通信対応 ==================================================================
	// 全部読み終わった後にL2にLTEの値をセットする
	if ( m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE )	// L2に入れた情報はLTEのもので、本当のL2情報はdescriptonに定義してある
	{
		SDkConnectInfoNew *pInfo = GetAdslInfo();
		
		ZeroMemory(&m_sRouter.sLte, sizeof(m_sRouter.sLte));
		
		// L2情報(configに書かれているのはLTEの情報)をLTEにいれる
		CopyMemory(m_sRouter.sLte.ip, pInfo->fixIp, sizeof(m_sRouter.sLte.ip));
		m_sRouter.sLte.nMTU		= m_sRouter.nMTU;
		m_sRouter.sLte.nLuaPort = m_sRouter.nLuaPort;
		lstrcpy(m_sRouter.sLte.szPH_CodeA2910, m_sRouter.szPH_CodeA2910);
		lstrcpy(m_sRouter.sLte.szID, m_sRouter.szAdslID);
		lstrcpy(m_sRouter.sLte.szPass, m_sRouter.szAdslPass);		
		
		// discriptionの情報をL2にいれる
		pInfo->type = 0;
		m_sRouter.port[ePortIdxL2].kaisen = m_sRouter.sTmp.nKaisenKind;
		switch ( m_sRouter.sTmp.nKaisenKind )
		{
		case eKaisenADSL:
			pInfo->type = eADSL;
			break;
		case eKaisenFixADSL:
			pInfo->type = eFixADSL;
			break;
		case eKaisenCableHikari:
			pInfo->type = eFixADSL;
			break;
		}
		
		CopyMemory(pInfo->fixIp, m_sRouter.sTmp.ip, sizeof(m_sRouter.sTmp.ip));
		CopyMemory(pInfo->yobi1, m_sRouter.sTmp.netmask, sizeof(m_sRouter.sTmp.netmask));
		CopyMemory(pInfo->yobi2, m_sRouter.sTmp.gateway, sizeof(m_sRouter.sTmp.gateway));
		pInfo->port = ePortL2;
		
		m_sRouter.nMTU		= m_sRouter.sTmp.nMTU;
		m_sRouter.nLuaPort	= m_sRouter.sTmp.nLuaPort;
		lstrcpy(m_sRouter.szPH_CodeA2910, m_sRouter.sTmp.szPH_CodeA2910);
		lstrcpy(m_sRouter.szAdslID, m_sRouter.sTmp.szID);
		lstrcpy(m_sRouter.szAdslPass, m_sRouter.sTmp.szPass);
	}
//===== ↑LTE通信対応 ==================================================================
	
	fclose(fp);
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	ルータ機種を判別する
//	<引数>	char *buf = ルータ機種が記載されていると思われる注釈文
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
int CDkRouter1200::AnalizeRouterType(char *buf)
{
	char	*p;
	BOOL	fFind = FALSE;
	
	if ( *buf != '#' )		// 注釈以外
		return 1;
	
	if ( m_fReadRtType )	// 処理済みなら何もせん
		return 1;
	
	p = strtok(buf+1, SEARCH_CHR);
	if ( p == NULL )
		return 1;
	
	//	RTxxx
	for ( int type = 0; type < DK_Number(szRtType); type++ )
	{
		if ( szRtType[type] == NULL )
			continue;
		
		if (p && strncmp(p, szRtType[type], lstrlen(szRtType[type])) == 0)
		{
			m_sRouter.kind = type + 1;
			fFind = TRUE;
			
			switch ( m_sRouter.kind )  // ルータ種別
			{
			case eRT103i:	case eRT52pro:	case eRTA52i:
				m_sRouter.port[0].kaisen = eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[1].kaisen = eKaisenNoEdit;	// 使用不可
				m_sRouter.port[2].kaisen = eKaisenNoEdit;	// 使用不可
				m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenNoEdit;	// 使用不可
				DebDisp("読み込んだルータ設定ファイルはＩＳＤＮルータ");
				m_nRtTypeKind = eRtTypeISDN;	// ＩＳＤＮルータ
				break;	
			case eRTA54i:	case eRTA55i:	case eRT57i:	case eRT58i:
				m_sRouter.port[0].kaisen = eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[1].kaisen = eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[2].kaisen = eKaisenNoEdit;	// 使用不可
				m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenNoEdit;	// 使用不可
				DebDisp("読み込んだルータ設定ファイルはブロードバンドルータ");
				m_nRtTypeKind = eRtTypeBB;		// ブロードバンドルータ
				break;
			case eRTX1000: case eRTX1000_OA: case eRTX1100: case eRTX1100_OA:
				m_sRouter.port[0].kaisen = eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[1].kaisen = eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[2].kaisen = eKaisenBlank;	// とりあえずブランク
//===== ↓LTE対応 ======================================================================
				m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenNoEdit;	// 使用不可
//===== ↑LTE対応 ======================================================================
				
				DebDisp("読み込んだルータ設定ファイルはIPSEC対応ルータ");
				m_nRtTypeKind = eRtType1000;	// IPSEC対応ルータ
				
				// OA_LANのＩＰアドレス
				if ( m_thisIp[0] == 192 && m_thisIp[1] == 168 && (m_thisIp[2] == 60 || m_thisIp[2] == 61) && m_thisIp[3] == 1 ) 
				{
					m_fOA_LAN = TRUE;
					if ( m_sRouter.kind == eRTX1000 )
						m_sRouter.kind = eRTX1000_OA;
					else if ( m_sRouter.kind == eRTX1100 )
						m_sRouter.kind = eRTX1100_OA;
				}
				break;
			case eRTX1200: case eRTX1200_OA: case eRTX1210: case eRTX1210_OA: case eRTX1220: case eRTX1220_OA:
				m_sRouter.port[0].kaisen = (m_sRouter.kind == eRTX1220 || m_sRouter.kind == eRTX1220_OA) ? eKaisenNoEdit : eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[1].kaisen = eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[2].kaisen = eKaisenBlank;	// とりあえずブランク
//===== ↓LTE対応 ======================================================================
				m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenNoEdit;	// 使用不可
				if ( CDkM200Common::GetOptionInfo(eDkGyoumuCommon, eOptLte) )
					m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenBlank;// とりあえずブランク
//===== ↑LTE対応 ======================================================================
				DebDisp("読み込んだルータ設定ファイルはIPSEC対応ルータ");
				m_nRtTypeKind = eRtType1200;	// IPSEC対応ルータ
				
				// OA_LANのＩＰアドレス
				if ( m_thisIp[0] == 192 && m_thisIp[1] == 168 && (m_thisIp[2] == 60 || m_thisIp[2] == 61) && m_thisIp[3] == 1 ) 
				{
					m_fOA_LAN = TRUE;
					if ( m_sRouter.kind == eRTX1200 )
						m_sRouter.kind = eRTX1200_OA;
					else if ( m_sRouter.kind == eRTX1210 )
						m_sRouter.kind = eRTX1210_OA;
					else if ( m_sRouter.kind == eRTX1220 )
						m_sRouter.kind = eRTX1220_OA;
				}
				ZeroMemory(&m_sPrm, sizeof(m_sPrm));
				m_sPrm.nLuaTunnelMin = 80;
				m_sPrm.nLuaTunnelMax = 95;
				break;
				
			case eRTX830: case eRTX830_OA:
				m_sRouter.port[0].kaisen = eKaisenNoEdit;	// 使用不可
				m_sRouter.port[1].kaisen = eKaisenBlank;	// とりあえずブランク
				m_sRouter.port[2].kaisen = eKaisenNoEdit;	// 使用不可
//===== ↓LTE対応 ======================================================================
				m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenNoEdit;	// 使用不可
				if ( CDkM200Common::GetOptionInfo(eDkGyoumuCommon, eOptLte) )
					m_sRouter.port[ePortIdxUSB1].kaisen = eKaisenBlank;// とりあえずブランク
//===== ↑LTE対応 ======================================================================
				DebDisp("読み込んだルータ設定ファイルはIPSEC対応ルータ");
				m_nRtTypeKind = eRtType1200;	// IPSEC対応ルータ
				
				// OA_LANのＩＰアドレス
				if ( m_thisIp[0] == 192 && m_thisIp[1] == 168 && (m_thisIp[2] == 60 || m_thisIp[2] == 61) && m_thisIp[3] == 1 ) 
				{
					m_fOA_LAN = TRUE;
					if ( m_sRouter.kind == eRTX830 )
						m_sRouter.kind = eRTX830_OA;
				}
				
				m_sPrm.nLuaTunnelMin = 16;
				m_sPrm.nLuaTunnelMax = 20;
				break;
			}
			break;
		}
	}
	
	if ( fFind )	// ルータタイプが判明した
	{
		m_fReadRtType = TRUE;	// ルータタイプ判明済みフラグON
		p = strtok(NULL, SEARCH_CHR);
		if (p) {
			strncpy0(m_sRouter.szRev, p+4, sizeof(m_sRouter.szRev));	// ルータリビジョンセット
		}
	}
	
	return eNon;	// ただの注釈文の場合はeNon
}

//-----------------------------------------------------------------------------
//	<概要>	構文を解析する
//	<引数>	char *buf = 対象となる構文
//	<戻値>	 0(=eNon)		= 無害な無効構文(注釈など)
//			-1(=eSyntaxErr)	= 解析不可能な構文
//			 1〜			= 有効な構文であって、処理した処理タイプ
//-----------------------------------------------------------------------------
int CDkRouter1200::Analize(char *buf)
{
 	if ( m_nRtTypeKind == eRtTypeISDN )	// ISDNルータ専用
 	{
 	// 	return CDkRouter::Analize(buf);
		return -1;
 	}
 	if ( m_nRtTypeKind == eRtTypeBB )	// ＢＢルータ専用
 	{
 	// 	return CDkRouterBB::Analize(buf);
		return -1;
 	}
	if ( m_nRtTypeKind == eRtType1000 )	// RTX1000ルータ専用
	{
		return CDkRouter1000::Analize(buf);
 	}

	const char *szCmd01[] = {	// RTX1000ルータ専用
		"pp ",	"ip ",	"isdn ",	"nat ",	"dns ",	"analog ", "pppoe ", "tunnel", "ipsec", "dhcp"
	//	以下処理しない
	//	"administrator",			"login",		"ppp",
	//	"alarm",	"analog",		"clear",		"dhcp",		/*"dns"*/,
	//	"save",		"security",		"tftp",			"tunnel",
	};
	
	if ( *buf == '\n' )	// 空白行(改行)
		return eNon;
	
	if ( AnalizeRouterType(buf) == eNon )
		return eNon;
	
	int nResult = eNon;	// 各処理の結果
	
	// 第１引数検索
	BOOL fFind = FALSE;
	for ( int type = 0; type < DK_Number(szCmd01); type++ ) 	// マッチする構文をszCmd01内で探す
	{
		if (strncmp(buf, szCmd01[type], lstrlen(szCmd01[type])) == 0) {
			fFind = TRUE;
			break;
		}
	}
	if ( fFind == FALSE )	// 解析不可能な第１引数の構文
		return eNon;
	
	char *p = strtok(buf, SEARCH_CHR);
	type++;
	
	switch ( type )
	{
	case 1:		//	pp
		nResult = ProcPP(p);
		break;
	case 2:		//	ip
		nResult = ProcIP(p);
		break;
	case 3:		//	isdn
		nResult = CDkRouter::ProcISDN(p);
		break;
	case 4:		//	nat
		nResult = ProcNAT(p);
		break;
	case 5:		//	dns
		nResult = ProcDNS(p);
		break;
	case 6:		//	analog
		nResult = ProcAnalog(p);
		break;
	case 7:		//	pppoe
		nResult = ProcPPPOE(p);
		break;
	case 8:		//	tunnel
		nResult = ProcTunnel(p);
		break;
	case 9:		//  ipsec
		nResult = ProcIPSEC(p);
		break;
	case 10:	//  dhcp
		nResult = ProcDHCP(p);
		break;
	default :
		break;
	}
	
	if ( nResult == eSyntaxErr )  // 構文エラーが発生した
		return eSyntaxErr;
	
	return type;
}

// PPPOEの処理をする
int CDkRouter1200::ProcPPPOE(char *buf)
{
	char *p = strtok(NULL, SEARCH_CHR);
	
	if ( p && strcmp(p, "use") == 0 )
	{
		p = strtok(NULL, SEARCH_CHR);	// プライマリDNSのIP取得
		if ( p && strcmp(p, "lan2") == 0 ) {
			m_sRouter.m_nAdslNo = m_nCurrentPP+1;	// ADSL接続のppである VWnoは他の番号である事が確定
			if ( m_sRouter.port[1].kaisen == eKaisenBlank )
				m_sRouter.port[1].kaisen = eKaisenADSL;
		}
	}
	
	return eNon;
}

// DNSの処理をする
int CDkRouter1200::ProcDNS(char *buf)
{
	char *p = strtok(NULL, SEARCH_CHR);
//===== ↓ケーブル光対応 ==================================================================
	int count = 0;
    for (int i = 0; i < m_strReadLine.GetLength(); i++)
	{
        if (m_strReadLine[i] == '.')
            count++;
    }
//===== ↑ケーブル光対応 ==================================================================

	if ( p && strcmp(p, "server") == 0 )
	{
		p = strtok(NULL, SEARCH_CHR);	// プライマリDNSのIP取得
		if ( p && (strcmp(p, "pp") != 0 && count > 3) )	// ip アドレスが2つ定義されていること
		{	
			if ( m_sRouter.fBbUse == eLaboBB_KobetuLan )  // 個別LAN
			{
				ConvertIpDelmit(p, m_sRouter.sLAN3.sPriSecIp.ip);	// プライマリDNSのIP配列に格納
				p = strtok(NULL, SEARCH_CHR);	// セカンダリDNSのIP取得
				ConvertIpDelmit(p, m_sRouter.sLAN3.sPriSecIp.mask);	// セカンダリDNSのIP配列に格納
			}
			else
			{
				for (int n = 0; n < SETU_MAX; n++)
				{
					if ( m_sRouter.m_nVWNo	 == m_sRouter.con[n].no &&  m_sRouter.fBbUse <= 0 ||	// VWの設定のみ時
						 m_sRouter.m_nAdslNo == m_sRouter.con[n].no &&  m_sRouter.fBbUse >  0)		// ADSLの設定のとき
					{
						m_sRouter.provider = GetProviderNo(p, m_sRouter.con[n].ip, m_sRouter.con[n].mask);	// プロバイダ番号取得
						ConvertIpDelmit(p, m_sRouter.con[n].ip);	// プライマリDNSのIP配列に格納
						p = strtok(NULL, SEARCH_CHR);	// セカンダリDNSのIP取得
						ConvertIpDelmit(p, m_sRouter.con[n].mask);	// セカンダリDNSのIP配列に格納
						break;
					}
				}
			}
		}
	}
	
	return eNon;
}

// analogの処理をする
int CDkRouter1200::ProcAnalog(char *buf)
{
	const char *use = "use";
	const char *off = "off";
	char *p;
	
	p = strtok(NULL, SEARCH_CHR);	// use
	if ( p == NULL )
		return eNon;
	if ( lstrcmp(p ,use) != 0 )
		return eNon;
	
	p = strtok(NULL, SEARCH_CHR);	// コネクタ番号取得
	if ( p == NULL )
		return eNon;
	if ( isalpha(*p) )
		return eNon;
	
	p = strtok(NULL, SEARCH_CHR);	// off
	if ( p == NULL )
		return eNon;
	if ( lstrcmp(p ,off) != 0 )
		return eNon;

//    m_fAnalog = TRUE;

	return eNon;
}

//	PPの処理をする
int CDkRouter1200::ProcPP(char *buf)
{
	const char *szCmd01[] = {
		"auth",		"line",		"select",	"enable",	"always-on", "disable"
	};
	char	*p;
	static BOOL	fAuth;			// 「pp select 〜 pp enable」の間に"pp auth"句が出たか？
	
	p = strtok(NULL, SEARCH_CHR);
	
	//	第２引数検索
	BOOL fFind = FALSE;
	for (int type = 0; type < DK_Number(szCmd01); type++) {
		if (p && strncmp(p, szCmd01[type], lstrlen(szCmd01[type])) == 0) {
			fFind = TRUE;
			break;
		}
	}
	if (fFind == FALSE)
		return eNon;
	
	p = strtok(NULL, SEARCH_CHR);
	type++;
	switch (type)
	{
	case 1:		//	auth
		{
			const char	*myname		= "myname",
						*sis		= "tao",
						*username	= "username",
						*phs		= "openuser";
			fAuth = TRUE;	// 「pp select 〜 pp enable」の間に"pp auth"句が発生した
			
			//	Not myname
// 			if ( p && strncmp(myname, p, lstrlen(myname)) != 0 )
// 				break;
			if ( strncmp(myname, p, lstrlen(myname)) == 0 )				// ----- myname -------------
			{	
				p = strtok(NULL, SEARCH_CHR);
				
				if ( p && strncmp(sis, p, lstrlen(sis)) == 0 ) {				// decide SIS
					m_sRouter.m_nSISNo = m_nCurrentPP + 1;
					m_fSisRoboVwM = TRUE;
					m_sRouter.ncon[m_nCurrentPP].type = eSIS;
					break;
				}
				
				if ( m_nCurrentPP == (m_sRouter.m_nRoboNo - 1) && m_sRouter.m_nRoboNo != 0 ) {	// decide Robo
					if ( p ) //	user＝ホールコード
						strncpy0(m_sRouter.szRoboID, p, sizeof(m_sRouter.szRoboID));
					p = strtok(NULL, SEARCH_CHR);
					if ( p ) //	pass
						strncpy0(m_sRouter.szRoboPass, p, sizeof(m_sRouter.szRoboPass));
					
					m_fSisRoboVwM = TRUE;
					m_sRouter.ncon[m_nCurrentPP].type = eRobo;
					m_sRouter.con[m_nCurrentPP].port  = ePortB1;
					break;
				}
				
				if ( m_nCurrentPP == (m_sRouter.m_nAdslNo - 1) ) {				// decide ADSL
					if ( p ) //	user
						strncpy0(m_sRouter.szAdslID, p, sizeof(m_sRouter.szAdslID));
					p = strtok(NULL, SEARCH_CHR);
					if ( p ) //	pass
						strncpy0(m_sRouter.szAdslPass, p, sizeof(m_sRouter.szAdslPass));
					m_fSisRoboVwM = TRUE;
					m_sRouter.con[m_nCurrentPP].port	= ePortL2;
					break;
				}
				
				if ( m_nCurrentPP == (m_sRouter.sSIS05.no - 1) && m_sRouter.sSIS05.no != 0 )			// decide SIS05
				{
					if ( p ) //	user＝ホールコード
						strncpy0(m_sRouter.sSIS05.szHoleCode, p, sizeof(m_sRouter.sSIS05.szHoleCode));
					p = strtok(NULL, SEARCH_CHR);
					if ( p ) //	pass
						ConvertIpDelmit(p, m_sRouter.sSIS05.passIp);
					
					m_fSisRoboVwM = TRUE;
					if ( m_sRouter.ncon[m_nCurrentPP].type >= 0 ) {
						m_sRouter.ncon[m_nCurrentPP].type = eSIS05;
						m_sRouter.con[m_nCurrentPP].port  = ePortB1;
					}
					break;
				}
				
				if ( p ) //	user
					strncpy0(m_sRouter.szVwID, p, sizeof(m_sRouter.szVwID));
				p = strtok(NULL, SEARCH_CHR);
				if ( p ) //	pass
					strncpy0(m_sRouter.szVwPass, p, sizeof(m_sRouter.szVwPass));
				m_fSisRoboVwM = TRUE;
				m_sRouter.ncon[m_nCurrentPP].type = eVw;
				m_sRouter.m_nVWNo = m_nCurrentPP+1;
				break;
			} else if ( strncmp(username, p, lstrlen(username)) == 0 ) 	// ----- username -------------
			{
				p = strtok(NULL, SEARCH_CHR);
				
				if ( p && strncmp(phs, p, lstrlen(phs)) == 0 ) 			// ユーザ名	// decide PHS
				{
					m_sRouter.con[m_nCurrentPP].no = m_nCurrentPP + 1;
					p = strtok(NULL, SEARCH_CHR);
					
					if ( p && strncmp(phs, p, lstrlen(phs)) == 0 ) {	// パスワード	// decide PHS
						m_fSisRoboVwM = TRUE;
						if ( m_sRouter.ncon[m_nCurrentPP].type == 0 )
						{
							m_sRouter.ncon[m_nCurrentPP].type = ePHSfree;	// 形態が決まってないときはとりあえず接続形態は"PHSフリー"にしておく
						}
						break;
					}
					break;
				}
			}
		}
		break;
	case 2:		//	line  --- (回線種類) --- 
		{
		}
		break;
	case 3:		//	select	(PP処理の開始)
		{
			fAuth			= FALSE; // 「pp select 〜 pp enable」の間に"pp auth"句フラグをFALSEにする
			m_fSisRoboVwM	= FALSE;
			const char *anonymous = "anonymous";
			if ( p && strncmp(p, anonymous, lstrlen(anonymous)) == 0 ) {
				m_nCurrentPP	= SETU_MAX - 1;
				m_fAnonymous	= TRUE;					// Anonymous フラグＯＮ
				if ( !(GetRtCategory(m_sRouter.kind) == eRtType1000_OA) && 
					   m_sRouter.fMent <= eMentAnonymous && m_sRouter.fMent >= 0 )	// OAはメンテフラグ常にオフ
				{
					m_sRouter.fMent = m_sRouter.fMent ? m_sRouter.fMent : eMentAnonymous;	// メンテナンス設定ＯＮ
				}
				m_fSisRoboVwM	= TRUE;
			} else {
				m_nCurrentPP = atoi(p);	// 接続先番号
				m_nCurrentPP--;
				m_nArray++;
			}
			m_sRouter.con[m_nCurrentPP].port = ePortB1;
		}
		break;
	case 4:		// enable (PP処理の終了)
		{
			if ( fAuth && !m_fSisRoboVwM )  // SIS,ROBO,VW,PHSのpp selectではないのにauth句が出た
				return eSyntaxErr;
			
			fAuth			= FALSE; // 「pp select 〜 pp enable」の間に"pp auth"句フラグをFALSEにする
			m_fSisRoboVwM	= FALSE;
		}
		break;
	case 5:		// "pp always-on",
		{
			if ( m_fOA_LAN )	// RTX1000_OAはこれを見ないと判断がつかない
			{
				m_sRouter.ncon[m_nCurrentPP].type = eADSL;	// ADSLがMIRAIGATE閲覧BBと設定してある可能性有
				if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
				{
					m_sRouter.fBbUse = eLaboBB_Adsl;
				}
			}
		}
		break;
//===== ↓ケーブル光対応 ==================================================================
	case 6:		// disable (PP処理の終了 ケーブル光でADSLのppの場合)
		{
			if ( fAuth && !m_fSisRoboVwM )  // SIS,ROBO,VW,PHSのpp selectではないのにauth句が出た
				return eSyntaxErr;
			
			fAuth			= FALSE; // 「pp select 〜 pp enable」の間に"pp auth"句フラグをFALSEにする
			m_fSisRoboVwM	= FALSE;
			
			//	all
			if ( p && strncmp(p, "all", lstrlen("all")) == 0 )
				break;
			
			m_sRouter.port[1].kaisen = eKaisenCableHikari;
			
//===== ↓DHCP対応 ====================================================================
			if ( m_sRouter.fUseLan2DHCP == TRUE )
			{
				
			}
			else
			{
				CopyMemory(m_sRouter.ncon[m_nCurrentPP].fixIp, m_sWkHikari.fixIp, sizeof(m_sWkHikari.fixIp));
				CopyMemory(m_sRouter.ncon[m_nCurrentPP].yobi1, m_sWkHikari.yobi1, sizeof(m_sWkHikari.yobi1));
				CopyMemory(m_sRouter.ncon[m_nCurrentPP].yobi2, m_sWkHikari.yobi2, sizeof(m_sWkHikari.yobi2));
			}
//===== ↑DHCP対応 ====================================================================			
		}
		break;
//===== ↑ケーブル光対応 ==================================================================
	}//	end of switch
	return TRUE;
}

//	IPの処理をする
int CDkRouter1200::ProcIP(char *buf)
{
	const char  *szCmd01[]	= {"lan2",	"lan3", "lan",		"pp",		"route",	"filter",	"keepalive"},
				*secondary  = "secondary",
				*route		= "route",
				*address	= "address",
				*add		= "add",
				*net		= "net",
				*host		= "host",
				*_default	= "default",
				*gateway	= "gateway",
				*remote		= "remote",
				*tcp		= "tcp",
				*mss		= "mss",
				*limit		= "limit";
	char		*p;
	
	static		BOOL	fTunnelFix = FALSE;
	
	p = strtok(NULL, SEARCH_CHR);
	
	//	第２引数検索
	BOOL	fFind = FALSE;
	for (int type = 0; type < DK_Number(szCmd01); type++) {
		if (p && strncmp(p, szCmd01[type], lstrlen(szCmd01[type])) == 0) {
			fFind = TRUE;
			break;
		}
	}
	if (fFind == FALSE)
		return eNon;
	
	p = strtok(NULL, SEARCH_CHR);
	type++;
	switch (type)
	{
	case 1:		// lan2
//===== ↓ケーブル光対応 ==================================================================
		{
			if (p && strncmp(address, p, lstrlen(address)) == 0 )
			{
				if (m_sRouter.kind == eRTA54i && m_sRouter.kind == eRTA55i )
				{
					return eSyntaxErr;	// "lan2 address"が存在した時点でアウト
				}
				else
				{
					//===== ↓DHCP対応 ====================================================================
					p = strtok(NULL, SEARCH_CHR);
					if ( p && strncmp("dhcp", p, lstrlen("dhcp")) == 0 )
					{
						m_sRouter.fUseLan2DHCP = TRUE;
					}
					//===== ↑DHCP対応 ====================================================================
					else
					{
						SetIpC2B(p, m_sWkHikari.fixIp, m_sWkHikari.yobi1);	// 固定IPアドレスとネットマスク 記憶しておく
					}
				}
			}
		}
		break;
//===== ↑ケーブル光対応 ==================================================================
	case 2:		// lan3
		{
			if (p && strncmp(address, p, lstrlen(address)) == 0 ) 	// "lan3 address"が存在する＝個別LAN or ＩＰ電話あり
			{
				p = strtok(NULL, SEARCH_CHR);
				
				// 172.21.0.0があり、ip lan1 secondaryが定義されていない場合はIP電話が選択されているものとする
				if ( (m_sRouter.m_nIpTelNo > 0 || m_sRouter.m_nIpTelNoBk > 0) && m_sRouter.fSetIpTelFlg != eIpTelLAN1 )	// IP電話
				{
					SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);			// LAN3固定ＩＰアドレスの取得
					m_sRouter.port[2].kaisen = eKaisenIP_TEL;				// L3ポートにIP電話をセット
					m_sRouter.fSetIpTelFlg = eIpTelLAN3;					// IP電話はＬＡＮ３とする
				}
				else	// 個別LAN
				{
					if ( m_sRouter.m_nFqdnState != eFqdnLAN3 )	// クラウドIP電話(LAN3)のみが有効の場合に個別LANが設定できない
					{
						SetIpC2B(p, m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask);	// LAN3固定ＩＰアドレスの取得
						m_sRouter.port[2].kaisen = eKaisenLAN;					// L3ポートに個別LANをセット
					}
				}
			}
		}
		break;
	case 3:		//	lan(1)
		if (p && strncmp(address, p, lstrlen(address)) == 0)	// address
		{
			p = strtok(NULL, SEARCH_CHR);
			SetIpC2B(p, m_sRouter.ip, m_sRouter.mask);
		}
		if ( p && strncmp(secondary, p, lstrlen(secondary)) == 0 )	// secondary
		{	
			p = strtok(NULL, SEARCH_CHR);
			if (p && strncmp(address, p, lstrlen(address)) == 0)	// address
			{
				p = strtok(NULL, SEARCH_CHR);
				SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);
				m_sRouter.fSetIpTelFlg = eIpTelLAN1;	// ＩＰ電話がＬＡＮ１で設定されていることが確定
			}
		}
		
		if ( p && strncmp(tcp, p, lstrlen(tcp)) == 0 )	// tcp
		{	
			p = strtok(NULL, SEARCH_CHR);
			if (p && strncmp(mss, p, lstrlen(mss)) == 0)	// mss
			{
				p = strtok(NULL, SEARCH_CHR);
				if (p && strncmp(limit, p, lstrlen(limit)) == 0)	// limit
				{
					p = strtok(NULL, SEARCH_CHR);
					m_sRouter.m_nMSS = atoi(p);
				}
			}
		}
		
		break;
	case 4:		//	pp
		if (p && strncmp(remote, p, lstrlen(remote)) == 0)	// PHS
		{
			BYTE bIP[256];
			
			//	address
			p = strtok(NULL, SEARCH_CHR);
			if (p && strncmp(address, p, lstrlen(address)) != 0)
				break;
			//	IPアドレス
			p = strtok(NULL, SEARCH_CHR);
			ConvertIpDelmit(p, bIP);
			
			m_sRouter.ncon[m_nCurrentPP].type = bIP[3] - 47;			// PHS番号
			
		} else if ( p && strncmp(address, p, lstrlen(address)) == 0 )	// ADSL固定IPの疑い
		{	
			// address(ip △ pp △ address △ 100.100.100.100[⑦ 固定IPアドレス] /32)
			p = strtok(NULL, SEARCH_CHR);	// 固定IPアドレスの取得
			SetIpC2B(p, m_sRouter.ncon[m_nCurrentPP].fixIp);
			m_sRouter.port[1].kaisen = eKaisenFixADSL;
			
		} else if ( p && strncmp("secure", p, lstrlen("secure")) == 0 )
		{
			p = strtok(NULL, SEARCH_CHR);	// filter
			p = strtok(NULL, SEARCH_CHR);	// name
			p = strtok(NULL, SEARCH_CHR);	// "oa_pp_lab" : "dk_pp_lab"
			
			if ( (m_nCurrentPP+1) == m_nAdslProc )	// ADSL
			{
				m_sRouter.con[m_nCurrentPP].port	= ePortL2;
				m_fSisRoboVwM = TRUE;
				
				if ( (lstrcmp(p, "oa_pp_lab") == 0 || lstrcmp(p, "dk_pp_lab") == 0) )	
				{
					m_sRouter.ncon[m_nCurrentPP].type = eADSL;	// ADSLがMIRAIGATE閲覧BBと設定してある可能性有
					if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
					{
						m_sRouter.fBbUse = eLaboBB_Adsl;
					}
				} else if ( lstrcmp(p, "dk_pp_lan2") == 0 ) {	// ADSL固定がMIRAIGATE閲覧BBと設定してある可能性有
					m_sRouter.ncon[m_nCurrentPP].type = eFixADSL;
					if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
					{
						m_sRouter.fBbUse = eLaboBB_FixAdsl;
					}
				}
			} else {	// Labo-INS接続先だと思う
				m_fSisRoboVwM = TRUE;
				m_sRouter.ncon[m_nCurrentPP].type	= eVw;
				m_sRouter.m_nVWNo					= m_nCurrentPP+1;
			}
		} else if ( p && strncmp("nat", p, lstrlen("nat")) == 0 )
		{
			p = strtok(NULL, SEARCH_CHR);	// descriptor
			p = strtok(NULL, SEARCH_CHR);	// 2000 or 3
			
			if ( m_nAdslProc == m_nCurrentPP+1 )
			{
				int nNatDescriptor = atoi(p);
				if ( nNatDescriptor == 2000 )
				{
					m_sRouter.ncon[m_nCurrentPP].type = eFixADSL;
					if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
					{
						m_sRouter.fBbUse = eLaboBB_FixAdsl;
					}
					
				} else if ( nNatDescriptor == 3 )
				{
					m_sRouter.ncon[m_nCurrentPP].type = eADSL;	// ADSLがMIRAIGATE閲覧BBと設定してある可能性有
					if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
					{
						m_sRouter.fBbUse = eLaboBB_Adsl;
					}
				}
			}
		} 
		else if ( p && strncmp("mtu", p, lstrlen("mtu")) == 0 && (m_nAdslProc == m_nCurrentPP+1) )
		{
			p = strtok(NULL, SEARCH_CHR);	// MTU値
			m_sRouter.nMTU = atoi(p);
		}
		break;
	case 5:		//	route
		{
			char	szIpMask[32];
			strncpy0(szIpMask, p, sizeof(szIpMask));			//	IPアドレス等等
			char	*gateway = "gateway",	//	gateway
					*pp		 = "pp";
//===== ↓J-NET顧客代行対応 ==========================================================
			if ( p && lstrcmp(p, "10.0.241.0/24") == 0 ) 
			{
				p = strtok(NULL, SEARCH_CHR);	// gateway
				p = strtok(NULL, SEARCH_CHR);	// tunnel
				p = strtok(NULL, SEARCH_CHR);	// 接続先番号
				m_sRouter.m_nJnetMent = atoi(p);
				break;
			}
//===== ↑J-NET顧客代行対応 ==========================================================
			
			if ( p && strncmp("default", szIpMask, lstrlen("default")) == 0 )  // default
			{
//===== ↓ケーブル光対応 ==================================================================
				p = strtok(NULL, SEARCH_CHR);
				if ( p && strncmp(gateway, p, lstrlen(gateway)) == 0 ) 	// Gateway
				{
					p = strtok(NULL, SEARCH_CHR);
					SetIpC2B(p, m_sWkHikari.yobi2);	// Gateway 記憶しておく
				}
//===== ↑ケーブル光対応 ==================================================================
				break;
			}
			
			p = strtok(NULL, SEARCH_CHR);

			if ( p && strncmp(gateway, p, lstrlen(gateway)) == 0 ) 	// Gateway
			{
				p = strtok(NULL, SEARCH_CHR);
				
				if ( p && isdigit(*p) )	// gatewayの後はIPアドレスであった
				{
//===== ↓ケーブル光対応 ==================================================================
					if ( !m_fBBsetLAN3 )
					{
						m_sRouter.fBbUse = eLaboBB_FixAdsl;
					}
					else
					{
//===== ↑ケーブル光対応 ==================================================================
						if ( lstrcmp(szIpMask, "61.114.182.54"	) == 0	||
							 lstrcmp(szIpMask, "61.114.182.53"	) == 0	||
							 lstrcmp(szIpMask, "61.114.186.0/25") == 0	||
							 lstrcmp(szIpMask, "61.114.177.148"	) == 0	||
							 strncmp(_default, szIpMask, lstrlen(_default)) == 0 )
						{
							m_sRouter.fBbUse = eLaboBB_KobetuLan;	// BB接続は個別LANで決定
							// LAN3のゲートウェイアドレスのセット
							SetIpC2B(p, m_sRouter.sLAN3.ipGateWay);
							m_bL3GateWay = TRUE;
							break;
						}
						
						// LAN3のゲートウェイアドレスのセット(LAN3のゲートウェイアドレスに172.16.XX.XXが入ることはない)
						if ( strncmp(p, "172.16.", 7) != 0 )	// PVⅢネットワークについてのgatewayアドレスは除外
						{
							SetIpC2B(p, m_sRouter.sLAN3.ipGateWay);
							m_bL3GateWay = TRUE;
						}
					}
					break;
				} else if (p && strncmp("tunnel", p, lstrlen("tunnel")) == 0) {	// Tunnel
					
					if ( strcmp(szIpMask, SIS_IP) == 0 )	// SIS接続先
					{
						p = strtok(NULL, SEARCH_CHR);	// 接続先番号
						m_sRouter.m_nSISNo = atoi(p);
						break;
					}
					if ( strcmp(szIpMask, IP_TEL_IP) == 0 )	// IP電話のＩＰアドレス
					{
						p = strtok(NULL, SEARCH_CHR);	// 接続先番号
						
						if ( m_sRouter.m_nIpTelNo != atoi(p) )	// m_nIpTelNoはpre関数で入る
						{
							m_sRouter.m_nIpTelNoBk = atoi(p);	
							DebDisp("IP電話BKのtunnel番号=%d", m_sRouter.m_nIpTelNoBk);
						}
						
						p = strtok(NULL, SEARCH_CHR);
						if ( p && strncmp("keepalive", p, lstrlen("keepalive")) == 0 )	// 続きがある＝両方セットしてある
						{
							p = strtok(NULL, SEARCH_CHR);	// 10
							if ( p == NULL )
								break;							
							p = strtok(NULL, SEARCH_CHR);	// gateway
							if ( p == NULL )
								break;							
							p = strtok(NULL, SEARCH_CHR);	// tunnel
							if ( p == NULL )
								break;
							p = strtok(NULL, SEARCH_CHR);	// tunnel
							if ( p == NULL )
								break;		
							
							m_sRouter.m_nIpTelNoBk = atoi(p);	
						}
						
						break;
					}
					if ( strcmp(szIpMask, IP_CAR_MENT_IP) == 0 )	// 車両管理システムメンテナンス用設定のＩＰ
					{
						p = strtok(NULL, SEARCH_CHR);	// 接続先番号
						m_sRouter.m_nCarMentNo = atoi(p);
						DebDisp("車両管理システムメンテナンスのtunnel番号=%d", m_sRouter.m_nCarMentNo);
						break;
					}
					
					p = strtok(NULL, SEARCH_CHR);	// 接続先番号
					
					m_nCurrentPP = atoi(p) - 1;
					
					if ( strcmp(szIpMask, "10.0.208.0/20") == 0 ) // anonymousでDKVPN関連の接続先が設定してある
					{
						if ( atoi(p) == m_nAdslProc ) { // ADSL固定
							m_sRouter.fMent = eMentFixADSL;	// メンテナンス設定ＯＮ
							break;
						} else {
							m_sRouter.fMent = 0 - atoi(p);	// ADSL固定が選択されていたわけではない
							break;
						}
					}
					if ( strcmp(szIpMask, "10.0.224.0/20") == 0 ) // anonymousでDKVPN関連
					{
						break;
					}
					if ( strcmp(szIpMask, SIS05_IP) == 0 ) {
						// m_sRouter.ncon[nSetuzokuNum].type = eSIS05;
						m_sRouter.sSIS05.no = atoi(p);	// pp auth句の前に必ずくるはず
						m_fSisRoboVwM = TRUE;
						break;
					}
					if ( strcmp(szIpMask, KEIHIN_MOLE) == 0 )	// 景品モールは無視でいい
					{
						return TRUE;
					}
					int	nSetuzokuNum = atoi(p);	// 1〜
					if ( (nSetuzokuNum-1) < 0 )
					{
						if ( strcmp(szIpMask, REOMOTE_MENT_IP_AT) == 0 )
						{
							m_sRouter.fMent = eMentAuto;	// auto確定
						}
						break;
					}
					break;
				} else if (p && strncmp("pp", p, lstrlen("pp")) == 0) {	// pp
					
					if ( strcmp(szIpMask, SIS_IP) == 0 )
					{
						p = strtok(NULL, SEARCH_CHR);	// 接続先番号
						m_sRouter.m_nSISNo = atoi(p);
						break;
					}
					
					if ( strcmp(szIpMask, ROBO_IP) == 0 )
					{
						p = strtok(NULL, SEARCH_CHR);	// 接続先番号
						m_sRouter.m_nRoboNo = atoi(p);
						SetIpC2B(szIpMask, m_sRouter.con[m_sRouter.m_nRoboNo-1].ip);
						break;
					}
					
					if ( strcmp(szIpMask, KEIHIN_MOLE) == 0 )	// 景品モールは無視でいい
					{
						return TRUE;
					}
					
					if ( strcmp(szIpMask, VUP_IP) == 0 )	// VUPIPは無視でいい
					{
						return TRUE;
					}
					
					if ( strcmp(szIpMask, "210.227.65.146") == 0 || strcmp(szIpMask, "122.103.88.162") == 0 )	// J-NET IPは無視でいい
					{
						return TRUE;
					}
					
					int	nSetuzokuNum = atoi(p);	// 1〜
					if ( (nSetuzokuNum-1) < 0 )
					{
						if ( strcmp(szIpMask, REOMOTE_MENT_IP_AT) == 0 )
						{
							m_sRouter.fMent = eMentAuto;	// auto確定
							break;
						}
					}
				}
				
			} else if (p && strncmp(pp, p, lstrlen(pp)) != 0) {
				break;
			} else if (!p) {
				break;
			}
			
			// ip route XXX.XXX.XXX.XXX gateway (pp anonymous or tunnnel X or pp X) metric 2
			p = strtok(NULL, SEARCH_CHR);	// 接続先番号
			
			if ( p == NULL )
				break;
			
			if ( p && strncmp("anonymous", p, lstrlen("anonymous")) == 0 )	// アノニマス定義
			{	// OAはメンテフラグ常にオフ
				if ( !(GetRtCategory(m_sRouter.kind) == eRtType1000_OA) && m_sRouter.fMent == eMentNoUse )
					m_sRouter.fMent = /*m_sRouter.fMent ? m_sRouter.fMent :*/ eMentAnonymous;	// メンテナンス設定ＯＮ
				break;
			}
			
			int	nSetuzokuNum = atoi(p) - 1;	// 1〜
			if ( nSetuzokuNum < 0 ) break;	// anonymousは何もしない
			
			if ( lstrcmp(szIpMask, "61.114.182.54"	) == 0	||
				 lstrcmp(szIpMask, "61.114.182.53"	) == 0	||
				 lstrcmp(szIpMask, "61.114.186.0/25") == 0	||
				 lstrcmp(szIpMask, "61.114.177.148"	) == 0	||
				 strncmp(_default, szIpMask, lstrlen(_default)) == 0 )
			{
				if ( m_nAdslProc == nSetuzokuNum + 1 ) 	// ADSL接続先と思われる接続先番号
				{
					if ( m_sRouter.fBbUse >= 0 || m_sRouter.fBbUse != eLaboBB_KobetuLan )
					{
						m_sRouter.fBbUse = -1;// abs(m_sRouter.fBbUse);	// Labo-BB閲覧の接続先番号にADSLが選択されている?
					}
					
				} else {
					m_sRouter.m_nVWNo = nSetuzokuNum + 1;
					
					if ( m_nStockVwNo == m_nCurrentPP && m_nStockVwNo != 0 ) { // 覚えておいたのPP番号とひとしい
						m_nStockVwNo = 0;
						strncpy0(m_sRouter.szVwID, m_szStockVwID, sizeof(m_sRouter.szVwID));
						strncpy0(m_sRouter.szVwPass, m_szStockVwPass, sizeof(m_sRouter.szVwPass));
						m_fSisRoboVwM = TRUE;
					}
				} 
			} else if ( strcmp(szIpMask, SIS05_IP) == 0 ) {	// 172.31.191.0/24
				// m_sRouter.ncon[nSetuzokuNum].type = eSIS05;
				m_sRouter.sSIS05.no = atoi(p);	// pp auth句の前に必ずくるはず
				m_fSisRoboVwM = TRUE;
				break;
			}
// 			else {
// 				SetIpC2B(szIpMask, m_sRouter.con[nSetuzokuNum].ip, m_sRouter.con[nSetuzokuNum].mask);
// 			}
			
			p = strtok(NULL, SEARCH_CHR);	// metric 2がないやつはいい(VPN or LaboBB=ADSL(固定)でLaboINSがないとかSIS05ポートL2B1とか)
			if ( p == NULL )
				break;
			
			SetIpC2B(szIpMask, m_sRouter.con[nSetuzokuNum].ip, m_sRouter.con[nSetuzokuNum].mask);
			
			break;
		}
	case 6:	// filter
		{
			// フィルタ規定番号(これらの数値のfilter定義はＯＫ) ※ルータ初期化で設定される値も考慮
			int nFilterNo[] = {	1,2,3,4,5,6,7,9,10,15,20,21,22,23,25,26,27,28,29,30,31,32,33,35,36,37,38,
								40,41,42,43,45,46,47,48,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,
								65,66,67,68,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,
								200,
								1400,1401,1405,1406,1410,1430,1431,1432,1433,1434,1435,1436,1437,1438,1440,1441,1446,1447,
								2010,2015,2025,2026,2027,2028,2029,2030,2031,2032,2033,2035,2036,2037,2038,2040,2041,
								2042,2043,2045,2046,2047,2048,2050,2051,2052,2053,2054,2055,2056,2057,2058,2059,2060,
								2061,2062,2063,2065,2066,2067,2068,2070,2071,2072,2073,2074,2075,2076,2077,2078,2079,
								2080,2081,2082,2083,2084,2085,2086,2087,2088,2089,2090,2091,2092,2093,2094,2095,2096,2097,2098,2099,
								2200,2201,2202,2203,2204,2205,2206,2207,2208,2209,2210,2211,2212,2213,2214,2215,2216,
								2217,2218,2219,2220,2221,2222,2223,2224,2225,2226,2227,2228,2229,2230,2231,2232,2233,
								2234,2235,2236,2237,2238,2239,2240,2241,2242,2243,2244,2245,2246,2247,2248,2249,
								2250,2251,2252,2253,2254,2255,2256,2257,2258,2259,2260,2261,2262,2263,2264,2265,2266,
								2267,2268,2269,2270,2271,2272,2273,2274,2275,2276,2277,2278,2279,2280,2281,2282,2283,
								2284,2285,2286,2287,2288,2289,2290,2291,2292,2293,2294,2295,2296,2297,2298,2299,
								2300,2301,2302,2303,2304,2305,2306,2307,2308,2309,2310,2311,2312,2313,2314,2315,2316,
								2317,2318,2319,2320,2321,2322,2323,2324,2325,2326,2327,2328,2329,2330,2331,2332,2333,
								2334,2335,2336,2337,2338,2339,2340,2341,2342,2343,2344,2345,2346,2347,2348,2349,
								2350,2351,2352,2353,2354,2355,2356,2357,2358,2359,2360,2361,2362,2363,2364,2365,2366,
								2367,2368,2369,2370,2371,2372,2373,2374,2375,2376,2377,2378,2379,2380,2381,2382,2383,
								2384,2385,2386,2387,2388,2389,2390,2391,2392,2393,2394,2395,2396,2397,2398,2399,
								2444,2445,2450,2451,3450,3451,
								2600,2601,2602,2603,2604,2605,2606,2607,2608,2609,2610,2611,2612,2613,2614,
								2615,2616,2617,2618,2619,2620,2621,2622,2623,2624,2625,2626,2627,2628,
								2629,2630,
								2631,2632,2633,2634,2635,2636,2637,2638,2639,2640,2641,2642,2643,2644,2645,2646,2647,2648,2649,
								2650,2651,2652,2653,2654,2655,2656,2657,2658,2659,2660,2661,
								2662,2663,2664,2665,2666,2667,2668,2669,2670,2671,2672,2673,2674,2675,
								2676,2677,2678,2679,2680,
								2681,2682,2683,2684,2685,2686,2687,2688,2689,2690,2691,2692,2693,2694,2695,2696,2697,2698,2699,
								2700,2701,2702,2703,2704,2705,2706,2707,2708,
								2709,2710,2711,2712,2713,2714,2715,2716,2717,2718,2719,2720,2721,2722,
								2723,2724,2725,2726,2727,2728,2729,2730,
								2731,2732,2733,2734,2735,2736,2737,2738,2739,2740,2741,2742,2743,2744,2745,2746,2747,2748,2749,
								2750,2751,2752,2753,2754,2755,
								2756,2757,2758,2759,2760,2761,2762,2763,2764,2765,2766,2767,2768,2769,
								2770,2771,2772,2773,2774,2775,2776,2777,2778,2779,2780,
								2781,2782,2783,2784,2785,2786,2787,2788,2789,2790,2791,2792,2793,2794,2795,2796,2797,2798,2799,
								2900,2901,2902,2903,2904,2905,2906,2907,2908,2909,2910,2911,2912,2913,2914,2915,2916,
								2917,2918,2919,2920,2921,2922,2923,2924,2925,2926,2927,2928,2929,2930,
								2931,2932,2933,2934,2935,2936,2937,2938,2939,2940,2941,2942,2943,2944,2945,2946,2947,2948,2949,
								2950,2951,2952,2953,2954,2955,2956,2957,2958,2959,2960,2961,2962,2963,
								2964,2965,2966,2967,2968,2969,2970,2971,2972,2973,2974,2975,2976,2977,2978,2979,2980,
								2981,2982,2983,2984,2985,2986,2987,2988,2989,2990,2991,2992,2993,2994,2995,2996,2997,2998,2999,
								3010,3015,3025,3026,3027,3028,3029,3030,3031,3032,3033,3035,3036,3037,3038,
								3040,3041,3042,3043,3045,3046,3047,3048,3050,3051,3052,3053,3054,3055,
								3056,3057,3058,3059,3060,3061,3062,3063,3065,3066,3067,3068,
								3070,3071,3072,3073,3074,3075,3076,3077,3078,3079,3080,3081,3082,3083,3084,3085,3086,3087,3088,3089,3090,3091,3092,3093,3094,3095,3096,3097,3098,3099,
								3420,3421,3422,3427,3425,3426,3442,3443,3444,3445,3446,3447,3448,3449,3500,3501,3505,3506,3510,3515,3520,3521,3525,3526,3530,
								3535,3540,3545,3550,3555,3560,3561,3565,3566,3570,3575,3580,3581,3585,3586,3590,3595,3600,
								3601,3602,3603,3604,3605,3606,3607,3608,3609,3610,3611,3612,3613,3614,
								3615,3616,3617,3618,3619,3620,3621,3622,3623,3624,3625,3626,3627,3628,
								3629,3630,
								3631,3632,3633,3634,3635,3636,3637,3638,3639,3640,3641,3642,3643,3644,3645,3646,3647,3648,3649,
								3650,3651,3652,3653,3654,3655,3656,3657,3658,3659,3660,3661,
								3662,3663,3664,3665,3666,3667,3668,3669,3670,3671,3672,3673,3674,3675,
								3676,3677,3678,3679,3680,
								3681,3682,3683,3684,3685,3686,3687,3688,3689,3690,3691,3692,3693,3694,3695,3696,3697,3698,3699,	
								3700,3701,3702,3703,3704,3705,3706,3707,3708,
								3709,3710,3711,3712,3713,3714,3715,3716,3717,3718,3719,3720,3721,3722,
								3723,3724,3725,3726,3727,3728,3729,3730,
								3731,3732,3733,3734,3735,3736,3737,3738,3739,3740,3741,3742,3743,3744,3745,3746,3747,3748,3749,
								3750,3751,3752,3753,3754,3755,
								3756,3757,3758,3759,3760,3761,3762,3763,3764,3765,3766,3767,3768,3769,
								3770,3771,3772,3773,3774,3775,3776,3777,3778,3779,3780,
								3781,3782,3783,3784,3785,3786,3787,3788,3789,3790,3791,3792,3793,3794,3795,3796,3797,3798,3799,
								3800,3801,3802,
								3803,3804,3805,3806,3807,3808,3809,3900,3901,3902,3903,3904,3905,3906,
								3907,3908,3909,3910,3911,3912,3913,3914,3915,3916,3917,3918,3919,3920,
								3921,3922,3923,3924,3925,3926,3927,3928,3929,3930,
								3931,3932,3933,3934,3935,3936,3937,3938,3939,3940,3941,3942,3943,3944,3945,3946,3947,3948,3949,
								3950,3951,3952,3953,3954,3955,3956,3957,3958,3959,3960,3961,3962,3963,
								3964,3965,3966,3967,3968,3969,3970,3971,3972,3973,3974,3975,3976,3977,
								3978,3979,3980,
								3981,3982,3983,3984,3985,3986,3987,3988,3989,3990,3991,3992,3993,3994,3995,3996,3997,3998,3999,
								5010,5015,5025,5026,5028,5030,5031,5033,5035,5036,5038,5040,5041,5043,
								5045,5046,5048,5050,5051,5053,5055,5056,5058,5060,5061,5063,5065,5066,5068,
								6400,6405,6410,
								8010,8015,8025,8026,8028,8030,8031,8033,8035,8036,8038,8040,8041,8043,
								8045,8046,8048,8050,8051,8053,8055,8056,8058,8060,8061,8063,8065,8066,8068,
								8420,8421,8425,8426,8500,8505,8510,8515,8520,8525,8530,8535,8540,8545,
								8550,8555,8560,8565,8570,8575,8580,8585,8590,8595,
								10000,10005,10010,10015,10020,10025,10030,10035,
								10100,10110,10120,10125,10130,10135,10200,10201,10205,10206,10210,10211,
								10500,10505,10510,10520,10525,10550,10560,10575,10600,10605,10610,10615,10620,10625,
								10650,10655,10660,10665,10670,10675,
								101402,101404,101406,101407,101408,101409,10800,10801,
//===== ↓クラウドリニューアル対応 ========================================================
								12600,12650,12700,12750,
								20050,20051,20052,20053,20054,20055,20056,20057,20058,20059,
								22050,22051,22052,22053,22054,22055,22056,22057,22058,22059,
								23050,23051,23052,23053,23054,23055,23056,23057,23058,23059,
								20082,20083,20084,20085,20086,20087,20088,20089,20090,20091,
								22082,22083,22084,22085,22086,22087,22088,22089,22090,22091,
								23082,23083,23084,23085,23086,23087,23088,23089,23090,23091,
								22450,22451,22452,22453,22454,22455,22456,22457,22458,22459,
								23450,23451,23452,23453,23454,23455,23456,23457,23458,23459,
								25050,25051,25052,25053,25054,25055,25056,25057,25058,25059,
								28050,28051,28052,28053,28054,28055,28056,28057,28058,28059,
								20001,20002,
//===== ↑クラウドリニューアル対応 ========================================================
//===== ↓J-NET顧客代行対応 ==========================================================
								20010,20015,20020,20025,20030,20035,
//===== ↑J-NET顧客代行対応 ==========================================================		
			};
			
			if ( isdigit(*p) ) 
			{
				// p = strtok(NULL, SEARCH_CHR);	// フィルタ番号取得
				if ( p == NULL )
					return eSyntaxErr;
				
				BYTE bMaskedIP[IP_BYTE];	// マスクをかけた後のIP(ネットワークアドレス)
				CopyMemory(bMaskedIP, m_sRouter.ip, sizeof(bMaskedIP));
				for ( int n = 0; n < IP_BYTE; n++ ) {
					bMaskedIP[n] = m_sRouter.ip[n] & m_sRouter.mask[n];
				}
				
				for ( int i = 0; i < DK_Number(nFilterNo); i++ ) // 規定番号を一つずつ見ていってチェックする
				{	
					int nFilter = atoi(p);
					if ( nFilterNo[i] == nFilter )	// 規定番号にマッチした
					{
//===== ↓J-NET顧客代行対応 ======================================================================
						if ( nFilter == 20010 )
						{
							m_sRouter.ncon[m_sRouter.m_nJnetMent-1].type= eJnetKokyakuMent;
							m_sRouter.con[m_sRouter.m_nJnetMent-1].port	= ePortL2;
						}
//===== ↑J-NET顧客代行対応 ======================================================================
						if ( nFilter >= 3801 && nFilter <= 3804 )	// 信頼できるネットワーク
						{
							p = strtok(NULL, SEARCH_CHR);	// pass-log
							p = strtok(NULL, SEARCH_CHR);	// [IPアドレス]/[ネットマスク]
							
							if ( lstrcmp(p, GetIpB2C(bMaskedIP, m_sRouter.mask)) != 0 ) {
								m_sRouter.sLAN3.sSinraiCon[nFilter-3801].no = 1;
								SetIpC2B(p, m_sRouter.sLAN3.sSinraiCon[nFilter-3801].ip, m_sRouter.sLAN3.sSinraiCon[nFilter-3801].mask);
							}
						}
						if ( nFilter >= 2601 && nFilter <= 2649 )  // DKVPN
						{
							//===== ↓クラウドリニューアル対応 ========================================================
							if ( (nFilter-2600) == m_sRouter.m_nNonConnect )
							{
								m_sRouter.ncon[nFilter-2601].type	= eNoConnect;
								m_sRouter.con[nFilter-2601].port	= ePortL2;
								return TRUE;// 規定番号の定義に当てはまるので次のコマンドへ
							}
							if ( (nFilter-2600) == m_sRouter.m_nNonConnectBk )
							{
								m_sRouter.ncon[nFilter-2601].type	= eNoConnectSub;
								m_sRouter.con[nFilter-2601].port	= ePortL2;
								return TRUE;// 規定番号の定義に当てはまるので次のコマンドへ
							}
							//===== ↑クラウドリニューアル対応 ========================================================
							
							if ( (nFilter-2600) == m_sRouter.m_nJnetNo )
							{
								m_sRouter.ncon[nFilter-2601].type	= eJnetMain;
								m_sRouter.con[nFilter-2601].port	= ePortL2;
								return TRUE;// 規定番号の定義に当てはまるので次のコマンドへ
							}
							if ( (nFilter-2600) == m_sRouter.m_nJnetNo2 )
							{
								m_sRouter.ncon[nFilter-2601].type	= eJnetSub;
								m_sRouter.con[nFilter-2601].port	= ePortL2;
								return TRUE;// 規定番号の定義に当てはまるので次のコマンドへ
							}
							
							p = strtok(NULL, SEARCH_CHR);	// pass pass-log
							p = strtok(NULL, SEARCH_CHR);
							
							if ( m_sRouter.ncon[nFilter-2601].type < 0 )  // DKVPNか個別VPNという認識がある
							{
								if ( m_sRouter.ncon[nFilter-2601].type == -1 )			// 接続形態はDKVPN+Dialupである
								{
									m_sRouter.ncon[nFilter-2601].type	= eDKVPN_DialUp;
									m_sRouter.con[nFilter-2601].port	= ePortL2B1;
								} else if ( m_sRouter.ncon[nFilter-2601].type == -2 ) {	// 接続形態はDKVPNである
									m_sRouter.ncon[nFilter-2601].type	= eDKVPN;
									m_sRouter.con[nFilter-2601].port	= ePortL2;
								} else if ( m_sRouter.ncon[nFilter-2601].type < -10 ) {	// 接続形態はSIS05でもある
 									m_sRouter.con[nFilter-2601].port	= (m_sRouter.ncon[nFilter-2601].type == -11) ? ePortL2B1 : ePortL2;
									m_sRouter.ncon[nFilter-2601].type	= eSIS05;
								}
							}
						}
						if ( nFilter >= 3601 && nFilter <= 3649 )  // 個別VPN
						{
							p = strtok(NULL, SEARCH_CHR);
							p = strtok(NULL, SEARCH_CHR);
							
							if ( m_sRouter.ncon[nFilter-3601].type < 0 )  // DKVPNか個別VPNという認識がある
							{
								if ( m_sRouter.ncon[nFilter-3601].type == -1 )  // 接続形態は個別VPN+Dialupである
								{
									m_sRouter.ncon[nFilter-3601].type	= eKobetuVPN_DialUp;
									m_sRouter.con[nFilter-3601].port	= ePortL3B1;
								} else if ( m_sRouter.ncon[nFilter-3601].type == -2 ) {	// 接続形態は個別VPNである
									m_sRouter.ncon[nFilter-3601].type	= eKobetuVPN;
									m_sRouter.con[nFilter-3601].port	= ePortL3;
								} else if (  m_sRouter.ncon[nFilter-3601].type < -10 ) {// 接続形態はSIS05でもある
									m_sRouter.con[nFilter-3601].port	= (m_sRouter.ncon[nFilter-3601].type == -11) ? ePortL2B1 : ePortL2;
									m_sRouter.ncon[nFilter-3601].type	= eSIS05;
								}
							}
						}
						if ( nFilter == 2450 || nFilter == 22450 )  // Χai-TACT接続有
						{
							m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed = TRUE;
						}
						if ( nFilter == 10505 )  // LUA待受ポート
						{
							p = strtok(NULL, SEARCH_CHR);	// pass
							p = strtok(NULL, SEARCH_CHR);	// ルータのIP
							p = strtok(NULL, SEARCH_CHR);	// *
							p = strtok(NULL, SEARCH_CHR);	// tcp
							p = strtok(NULL, SEARCH_CHR);
							m_sRouter.nLuaPort = atoi(p);
						}
//===== ↓クラウドIP電話対応 ============================================================
						if ( nFilter == 101406 )  // クラウドIP電話サーバーアドレス
						{
							p = strtok(NULL, SEARCH_CHR);	// pass
							p = strtok(NULL, SEARCH_CHR);	// DHCP-DHCP
							p = strtok(NULL, SEARCH_CHR);	// クラウド電話割当IP
							CString strIp = p;
							m_cRtPassEx.ListCommaToArray(&strIp, &m_strRouterGetIpArray);
						}
//===== ↑クラウドIP電話対応 ============================================================
						return TRUE;// 規定番号の定義に当てはまるので次のコマンドへ
					}
				}
				return eSyntaxErr;	// 規定番号にマッチしなかった！
				break;
			}
			else if ( p && strncmp("set", p, lstrlen("set")) == 0 ) 
			{	// LAN3ポート詳細設定でお客様が必要な通信の設定を取得する
				if ( GetRtCategory(m_sRouter.kind) == eRtType1000 )	// RTX1000の場合
				{
					CString strFil[9][5] = {
						{"3500","3520"},	// [① NetBIOS]
						{"3530"},			// [② FTP]
						{"3550"},			// [③ DB]
						{"3560","3580"},	// [④ CSV] 
						{""},				// [⑤
						{""},				// [⑥
						{"3037","3038","3057","3058"},	// [⑦ Labo]
						{"3047","3048","3067","3068"},	// [⑧ CA局]
						{"3590"},			// [⑨ PHS]
					};
					
					p = strtok(NULL, SEARCH_CHR);
					while ( p )
					{
						for ( int i = 0; i < DK_Number(strFil); i++ )
						{
							for ( int n = 0; !strFil[i][n].IsEmpty() && n < 5; n++ )
							{
								if ( strFil[i][n] == p )
								{
									m_sRouter.sLAN3.sConNaiyo[i].fNeed = TRUE;
								}
							}
						}
						p = strtok(NULL, SEARCH_CHR);
					}
				} else {								// RTX1000-OAの場合
					CString strFil[9][5] = {
						{"8500","8520"},	// [① NetBIOS]
						{"8530"},			// [② FTP]
						{"8550"},			// [③ DB]
						{"8560","8580"},	// [④ CSV] 
						{""},				// [⑤
						{""},				// [⑥
						{"8038"},			// [⑦ Labo]
						{"8048"},			// [⑧ CA局]
						{"8590"},			// [⑨ PHS]
					};
					
					p = strtok(NULL, SEARCH_CHR);
					while ( p )
					{
						for ( int i = 0; i < DK_Number(strFil); i++ )
						{
							for ( int n = 0; !strFil[i][n].IsEmpty() && n < 5; n++ )
							{
								if ( strFil[i][n] == p )
								{
									m_sRouter.sLAN3.sConNaiyo[i].fNeed = TRUE;
								}
							}
						}
						p = strtok(NULL, SEARCH_CHR);
					}
				}
			}
			break;
		}
		case 7:// keepalive
		{
			p = strtok(NULL, SEARCH_CHR);	// icmp-echo
			if ( p == NULL )
				break;
			p = strtok(NULL, SEARCH_CHR);	// 60
			if ( p == NULL )
				break;
			p = strtok(NULL, SEARCH_CHR);	// 3
			if ( p == NULL )
				break;
			p = strtok(NULL, SEARCH_CHR);	// サーバアドレス
			if ( p == NULL )
				break;
			
			SetIpC2B(p, m_sRouter.m_ptcSvIp);
			break;
		}
	}	//	end of switch
	return TRUE;
}

//	NATの処理をする
int CDkRouter1200::ProcNAT(char *buf)
{
	const char 	*descriptor = "descriptor",
				*static_	= "static",
				*num1		= "1",
				*type		= "type",
				*address	= "address";
	
	char		*p;
	
	p = strtok(NULL, SEARCH_CHR);
	
	//	descriptor
	if (p && strncmp(p, descriptor, lstrlen(descriptor)) == 0)
	{
		//	static
		p = strtok(NULL, SEARCH_CHR);
		if (p && strncmp(p, static_, lstrlen(static_)) == 0)
		{
			//	1固定
			p = strtok(NULL, SEARCH_CHR);
			
			if (p && (strcmp(p, "3000") == 0 || strcmp(p, "8000") == 0) )
			{
				p = strtok(NULL, SEARCH_CHR);
				if (p)
				{
					if ( atoi(p) == 60 )
					{
						p = strtok(NULL, SEARCH_CHR);	// LAN3のMS-55IPアドレス
						SetIpC2B(p, m_sRouter.sLAN3.ipMS55, NULL);
						m_fMS55IpBrank = FALSE;			// ＭＳ−５５ＩＰアドレスはブランクではない
						p = strtok(NULL, SEARCH_CHR);	// 個数
						m_sRouter.sLAN3.kosuu = atoi(p);
					}
					else if ( atoi(p) == 252 && m_sRouter.kind != eRTX1000 && m_sRouter.kind != eRTX1000_OA )
					{
						p = strtok(NULL, SEARCH_CHR);	// LAN3のウイルス対策IPアドレス
						SetIpC2B(p, m_sRouter.sLAN3.ipVirus, NULL);
						m_sRouter.sLAN3.fVirusUse = TRUE;						
					}
				}
			}
			
			if ( p && strcmp(p, "2012") == 0 )
			{
				p = strtok(NULL, SEARCH_CHR);
				if ( p && strcmp(p, "7") == 0 )
				{
					p = strtok(NULL, SEARCH_CHR);	// LAN3のMS-55IPアドレス
					
					if ( p )	// 123.123.132.139=172.16.15.nn(号機番号)
					{
						CString str = p;
						m_sRouter.m_nMtGokiNo = atoi(str.Mid(str.ReverseFind('.')+1));
					}
				}
				return TRUE;
			}
			
			//	1?
			p = strtok(NULL, SEARCH_CHR);
			if (p && strcmp(p, num1) == 0)
			{
				//	x.x.x.x =
				p = strtok(NULL, " =\n");
				if (p)
				{
					SetIpC2B(p, m_sRouter.uip, NULL);
				}
				if (m_sRouter.uip[0] == 10)
				{
					//	リモート
					if (m_sRouter.uip[1] == 0)
						m_sRouter.uip[3]--;
					else {
						//	C
						m_sRouter.uip[2] = 0;
						m_sRouter.uip[3] = 0;
					}
				}
			}
		}
		// type
		if ( p && strcmp(p, type) == 0 )
		{
			p = strtok(NULL, SEARCH_CHR);
			
			if ( p == NULL )
				return eSyntaxErr;
			
			int nFilterNo[] = {1,2,3,4,12,1000,2000,2012,3000,3013,5000,8000};	// フィルタ規定番号(これらの数値のfilter定義はＯＫ)
			
			if ( isdigit(*p) ) 
			{	
				for ( int i = 0; i < DK_Number(nFilterNo); i++ ) {
					if ( nFilterNo[i] == atoi(p) )	// 規定番号にマッチした
						return TRUE;
				}
				return eSyntaxErr;	// 規定番号にマッチしなかった！
			}
		}
		// address(SIS05のポートがL2かL2B1の時のみ出現)
		if  ( p && strcmp(p, address) == 0 )
		{
			//	outer
			p = strtok(NULL, SEARCH_CHR);
			if (p && strncmp(p, "outer", lstrlen("outer")) == 0)
			{
				p = strtok(NULL, SEARCH_CHR);
				if (p && strncmp(p, "2012", lstrlen("2012")) == 0)
				{
					p = strtok(NULL, SEARCH_CHR);
					SetIpC2B(p, m_sRouter.sSIS05.vpn_passIp);
					m_sRouter.sSIS05.vpn_passIp[3] -= 1;
				}
			}
			// inner
			if (p && strncmp(p, "inner", lstrlen("inner")) == 0)
			{
				p = strtok(NULL, SEARCH_CHR);
				if (p && strncmp(p, "1000", lstrlen("1000")) == 0 && m_sRouter.kind != eRTX1000 && m_sRouter.kind != eRTX1000_OA)
				{
					p = strtok(NULL, SEARCH_CHR);	// LAN3のウイルス対策IPアドレス
					SetIpC2B(p, m_sRouter.sLAN3.ipVirus, NULL);
					m_sRouter.sLAN3.fVirusUse = TRUE;	
				}
			}// at descriptor address inner 1000 %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		}
	}
	
	return TRUE;
}

BOOL CDkRouter1200::ProcTunnel(char *buf)
{
	if ( (m_nCurrentPP+1) >= m_sPrm.nLuaTunnelMin && (m_nCurrentPP+1) <= m_sPrm.nLuaTunnelMax )	// インターネットからのフリーPC接続用設定
		return TRUE;
	
	char *p = strtok(NULL, SEARCH_CHR);
	
	if ( p && strncmp("select", p, lstrlen("select")) == 0 ) 
	{
		m_fSisRoboVwM = FALSE;
		const char *anonymous = "anonymous";
		p = strtok(NULL, SEARCH_CHR);
		
		g_fBackUpCmd = FALSE;
		g_fKeepAlive = FALSE;
		
		m_nCurrentPP = atoi(p);	// 接続先番号
		m_nCurrentPP--;
		
	} else if ( p && strncmp("backup", p, lstrlen("backup")) == 0 ) {
		
		p = strtok(NULL, SEARCH_CHR);
		if ( p && strncmp("pp", p, lstrlen("pp")) ) 	// (DKVPN or 個別VPN)+DialUpだった
		{
			m_nCurrentPP = atoi(p);			// 接続先番号
			m_nCurrentPP--;
		} 
		
		g_fBackUpCmd = TRUE;
	}
	else if ( p && strncmp("endpoint", p, lstrlen("endpoint")) == 0 )
	{
		p = strtok(NULL, SEARCH_CHR);
		if ( p && strncmp("address", p, lstrlen("address")) == 0 )	// PHSVPN
		{
			p = strtok(NULL, SEARCH_CHR);
			SetIpC2B(p, m_sRouter.ncon[m_nCurrentPP].fixIp);	// 固定IPアドレス
			m_sRouter.con[m_nCurrentPP].no		= m_nCurrentPP+1;
			m_sRouter.ncon[m_nCurrentPP].type	= ePHSVPN;
			m_sRouter.con[m_nCurrentPP].port	= ePortL2;
		} 
	}
	else if ( p && strncmp("enable", p, lstrlen("enable")) == 0 )
	{
//===== ↓クラウドリニューアル対応 ========================================================
		if ( m_sRouter.m_nNonConnect == m_nCurrentPP+1 )	// 未定義番号はいい
			return TRUE;
		if ( m_sRouter.m_nNonConnectBk == m_nCurrentPP+1 )	// 未定義番号はいい
			return TRUE;
//===== ↑クラウドリニューアル対応 ========================================================
//===== ↓J-NET顧客代行対応 ==========================================================
		if ( m_sRouter.m_nJnetMent == m_nCurrentPP+1 )	// J-NETメンテはいい
			return TRUE;
//===== ↑J-NET顧客代行対応 ==========================================================
		if ( m_sRouter.m_nJnetNo == m_nCurrentPP+1 )	// J-NETメインはいい
			return TRUE;
		if ( m_sRouter.m_nJnetNo2 == m_nCurrentPP+1 )	// J-NETサブはいい
			return TRUE;
		if ( m_sRouter.m_nIpTelNo == m_nCurrentPP+1 )	// IP電話はいい
			return TRUE;
		if ( m_sRouter.m_nIpTelNoBk == m_nCurrentPP+1 )	// IP電話BKはいい
			return TRUE;
		if ( m_sRouter.m_nCarMentNo == m_nCurrentPP+1 )	// 車両管理メンテ番号はいい
			return TRUE;
		
		// backupがない＆keepalive句がある場合、DELTANETバックアップ先
		// backupがない＆keepalive句もない場合、メンテナンス設定auto
		if ( !g_fBackUpCmd && g_fKeepAlive )
		{
			m_sRouter.ncon[m_nCurrentPP].type = eDeltaNetBk;
			m_sRouter.con[m_nCurrentPP].port = ePortL2;
			
			// ProcIPSECではfixipに自動的に入ってしまうため
			CopyMemory(m_sRouter.con[m_nCurrentPP].ip, m_sRouter.ncon[m_nCurrentPP].fixIp, sizeof(m_sRouter.ncon[m_nCurrentPP].fixIp));
			// ZeroMemory(m_sRouter.ncon[m_nCurrentPP].fixIp, sizeof(m_sRouter.ncon[m_nCurrentPP].fixIp));
			
			m_sRouter.m_nDeltaBkNo = m_nCurrentPP+1;
			m_sRouter.con[m_nCurrentPP].no = m_nCurrentPP+1;
		}
	}
	return TRUE;
}

BOOL CDkRouter1200::ProcDHCP(char *buf)
{
//===== ↓クラウドIP電話対応 ============================================================
	char	*p = strtok(NULL, SEARCH_CHR);
	
	// 	p = strtok(NULL, SEARCH_CHR);
	if (p && strncmp("scope", p, lstrlen("scope")) == 0)
	{
		p = strtok(NULL, SEARCH_CHR);
		if (p && strncmp("1", p, lstrlen("1")) == 0)
		{
			m_sRouter.m_fDHCP = TRUE;
		}
		if (p && strncmp("option", p, lstrlen("option")) == 0)
		{
			p = strtok(NULL, SEARCH_CHR);
			if (p) {
				m_sRouter.m_nFqdnState = (atoi(p) == 1) ? eFqdnLAN1 : eFqdnLAN3;
				if ( m_sRouter.m_nFqdnState == eFqdnLAN3 )
				{
					p = strtok(NULL, SEARCH_CHR);	// dns=XXX.xxx.xxx.xxx
					SetIpC2B(p+4, m_sRouter.ip_2_cloud/*, m_sRouter.mask_2_cloud*/);
				}
			}
		}
	}
//===== ↑クラウドIP電話対応 ============================================================

	return TRUE;	
}

BOOL CDkRouter1200::ProcIPSEC(char *buf)
{
	char	*p = strtok(NULL, SEARCH_CHR);	// ike
	
	p = strtok(NULL, SEARCH_CHR);
	
	if (p && strncmp("pre-shared-key", p, lstrlen("pre-shared-key")) == 0)
	{
		p = strtok(NULL, SEARCH_CHR);	// 接続先番号
		if ( /*atoi(p) == m_nAdslProc &&*/ atoi(p) != m_sRouter.m_nJnetNo )
			return TRUE;
		
		p = strtok(NULL, SEARCH_CHR);	// text
		if ( p == NULL )
			return TRUE;
		
		p = strtok(NULL, SEARCH_CHR);	// 会社番号 + "DN" + ホール番号
		if ( p == NULL )
			return TRUE;
 		
 		CString str = p;
 		lstrcpy(m_sRouter.m_szJnetHoujin, (CStoS)str.Left(JNET_HOUJIN_LEN_MAX));
 		lstrcpy(m_sRouter.m_szJnetHall, (CStoS)str.Mid(JNET_HOUJIN_LEN_MAX + 2, JNET_HALL_LEN_MAX));
	} else
	if (p && strncmp("local", p, lstrlen("local")) == 0)
	{
		p = strtok(NULL, SEARCH_CHR);	// name
		if (p && strncmp("name", p, lstrlen("name")) != 0)
			return TRUE;

		p = strtok(NULL, SEARCH_CHR);	// 接続先番号
		if ( /*atoi(p) == m_nAdslProc &&*/ atoi(p) != m_sRouter.m_nJnetNo )
			return TRUE;
		
		p = strtok(NULL, SEARCH_CHR);	// vpn local name
		if ( p == NULL )
			return TRUE;
		
		lstrcpy(m_sRouter.m_szJnetVpnName, p);
	} else
	if (p && strncmp("keepalive", p, lstrlen("keepalive")) == 0)
	{
		CString dgf =m_strReadLine.Right(1);
		if ( dgf != "2" )	// "ipsec ikr keepalive use [ADSL接続先番号] on neartbeat 10 2"はg_fKeepAlive有のフラグは立てない
			g_fKeepAlive = TRUE;
	} else
	if (p && strncmp("remote", p, lstrlen("remote")) == 0)
	{
		p = strtok(NULL, SEARCH_CHR);
		if (p && strncmp("address", p, lstrlen("address")) == 0 )
		{
			p = strtok(NULL, SEARCH_CHR);	// 接続先番号
			if ( atoi(p) == m_nAdslProc && atoi(p) != m_sRouter.m_nIpTelNo ) {
				return TRUE;	// ADSL接続先の情報は必要ない
			}
			
			m_nCurrentPP = atoi(p) - 1;
			p = strtok(NULL, SEARCH_CHR);
			if ( p == NULL )
				return TRUE;
			
			if ( m_nCurrentPP+1 == m_sRouter.sSIS05.no ) // DELTANET通信用設定
			{
				if ( p == NULL )
					return TRUE;
				
				m_sRouter.ncon[m_nCurrentPP].no = m_nCurrentPP+1;
				SetIpC2B(p, m_sRouter.ncon[m_nCurrentPP].fixIp);
				
				if ( m_sRouter.ncon[m_nCurrentPP].type != -1 && m_sRouter.ncon[m_nCurrentPP].type != -2 ) 
				{
					CopyMemory(m_sRouter.sSIS05.accessIp, m_sRouter.ncon[m_nCurrentPP].fixIp, sizeof(m_sRouter.sSIS05.accessIp));
					ZeroMemory(m_sRouter.ncon[m_nCurrentPP].fixIp, sizeof(m_sRouter.ncon[m_nCurrentPP].fixIp));
				}
			}
			else if ( m_sRouter.m_nIpTelNoBk == m_nCurrentPP+1 )	// IP電話Bk
			{
				SetIpC2B(p, m_sRouter.center_ip);	// BKルータアドレス
				
				m_sRouter.ncon[m_nCurrentPP].type = eIpTelBk;
				m_sRouter.con[m_nCurrentPP].port = ePortL2;
			}
			else if ( m_sRouter.m_nIpTelNo == m_nCurrentPP+1 )	// IP電話
			{
				SetIpC2B(p, m_sRouter.ptc_ip);	// メインルータアドレス
				
				m_sRouter.ncon[m_nCurrentPP].type = eIpTel;
				m_sRouter.con[m_nCurrentPP].port = ePortL2;
			}
			else if ( m_sRouter.m_nCarMentNo == m_nCurrentPP+1 )	// 車両管理メンテ番号
			{
				m_sRouter.ncon[m_nCurrentPP].type	= eCarMent;
				m_sRouter.con[m_nCurrentPP].port	= ePortL2;
			}
			else
			{
				SetIpC2B(p, m_sRouter.ncon[m_nCurrentPP].fixIp);
			}
		} else if (p && strncmp("id", p, lstrlen("id")) == 0 )
		{
			p = strtok(NULL, SEARCH_CHR);	// 接続先番号
			if ( atoi(p) == m_nAdslProc ) {
				return TRUE;	// ADSL接続先の情報は必要ない
			}
			m_nCurrentPP = atoi(p) - 1;
			p = strtok(NULL, SEARCH_CHR);	// IPアドレス
			SetIpC2B(p, m_sRouter.con[m_nCurrentPP].ip, m_sRouter.con[m_nCurrentPP].mask);
		} else if (p && strncmp("name", p, lstrlen("name")) == 0 )	// "name"はリモートVPNが選択されている場合にのみ出現する
		{
			p = strtok(NULL, SEARCH_CHR);	// 接続先番号
			if ( atoi(p) == m_nAdslProc ) {
				m_sRouter.fMent = m_sRouter.fMent ? m_sRouter.fMent : eMentRemoteVpn;
			}
		}
	}
	
	return TRUE;
}

//-----------------------------------------------------------------------------
//	<概要>	TFTPファイルに書き出す(m_sRouterのデータを書き出す)
//	<引数>	int		no		= ルータNO
//          char	*szFile = ファイル名
//			BOOL	fStatic	= 0:保存用ファイル登録　1:送信ファイルの登録
//	<戻値>	int 1:成功		0:登録エラー	-1:静的ルートセットエラー
//-----------------------------------------------------------------------------
int CDkRouter1200::WriteTftpFile(int no, char *szFile/*=NULL*/, BOOL fStatic/*=FALSE*/, char *szPH_CodeA2910/*=""*/, BYTE *pSendSrcIp/*=NULL*/)
{
//===== ↓クラウドリニューアル対応 ===========================================
	CopyMemory(&m_sPutPreRouter, &m_sRouter, sizeof(SDkRouter));
	if ( m_sIniFileInf.nStatus >= SDkKanshiSrvInfo::eLiftMode )
	{
		// A状態でかきこみたいので、現在のMIRAIGATE接続先を過去のものにする
		
		for ( int i = 0; i < SETU_MAX; i++ )
		{
			if ( m_sRouter.ncon[i].no == m_sRouter.sSIS05.no && m_sRouter.ncon[i].type != eNoConnect )
			{
				ConvertIpDelmit(m_sIniFileInf.szPreIp1, m_sRouter.ncon[i].fixIp);
				m_sRouter.ncon[i].no = m_sIniFileInf.nPreTunnelNo;
			}
			if ( m_sRouter.ncon[i].no == m_sRouter.m_nDeltaBkNo && m_sRouter.ncon[i].type != eNoConnectSub )
			{
				ConvertIpDelmit(m_sIniFileInf.szPreIp2, m_sRouter.ncon[i].ip);
				ConvertIpDelmit(m_sIniFileInf.szPreIp2, m_sRouter.ncon[i].fixIp);	// ???????????????????????
				m_sRouter.ncon[i].no = m_sIniFileInf.nPreSubTunnelNo;
			}
		}
		
		m_sRouter.sSIS05.no = m_sIniFileInf.nPreTunnelNo;
		m_sRouter.m_nDeltaBkNo = m_sIniFileInf.nPreSubTunnelNo;
		ConvertIpDelmit(m_sIniFileInf.szPreIp1, m_sRouter.sSIS05.accessIp);	
	}
//===== ↑クラウドリニューアル対応 ===========================================
	
//===== ↓LTE通信対応 ==================================================================
	char szPhCode[8+1];
	lstrcpy(szPhCode, m_sRouter.szPH_CodeA2910);	// L2のPH会員コード
	if ( m_sRouter.port[ePorIdxUSB1].kaisen == eKaisenLTE )
	{
		m_sRouter.port[1].kaisen = eKaisenFixADSL;	// ADSL固定に強制
		
		// VWのセカンダリ、プライマリＩＰをコピーする
		// SDkConnectInfoNew *pVw	 = GetVwInfo(&m_sRouter);
		SDkConnectInfoNew *pADSL = GetAdslInfo();	// ADSLレコード LTE有効なら必ず設定されているはず
// 		if ( pVw != NULL ) {
// 			CopyMemory(pADSL->ip,	pVw->ip,	sizeof(pVw->ip));
// 			CopyMemory(pADSL->mask, pVw->mask,	sizeof(pVw->mask));
// 		}
		
		m_sRouter.nMTU = m_sRouter.sLte.nMTU;
		m_sRouter.nLuaPort = m_sRouter.sLte.nLuaPort;// Χai-TACT認証Pのセット
		
		lstrcpy(m_sRouter.szAdslID,	m_sRouter.sLte.szID);		// ユーザＩＤ
		lstrcpy(m_sRouter.szAdslPass, m_sRouter.sLte.szPass);	// パスワード
		
		pADSL->type = eFixADSL;
		pADSL->port = ePortL2;
		
		CopyMemory(pADSL->fixIp, m_sRouter.sLte.ip, sizeof(BYTE) * IP_BYTE);	// 固定IPアドレス
		lstrcpy(m_sRouter.szPH_CodeA2910, m_sRouter.sLte.szPH_CodeA2910);		// LTEのPH会員コード
		lstrcpy(szPhCode, m_sRouter.sLte.szPH_CodeA2910);						// ini書き込み用LTEのPH会員コード
		
		m_sRouter.fBbUse = eLaboBB_FixAdsl;	// ADSL固定に強制
	}
//===== ↑LTE通信対応 ==================================================================

	
	//===== ↓ A29XX PH会員番号入力対応版はiniにPH会員番号を書いて再読み込み 2019/08/21 ==============
#if A2910_PH_INP_SHOW
	if ( GetReleaseVersion() < A3000 || !IsXai() )
	{
		char szIniFile[MAX_PATH];
		wsprintf(szIniFile, "%s%s\\%s", pMxBas->GetBaseDir(), MX_DIR_CONF, MX_COMMON_INI);
		WritePrivateProfileString("MSINFO", "PHCODE", /*szPH_CodeA2910*/szPhCode, szIniFile);
		m_cRtPassEx.m_nPH_Code = m_cRtPassEx.GetPHcode();	// 再読み込み
	}
#endif
	//======================================================================================
	
	// ※ルータ種別がRTX1000以下の場合は継承元を使う
	if ( m_sRouter.kind <= eRTX1000 || m_sRouter.kind == eRTX1000 || m_sRouter.kind == eRTX1000_OA || m_sRouter.kind == eRTX1100 || m_sRouter.kind == eRTX1100_OA )
	{
		int nRet = CDkRouter1000::WriteTftpFile(no, szFile, fStatic, szPH_CodeA2910, pSendSrcIp);
//===== ↓クラウドリニューアル対応 ========================================================
		// 書き終わった後にクラウド移行の処理をする
		Sleep(1000);
		RouterCloudWrite(&m_sRouter, (szFile != NULL) ? szFile : GetFileName(no, eWrite));
// 		RouterJnetMentWrite(&m_sRouter, (szFile != NULL) ? szFile : GetFileName(no, eWrite));
		CopyMemory(&m_sRouter, &m_sPutPreRouter, sizeof(SDkRouter));	// 変更分をもとにもどす
//===== ↑クラウドリニューアル対応 ========================================================
		return nRet;
	}
	
//===== ↓追加 ============================================================================
	char szGlobalIp[20];	// 画面データが確定しないとグローバルIPが確定しないためここでしか値をセットできない
//	wsprintf(szGlobalIp, "%d.%d.%d.%d", m_sRouter.sLAN3.ip[0], m_sRouter.sLAN3.ip[1], m_sRouter.sLAN3.ip[2], m_sRouter.sLAN3.ip[3]);
	SDkConnectInfoNew *pInfo = GetAdslInfo();
	if (pInfo)
	{
		BYTE *p = pInfo->fixIp;
		sprintf(szGlobalIp, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	} else {
		strcpy(szGlobalIp, "0.0.0.0");
	}
	
	int nLuaPort;	// ※⑤ LUA待受ポートはルータ設定画面でいれる
	CString sMac, localIp;
	
	if ( fStatic == TRUE )
	{
		sMac = "00:A0:DE:65:60:3C";
		DebDisp("設定保存のみの為、ダミーのMACアドレスを使用します");
	}
	else
	{
		// 送信先のIPからMACアドレスを取得する(2021/01/20 DK様要望)
		localIp.Format("%d.%d.%d.%d", pSendSrcIp[0], pSendSrcIp[1], pSendSrcIp[2], pSendSrcIp[3]);
		// localIp.Format("%d.%d.%d.%d", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], m_sRouter.ip[3]);//m_sRouter.ip pSendSrcIp
		if ( pSendSrcIp != NULL )
			DebDisp("送信先=%s 送信内容=%d.%d.%d.%d", localIp, m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], m_sRouter.ip[3]);
		
		DWORD dwRet = CDkRoutePass::getMacAdress(localIp, sMac);
		if ( dwRet != ERROR_SUCCESS )
		{
			CString strMsg;
			strMsg.Format("MACアドレスの取得に失敗しました err=%d", dwRet);
			DebDisp(strMsg);
			return -1;	// 登録失敗
		}
	}

	lstrcpy(m_sRouter.szAuthId, (CStoS)m_strAUTH_ID);													// ① リモートメンテナンス接続ユーザ名（RouterInfo.inf の AUTH_ID）
// 	CString sVPN_UserKey =		  CDkRoutePass::getVpnKey(m_cRtPassEx.m_nPH_Code, szGlobalIp, 0, 0);
	CString sVPN_UserKey =		  CDkRoutePass::createKeyByMac(sMac, m_cRtPassEx.m_nPH_Code);			// ② VPNユーザKEY->名
	lstrcpy(m_sRouter.szVPN_User, CDkRoutePass::getVpnParam(sVPN_UserKey, eRtUser));
	lstrcpy(m_sRouter.szVPN_Pass, CDkRoutePass::getVpnKey(m_cRtPassEx.m_nPH_Code, szGlobalIp, 1, 0));	// ③ VPNパスワード
	CString sVPN_Key =			  CDkRoutePass::getVpnKey(m_cRtPassEx.m_nPH_Code, szGlobalIp, 2, 0);	// ④ VPN PRE-SHARED-KEY
	lstrcpy(m_sRouter.szVPN_Key , CDkRoutePass::getVpnParam(sVPN_Key, eRtPreSk));
	
	// ⑤ LUA待受ポート, ⑥ フリーPC待受ポート, ⑧ RDP待受ポート	
	CDkRoutePass::calcPort(m_cRtPassEx.m_nPH_Code, &nLuaPort, &m_sRouter.nFreePcPort, &m_sRouter.nRdpRecvPort);
	m_sRouter.nRdpOpenPort = 58301;	 	// ⑦ RDP開放ポート	 固定
	
	m_nRegRdpPort = m_sRouter.nRdpRecvPort;	// レジストリ書き込み用
	
	// prmファイルの作成
	char szConfigFile[MAX_PATH];
	wsprintf(szConfigFile, "%s\\%s\\%s\\router\\router%02dW.prm", CDkMxBas::GetBaseDir(), MX_DIR_DATA, MX_DIR_CM, no);
	m_cRtPassEx.WriteRouterPrmFile(szConfigFile, m_cRtPassEx.m_nPH_Code, szGlobalIp, m_sRouter.nLuaPort, sMac);
//===== ↑追加 ============================================================================

	for ( int i = 0; i < DK_Number(m_sRouter.con); i++ )  // 新規対応構造体にデータが入ってるので従来の構造体変数にも反映させる
	{
		memcpy((void*)&m_sRouter.con[i], (void*)&m_sRouter.ncon[i], sizeof(SDkConnectInfo));
	}
	
	BOOL b = FALSE;
	
	if ( szFile == NULL ) {
		if (!fn_CreateDirWithFileName(GetFileName(no, eWrite)))
			return 0;
	} else {
		strncpy0(m_szFileName, szFile, sizeof(m_szFileName));
	}
	
	m_Fp = fopen(m_szFileName, "w");
	if (!m_Fp)
	{
// 		CDkMxBas::WriteSindanReportFileIO(eDkGyoumuCommon, eFileOpen, m_szFileName, 0, 0);
		return FALSE;
	}
	//	タイプの決定
	if (m_sRouter.uip[0] == 10)
	{
		if (m_sRouter.uip[1] == 0) {
			m_nUseType = eUseRemote;	// リモート
			m_nUipMask = 28;			// 上位接続IPのネットマスク
		} else {
			m_nUseType = eUseC;			// Ｃ店舗
			m_nUipMask = 16;			// 上位接続IPのネットマスク
		}
	} else {
		m_nUseType = eUseHonbu;			// 本部
		m_nUipMask = 24;				// 上位接続IPのネットマスク
	}
	
	m_fOA_LAN = FALSE;	// IPアドレスがOA_LANかDK_LANか？
	if ( m_sRouter.ip[0] == 192 && m_sRouter.ip[1] == 168 && (m_sRouter.ip[2] == 60 || m_sRouter.ip[2] == 61) && m_sRouter.ip[3] == 1 )
	{// (OA_LAN以外のルータの設定で、このＩＰで登録したときはエラーになって登録不可となるので事実上ここにくることは無い)
		m_fOA_LAN = TRUE;	// OA_LAN
		if ( m_sRouter.kind == eRTX1000 )
			m_sRouter.kind = eRTX1000_OA;
		else if ( m_sRouter.kind == eRTX1100 )
			m_sRouter.kind = eRTX1100_OA;
	}
	
	m_fSendFile = FALSE;
	char szSendFile[MAX_PATH];
	wsprintf(szSendFile, "%s%s\\%s", pMxBas->GetBaseDir(), ROUTER_TMP_DIR, RT_SEND_FILE);
	if ( lstrcmp(m_szFileName, szSendFile) == 0 ) 
	{
		m_fSendFile = TRUE;
	}
	
	m_nMentType = 0;
	for ( int n = 0; n < SETU_MAX; n++ )
	{
		if ( m_sRouter.con[n].no == abs(m_sRouter.fMent) && m_sRouter.fMent < 0 )	// メンテ接続先について
		{
			m_nMentType = m_sRouter.ncon[n].type;	// メンテ接続先の接続形態について
		}
	}
	
	//	順次書き出し
	for (int loop = 0; loop < 1; loop++)
	{
		//	# System configuration
		if (!PutConfig("System configuration"))
			break;
		
		if ( !PutIpRoute() )	// ip-route Put
			break;
		
		//	# LAN/IP configuration
		if (!PutLanIp("LAN/IP configuration"))
			break;
		
		//	# ISDN configuration
		if (!PutIsdn("ISDN configuration"))
			break;
		
		//	# PP configuration
		if (!CDkRouter::PutPpHead("PP configuration"))
			break;
		
		//	# PP
		if (!PutPp())
			break;
		
		//	### PP anonymous ###
		if ( !PutPpAnonymous("### PP anonymous ###") )
			break;
		
		//  # Tunnel
		if (!PutTunnel())
			break;
		
		//  # ipsec
		if (!PutIpsec())
			break;
		
		//	# IP filter configuration
		if (!PutFilter("IP filter configuration"))
			break;
		
		//	# NAT Descriptor configuration
		if ( !m_fOA_LAN )  // DK_LANの場合のみ
		{
			switch (m_nUseType)
			{
			case eUseC:
				m_nUipMask = 16;	// 上位接続IPのネットマスク
				if (!PutNat_C("NAT Descriptor configuration"))
					continue;
				break;
			case eUseRemote:
				m_nUipMask = 28;	// 上位接続IPのネットマスク
				if (!PutNat_R("NAT Descriptor configuration"))
					continue;
				break;
			case eUseHonbu:
				m_nUipMask = 24;	// 上位接続IPのネットマスク
				if (!PutNat_H("NAT Descriptor configuration"))
					continue;
				break;
			default :
				break;
			}
		}
		//	# PP Head
		if (!PutNatOther("NAT Descriptor configuration other"))
			break;
		
		//	#Other configuration
		if (!PutOther("Other configuration"))
			break;
		
		b = TRUE;
		break;
	}
	if (m_Fp)
		fclose(m_Fp);
	
	if ( !b )
	{
		CString str;
		str.Format("設定書込エラー %s", m_szDebug);
		DebDisp(str);
	}

//===== ↓クラウドリニューアル対応 ========================================================
	// 書き終わった後にクラウド移行の処理をする
	Sleep(1000);
	RouterCloudWrite(&m_sRouter, (szFile != NULL) ? szFile : GetFileName(no, eWrite));
// 	RouterJnetMentWrite(&m_sRouter, (szFile != NULL) ? szFile : GetFileName(no, eWrite));	// CMS0334.iniのJNET_ADD_TUNNELを参照して値があれば無条件で書く->CMS0330では一切書かないことになった
	CopyMemory(&m_sRouter, &m_sPutPreRouter, sizeof(SDkRouter));	// 変更分をもとにもどす
//===== ↑クラウドリニューアル対応 ========================================================
	return b;
}

//	Config 書き出し
BOOL CDkRouter1200::PutConfig(char *tit)
{
	char	szBuf[256];
	
	if (!_Put("#\tDK-ROUTER-CONFIGURATION Ver A1.000"))
		return FALSE;
	
	wsprintf(szBuf, "#\t%s Rev.%s", szRtType[m_sRouter.kind - 1], m_sRouter.szRev);
	if (!_Put(szBuf))
		return FALSE;
	
	if (!_PutTit(tit))
		return FALSE;
	
	if (!_Put("clear configuration"))
		return FALSE;
	
	const char *pCmd01[] = {	// scheduleコマンドはclear configurationで消去不可なのでこのコマンドで消す
		"no schedule at 1",
		"no schedule at 2",
		"no schedule at 3",
		"no schedule at 4",
	};
	if (!_Put(pCmd01, DK_Number(pCmd01)))
		return FALSE;

//===== ↓追加  ==============================================================================
	CString strPRE_SHARED_KEY, strADMIN_PASS, strAUTH_ID, strAUTH_PASS,	strADMIN_PASS_HALL;
	int nIpIdx = m_cRtPassEx.GetIpToRtInfoIdx(CIntStr("%d.%d.%d.%d", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], m_sRouter.ip[3]));
	m_cRtPassEx.GetRouterInfo(nIpIdx, strPRE_SHARED_KEY, strADMIN_PASS,	strAUTH_ID, strAUTH_PASS, strADMIN_PASS_HALL);

	CString strAngo_Admin32, strAngo_Login32;
	BOOL fLoginAngo = FALSE, fAdminAngo = FALSE;
	int nHint = 0;	// PHコードからAdminパスワード生成

	// ========= loginパスワード =========
	if ( strADMIN_PASS.Compare("") == 0 )
	{
		m_strNewLoginPass = "*";
		strAngo_Login32 = "7RxieJVDdQrapYbG2lAB66pjOJQDVoa7";
		fLoginAngo = TRUE;	// encryptして書く
	}
	else
	{
		m_strNewLoginPass = strADMIN_PASS;
	}
	
	// ========= Adminパスワード =========
	if ( m_cRtPassEx.m_nPH_Code > 0 )
	{
		CDkRoutePass::calcRtPass(m_cRtPassEx.m_nPH_Code, nHint, m_strNewAdminPass, strAngo_Admin32);// PHコードからAdminパスワード生成(都度変わる)
		fAdminAngo = TRUE;	// encryptして書く
	}
	else
	{
		if ( strADMIN_PASS_HALL.Compare("*") == 0 )
		{
			strAngo_Admin32 = ".Z0ys2.UfBAKcpCGsTuhP.B.dT1CZT9g";
			fAdminAngo = TRUE;	// encryptして書く
		}
		else
		{
			m_strNewAdminPass = strADMIN_PASS_HALL;
		}
	}
	
	if (noAngoPass)
	{
		CString str;
		str.Format("login password %s", m_strADMIN_PASS);
		if ( !_Put(str) )
			return FALSE;
		
		str.Format("administrator password %s", m_strADMIN_PASS);
		if ( !_Put(str) )
			return FALSE;
	} else {
		// loginパスワード
		if ( fLoginAngo )
		{
			if ( !PutFmt("login password encrypted %s", strAngo_Login32) )
				return FALSE;
		}
		else
		{
			if ( !PutFmt("login password %s", m_strNewLoginPass) )
				return FALSE;
		}
		
		// adminパスワード
		if ( fAdminAngo )
		{
			if ( !PutFmt("administrator password encrypted %s", strAngo_Admin32) )
				return FALSE;
		}
		else
		{
			if ( !PutFmt("administrator password %s", m_strNewAdminPass) )
				return FALSE;
		}
	}
	
	// PHコード、ヒントコード
	if ( m_cRtPassEx.m_nPH_Code )
	{
		if ( !PutFmt("snmp syslocation \"Hall %d.%d\"", m_cRtPassEx.m_nPH_Code, nHint) )
			return FALSE;
	}
	else
	{		
		if ( !_Put("snmp syslocation \"Hall .\"") )
			return FALSE;
	}
//===== ↑  ==============================================================================

	const char *pCmd02[] = {
		"security class 1 on on",
		"console info off",
	};
	if ( !_Put(pCmd02, DK_Number(pCmd02)) )
		return FALSE;
	
	return TRUE;
}
 
//	ルータにテルネット接続しrestart
BOOL CDkRouter1200::TelnetRestart(LPCTSTR distAddress, LPCTSTR adminPass)
{
	CDkMxTelnet	cTel;
	CString		szError = "";
	int			b;
	const int	max = 30;
	for (int loop = 0; loop < max; loop++, DoIdle(30))
	{
		b = cTel.Open(distAddress);
		if (!b) {
			szError = "ルータへのＴＥＬＮＥＴ接続に失敗しました";
			break;
		}
		Sleep(200);
		b = cTel.Login("*", adminPass);
		if (!b)
		{
			if (loop == max -1)
			{
				szError = "ルータへのＴＥＬＮＥＴログインに失敗しました";
				break;
			}
			cTel.Close();
			Sleep(2500);
			continue;
		}
		DoIdle(50);
		b = cTel.Command("restart\r\n");
		if (!b)
		{
			szError = "ルータへのrestart 送信に失敗しました";
		}
		break;
	}

	if (!b)
	{
		DebDisp(szError);
	}
	return (b == 0) ? 0 : 1;
}

//	ISDN情報書き出し
BOOL CDkRouter1200::PutIsdn(char *tit)
{
	char	szBuf[256];
	
	if (!_PutTit(tit))
		return FALSE;
	
	// 回線種類(ブランクでもISDN-NTT)
	wsprintf(szBuf, "line type bri1 isdn-ntt");
	if (!_Put(szBuf))
		return FALSE;
	
	if ( m_sRouter.port[0].kaisen != eKaisenNoEdit && m_sRouter.port[0].kaisen != eKaisenBlank )
	{
		wsprintf(szBuf, "isdn local address bri1 %s", m_sRouter.port[0].Tel);
		if (!_Put(szBuf))
			return FALSE;
	}
	
	if (!_Put("isdn terminator bri1 on"))
		return FALSE;
	
	return TRUE;
}

//----------------------------------------------------------------------------------------------------
//	<概要>	フィルター情報書き出し時の定型書き込み
//	<引数>	int	 nFilterOrg = 始点番号
//          int	 nPass		= 0:"pass"パターン1 1:"pass"パターン2 2:"pass-log"パターン1 3:"pass-log"パターン2 
//          BOOL fRtIpBefor = "pass *" の後にIPアドレスを書き込むのか？"pass"の後にIPアドレスを書き込むのか？
//          BOOL fNetAddress= フィルターパスとしてネットアドレスを書き込む
//	<戻値>	TRUE = 成功    FALSE = 失敗
//=----------------------------------------------------------------------------------------------------
BOOL CDkRouter1200::PutFilterPass(int nFilterOrg, int nPass, BOOL fRtIpBefor, BOOL fNetAddress/*=FALSE*/)
{
	CString strPut;
	char szPut[RT_CMD_LINE_MAX] = {0};
	BOOL fPassLog = FALSE;
	BOOL fReberth = FALSE;			// ルータのIPアドレスが前にくるのか？
	
	char *szLast[] = {"udp * 500", "esp * *", "icmp * *", "tcp * 1723", "tcp 1723 *", "gre * *"};
	
	BYTE bMaskedIP[IP_BYTE];	// マスクをかけた後のIP(ネットワークアドレス)
	CopyMemory(bMaskedIP, m_sRouter.ip, sizeof(bMaskedIP));
	for ( int i = 0; i < IP_BYTE; i++ ) {
		bMaskedIP[i] = m_sRouter.ip[i] & m_sRouter.mask[i];
	}
	
	CString strIKE2 = "*";
	if ( nFilterOrg == 2600 || nFilterOrg == 2650 || nFilterOrg == 2700 || nFilterOrg == 2750 )
	{	// 現状の”不定IP”からのIKE2セッションを許可しているフィルタを以下のように変更し、”不定IP”からの接続を不許可とする。
		// strIKE2 = "180.42.57.138,180.42.57.139,180.42.57.140,180.42.57.141,180.42.57.142,60.32.143.115,202.41.213.169,222.151.236.147,125.206.255.43,202.124.220.210,202.124.220.211,203.129.125.175,180.42.70.99,202.124.220.212,202.124.220.213,202.124.220.214,202.124.220.215,202.124.220.216,202.124.220.217,202.124.220.218,202.124.220.219,202.124.220.220,202.124.220.221,202.124.220.222,203.129.125.172,203.129.125.173,203.129.125.174,203.129.125.171,14.192.44.211,14.192.44.212,14.192.44.213,14.192.44.214,14.192.44.215,14.192.44.216,14.192.44.217,14.192.44.218,14.192.44.219,14.192.44.220,14.192.44.221,14.192.44.222,180.42.70.98,180.42.70.101,114.179.30.27,114.179.105.106,114.179.105.107,118.22.14.107,118.22.14.106,124.110.99.116,124.110.99.117,124.110.99.118,124.110.99.119,124.110.99.120,124.110.99.121,124.110.99.122,124.110.99.123,210.162.143.68,210.162.143.69,210.162.143.70,210.162.143.71,122.20.254.42";
		strIKE2 = "180.42.57.138,180.42.57.139,180.42.57.140,180.42.57.141,180.42.57.142,60.32.143.115,202.41.213.169,222.151.236.147,125.206.255.43,202.124.220.210,202.124.220.211,203.129.125.175,180.42.70.99,202.124.220.212,202.124.220.213,202.124.220.214,202.124.220.215,202.124.220.216,202.124.220.217,202.124.220.218,202.124.220.219,202.124.220.220,202.124.220.221,202.124.220.222,203.129.125.172,203.129.125.173,203.129.125.174,203.129.125.171,14.192.44.211,14.192.44.212,14.192.44.213,14.192.44.214,14.192.44.215,14.192.44.216,14.192.44.217,14.192.44.218,14.192.44.219,14.192.44.220,14.192.44.221,14.192.44.222,180.42.70.98,180.42.70.101,114.179.30.27,114.179.105.106,114.179.105.107,118.22.14.107,118.22.14.106,124.110.99.116,124.110.99.117,124.110.99.118,124.110.99.119,124.110.99.120,124.110.99.121,124.110.99.122,124.110.99.123,210.162.143.68,210.162.143.69,210.162.143.70,210.162.143.71,210.227.65.146,210.227.65.147,210.227.65.148,210.227.65.149,122.103.88.162,122.103.88.163,122.103.88.164,122.103.88.165,122.20.254.42,153.150.112.121,153.150.112.122";
	}
	
	if ( fRtIpBefor )
	{
		strPut.Format(	"ip filter %d pass %s %s %s",
						nFilterOrg,
						strIKE2,
						fNetAddress ? GetIpB2C(bMaskedIP, m_sRouter.mask) : GetIpB2C(m_sRouter.ip),
						szLast[nPass]);
	} else {
		strPut.Format(	"ip filter %d pass %s %s %s",
						nFilterOrg, 
						fNetAddress ? GetIpB2C(bMaskedIP, m_sRouter.mask) : GetIpB2C(m_sRouter.ip),
						strIKE2,
						szLast[nPass]);
	}
	
	if (!_Put((CStoS)strPut))
		return FALSE;
	
	CString strWriteIp1;	// 書き込み用ＩＰアドレス
	CString strWriteIp2;
	for ( int nFilter = nFilterOrg; nFilter <= (nFilterOrg+m_nConnectMax); nFilter++ ) 
	{
		if ( m_sRouter.con[nFilter-nFilterOrg].no == 0 )
			continue;
		
		if ( nFilterOrg >= 2200 && nFilterOrg <= 2399 )
		{
 			if ( m_sRouter.ncon[nFilter-nFilterOrg].type != ePHSVPN )
 				continue;
 		}

		if ( nFilterOrg < 3000 )
		{
			if ( m_sRouter.ncon[nFilter-nFilterOrg].type == eKobetuVPN ||
				 m_sRouter.ncon[nFilter-nFilterOrg].type == eKobetuVPN_DialUp )		// その接続形態は設定があっても書かない
			{
				continue;
			}
		} else if ( nFilterOrg >= 3000 )
		{
			if ( m_sRouter.ncon[nFilter-nFilterOrg].type != eKobetuVPN &&
				 m_sRouter.ncon[nFilter-nFilterOrg].type != eKobetuVPN_DialUp ||
				 m_sRouter.ncon[nFilter-nFilterOrg].type == eFixADSL )		// その接続形態は設定があっても書かない
			{
				continue;
			}
		}

		switch ( m_sRouter.ncon[nFilter-nFilterOrg].type )
		{
		case eDKVPN:	case eDKVPN_DialUp:		// DKVPN or DKVPN+Dialup
			strWriteIp1 = fNetAddress ? GetIpB2C(bMaskedIP, m_sRouter.mask) : GetIpB2C(m_sRouter.ip);
			strWriteIp2 = GetIpB2C(m_sRouter.ncon[nFilter-nFilterOrg].fixIp);
			break;
		case eKobetuVPN:case eKobetuVPN_DialUp:	// 個別VPN or 個別VPN+Dialup
			strWriteIp1 = fNetAddress ? GetIpB2C(bMaskedIP, m_sRouter.mask) : GetIpB2C(m_sRouter.ip);
			strWriteIp2 = GetIpB2C(m_sRouter.ncon[nFilter-nFilterOrg].fixIp);
			break;
		case eSIS05:		// SIS05通信
			{
				for ( int i = 0; i < m_nConnectMax; i++ )
				{
					if ( m_sRouter.sSIS05.no == m_sRouter.ncon[i].no && m_sRouter.ncon[i].type != eNoConnect )
						break;
				}
				if ( i >= m_nConnectMax )	// 見つからない
					return FALSE;
				
				if ( m_sRouter.sSIS05.accessIp[0] == 0 && m_sRouter.sSIS05.accessIp[1] == 0 && 
					 m_sRouter.sSIS05.accessIp[2] == 0 && m_sRouter.sSIS05.accessIp[3] == 0 ||
					i >= m_nConnectMax || m_sRouter.ncon[i].port == ePortB1 )
				{
					continue;
				}
				strWriteIp1 = fNetAddress ? GetIpB2C(bMaskedIP, m_sRouter.mask) : GetIpB2C(m_sRouter.ip);
				strWriteIp2 = GetIpB2C(m_sRouter.sSIS05.accessIp);
			}
			break;
		case eFixADSL:		// ADSL固定IP接続
			{
				if ( !m_fMxRMAddRead )	// RMAddressが不定なので書き込めない
					continue;
				
				if ( fNetAddress == FALSE ) {
					strWriteIp1 = GetIpB2C(m_sRouter.ip);
					strWriteIp2 = m_szRMAdd;	// GetIpB2C(m_bRMAdd);
				} else {
					strWriteIp1 = GetIpB2C(bMaskedIP, m_sRouter.mask);
					strWriteIp2 = m_szRMAdd;	// GetIpB2C(m_bRMAdd);
				}
			}
			break;
		case eDeltaNetBk:
			{
				for ( int i = 0; i < m_nConnectMax; i++ )
				{
					if ( m_sRouter.m_nDeltaBkNo == m_sRouter.ncon[i].no && m_sRouter.ncon[i].type != eNoConnectSub )
						break;
				}
				
				if ( fNetAddress == FALSE ) {
					strWriteIp1 = GetIpB2C(m_sRouter.ip);
					strWriteIp2 = GetIpB2C(m_sRouter.ncon[i].ip);
				} else {
					strWriteIp1 = GetIpB2C(bMaskedIP, m_sRouter.mask);
					strWriteIp2 = GetIpB2C(m_sRouter.ncon[i].ip);
				}
			}
			break;
		case ePHSVPN:
			{
				strWriteIp1 = fNetAddress ? GetIpB2C(bMaskedIP, m_sRouter.mask) : GetIpB2C(m_sRouter.ip);
				strWriteIp2 = GetIpB2C(m_sRouter.ncon[nFilter - nFilterOrg].fixIp);
			}
			break;
		case eJnetMain:	// JNETメイン
			{
				strWriteIp1 = GetIpB2C(m_sRouter.ip);
				strWriteIp2 = IP_JNET_MAIN_IP;
			}
			break;
		case eJnetSub:	// JNETサブ
			{
				strWriteIp1 = GetIpB2C(m_sRouter.ip);
				strWriteIp2 = IP_JNET_SUB_IP;
			}
			break;
		default:			// そのほかの接続形態
			{
				continue;
			}
			break;
		}
		
		if ( !fRtIpBefor ) {
			strPut.Format(	"ip filter %d pass %s %s %s", 
							nFilterOrg+m_sRouter.con[nFilter-nFilterOrg].no, strWriteIp1, strWriteIp2, szLast[nPass]);	
		} else {
			strPut.Format(	"ip filter %d pass %s %s %s", 
							nFilterOrg+m_sRouter.con[nFilter-nFilterOrg].no, strWriteIp2, strWriteIp1, szLast[nPass]);
		}
		
		if (!_Put((CStoS)strPut))
			return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------------------
//	<目的>	フィルター情報書き出し(OA_LAN専用)
//	<戻値>	TRUE = 成功		FALSE = 失敗
//-----------------------------------------------------------------------------------------
BOOL CDkRouter1200::PutFilterOA_LAN(char *tit)
{
	char szPut[RT_CMD_LINE_MAX];
	char szIp[20], szIp2[20];
	if (!_PutTit(tit))
		return FALSE;
	
	int nRouterIP3 = m_sRouter.ip[2];
	
	CString strRepXX;
	strRepXX.Format("%d", nRouterIP3);
	
	BYTE bL3MaskedIP[IP_BYTE];	// お客様の固定IPアドレスにマスクをかけた後のIP
	CopyMemory(bL3MaskedIP, m_sRouter.sLAN3.ip, sizeof(bL3MaskedIP));
	for ( int i = 0; i < IP_BYTE; i++ ) {
		bL3MaskedIP[i] = m_sRouter.sLAN3.ip[i] & m_sRouter.sLAN3.mask[i];
	}
	
	const char *pCmd01[] = {
		"ip filter source-route on",
		"ip filter directed-broadcast on",
		"ip filter 1 reject * * udp,tcp netbios_ns-netbios_ssn *",
		"ip filter 2 reject * * udp,tcp * netbios_ns-netbios_ssn",
		"ip filter 3 reject * * udp,tcp 135 *",
		"ip filter 4 reject * * udp,tcp * 135",
		"ip filter 5 reject * * udp,tcp 445 *",
		"ip filter 6 reject * * udp,tcp * 445",
		"ip filter 9 reject * * * * *",
		"ip filter 100 pass-log * * * * *",
		"ip filter 101 reject * * established * netbios_ns-netbios_ssn",
		"ip filter 102 reject * * established * 135",
		"ip filter 103 reject * * established * 445",
		"ip filter 104 reject * * established * https",
		"ip filter 105 reject * * established * www",
	};
	if (!_Put(pCmd01, DK_Number(pCmd01)))
		return FALSE;
	
	// ⑥OAネットワーク（192.168.60.0/24）側PPインタフェースで適用する静的フィルタ　（5000番台）
	wsprintf(szPut, "ip filter 5010 reject 192.168.%d.0/24 * * * *", nRouterIP3);
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 5015 reject * 192.168.%d.0/24 * * *", nRouterIP3);
	if (!_Put(szPut))	return FALSE;
	
	if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
	{
		if ( m_sRouter.fBbUse == eLaboBB_KobetuLan ) {
			wsprintf(szIp, "%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip));
			wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.mask));
		} else {
			for ( int n = 0; n < SETU_MAX; n++ ) {
				
				if ( m_sRouter.con[n].no == m_sRouter.m_nVWNo && m_sRouter.m_nVWNo != 0 && m_sRouter.fBbUse == 0 ) {	//	VWの時(ただしADSLが設定されてないときのみ)
					wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
					wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
					break;
				}
				if ( m_sRouter.con[n].no == m_sRouter.m_nAdslNo && m_sRouter.m_nAdslNo != 0 && m_sRouter.fBbUse > 0 ) {
					wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
					wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
					break;	
				}
			}
		}
		
		wsprintf(szPut,"ip filter 5025 pass 192.168.%d.0/24 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 5026 pass 192.168.%d.1 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 5028 pass 192.168.%d.2-192.168.%d.254 %s,%s udp * domain", nRouterIP3, nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
	}
	
	CString str5030[] = {
		"ip filter 5030 pass 61.114.182.53 192.168.XX.0/24 tcp https *",
		"ip filter 5031 pass 61.114.182.53 192.168.XX.1 tcp https *",
		"ip filter 5033 pass 61.114.182.53 192.168.XX.2-192.168.XX.254 tcp https *",
		"ip filter 5035 pass 192.168.XX.0/24 61.114.182.53 tcp * https",
		"ip filter 5036 pass 192.168.XX.1 61.114.182.53 tcp * https",
		"ip filter 5038 pass 192.168.XX.2-192.168.XX.254 61.114.182.53 tcp * https",
		"ip filter 5040 pass 61.114.182.54 192.168.XX.0/24 tcp www *",
		"ip filter 5041 pass 61.114.182.54 192.168.XX.1 tcp www *",
		"ip filter 5043 pass 61.114.182.54 192.168.XX.2-192.168.XX.254 tcp www *",
		"ip filter 5045 pass 192.168.XX.0/24 61.114.182.54 tcp * www",
		"ip filter 5046 pass 192.168.XX.1 61.114.182.54 tcp * www",
		"ip filter 5048 pass 192.168.XX.2-192.168.XX.254 61.114.182.54 tcp * www",
		"ip filter 5050 pass 61.114.186.0/25 192.168.XX.0/24 tcp www,https *",
		"ip filter 5051 pass 61.114.186.0/25 192.168.XX.1 tcp www,https *",
		"ip filter 5053 pass 61.114.186.0/25 192.168.XX.2-192.168.XX.254 tcp www,https *",
		"ip filter 5055 pass 192.168.XX.0/24 61.114.186.0/25 tcp * www,https",
		"ip filter 5056 pass 192.168.XX.1 61.114.186.0/25 tcp * www,https",
		"ip filter 5058 pass 192.168.XX.2-192.168.XX.254 61.114.186.0/25 tcp * www,https",
		"ip filter 5060 pass 61.114.177.148 192.168.XX.0/24 tcp https *",
		"ip filter 5061 pass 61.114.177.148 192.168.XX.1 tcp https *",
		"ip filter 5063 pass 61.114.177.148 192.168.XX.2-192.168.XX.254 tcp https *",
		"ip filter 5065 pass 192.168.XX.0/24 61.114.177.148 tcp * https",
		"ip filter 5066 pass 192.168.XX.1 61.114.177.148 tcp * https",
		"ip filter 5068 pass 192.168.XX.2-192.168.XX.254 61.114.177.148 tcp * https"
	};
	
	for ( i = 0; i < DK_Number(str5030); i++ )
	{
		str5030[i].Replace("XX", strRepXX);
		if (!_Put((CStoS)str5030[i])) return FALSE;
	}
	
	wsprintf(szPut,"ip filter 6400 pass 192.168.%d.0/24 * * * *", nRouterIP3);
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut,"ip filter 6405 pass * 192.168.%d.0/24 * * *", nRouterIP3);
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut,"ip filter 6410 pass * * udp dhcpc dhcps");
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut,"ip filter 8010 reject 192.168.%d.0/24 * * * *", nRouterIP3);
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut,"ip filter 8015 reject * 192.168.%d.0/24 * * *", nRouterIP3);
	if (!_Put(szPut))	return FALSE;
	
	if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
	{
		wsprintf(szPut,"ip filter 8025 pass 192.168.%d.0/24 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 8026 pass 192.168.%d.1 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 8028 pass 192.168.%d.2-192.168.%d.254 %s,%s udp * domain", nRouterIP3, nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
	}
	
	CString str8030[] = {
		"ip filter 8030 pass 61.114.182.53 192.168.XX.0/24 tcp https *",
		"ip filter 8031 pass 61.114.182.53 192.168.XX.1 tcp https *",
		"ip filter 8033 pass 61.114.182.53 192.168.XX.2-192.168.XX.254 tcp https *",
		"ip filter 8035 pass 192.168.XX.0/24 61.114.182.53 tcp * https",
		"ip filter 8036 pass 192.168.XX.1 61.114.182.53 tcp * https",
		"ip filter 8038 pass 192.168.XX.2-192.168.XX.254 61.114.182.53 tcp * https",
		"ip filter 8040 pass 61.114.182.54 192.168.XX.0/24 tcp www *",
		"ip filter 8041 pass 61.114.182.54 192.168.XX.1 tcp www *",
		"ip filter 8043 pass 61.114.182.54 192.168.XX.2-192.168.XX.254 tcp www *",
		"ip filter 8045 pass 192.168.XX.0/24 61.114.182.54 tcp * www",
		"ip filter 8046 pass 192.168.XX.1 61.114.182.54 tcp * www",
		"ip filter 8048 pass 192.168.XX.2-192.168.XX.254 61.114.182.54 tcp * www",
		"ip filter 8050 pass 61.114.186.0/25 192.168.XX.0/24 tcp www,https *",
		"ip filter 8051 pass 61.114.186.0/25 192.168.XX.1 tcp www,https *",
		"ip filter 8053 pass 61.114.186.0/25 192.168.XX.2-192.168.XX.254 tcp www,https *",
		"ip filter 8055 pass 192.168.XX.0/24 61.114.186.0/25 tcp * www,https",
		"ip filter 8056 pass 192.168.XX.1 61.114.186.0/25 tcp * www,https",
		"ip filter 8058 pass 192.168.XX.2-192.168.XX.254 61.114.186.0/25 tcp * www,https",
		"ip filter 8060 pass 61.114.177.148 192.168.XX.0/24 tcp https *",
		"ip filter 8061 pass 61.114.177.148 192.168.XX.1 tcp https *",
		"ip filter 8063 pass 61.114.177.148 192.168.XX.2-192.168.XX.254 tcp https *",
		"ip filter 8065 pass 192.168.XX.0/24 61.114.177.148 tcp * https",
		"ip filter 8066 pass 192.168.XX.1 61.114.177.148 tcp * https",
		"ip filter 8068 pass 192.168.XX.2-192.168.XX.254 61.114.177.148 tcp * https"
	};
	for ( i = 0; i < DK_Number(str8030); i++ )
	{
		str8030[i].Replace("XX", strRepXX);
		if (!_Put((CStoS)str8030[i])) return FALSE;
	}
	
 	wsprintf(szPut,"ip filter 8420 pass-log %s %s icmp * *", 
 					GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), GetIpB2C_Second(bL3MaskedIP));
 	if (!_Put(szPut))	return FALSE;
 	
 	wsprintf(szPut,"ip filter 8425 pass-log %s %s icmp * *",
 					GetIpB2C(bL3MaskedIP), GetIpB2C_Second(bL3MaskedIP, m_sRouter.sLAN3.mask));
 	if (!_Put(szPut))	return FALSE;
	
	CString str8420[] = {
		"ip filter 8420 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.1 icmp * *",
		"ip filter 8421 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 icmp * *",
		"ip filter 8425 pass-log 192.168.XX.1 ccc.ccc.ccc.ccc/cc icmp * *",
		"ip filter 8426 pass-log 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc icmp * *",
		"ip filter 8500 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp,udp * netbios_ns-netbios_ssn",
		"ip filter 8505 pass-log 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp,udp netbios_ns-netbios_ssn *",
		"ip filter 8510 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp,udp * 135",
		"ip filter 8515 pass-log 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp,udp 135 *",
		"ip filter 8520 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp,udp * 445",
		"ip filter 8525 pass-log 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp,udp 445 *",
		"ip filter 8530 pass ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp * 21",
		"ip filter 8535 pass 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp 21 *",
		"ip filter 8540 pass ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp * ftpdata",
		"ip filter 8545 pass 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp ftpdata *",
		"ip filter 8550 pass ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp 58010-58019 2638",
		"ip filter 8555 pass 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp 2638 58010-58019",
		"ip filter 8560 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp,udp netbios_ns-netbios_ssn *",
		"ip filter 8565 pass-log 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp,udp * netbios_ns-netbios_ssn",
		"ip filter 8570 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp,udp 135 *",
		"ip filter 8575 pass-log 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp,udp * 135",
		"ip filter 8580 pass-log ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp,udp 445 *",
		"ip filter 8585 pass-log 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp,udp * 445",
		"ip filter 8590 pass ccc.ccc.ccc.ccc/cc 192.168.XX.2-192.168.XX.254 tcp * 58006",
		"ip filter 8595 pass 192.168.XX.2-192.168.XX.254 ccc.ccc.ccc.ccc/cc tcp 58006 *",
	};
	for ( i = 0; i < DK_Number(str8420); i++ )
	{
		str8420[i].Replace("XX", strRepXX);
		str8420[i].Replace("ccc.ccc.ccc.ccc/cc", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put((CStoS)str8420[i])) return FALSE;
	}
	
	if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
	{
		// ④OAネットワーク（192.168.60.0/24）側PPインタフェースで適用する動的フィルタ　（5000番台）
		wsprintf(szPut,"ip filter dynamic 5025 192.168.%d.0/24 %s,%s domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 5026 192.168.%d.1 %s,%s domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 5028 192.168.%d.2-192.168.%d.254 %s,%s domain", nRouterIP3, nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
	}
	
	CString str5035[] = {	
		"ip filter dynamic 5035 192.168.XX.0/24 61.114.182.53 filter 5035 in 5030",
		"ip filter dynamic 5036 192.168.XX.1 61.114.182.53 filter 5036 in 5031",
		"ip filter dynamic 5038 192.168.XX.2-192.168.XX.254 61.114.182.53 filter 5038 in 5033",
		"ip filter dynamic 5045 192.168.XX.0/24 61.114.182.54 filter 5045 in 5040",
		"ip filter dynamic 5046 192.168.XX.1 61.114.182.54 filter 5046 in 5041",
		"ip filter dynamic 5048 192.168.XX.2-192.168.XX.254 61.114.182.54 filter 5048 in 5043",
	};
	for ( i = 0; i < DK_Number(str5035); i++ )
	{
		str5035[i].Replace("XX", strRepXX);
		if (!_Put((CStoS)str5035[i])) return FALSE;
	}
	
	CString str5055[] = {
		"ip filter dynamic 5055 192.168.XX.0/24 61.114.186.0/25 filter 5055 in 5050",
		"ip filter dynamic 5056 192.168.XX.1 61.114.186.0/25 filter 5056 in 5051",
		"ip filter dynamic 5058 192.168.XX.2-192.168.XX.254 61.114.186.0/25 filter 5058 in 5053",
		"ip filter dynamic 5065 192.168.XX.0/24 61.114.177.148 filter 5065 in 5060",
		"ip filter dynamic 5066 192.168.XX.1 61.114.177.148 filter 5066 in 5061",
		"ip filter dynamic 5068 192.168.XX.2-192.168.XX.254 61.114.177.148 filter 5068 in 5063",
	};
	for ( i = 0; i < DK_Number(str5055); i++ )
	{
		str5055[i].Replace("XX", strRepXX);
		if (!_Put((CStoS)str5055[i])) return FALSE;
	}
	
	if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
	{
		// ⑤OAネットワーク（192.168.60.0/24）側Lan3インタフェースで適用する動的フィルタ　（8000番台）
		wsprintf(szPut,"ip filter dynamic 8025 192.168.%d.0/24 %s,%s domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 8026 192.168.%d.1 %s,%s domain", nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 8028 192.168.%d.2-192.168.%d.254 %s,%s domain", nRouterIP3, nRouterIP3, szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
	}
	
	CString str8035[] = {	
		"ip filter dynamic 8035 192.168.XX.0/24 61.114.182.53 filter 8035 in 8030",
		"ip filter dynamic 8036 192.168.XX.1 61.114.182.53 filter 8036 in 8031",
		"ip filter dynamic 8038 192.168.XX.2-192.168.XX.254 61.114.182.53 filter 8038 in 8033",
		"ip filter dynamic 8045 192.168.XX.0/24 61.114.182.54 filter 8045 in 8040",
		"ip filter dynamic 8046 192.168.XX.1 61.114.182.54 filter 8046 in 8041",
		"ip filter dynamic 8048 192.168.XX.2-192.168.XX.254 61.114.182.54 filter 8048 in 8043",
		"ip filter dynamic 8055 192.168.XX.0/24 61.114.186.0/25 filter 8055 in 8050",
		"ip filter dynamic 8056 192.168.XX.1 61.114.186.0/25 filter 8056 in 8051",
		"ip filter dynamic 8058 192.168.XX.2-192.168.XX.254 61.114.186.0/25 filter 8058 in 8053",
		"ip filter dynamic 8065 192.168.XX.0/24 61.114.177.148 filter 8065 in 8060",
		"ip filter dynamic 8066 192.168.XX.1 61.114.177.148 filter 8066 in 8061",
		"ip filter dynamic 8068 192.168.XX.2-192.168.XX.254 61.114.177.148 filter 8068 in 8063",
	};
	for ( i = 0; i < DK_Number(str8035); i++ )
	{
		str8035[i].Replace("XX", strRepXX);
		if (!_Put((CStoS)str8035[i])) return FALSE;
	}
	
	if ( m_sRouter.port[2].kaisen == eKaisenLAN ) 
	{
		wsprintf(szPut,"ip filter dynamic 8530 %s 192.168.%d.2-192.168.%d.254 ftp", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), nRouterIP3, nRouterIP3);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 8550 %s 192.168.%d.2-192.168.%d.254 filter 8550 in 8555", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask),nRouterIP3, nRouterIP3);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 8590 %s 192.168.%d.2-192.168.%d.254 filter 8590 in 8595", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask),nRouterIP3, nRouterIP3);
		if (!_Put(szPut))	return FALSE;
	}
	
	const char *pCmd02[] = {
		"ip filter set oa_lan1 in 6400 6410",
		"ip filter set oa_lan1 out 6405"
	};
	if (!_Put(pCmd02, DK_Number(pCmd02)))
		return FALSE;
	
	if (!_Put("ip filter set oa_lan3 in 3 4 8010 8420 8421", FALSE))
		return FALSE;
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed	|| 
		 m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed		|| m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed		|| m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed )
	{
		wsprintf(szPut, "%s%s%s%s%s%s%s",	
						m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	? " 8500" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	? " 8520" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed		? " 8530" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed		? " 8550" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed			? " 8560" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed			? " 8580" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed		? " 8590" : "");
		if (!_Put(szPut, FALSE))	return FALSE;	
	}	     
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed )
	{
		wsprintf(szPut," dynamic%s%s%s", 
						m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed	? " 8530" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed	? " 8550" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed	? " 8590" : "");
		if (!_Put(szPut, FALSE))	return FALSE;
	}
	if (!_Put(""))
		return FALSE;
	
	wsprintf(szPut,"ip filter set oa_lan3 out 3 4 104 105 8015%s%s%s%s 8425 8426%s%s%s%s",
					(m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed	 && DK_BitChk(m_dwSISAddressFlag, 0)) ? " 8038" : "",
					(m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed && DK_BitChk(m_dwSISAddressFlag, 1)) ? " 8048" : "",
					(m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed	 && DK_BitChk(m_dwSISAddressFlag, 2)) ? " 8058" : "",
					(m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed && DK_BitChk(m_dwSISAddressFlag, 3)) ? " 8068" : "",
					 m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	 ? " 8505" : "",
					 m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	 ? " 8525" : "",
					 m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed		 ? " 8565" : "",
					 m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed ? " 8585" : "");
	if (!_Put(szPut, FALSE))	return FALSE;
	
	if ( (m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed) && m_dwSISAddressFlag != 0 )
	{
		wsprintf(szPut," dynamic%s%s%s%s", 
						(m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed	 && DK_BitChk(m_dwSISAddressFlag, 0)) ? " 8038" : "",
						(m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed && DK_BitChk(m_dwSISAddressFlag, 1)) ? " 8048" : "",
						(m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed	 && DK_BitChk(m_dwSISAddressFlag, 2)) ? " 8058" : "",
						(m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed && DK_BitChk(m_dwSISAddressFlag, 3)) ? " 8068" : "");
		if (!_Put(szPut, FALSE))	return FALSE;
	}
	if (!_Put(""))
		return FALSE;
	
	if (!_Put("ip filter set oa_pp_lab in 9"))
		return FALSE;

	wsprintf(szPut, "ip filter set oa_pp_lab out 1 2 3 4 5 6 104 105 5015%s%s%s%s",
					DK_BitChk(m_dwSISAddressFlag, 0) ? " 5038" : "",
					DK_BitChk(m_dwSISAddressFlag, 1) ? " 5048" : "",
					DK_BitChk(m_dwSISAddressFlag, 2) ? " 5058" : "",
					DK_BitChk(m_dwSISAddressFlag, 3) ? " 5068" : "");
	if (!_Put(szPut, FALSE))	return FALSE;

	if ( m_dwSISAddressFlag != 0 )
	{
		wsprintf(szPut," dynamic%s%s%s%s", 
						DK_BitChk(m_dwSISAddressFlag, 0) ? " 5038" : "",
						DK_BitChk(m_dwSISAddressFlag, 1) ? " 5048" : "",
						DK_BitChk(m_dwSISAddressFlag, 2) ? " 5058" : "",
						DK_BitChk(m_dwSISAddressFlag, 3) ? " 5068" : "");
		if (!_Put(szPut, FALSE))	return FALSE;
	}
	
	if (!_Put(""))
		return FALSE;
	
	const char *pCmd03[] = {
		"ip filter set getlog in 1 2 3 4 5 6 100",
		"ip filter set getlog out 1 2 3 4 5 6 100",
	};
	if (!_Put(pCmd03, DK_Number(pCmd03)))
		return FALSE;
	
	return TRUE;
}

//-----------------------------------------------------------------------------------------
//	<目的>	フィルター情報書き出し(DK_LAN専用)
//	<戻値>	TRUE = 成功		FALSE = 失敗
//-----------------------------------------------------------------------------------------
BOOL CDkRouter1200::PutFilter(char *tit)
{
	// 現在のルータの接続先最大設定数
	m_nConnectMax = CDkRouter::GetConnectMax(m_sRouter.kind);
	
	if ( m_fOA_LAN )  // OA_LANはこちら
	{
		return PutFilterOA_LAN(tit);
	} 
	
	char szPut[RT_CMD_LINE_MAX];
	char szIp[20], szIp2[20];
	if (!_PutTit(tit))
		return FALSE;
	
	BYTE bMaskedIP[IP_BYTE];	// マスクをかけた後のIP(ネットワークアドレス)
	CopyMemory(bMaskedIP, m_sRouter.ip, sizeof(bMaskedIP));
	for ( int i = 0; i < IP_BYTE; i++ ) {
		bMaskedIP[i] = m_sRouter.ip[i] & m_sRouter.mask[i];
	}
	
	BYTE bL3MaskedIP[IP_BYTE];	// LAN3ポート詳細設定でお客様の固定IPアドレスにマスクをかけた後のIP
	CopyMemory(bL3MaskedIP, m_sRouter.sLAN3.ip, sizeof(bL3MaskedIP));
	for ( i = 0; i < IP_BYTE; i++ ) {
		bL3MaskedIP[i] = m_sRouter.sLAN3.ip[i] & m_sRouter.sLAN3.mask[i];
	}
	
	// ①基本フィルタ
	const char *pCmd01[] = {
		"ip filter source-route on",
		"ip filter directed-broadcast on",
		"ip filter 1 reject * * udp,tcp netbios_ns-netbios_ssn *",
		"ip filter 2 reject * * udp,tcp * netbios_ns-netbios_ssn",
		"ip filter 3 reject * * udp,tcp 135 *",
		"ip filter 4 reject * * udp,tcp * 135",
		"ip filter 5 reject * * udp,tcp 445 *",
		"ip filter 6 reject * * udp,tcp * 445",
		"ip filter 9 reject * * * * *",
	};
	if (!_Put(pCmd01, DK_Number(pCmd01)))
		return FALSE;
	
	// ②DKネットワーク（172.16.0.0/16）側PPインタフェースで適用する静的フィルタ　（0番台）
	wsprintf(szPut, "ip filter 10 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 15 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	
	if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
	{
		if ( m_sRouter.fBbUse == eLaboBB_KobetuLan ) {
			wsprintf(szIp, "%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip));
			wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.mask));
		} else {
			for ( int n = 0; n < SETU_MAX; n++ ) {
				
				if ( m_sRouter.con[n].no == m_sRouter.m_nVWNo && m_sRouter.m_nVWNo != 0 && m_sRouter.fBbUse == eLaboBB_NoUse ) {	//	VWの時(ただしADSLが設定されてないときのみ)
					wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
					wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
					break;
				}
				if ( m_sRouter.con[n].no == m_sRouter.m_nAdslNo && m_sRouter.m_nAdslNo != 0 && m_sRouter.fBbUse > 0 ) {
					wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
					wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
					break;	
				}
			}
		}

		char szPut[256];
		wsprintf(szPut, "ip filter 20 pass %s,%s 172.16.0.0/16 udp domain *", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 21 pass %s,%s 172.16.5.0/24 udp domain *", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 22 pass %s,%s 172.16.15.0/24 udp domain *", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 23 pass %s,%s 172.16.60.0/24 udp domain *", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 25 pass 172.16.0.0/16 %s,%s udp * domain", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 26 pass 172.16.5.0/24 %s,%s udp * domain", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 27 pass 172.16.15.0/24 %s,%s udp * domain", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 28 pass 172.16.60.0/24 %s,%s udp * domain", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		
		if ( m_dwDnsFlag )	// レジストリ設定ダイアログにて①「DNSサーバ接続」が○だった場合のみ、以下の設定を追加する。
		{
			wsprintf(szPut, "ip filter 29 pass * %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
		}
	}
	
	// ①基本フィルタ
	const char *pCmd02[] = {
		"ip filter 30 pass 61.114.182.53 172.16.0.0/16 tcp https *",
		"ip filter 31 pass 61.114.182.53 172.16.5.0/24 tcp https *",
		"ip filter 32 pass 61.114.182.53 172.16.15.0/24 tcp https *",
		"ip filter 33 pass 61.114.182.53 172.16.60.0/24 tcp https *",
		"ip filter 35 pass 172.16.0.0/16 61.114.182.53 tcp * https",
		"ip filter 36 pass 172.16.5.0/24 61.114.182.53 tcp * https",
		"ip filter 37 pass 172.16.15.0/24 61.114.182.53 tcp * https",
		"ip filter 38 pass 172.16.60.0/24 61.114.182.53 tcp * https",
		"ip filter 40 pass 61.114.182.54 172.16.0.0/16 tcp www *",
		"ip filter 41 pass 61.114.182.54 172.16.5.0/24 tcp www *",
		"ip filter 42 pass 61.114.182.54 172.16.15.0/24 tcp www *",
		"ip filter 43 pass 61.114.182.54 172.16.60.0/24 tcp www *",
		"ip filter 45 pass 172.16.0.0/16 61.114.182.54 tcp * www",
		"ip filter 46 pass 172.16.5.0/24 61.114.182.54 tcp * www",
		"ip filter 47 pass 172.16.15.0/24 61.114.182.54 tcp * www",
		"ip filter 48 pass 172.16.60.0/24 61.114.182.54 tcp * www",
		"ip filter 50 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *",
		"ip filter 51 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *",
		"ip filter 52 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *",
		"ip filter 53 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *",
		"ip filter 54 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *",
		"ip filter 55 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https",
		"ip filter 56 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https",
		"ip filter 57 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https",
		"ip filter 58 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https",
		"ip filter 59 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https",
		"ip filter 60 pass 61.114.177.148 172.16.0.0/16 tcp https *",
		"ip filter 61 pass 61.114.177.148 172.16.5.0/24 tcp https *",
		"ip filter 62 pass 61.114.177.148 172.16.15.0/24 tcp https *",
		"ip filter 63 pass 61.114.177.148 172.16.60.0/24 tcp https *",
		"ip filter 65 pass 172.16.0.0/16 61.114.177.148 tcp * https",
		"ip filter 66 pass 172.16.5.0/24 61.114.177.148 tcp * https",
		"ip filter 67 pass 172.16.15.0/24 61.114.177.148 tcp * https",
		"ip filter 68 pass 172.16.60.0/24 61.114.177.148 tcp * https",
		"ip filter 70 pass 202.229.106.177 172.16.0.0/16 tcp www,https *",
		"ip filter 71 pass 202.229.106.177 172.16.5.0/24 tcp www,https *",
		"ip filter 72 pass 202.229.106.177 172.16.15.0/24 tcp www,https *",
		"ip filter 73 pass 202.229.106.177 172.16.60.0/24 tcp www,https *",
		"ip filter 74 pass 202.229.106.177 172.16.80.0/22 tcp www,https *",
		"ip filter 75 pass 172.16.0.0/16 202.229.106.177 tcp * www,https",
		"ip filter 76 pass 172.16.5.0/24 202.229.106.177 tcp * www,https",
		"ip filter 77 pass 172.16.15.0/24 202.229.106.177 tcp * www,https",
		"ip filter 78 pass 172.16.60.0/24 202.229.106.177 tcp * www,https",
		"ip filter 79 pass 172.16.80.0/22 202.229.106.177 tcp * www,https",
	};
	if (!_Put(pCmd02, DK_Number(pCmd02)))
		return FALSE;
	
	wsprintf(szPut, "ip filter 80 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 81 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 82 pass %s 172.16.15.0/24 tcp www,https *", IP_MG_WEB_IP);
	if (!_Put(szPut))	return FALSE;

	if (!_Put("ip filter 83 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *"))
		return FALSE;
	
	if (!_Put("ip filter 84 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *"))
 		return FALSE;
	
	wsprintf(szPut, "ip filter 85 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 86 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 87 pass 172.16.15.0/24 %s tcp * www,https", IP_MG_WEB_IP);
	if (!_Put(szPut))	return FALSE;
	
	if (!_Put("ip filter 88 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https"))
		return FALSE;
	
	if (!_Put("ip filter 89 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https"))
		return FALSE;
	
	if ( m_dwPop3Flag )
	{
		wsprintf(szPut, "ip filter 90 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 95 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}	
	if ( m_dwSmtpFlag )
	{
		wsprintf(szPut, "ip filter 91 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 96 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	
	const char *pCmd02_1[] = {
		"ip filter 92 pass * 172.16.5.0/24 tcp https,5222,5223 *",
		"ip filter 93 pass * 172.16.5.0/24 udp domain,ntp *",
		"ip filter 94 pass * 172.16.5.0/24 icmp",
		"ip filter 97 pass 172.16.5.0/24 * tcp * https,5222,5223",
		"ip filter 98 pass 172.16.5.0/24 * udp * domain,ntp",
		"ip filter 99 pass 172.16.5.0/24 * icmp",
	};
	if (!_Put(pCmd02_1, DK_Number(pCmd02_1)))
		return FALSE;
		
	const char *pCmd02_2[] = {
		"ip filter 100 pass-log * * * * *",
		"ip filter 101 reject * * established * netbios_ns-netbios_ssn",
		"ip filter 102 reject * * established * 135",
		"ip filter 103 reject * * established * 445",
		"ip filter 104 reject * * established * https",
		"ip filter 105 reject * * established * www",
		"ip filter 106 reject * * established * pop3",
		"ip filter 107 reject * * established * smtp",
	};
	if (!_Put(pCmd02_2, DK_Number(pCmd02_2)))
		return FALSE;
	
	// IP電話の設定がある場合は書き込む
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon )
	{
		if ( !_Put("ip filter 200 pass 172.21.0.0/16 * * * *") )
			return FALSE;
	}
	// ③DKネットワーク（172.16.0.0/16）側Lan1インタフェースで適用する静的フィルタ　（1000番台）
	wsprintf(szPut, "ip filter 1400 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))
		return FALSE;
	
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon || m_sRouter.m_nFqdnState != eFqdnNon )
	{
		if (!_Put("ip filter 1401 pass 172.21.0.0/16 * * * *"))	
			return FALSE;
	}
	
	wsprintf(szPut, "ip filter 1405 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))
		return FALSE;
	
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon || m_sRouter.m_nFqdnState != eFqdnNon )
	{
		if (!_Put("ip filter 1406 pass * 172.21.0.0/16 * * *"))	
			return FALSE;
	}

	// ①基本フィルタ
	const char *pCmd03[] = {
		"ip filter 1410 pass * * udp dhcpc dhcps",
		"ip filter 1430 pass 10.0.0.0/8 10.0.208.0/20 * * *",
		"ip filter 1431 pass 10.0.208.0/20 10.0.0.0/8 * * *",
		"ip filter 1432 pass 10.0.0.0/8 172.30.1.110 tcp * 21",
		"ip filter 1433 pass 172.30.1.110 10.0.0.0/8 tcp 21 *",
		"ip filter 1434 pass 10.0.0.0/8 172.30.1.110 tcp * ftpdata",
		"ip filter 1435 pass 172.30.1.110 10.0.0.0/8 tcp ftpdata *",
		"ip filter 1436 pass 10.0.0.0/8 172.31.191.0/24 * * *",
		"ip filter 1437 pass 172.31.191.0/24 10.0.0.0/8 * * *",
		"ip filter 1438 pass 10.0.0.0/8 10.0.0.0/8 * * *",
		"ip filter 1440 pass 172.18.0.0/24 10.0.192.0/18 * * *",
		"ip filter 1441 pass 10.0.192.0/18 172.18.0.0/24 * * *",
	};
	if (!_Put(pCmd03, DK_Number(pCmd03)))
		return FALSE;
	
	if ( m_sRouter.sLAN3.fVirusUse )
	{
		const char *pCmd03_1[] = {
			"ip filter 1446 pass 172.16.252.1 10.0.192.0/18 tcp 3389 *",
			"ip filter 1447 pass 10.0.192.0/18 172.16.252.1 tcp * 3389"
		};
		if (!_Put(pCmd03_1, DK_Number(pCmd03_1)))
			return FALSE;
	}
	
	if ( m_sRouter.port[1].kaisen == eKaisenADSL || ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL )
	{
		// ③DKネットワーク（172.16.0.0/16）側Lan1インタフェースで適用する静的フィルタ　（1000番台）
		wsprintf(szPut, "ip filter 2010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 2015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		
		if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
		{
			wsprintf(szPut,"ip filter 2025 pass 172.16.0.0/16 %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 2026 pass * %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 2027 pass 172.16.15.0/24 %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 2028 pass 172.16.60.0/24 %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			
			if ( m_dwDnsFlag )	// レジストリ設定ダイアログにて①「DNSサーバ接続」が○だった場合のみ、以下の設定を追加する。
			{
				wsprintf(szPut,"ip filter 2029 pass * %s,%s udp * domain", szIp, szIp2);
				if (!_Put(szPut))	return FALSE;
			}
		}

		const char *pCmd04[] = {
			"ip filter 2030 pass 61.114.182.53 172.16.0.0/16 tcp https *",
			"ip filter 2031 pass 61.114.182.53 172.16.5.0/24 tcp https *",
			"ip filter 2032 pass 61.114.182.53 172.16.15.0/24 tcp https *",
			"ip filter 2033 pass 61.114.182.53 172.16.60.0/24 tcp https *",
			"ip filter 2035 pass 172.16.0.0/16 61.114.182.53 tcp * https",
			"ip filter 2036 pass 172.16.5.0/24 61.114.182.53 tcp * https",
			"ip filter 2037 pass 172.16.15.0/24 61.114.182.53 tcp * https",
			"ip filter 2038 pass 172.16.60.0/24 61.114.182.53 tcp * https",
			"ip filter 2040 pass 61.114.182.54 172.16.0.0/16 tcp www *",
			"ip filter 2041 pass 61.114.182.54 172.16.5.0/24 tcp www *",
			"ip filter 2042 pass 61.114.182.54 172.16.15.0/24 tcp www *",
			"ip filter 2043 pass 61.114.182.54 172.16.60.0/24 tcp www *",
			"ip filter 2045 pass 172.16.0.0/16 61.114.182.54 tcp * www",
			"ip filter 2046 pass 172.16.5.0/24 61.114.182.54 tcp * www",
			"ip filter 2047 pass 172.16.15.0/24 61.114.182.54 tcp * www",
			"ip filter 2048 pass 172.16.60.0/24 61.114.182.54 tcp * www",
			"ip filter 2050 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *",
			"ip filter 2051 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *",
			"ip filter 2052 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *",
			"ip filter 2053 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *",
			"ip filter 2054 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *",
			"ip filter 2055 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https",
			"ip filter 2056 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https",
			"ip filter 2057 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https",
			"ip filter 2058 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https",
			"ip filter 2059 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https",
			"ip filter 2060 pass 61.114.177.148 172.16.0.0/16 tcp https *",
			"ip filter 2061 pass 61.114.177.148 172.16.5.0/24 tcp https *",
			"ip filter 2062 pass 61.114.177.148 172.16.15.0/24 tcp https *",
			"ip filter 2063 pass 61.114.177.148 172.16.60.0/24 tcp https *",
			"ip filter 2065 pass 172.16.0.0/16 61.114.177.148 tcp * https",
			"ip filter 2066 pass 172.16.5.0/24 61.114.177.148 tcp * https",
			"ip filter 2067 pass 172.16.15.0/24 61.114.177.148 tcp * https",
			"ip filter 2068 pass 172.16.60.0/24 61.114.177.148 tcp * https",
			"ip filter 2070 pass 202.229.106.177 172.16.0.0/16 tcp www,https *",
			"ip filter 2071 pass 202.229.106.177 172.16.5.0/24 tcp www,https *",
			"ip filter 2072 pass 202.229.106.177 172.16.15.0/24 tcp www,https *",
			"ip filter 2073 pass 202.229.106.177 172.16.60.0/24 tcp www,https *",
			"ip filter 2074 pass 202.229.106.177 172.16.80.0/22 tcp www,https *",
			"ip filter 2075 pass 172.16.0.0/16 202.229.106.177 tcp * www,https",
			"ip filter 2076 pass 172.16.5.0/24 202.229.106.177 tcp * www,https",
			"ip filter 2077 pass 172.16.15.0/24 202.229.106.177 tcp * www,https",
			"ip filter 2078 pass 172.16.60.0/24 202.229.106.177 tcp * www,https",
			"ip filter 2079 pass 172.16.80.0/22 202.229.106.177 tcp * www,https",
		};
		if (!_Put(pCmd04, DK_Number(pCmd04)))
			return FALSE;
	}// if ( m_sRouter.port[1].kaisen == eKaisenADSL || m_sRouter.port[1].kaisen == eKaisenFixADSL )
	
	wsprintf(szPut, "ip filter 2080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 2081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 2082 pass %s 172.16.15.0/24 tcp www,https *", IP_MG_WEB_IP);
	if (!_Put(szPut))	return FALSE;

	if (!_Put("ip filter 2083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *"))
		return FALSE;
	
	if (!_Put("ip filter 2084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *"))
 		return FALSE;
	
	wsprintf(szPut, "ip filter 2085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 2086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 2087 pass 172.16.15.0/24 %s tcp * www,https", IP_MG_WEB_IP);
	if (!_Put(szPut))	return FALSE;
	
	if (!_Put("ip filter 2088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https"))
		return FALSE;

	if (!_Put("ip filter 2089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https"))
 		return FALSE;
	
	if ( m_dwPop3Flag )
	{
		wsprintf(szPut, "ip filter 2090 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 2095 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	if ( m_dwSmtpFlag )
	{
		wsprintf(szPut, "ip filter 2091 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 2096 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	
	const char *pCmd04_1[] = {
		"ip filter 2092 pass * 172.16.5.0/24 tcp https,5222,5223 *",
		"ip filter 2093 pass * 172.16.5.0/24 udp domain,ntp *",
		"ip filter 2094 pass * 172.16.5.0/24 icmp",
		"ip filter 2097 pass 172.16.5.0/24 * tcp * https,5222,5223",
		"ip filter 2098 pass 172.16.5.0/24 * udp * domain,ntp",
		"ip filter 2099 pass 172.16.5.0/24 * icmp",
	};
	if (!_Put(pCmd04_1, DK_Number(pCmd04_1)))
		return FALSE;
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed )
	{
		wsprintf(szPut,"ip filter 2450 pass %s 61.114.186.0/25 tcp * https", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		wsprintf(szPut,"ip filter 2451 pass 61.114.186.0/25 %s tcp https *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		wsprintf(szPut,"ip filter 3450 pass %s 61.114.186.0/25 tcp * https", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3451 pass 61.114.186.0/25 %s tcp https *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;	
	}

	if ( m_sRouter.port[1].kaisen == eKaisenADSL || ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL )
	{	
		PutFilterPass(2200, 3, TRUE);
		PutFilterPass(2250, 4, FALSE);
		PutFilterPass(2300, 5, TRUE);
		PutFilterPass(2350, 5, FALSE);
		
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			wsprintf(szPut,"ip filter 2444 pass %s * tcp * www,https", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 2445 pass * %s tcp www,https *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
		}
		
		PutFilterPass(2600, 0, TRUE);
		PutFilterPass(2650, 0, FALSE);
		
		PutFilterPass(2700, 1, TRUE);
		PutFilterPass(2750, 1, FALSE);
		
		PutFilterPass(2900, 2, TRUE, TRUE);
		PutFilterPass(2950, 2, FALSE,TRUE);
	}
	
	if ( m_sRouter.port[2].kaisen == eKaisenLAN )
	{
		// ⑤DKネットワーク（172.16.0.0/16）側Lan3インタフェースで適用する静的フィルタ　（3000番台）
		wsprintf(szPut, "ip filter 3010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 3015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		
		if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
		{
			wsprintf(szPut,"ip filter 3025 pass 172.16.0.0/16 %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3026 pass 172.16.5.0/24 %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3027 pass 172.16.15.0/24 %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3028 pass 172.16.60.0/24 %s,%s udp * domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			
			if ( m_dwDnsFlag )	// レジストリ設定ダイアログにて①「DNSサーバ接続」が○だった場合のみ、以下の設定を追加する。
			{
				wsprintf(szPut,"ip filter 3029 pass * %s,%s udp * domain", szIp, szIp2);
				if (!_Put(szPut))	return FALSE;
			}
		}
		
		const char *pCmd05[] = {
			"ip filter 3030 pass 61.114.182.53 172.16.0.0/16 tcp https *",
			"ip filter 3031 pass 61.114.182.53 172.16.5.0/24 tcp https *",
			"ip filter 3032 pass 61.114.182.53 172.16.15.0/24 tcp https *",
			"ip filter 3033 pass 61.114.182.53 172.16.60.0/24 tcp https *",
			"ip filter 3035 pass 172.16.0.0/16 61.114.182.53 tcp * https",
			"ip filter 3036 pass 172.16.5.0/24 61.114.182.53 tcp * https",
			"ip filter 3037 pass 172.16.15.0/24 61.114.182.53 tcp * https",
			"ip filter 3038 pass 172.16.60.0/24 61.114.182.53 tcp * https",
			"ip filter 3040 pass 61.114.182.54 172.16.0.0/16 tcp www *",
			"ip filter 3041 pass 61.114.182.54 172.16.5.0/24 tcp www *",
			"ip filter 3042 pass 61.114.182.54 172.16.15.0/24 tcp www *",
			"ip filter 3043 pass 61.114.182.54 172.16.60.0/24 tcp www *",
			"ip filter 3045 pass 172.16.0.0/16 61.114.182.54 tcp * www",
			"ip filter 3046 pass 172.16.5.0/24 61.114.182.54 tcp * www",
			"ip filter 3047 pass 172.16.15.0/24 61.114.182.54 tcp * www",
			"ip filter 3048 pass 172.16.60.0/24 61.114.182.54 tcp * www",
			"ip filter 3050 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *",
			"ip filter 3051 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *",
			"ip filter 3052 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *",
			"ip filter 3053 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *",
			"ip filter 3054 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *",
			"ip filter 3055 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https",
			"ip filter 3056 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https",
			"ip filter 3057 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https",
			"ip filter 3058 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https",
			"ip filter 3059 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https",
			"ip filter 3060 pass 61.114.177.148 172.16.0.0/16 tcp https *",
			"ip filter 3061 pass 61.114.177.148 172.16.5.0/24 tcp https *",
			"ip filter 3062 pass 61.114.177.148 172.16.15.0/24 tcp https *",
			"ip filter 3063 pass 61.114.177.148 172.16.60.0/24 tcp https *",
			"ip filter 3065 pass 172.16.0.0/16 61.114.177.148 tcp * https",
			"ip filter 3066 pass 172.16.5.0/24 61.114.177.148 tcp * https",
			"ip filter 3067 pass 172.16.15.0/24 61.114.177.148 tcp * https",
			"ip filter 3068 pass 172.16.60.0/24 61.114.177.148 tcp * https",
			"ip filter 3070 pass 202.229.106.177 172.16.0.0/16 tcp www,https *",
			"ip filter 3071 pass 202.229.106.177 172.16.5.0/24 tcp www,https *",
			"ip filter 3072 pass 202.229.106.177 172.16.15.0/24 tcp www,https *",
			"ip filter 3073 pass 202.229.106.177 172.16.60.0/24 tcp www,https *",
			"ip filter 3074 pass 202.229.106.177 172.16.80.0/22 tcp www,https *",
			"ip filter 3075 pass 172.16.0.0/16 202.229.106.177 tcp * www,https",
			"ip filter 3076 pass 172.16.5.0/24 202.229.106.177 tcp * www,https",
			"ip filter 3077 pass 172.16.15.0/24 202.229.106.177 tcp * www,https",
			"ip filter 3078 pass 172.16.60.0/24 202.229.106.177 tcp * www,https",
			"ip filter 3079 pass 172.16.80.0/22 202.229.106.177 tcp * www,https",
		};
		if (!_Put(pCmd05, DK_Number(pCmd05)))
			return FALSE;
	}// if ( m_sRouter.port[2].kaisen == eKaisenLAN )	
	
	wsprintf(szPut, "ip filter 3080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 3081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 3082 pass %s 172.16.15.0/24 tcp www,https *", IP_MG_WEB_IP);
	if (!_Put(szPut))	return FALSE;
	
	if (!_Put("ip filter 3083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *"))
		return FALSE;
	
	if (!_Put("ip filter 3084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *"))
		return FALSE;
	
	wsprintf(szPut, "ip filter 3085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 3086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 3087 pass 172.16.15.0/24 %s tcp * www,https", IP_MG_WEB_IP);
	if (!_Put(szPut))	return FALSE;
	
	if (!_Put("ip filter 3088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https"))
		return FALSE;
	
	if (!_Put("ip filter 3089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https"))
 		return FALSE;
	
	if ( m_dwPop3Flag )
	{
		wsprintf(szPut, "ip filter 3090 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 3095 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	if ( m_dwSmtpFlag )
	{
		wsprintf(szPut, "ip filter 3091 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 3096 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	
	const char *pCmd04_2[] = {
		"ip filter 3092 pass * 172.16.5.0/24 tcp https,5222,5223 *",
		"ip filter 3093 pass * 172.16.5.0/24 udp domain,ntp *",
		"ip filter 3094 pass * 172.16.5.0/24 icmp",		
		"ip filter 3097 pass 172.16.5.0/24 * tcp * https,5222,5223",
		"ip filter 3098 pass 172.16.5.0/24 * udp * domain,ntp",
		"ip filter 3099 pass 172.16.5.0/24 * icmp",
	};
	if (!_Put(pCmd04_2, DK_Number(pCmd04_2)))
		return FALSE;

	if ( m_sRouter.port[2].kaisen == eKaisenLAN )
	{
		wsprintf(szPut,"ip filter 3420 pass-log %s %s icmp * *",
			GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), GetIpB2C_Second(m_sRouter.ip, NULL));
		if (!_Put(szPut))	return FALSE;
		
		wsprintf(szPut,"ip filter 3421 pass-log %s 172.16.60.0/24 icmp * *",
			GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			wsprintf(szPut,"ip filter 3422 pass-log %s 172.16.0.0/16 icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3427 pass-log 172.16.0.0/16 %s icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
		}
		
		wsprintf(szPut,"ip filter 3425 pass-log %s %s icmp * *",
			GetIpB2C(m_sRouter.ip, NULL), GetIpB2C_Second(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		
		wsprintf(szPut,"ip filter 3426 pass-log 172.16.60.0/24 %s icmp * *",
			GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			wsprintf(szPut,"ip filter 3442 pass %s %s udp * domain", GetIpB2C(m_sRouter.sLAN3.ipVirus), GetIpB2C_Second(m_sRouter.ip));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3443 pass %s %s udp domain *", GetIpB2C(m_sRouter.ip), GetIpB2C_Second(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3444 pass %s * tcp * www,https", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3445 pass * %s tcp www,https *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;			
			wsprintf(szPut,"ip filter 3446 pass-log %s 10.0.192.0/18 icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3447 pass-log 10.0.192.0/18 %s icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3448 pass-log %s 10.0.192.0/18 tcp 3389 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter 3449 pass-log 10.0.192.0/18 %s tcp * 3389", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			
// 			wsprintf(szPut,"ip filter 3450 pass-log %s 172.16.0.0/16 icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
// 			if (!_Put(szPut))	return FALSE;
// 			wsprintf(szPut,"ip filter 3451 pass-log 172.16.0.0/16 %s icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
// 			if (!_Put(szPut))	return FALSE;
		}
		
		wsprintf(szPut,"ip filter 3500 pass-log %s 172.16.60.0/24 tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			wsprintf(szPut, "ip filter 3501 pass-log %s 172.16.0.0/16 tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut, "ip filter 3506 pass-log 172.16.0.0/16 %s tcp,udp netbios_ns-netbios_ssn * ", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut, "ip filter 3521 pass-log %s 172.16.0.0/16 tcp,udp * 445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut, "ip filter 3526 pass-log 172.16.0.0/16 %s tcp,udp 445 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
		}
		
		wsprintf(szPut,"ip filter 3505 pass-log 172.16.60.0/24 %s tcp,udp netbios_ns-netbios_ssn *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3510 pass-log %s 172.16.60.0/24 tcp,udp * 135", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3515 pass-log 172.16.60.0/24 %s tcp,udp 135 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3520 pass-log %s 172.16.60.0/24 tcp,udp * 445", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3525 pass-log 172.16.60.0/24 %s tcp,udp 445 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3530 pass %s 172.16.60.0/24 tcp * 21", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3535 pass 172.16.60.0/24 %s tcp 21 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		wsprintf(szPut,"ip filter 3540 pass %s 172.16.60.0/24 tcp * ftpdata", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		wsprintf(szPut,"ip filter 3545 pass 172.16.60.0/24 %s tcp ftpdata *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		wsprintf(szPut,"ip filter 3550 pass %s 172.16.60.0/24 tcp 58010-58019 2638", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		wsprintf(szPut,"ip filter 3555 pass 172.16.60.0/24 %s tcp 2638 58010-58019", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		wsprintf(szPut,"ip filter 3560 pass-log %s 172.16.60.0/24 tcp,udp netbios_ns-netbios_ssn *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;		
		
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			wsprintf(szPut, "ip filter 3561 pass-log %s 172.16.0.0/16 tcp,udp netbios_ns-netbios_ssn *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut, "ip filter 3566 pass-log 172.16.0.0/16 %s tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut, "ip filter 3581 pass-log %s 172.16.0.0/16 tcp,udp 445 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut, "ip filter 3586 pass-log 172.16.0.0/16 %s tcp,udp * 445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
			if (!_Put(szPut))	return FALSE;
		}
		
		wsprintf(szPut,"ip filter 3565 pass-log 172.16.60.0/24 %s tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3570 pass-log %s 172.16.60.0/24 tcp,udp 135 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3575 pass-log 172.16.60.0/24 %s tcp,udp * 135", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;			
		wsprintf(szPut,"ip filter 3580 pass-log %s 172.16.60.0/24 tcp,udp 445 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;			
		wsprintf(szPut,"ip filter 3585 pass-log 172.16.60.0/24 %s tcp,udp * 445", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3590 pass %s 172.16.60.0/24 tcp * 58006", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter 3595 pass 172.16.60.0/24 %s tcp 58006 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		
		PutFilterPass(3600, 0, TRUE );
		PutFilterPass(3650, 0, FALSE);
		PutFilterPass(3700, 1, TRUE );
		PutFilterPass(3750, 1, FALSE);
		
		wsprintf(szPut,"ip filter 3800 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		for ( int nFinNo = 3801; nFinNo <= 3804; nFinNo++ )	// 信頼できるネットワーク
		{
			if ( m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].no )	// 該当行い設定がある
			{
				wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
			} else {
				wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo, GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
			}
			if (!_Put(szPut))	return FALSE;
		}
		
		wsprintf(szPut,"ip filter 3805 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
		for ( nFinNo = 3806; nFinNo <= 3809; nFinNo++ )	// 信頼できるネットワーク
		{
			if ( m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].no )	// 該当行い設定がある
			{
				wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].mask));
			} else {
				wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo, GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
			}
			if (!_Put(szPut))	return FALSE;
		}
		
		PutFilterPass(3900, 2, TRUE, TRUE);
		PutFilterPass(3950, 2, FALSE,TRUE);
	}// if ( m_sRouter.port[2].kaisen == eKaisenLAN )
	
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon )
	{
		BYTE bMySegmentIP[IP_BYTE];	// お客様の固定IPアドレスにマスクをかけた後のIP
		CopyMemory(bMySegmentIP, m_sRouter.ip_2, sizeof(bMySegmentIP));
		bMySegmentIP[3] -= 1;	// 必ず１以上が入っているはず
		
		for ( int n = 0; n < SETU_MAX; n++ )
		{
			if ( m_sRouter.ncon[n].type == eFixADSL ) 
				break;
		}
		if ( n == SETU_MAX )
			return FALSE;
		
		wsprintf( szPut, "ip filter 10000 reject %s * * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
		if (!_Put(szPut))	return FALSE;
		wsprintf( szPut, "ip filter 10005 reject * %s * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
		if (!_Put(szPut))	return FALSE;
//===== ↓2013/07/18 17:14:24   ==============================================================================
		char szMainRt[256];
		char szBackUpRt[256];
		char szLan2Fix[256];
		wsprintf(szMainRt,	"%s", GetIpB2C(m_sRouter.ptc_ip));				// メインルータ
		wsprintf(szBackUpRt, "%s", GetIpB2C_Second(m_sRouter.center_ip));	// バックアップルータ
		
//===== ↓LTE通信対応 ==================================================================
		if ( m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE ) 
			wsprintf(szLan2Fix,	"%s", GetIpB2C_Second(m_sRouter.sLte.ip));		// LANの固定IPアドレス
		else
			wsprintf(szLan2Fix,	"%s", GetIpB2C_Second(m_sRouter.ncon[n].fixIp));// LANの固定IPアドレス
//===== ↑LTE通信対応 ==================================================================		
		if ( m_sRouter.m_nIpTelNo > 0 && m_sRouter.m_nIpTelNoBk > 0 )
		{
			wsprintf( szPut, "ip filter 10010 pass %s,%s %s esp * *", szMainRt, szBackUpRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10015 pass %s %s,%s esp * *", szLan2Fix, szMainRt, szBackUpRt);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10020 pass %s,%s %s udp * 500", szMainRt, szBackUpRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10025 pass %s %s,%s udp * 500", szLan2Fix, szMainRt, szBackUpRt);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10030 pass %s,%s %s icmp * *", szMainRt, szBackUpRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10035 pass %s %s,%s icmp * *", szLan2Fix, szMainRt, szBackUpRt);
			if (!_Put(szPut))	return FALSE;
		}
		else if ( m_sRouter.m_nIpTelNo > 0 )
		{
			wsprintf( szPut, "ip filter 10010 pass %s %s esp * *", szMainRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10015 pass %s %s esp * *", szLan2Fix, szMainRt);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10020 pass %s %s udp * 500", szMainRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10025 pass %s %s udp * 500", szLan2Fix, szMainRt);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10030 pass %s %s icmp * *", szMainRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10035 pass %s %s icmp * *", szLan2Fix, szMainRt);
			if (!_Put(szPut))	return FALSE;
		}
		else if ( m_sRouter.m_nIpTelNoBk > 0 )
		{
			wsprintf( szPut, "ip filter 10010 pass %s %s esp * *", szBackUpRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10015 pass %s %s esp * *", szLan2Fix, szBackUpRt);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10020 pass %s %s udp * 500", szBackUpRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10025 pass %s %s udp * 500", szLan2Fix, szBackUpRt);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10030 pass %s %s icmp * *", szBackUpRt, szLan2Fix);
			if (!_Put(szPut))	return FALSE;
			wsprintf( szPut, "ip filter 10035 pass %s %s icmp * *", szLan2Fix, szBackUpRt);
			if (!_Put(szPut))	return FALSE;
		}
//===== ↑2013/07/18 17:14:24   ==============================================================================
	}
	
	if ( m_sRouter.m_nCarMentNo > 0 )
	{	
		// ※10100番台は、システム・エイ・ブイとの接続に適用するフィルタとする。
		const char *pCmd05_1[] =
		{
			"ip filter 10100 pass 10.0.240.0/24 172.16.7.1 icmp * *",
			"ip filter 10110 reject 10.0.240.0/24 172.16.7.1 * * *",
			"ip filter 10120 pass 10.0.240.0/24 172.16.7.0/24 * * *",
			"ip filter 10125 pass 172.16.7.0/24 10.0.240.0/24 * * *",
			"ip filter 10130 reject 10.0.240.0/24 172.16.0.0/16 * * *",
			"ip filter 10135 reject-log 172.16.0.0/16 10.0.240.0/24 * * *",
		};
		
		if (!_Put(pCmd05_1, DK_Number(pCmd05_1)))
			return FALSE;
	}
	
	if ( m_sRouter.m_nJnetNo > 0 )
	{
		wsprintf(szPut, "ip filter 10200 pass %s %s tcp * 4500", IP_JNET_MAIN_IP, GetIpB2C(m_sRouter.ip));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 10201 pass %s %s tcp * 4500", IP_JNET_SUB_IP, GetIpB2C(m_sRouter.ip));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 10205 pass %s %s tcp * 4500", GetIpB2C(m_sRouter.ip), IP_JNET_MAIN_IP);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 10206 pass %s %s tcp * 4500", GetIpB2C(m_sRouter.ip), IP_JNET_SUB_IP);
		if (!_Put(szPut))	return FALSE;
		
		wsprintf(szPut, "ip filter 10210 pass 172.16.21.1 192.168.210.3 tcp * 21");
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 10211 pass 172.16.21.1 192.168.210.4 tcp * 21");
		if (!_Put(szPut))	return FALSE;
	}

//===== ↓クラウドIP電話対応 ==============================================================================
	CopyMemory(m_byLAN3IP, m_sRouter.ip_2_cloud, sizeof(m_byLAN3IP));
	
	CopyMemory(m_byLan3Ip_plus2, m_sRouter.ip_2_cloud, sizeof(m_byLan3Ip_plus2));
	CopyMemory(m_byLan3Ip_plus5, m_sRouter.ip_2_cloud, sizeof(m_byLan3Ip_plus5));
	
	IpAddressCalc(m_byLan3Ip_plus2, 2);
	IpAddressCalc(m_byLan3Ip_plus5, 5);

	CString strCloudIpTel;	// クラウドIP電話サーバーアドレス一覧
	for ( int nGet = 0; nGet < m_strRouterSendIpArray.GetSize(); nGet++ )
	{
		if ( nGet > 0 )
			strCloudIpTel += ",";
		strCloudIpTel += m_strRouterSendIpArray[nGet];
	}
	
	if ( m_sRouter.m_nFqdnState == eFqdnLAN1 )
	{
		wsprintf(szPut, "ip filter 101402 pass * %s-%s udp ntp *", m_szFirstAdd, m_szLastAdd);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101404 pass * %s-%s tcp www,https *", m_szFirstAdd, m_szLastAdd);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101406 pass %s-%s %s udp * *", m_szFirstAdd, m_szLastAdd, strCloudIpTel);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101407 pass %s-%s * udp * ntp", m_szFirstAdd, m_szLastAdd);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101408 pass %s-%s * udp * domain", m_szFirstAdd, m_szLastAdd);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101409 pass %s-%s * tcp * www,https", m_szFirstAdd, m_szLastAdd);
		if (!_Put(szPut))	return FALSE;
	}
	
	if ( m_sRouter.m_nFqdnState == eFqdnLAN3 )
	{
		wsprintf(szPut, "ip filter 101402 pass * %s-%s udp ntp *", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101404 pass * %s-%s tcp www,https *", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101406 pass %s-%s %s udp * *", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5), strCloudIpTel);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101407 pass %s-%s * udp * ntp", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101408 pass %s-%s * udp * domain", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter 101409 pass %s-%s * tcp * www,https", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5));
		if (!_Put(szPut))	return FALSE;
	}
//===== ↑クラウドIP電話対応  ==============================================================================
	
	// チャットボット対応フィルタとする。
	const char *pCmdChatBot[] =
	{
		"ip filter 10800 pass 18.179.137.187,3.115.131.124 172.16.15.0/24 tcp www,https,8080 *",
		"ip filter 10801 pass 172.16.15.0/24 18.179.137.187,3.115.131.124 tcp * www,https,8080",
	};
	if (!_Put(pCmdChatBot, DK_Number(pCmdChatBot)))
		return FALSE;

	if ( m_sRouter.nLuaPort )
	{
//===== ↓LTE通信対応 ==================================================================
		if ( !PutFmt("ip filter 10500 pass * %s tcp * %d", GetIpB2C(m_sRouter.ip), (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nLuaPort : m_sRouter.nLuaPort) )	return FALSE;
		if ( !PutFmt("ip filter 10505 pass %s * tcp %d", GetIpB2C(m_sRouter.ip), (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nLuaPort : m_sRouter.nLuaPort) )	return FALSE;
//===== ↑LTE通信対応 ==================================================================
		if ( !PutFmt("ip filter 10510 pass %s %s udp * 500", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
		if ( !PutFmt("ip filter 10520 pass %s %s esp * *", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
		if ( !PutFmt("ip filter 10525 pass %s %s esp * *", GetIpB2C(m_sRouter.ip), GetLuaIP()) )	return FALSE;
		if ( !PutFmt("ip filter 10550 pass %s %s udp * 1701", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
		if ( !PutFmt("ip filter 10560 pass %s %s udp * 4500", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
		if ( !PutFmt("ip filter 10575 pass %s %s udp * *", GetIpB2C(m_sRouter.ip), GetLuaIP()) )	return FALSE;
		if ( !_Put("ip filter 10600 pass 172.16.60.0/24 172.16.60.0/24 tcp * 58300") )	return FALSE;
		if ( !_Put("ip filter 10605 pass 172.16.60.0/24 172.16.60.0/24 tcp 58300 *") )	return FALSE;
		if ( !_Put("ip filter 10610 pass 172.16.60.0/24 172.16.60.0/24 udp 58301 *") )	return FALSE;
		if ( !_Put("ip filter 10615 pass 172.16.60.0/24 172.16.60.0/24 udp * 58301") )	return FALSE;
		if ( !PutFmt("ip filter 10620 pass 172.16.60.0/24 172.16.60.0/24 tcp * %d", m_sRouter.nRdpRecvPort) ) return FALSE;
		if ( !PutFmt("ip filter 10625 pass 172.16.60.0/24 172.16.60.0/24 tcp %d *", m_sRouter.nRdpRecvPort) ) return FALSE;
	}
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed )
	{
		wsprintf(szPut,"ip filter 10650 pass %s * tcp * 58300", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;	
		wsprintf(szPut,"ip filter 10655 pass * %s tcp 58300 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;	
		wsprintf(szPut,"ip filter 10660 pass %s * udp 58301 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;	
		wsprintf(szPut,"ip filter 10665 pass * %s udp * 58301", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;	
		wsprintf(szPut,"ip filter 10670 pass %s * tcp * %d", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), m_sRouter.nRdpRecvPort);
		if (!_Put(szPut))	return FALSE;	
		wsprintf(szPut,"ip filter 10675 pass * %s tcp %d *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), m_sRouter.nRdpRecvPort);
		if (!_Put(szPut))	return FALSE;	
	}
	
	//===== ↓クラウドリニューアル時対応(サポ端末接続フィルタ追加) ========================================================
	// サポート端末接続用フィルタの追加
	CString strSptFilter = "221.186.163.29,180.42.107.1,153.139.247.107,222.151.227.83,221.186.163.27,221.186.163.28,122.1.9.170,122.1.9.171,122.1.9.172,122.1.9.173,122.1.9.174,125.206.196.139,125.206.210.19,60.32.159.235,60.32.143.116,60.32.143.117,202.41.213.170,202.41.213.171,202.41.213.172,202.41.213.173,221.186.110.83,60.32.158.219,114.156.254.18,222.151.236.148,222.151.236.149,222.151.236.146,153.231.254.66,153.231.254.67,60.32.188.115,60.32.188.116,60.32.123.123,60.32.123.124,125.206.255.44,125.206.255.45,60.32.122.123,60.32.122.124,180.42.70.100,180.42.70.102,180.42.70.103,180.42.70.104,180.42.70.105,118.22.14.108,118.22.14.109,118.22.42.66,118.22.42.67,118.22.42.68,118.22.42.69,60.32.143.118";
	wsprintf(szPut, "ip filter 12600 pass %s 172.16.5.0/24 udp * 500", strSptFilter);
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 12650 pass 172.16.5.0/24 %s udp * 500", strSptFilter);
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 12700 pass %s 172.16.5.0/24 esp * *", strSptFilter);
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter 12750 pass 172.16.5.0/24 %s esp * *", strSptFilter);
	if (!_Put(szPut))	return FALSE;
	//===== ↑クラウドリニューアル時対応(サポ端末接続フィルタ追加) ========================================================
	
	if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
	{
		// ①DKネットワーク（172.16.0.0/16）側PPインタフェースで適用する動的フィルタ　（0番台）
		wsprintf(szPut,"ip filter dynamic 25 172.16.0.0/16 %s,%s domain", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 26 172.16.5.0/24 %s,%s domain", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 27 172.16.15.0/24 %s,%s domain",szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 28 172.16.60.0/24 %s,%s domain", szIp, szIp2);
		if (!_Put(szPut))	return FALSE;
		
		if ( m_dwDnsFlag )	// レジストリ設定ダイアログにて①「DNSサーバ接続」が○だった場合のみ、以下の設定を追加する。
		{
			wsprintf(szPut,"ip filter dynamic 29 * %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
		}
	}
	
	const char *pCmd06[] = {
		"ip filter dynamic 35 172.16.0.0/16 61.114.182.53 filter 35 in 30",
		"ip filter dynamic 36 172.16.5.0/24 61.114.182.53 filter 36 in 31",
		"ip filter dynamic 37 172.16.15.0/24 61.114.182.53 filter 37 in 32",
		"ip filter dynamic 38 172.16.60.0/24 61.114.182.53 filter 38 in 33",
		"ip filter dynamic 45 172.16.0.0/16 61.114.182.54 filter 45 in 40",
		"ip filter dynamic 46 172.16.5.0/24 61.114.182.54 filter 46 in 41",
		"ip filter dynamic 47 172.16.15.0/24 61.114.182.54 filter 47 in 42",
		"ip filter dynamic 48 172.16.60.0/24 61.114.182.54 filter 48 in 43",
		"ip filter dynamic 55 172.16.0.0/16 61.114.186.0/25 filter 55 in 50",
		"ip filter dynamic 56 172.16.5.0/24 61.114.186.0/25 filter 56 in 51",
		"ip filter dynamic 57 172.16.15.0/24 61.114.186.0/25 filter 57 in 52",
		"ip filter dynamic 58 172.16.60.0/24 61.114.186.0/25 filter 58 in 53",
		"ip filter dynamic 59 172.16.80.0/22 61.114.186.0/25 filter 59 in 54",
		"ip filter dynamic 65 172.16.0.0/16 61.114.177.148 filter 65 in 60",
		"ip filter dynamic 66 172.16.5.0/24 61.114.177.148 filter 66 in 61",
		"ip filter dynamic 67 172.16.15.0/24 61.114.177.148 filter 67 in 62",
		"ip filter dynamic 68 172.16.60.0/24 61.114.177.148 filter 68 in 63",
		"ip filter dynamic 75 172.16.0.0/16 202.229.106.177 filter 75 in 70",
		"ip filter dynamic 76 172.16.5.0/24 202.229.106.177 filter 76 in 71",
		"ip filter dynamic 77 172.16.15.0/24 202.229.106.177 filter 77 in 72",
		"ip filter dynamic 78 172.16.60.0/24 202.229.106.177 filter 78 in 73",
		"ip filter dynamic 79 172.16.80.0/22 202.229.106.177 filter 79 in 74",
	};
	if (!_Put(pCmd06, DK_Number(pCmd06)))
		return FALSE;
	
	wsprintf(szPut, "ip filter dynamic 85 %s * filter 85 in 80", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter dynamic 86 %s * filter 86 in 81", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	
	if (!_Put("ip filter dynamic 87 172.16.15.0/24 * filter 87 in 82"))	return FALSE;
	
	if (!_Put("ip filter dynamic 88 172.16.0.0/16 * filter 88 in 83"))	return FALSE;
		
	if (!_Put("ip filter dynamic 89 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 89 in 84"))	return FALSE;
 	
	if ( m_dwPop3Flag )
	{
		wsprintf(szPut, "ip filter dynamic 95 %s * filter 95 in 90", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	if ( m_dwSmtpFlag )
	{
		wsprintf(szPut, "ip filter dynamic 96 %s * filter 96 in 91", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	
	const char *pCmd07_0[] = {
		"ip filter dynamic 97 172.16.5.0/24 * filter 97 in 92",
		"ip filter dynamic 98 172.16.5.0/24 * filter 98 in 93",
		"ip filter dynamic 99 172.16.5.0/24 * filter 99 in 94",
		"ip filter dynamic 2097 172.16.5.0/24 * filter 2097 in 2092",
		"ip filter dynamic 2098 172.16.5.0/24 * filter 2098 in 2093",
		"ip filter dynamic 2099 172.16.5.0/24 * filter 2099 in 2094",
		"ip filter dynamic 3097 172.16.5.0/24 * filter 3097 in 3092",
		"ip filter dynamic 3098 172.16.5.0/24 * filter 3098 in 3093",
		"ip filter dynamic 3099 172.16.5.0/24 * filter 3099 in 3094",
	};
	if (!_Put(pCmd07_0, DK_Number(pCmd07_0)))
		return FALSE;

	if ( m_sRouter.port[1].kaisen == eKaisenADSL || ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL )
	{	
		if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
		{
			// ②DKネットワーク（172.16.0.0/16）側Lan2インタフェースで適用する動的フィルタ　（2000番台）
			wsprintf(szPut,"ip filter dynamic 2025 172.16.0.0/16 %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter dynamic 2026 * %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter dynamic 2027 172.16.15.0/24 %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter dynamic 2028 172.16.60.0/24 %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			
			if ( m_dwDnsFlag )	// レジストリ設定ダイアログにて①「DNSサーバ接続」が○だった場合のみ、以下の設定を追加する。
			{
				wsprintf(szPut,"ip filter dynamic 2029 * %s,%s domain", szIp, szIp2);
				if (!_Put(szPut))	return FALSE;
			}
		}
		
		const char *pCmd07[] = {
			"ip filter dynamic 2035 172.16.0.0/16 61.114.182.53 filter 2035 in 2030",
			"ip filter dynamic 2036 172.16.5.0/24 61.114.182.53 filter 2036 in 2031",
			"ip filter dynamic 2037 172.16.15.0/24 61.114.182.53 filter 2037 in 2032",
			"ip filter dynamic 2038 172.16.60.0/24 61.114.182.53 filter 2038 in 2033",
			"ip filter dynamic 2045 172.16.0.0/16 61.114.182.54 filter 2045 in 2040",
			"ip filter dynamic 2046 172.16.5.0/24 61.114.182.54 filter 2046 in 2041",
			"ip filter dynamic 2047 172.16.15.0/24 61.114.182.54 filter 2047 in 2042",
			"ip filter dynamic 2048 172.16.60.0/24 61.114.182.54 filter 2048 in 2043",
			"ip filter dynamic 2055 172.16.0.0/16 61.114.186.0/25 filter 2055 in 2050",
			"ip filter dynamic 2056 172.16.5.0/24 61.114.186.0/25 filter 2056 in 2051",
			"ip filter dynamic 2057 172.16.15.0/24 61.114.186.0/25 filter 2057 in 2052",
			"ip filter dynamic 2058 172.16.60.0/24 61.114.186.0/25 filter 2058 in 2053",
			"ip filter dynamic 2059 172.16.80.0/22 61.114.186.0/25 filter 2059 in 2054",
			"ip filter dynamic 2065 172.16.0.0/16 61.114.177.148 filter 2065 in 2060",
			"ip filter dynamic 2066 172.16.5.0/24 61.114.177.148 filter 2066 in 2061",
			"ip filter dynamic 2067 172.16.15.0/24 61.114.177.148 filter 2067 in 2062",
			"ip filter dynamic 2068 172.16.60.0/24 61.114.177.148 filter 2068 in 2063",
			"ip filter dynamic 2075 172.16.0.0/16 202.229.106.177 filter 2075 in 2070",
			"ip filter dynamic 2076 172.16.5.0/24 202.229.106.177 filter 2076 in 2071",
			"ip filter dynamic 2077 172.16.15.0/24 202.229.106.177 filter 2077 in 2072",
			"ip filter dynamic 2078 172.16.60.0/24 202.229.106.177 filter 2078 in 2073",
			"ip filter dynamic 2079 172.16.80.0/22 202.229.106.177 filter 2079 in 2074",
		};
		if (!_Put(pCmd07, DK_Number(pCmd07)))
			return FALSE;
	}
	
	wsprintf(szPut, "ip filter dynamic 2085 %s * filter 2085 in 2080", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter dynamic 2086 %s * filter 2086 in 2081", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	
	if (!_Put("ip filter dynamic 2087 172.16.15.0/24 * filter 2087 in 2082"))	return FALSE;
	
	if (!_Put("ip filter dynamic 2088 172.16.0.0/16 * filter 2088 in 2083"))	return FALSE;
	
	if (!_Put("ip filter dynamic 2089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 2089 in 2084"))	return FALSE;
	
	if ( m_dwPop3Flag )
	{
		wsprintf(szPut, "ip filter dynamic 2095 %s * filter 2095 in 2090", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	if ( m_dwSmtpFlag )
	{
		wsprintf(szPut, "ip filter dynamic 2096 %s * filter 2096 in 2091", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}

	if ( m_sRouter.sLAN3.fVirusUse )
	{
		wsprintf(szPut, "ip filter dynamic 2444 %s * filter 2444 in 2445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter dynamic 3444 %s * filter 3444 out 3445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut))	return FALSE;
	}

	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed )
	{			
		wsprintf(szPut,"ip filter dynamic 2450 %s 61.114.186.0/25 filter 2450 in 2451", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;	
		wsprintf(szPut,"ip filter dynamic 3450 %s 61.114.186.0/25 filter 3450 out 3451", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;	
	}
	
//===== ↓クラウドIP電話対応  ==============================================================================
	if ( m_sRouter.m_nFqdnState != eFqdnNon )
	{
		char szSt[256], szEd[256];
		wsprintf(szSt, "%s", m_sRouter.m_nFqdnState == eFqdnLAN1 ? m_szFirstAdd : GetIpB2C(m_byLan3Ip_plus2));
		wsprintf(szEd, "%s", m_sRouter.m_nFqdnState == eFqdnLAN1 ? m_szLastAdd  : GetIpB2C(m_byLan3Ip_plus5));
		
		wsprintf(szPut, "ip filter dynamic 101406 %s-%s %s udp", szSt, szEd, strCloudIpTel);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter dynamic 101407 %s-%s * filter 101407 in 101402", szSt, szEd);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter dynamic 101408 %s-%s * domain", szSt, szEd);
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut, "ip filter dynamic 101409 %s-%s * filter 101409 in 101404", szSt, szEd);
		if (!_Put(szPut))	return FALSE;
	}
//===== ↑クラウドIP電話対応  ==============================================================================

	if ( m_sRouter.port[2].kaisen == eKaisenLAN )
	{
		if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse ) // SISLaboかADSLの設定がないなら何も書きこまない
		{
			// ③DKネットワーク（172.16.0.0/16）側Lan3インタフェースで適用する動的フィルタ　（3000番台）
			wsprintf(szPut,"ip filter dynamic 3025 172.16.0.0/16 %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter dynamic 3026 172.16.5.0/24 %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter dynamic 3027 172.16.15.0/24 %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			wsprintf(szPut,"ip filter dynamic 3028 172.16.60.0/24 %s,%s domain", szIp, szIp2);
			if (!_Put(szPut))	return FALSE;
			
			if ( m_dwDnsFlag )	// レジストリ設定ダイアログにて①「DNSサーバ接続」が○だった場合のみ、以下の設定を追加する。
			{
				wsprintf(szPut,"ip filter dynamic 3029 * %s,%s domain", szIp, szIp2);
				if (!_Put(szPut))	return FALSE;
			}
		}
		
		const char *pCmd08[] = {
			"ip filter dynamic 3035 172.16.0.0/16 61.114.182.53 filter 3035 in 3030",
			"ip filter dynamic 3036 172.16.5.0/24 61.114.182.53 filter 3036 in 3031",
			"ip filter dynamic 3037 172.16.15.0/24 61.114.182.53 filter 3037 in 3032",
			"ip filter dynamic 3038 172.16.60.0/24 61.114.182.53 filter 3038 in 3033",
			"ip filter dynamic 3045 172.16.0.0/16 61.114.182.54 filter 3045 in 3040",
			"ip filter dynamic 3046 172.16.5.0/24 61.114.182.54 filter 3046 in 3041",
			"ip filter dynamic 3047 172.16.15.0/24 61.114.182.54 filter 3047 in 3042",
			"ip filter dynamic 3048 172.16.60.0/24 61.114.182.54 filter 3048 in 3043",
			"ip filter dynamic 3055 172.16.0.0/16 61.114.186.0/25 filter 3055 in 3050",
			"ip filter dynamic 3056 172.16.5.0/24 61.114.186.0/25 filter 3056 in 3051",
			"ip filter dynamic 3057 172.16.15.0/24 61.114.186.0/25 filter 3057 in 3052",
			"ip filter dynamic 3058 172.16.60.0/24 61.114.186.0/25 filter 3058 in 3053",
			"ip filter dynamic 3059 172.16.80.0/22 61.114.186.0/25 filter 3059 in 3054",
			"ip filter dynamic 3065 172.16.0.0/16 61.114.177.148 filter 3065 in 3060",
			"ip filter dynamic 3066 172.16.5.0/24 61.114.177.148 filter 3066 in 3061",
			"ip filter dynamic 3067 172.16.15.0/24 61.114.177.148 filter 3067 in 3062",
			"ip filter dynamic 3068 172.16.60.0/24 61.114.177.148 filter 3068 in 3063",
			"ip filter dynamic 3075 172.16.0.0/16 202.229.106.177 filter 3075 in 3070",
			"ip filter dynamic 3076 172.16.5.0/24 202.229.106.177 filter 3076 in 3071",
			"ip filter dynamic 3077 172.16.15.0/24 202.229.106.177 filter 3077 in 3072",
			"ip filter dynamic 3078 172.16.60.0/24 202.229.106.177 filter 3078 in 3073",
			"ip filter dynamic 3079 172.16.80.0/22 202.229.106.177 filter 3079 in 3074",
		};
		if (!_Put(pCmd08, DK_Number(pCmd08)))
			return FALSE;
	}// if ( m_sRouter.port[2].kaisen == eKaisenLAN )
	
	wsprintf(szPut, "ip filter dynamic 3085 %s * filter 3085 in 3080", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	wsprintf(szPut, "ip filter dynamic 3086 %s * filter 3086 in 3081", GetIpB2C(bMaskedIP, m_sRouter.mask));
	if (!_Put(szPut))	return FALSE;
	
	if (!_Put("ip filter dynamic 3087 172.16.15.0/24 * filter 3087 in 3082"))	return FALSE;
	
	if (!_Put("ip filter dynamic 3088 172.16.0.0/16 * filter 3088 in 3083"))	return FALSE;
	
	if (!_Put("ip filter dynamic 3089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 3089 in 3084"))	return FALSE;
 	
	if ( m_dwPop3Flag )
	{
		wsprintf(szPut, "ip filter dynamic 3095 %s * filter 3095 in 3090", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}		
	if ( m_dwSmtpFlag )
	{
		wsprintf(szPut, "ip filter dynamic 3096 %s * filter 3096 in 3091", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut))	return FALSE;
	}
	
	if ( m_sRouter.port[2].kaisen == eKaisenLAN )
	{	
		wsprintf(szPut,"ip filter dynamic 3530 %s 172.16.60.0/24 ftp", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 3550 %s 172.16.60.0/24 filter 3550 in 3555", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
		wsprintf(szPut,"ip filter dynamic 3590 %s 172.16.60.0/24 filter 3590 in 3595", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
	}
	
	if ( m_sRouter.m_nJnetNo > 0 )
	{	
		if (!_Put("ip filter dynamic 10210 172.16.21.1 192.168.210.3 ftp"))
			return FALSE;
		if (!_Put("ip filter dynamic 10211 172.16.21.1 192.168.210.4 ftp"))
			return FALSE;
	}
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed )
	{			
		wsprintf(szPut,"ip filter dynamic 10665 * %s filter 10665 in 10660 10670 out 10675", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
		if (!_Put(szPut))	return FALSE;
	}

	if ( m_sRouter.nLuaPort )
	{
		const char *pCmd01[] = {
			"ip filter dynamic 10615 172.16.60.0/24 172.16.60.0/24 filter 10615 in 10610 10620 out 10625",
// 			"ip filter set xai_tunnel_lan2 in 10600",
			"ip filter set xai_tunnel_lan2 out 10605 10615 dynamic 10615",
		};
		if (!_Put(pCmd01, DK_Number(pCmd01)))
			return FALSE;
	}
	
	// ---------------- LAN1 Filter ----------------
	CString strFilNoIN[] = {"1400", "1401", "1405", "1406", "1410", "1430", "1431", "1432", "1433", "1434", "1435", "1436", "1437", "1438", "1440", "1441"}; 
	CString strFilNoOUT[] ={"1400", "1401", "1405", "1406", "1430", "1431", "1432", "1433", "1434", "1435", "1436", "1437", "1438", "1440", "1441"}; 
	
	CString strFilter = "ip filter set dk_lan1 in";
	for ( int n = 0; n < DK_Number(strFilNoIN); n++ )
	{
		if ( strFilNoIN[n] == "1438" && !m_dwViaRootFlag )
		{
			continue;
		}
		
		if ( (strFilNoIN[n] == "1401" || strFilNoIN[n] == "1406") && m_sRouter.fSetIpTelFlg == eIpTelNon )
		{
			continue;
		}
		
		strFilter = strFilter + " " + strFilNoIN[n];
	}
	
	if (!_Put(strFilter))
		return FALSE;
	
	strFilter = "ip filter set dk_lan1 out";
	for ( n = 0; n < DK_Number(strFilNoOUT); n++ )
	{
		if ( strFilNoOUT[n] == "1438" && !m_dwViaRootFlag )
		{
			continue;
		}
		
		if ( (strFilNoOUT[n] == "1401" || strFilNoOUT[n] == "1406") && m_sRouter.fSetIpTelFlg == eIpTelNon )
		{
			continue;
		}
		
		strFilter = strFilter + " " + strFilNoOUT[n];
	}
	
	if (!_Put(strFilter))
		return FALSE;

	// ---------------- LAN2 Filter ---------------- 
	if (!_Put("ip filter set dk_pp_lan2 in 1 2 3 4 5 6 2010 10800 12600 12700", FALSE))	// 12600 12700＝クラウドリニューアル時対応(サポ端末接続フィルタ追加)
		return FALSE;
	
	CString strPut;
	for ( int nFilNo = 2200; nFilNo < 2400; nFilNo += 100 )
	{
		if ( nFilNo == 2200 || nFilNo == 2300 )
		{
			strPut.Format(" %d", nFilNo);
			if (!_Put((CStoS)strPut, FALSE))
				return FALSE;
		}
		
		for ( i = 0; i < m_nConnectMax; i++ )
		{
			if ( m_sRouter.con[i].no == 0 )
				continue;
			if ( m_sRouter.ncon[i].type == ePHSVPN )
			{
				strPut.Format(" %d", nFilNo + m_sRouter.con[i].no);
				if (!_Put((CStoS)strPut, FALSE))
					return FALSE;
			}
		}
	}
	
	for ( nFilNo = 2600; nFilNo < 2930; nFilNo += 100 )
	{
		if ( nFilNo == 2800 ) 
			continue;
		
		// 接続先番号=auto かつ ポート=L2 の時のみ追加する。
		if ( (m_sRouter.fMent == eMentAuto && ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL) &&
			 (nFilNo == 2600 || nFilNo == 2700 /*|| nFilNo == 2900*/) )
		{
			strPut.Format(" %d", nFilNo);
			if (!_Put((CStoS)strPut, FALSE))
				return FALSE;
		}
		
		for ( i = 0; i < m_nConnectMax; i++ )
		{
			if ( m_sRouter.con[i].no == 0 )
				continue;
			if ( m_sRouter.ncon[i].type == eDKVPN || m_sRouter.ncon[i].type == eDKVPN_DialUp || /**/m_sRouter.ncon[i].type == ePHSVPN ||
				(m_sRouter.ncon[i].type == eFixADSL && m_fMxRMAddRead) || m_sRouter.ncon[i].type == eDeltaNetBk || 
				(m_sRouter.ncon[i].type == eSIS05 && (CString)GetIpB2C(m_sRouter.sSIS05.accessIp) != "0.0.0.0") )
			{
				strPut.Format(" %d", nFilNo + m_sRouter.con[i].no);
				if (!_Put((CStoS)strPut, FALSE))
					return FALSE;
			}
		}
	}
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon )
	{
		if (!_Put(" 10000 10010 10020 10030", FALSE))
			return FALSE;
	}
	if ( m_sRouter.m_nJnetNo > 0 )
	{
		strPut.Format(" %d", 2600 + m_sRouter.m_nJnetNo);
		
		if ( m_sRouter.m_nJnetNo2 > 0 )	// JNET BK
		{
			CString strPut2;
			strPut2.Format(" %d", 2600 + m_sRouter.m_nJnetNo2);
			strPut += strPut2;
		}
		
		strPut += " 10200 10201";
		
		if (!_Put((CStoS)strPut, FALSE))
			return FALSE;
	}

	if ( m_sRouter.nLuaPort )
	{
		if (!_Put(" 10500 10510 10520 10550 10560", FALSE))
			return FALSE;
	}

	if (!_Put(""))
		return FALSE;
//===== ↓クラウドIP電話対応 ==============================================================================
	// ＊＊＊ ip filter set dk_pp_lan2 out ＊＊
	strPut.Format("ip filter set dk_pp_lan2 out 1 2 3 4 5 6 104 105 106 107 2015%s%s%s%s%s%s%s 2085 2086 2087 2088 2089%s%s 2097 2098 2099%s%s%s 10801 12650 12750",	// 12650 12750＝クラウドリニューアル時対応
			/*m_sRouter.sLAN3.fVirusUse*/TRUE		 ? " 2026"			: "",
					m_dwDnsFlag						 ? " 2029"			: "",	// DNSサーバ接続
					DK_BitChk(m_dwSISAddressFlag, 0) ? " 2037 2038"		: "",
					DK_BitChk(m_dwSISAddressFlag, 1) ? " 2047 2048"		: "",
					DK_BitChk(m_dwSISAddressFlag, 2) ? " 2057 2058 2059": "",
					DK_BitChk(m_dwSISAddressFlag, 3) ? " 2067 2068"		: "",
					DK_BitChk(m_dwSISAddressFlag, 2) ? " 2077 2078 2079": "",
					m_dwPop3Flag					 ? " 2095"			: "",	// POP3サーバ接続
					m_dwSmtpFlag					 ? " 2096"			: "",	// SMTPサーバ接続
					m_sRouter.sLAN3.fVirusUse		 ? " 2444"			: "",
					m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed ? " 2450": "",
					m_sRouter.m_nFqdnState != eFqdnNon ? " 101406 101407 101408 101409" : "");	// クラウドIP電話

	
	if (!_Put((CStoS)strPut, FALSE))
		return FALSE;
//===== ↑クラウドIP電話対応  ==============================================================================

	for ( nFilNo = 2250; nFilNo < 2450; nFilNo += 100 )
	{
		// 接続先番号=auto かつ ポート=L2 の時のみ追加する。
		if ( nFilNo == 2250 || nFilNo == 2350 )
		{
			strPut.Format(" %d", nFilNo);
			if (!_Put((CStoS)strPut, FALSE))
				return FALSE;
		}
		
		for ( i = 0; i < m_nConnectMax; i++ )
		{
			if ( m_sRouter.con[i].no == 0 )
				continue;
			if ( m_sRouter.ncon[i].type == ePHSVPN )
			{
				strPut.Format(" %d", nFilNo + m_sRouter.con[i].no);
				if (!_Put((CStoS)strPut, FALSE))
					return FALSE;
			}
		}
	}
	
	for ( nFilNo = 2650; nFilNo < 2980; nFilNo += 100 )
	{
		if ( nFilNo == 2850 )
			continue;

		// 接続先番号=auto かつ ポート=L2 の時のみ追加する。
		if ( (m_sRouter.fMent == eMentAuto && ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL) &&
			 (nFilNo == 2650 || nFilNo == 2750 /*|| nFilNo == 2950*/) )
		{
			strPut.Format(" %d", nFilNo);
			if (!_Put((CStoS)strPut, FALSE))
				return FALSE;
		}
		
		for ( i = 0; i < m_nConnectMax; i++ )
		{
			if ( m_sRouter.con[i].no == 0 )
				continue;
			if ( m_sRouter.ncon[i].type == eDKVPN || m_sRouter.ncon[i].type == eDKVPN_DialUp || /**/m_sRouter.ncon[i].type == ePHSVPN ||
				(m_sRouter.ncon[i].type == eFixADSL && m_fMxRMAddRead) || m_sRouter.ncon[i].type == eDeltaNetBk || 
				(m_sRouter.ncon[i].type == eSIS05 && (CString)GetIpB2C(m_sRouter.sSIS05.accessIp) != "0.0.0.0") )
			{
				strPut.Format(" %d", nFilNo + m_sRouter.con[i].no);
				if (!_Put((CStoS)strPut, FALSE))
					return FALSE;
			}
		}
	}
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon )
	{
		if (!_Put(" 10005 10015 10025 10035", FALSE))
			return FALSE;
	}
	
	if ( m_sRouter.m_nJnetNo > 0 )
	{
		strPut.Format(" %d", 2650 + m_sRouter.m_nJnetNo);
		
		if ( m_sRouter.m_nJnetNo2 > 0 )	// JNET BK
		{
			CString strPut2;
			strPut2.Format(" %d", 2650 + m_sRouter.m_nJnetNo2);
			strPut += strPut2;
		}
		
		strPut += " 10205 10206";
		
		if (!_Put((CStoS)strPut, FALSE))
			return FALSE;
	}
	
	if ( m_sRouter.nLuaPort )
	{
		if (!_Put(" 10505 10525 10575", FALSE))
			return FALSE;
	}

	// ＊＊＊ ip filter set dk_pp_lan2 out dynamic ＊＊＊
	if ( m_dwSISAddressFlag != 0 )
	{
//===== ↓クラウドIP電話対応 ============================================================
		strPut.Format(" dynamic%s%s%s%s%s%s%s 2085 2086 2087 2088 2089%s%s 2097 2098 2099%s%s%s",
				/*m_sRouter.sLAN3.fVirusUse*/TRUE		 ? " 2026"			: "",
						m_dwDnsFlag						 ? " 2029"			: "",
						DK_BitChk(m_dwSISAddressFlag, 0) ? " 2037 2038"		: "",
						DK_BitChk(m_dwSISAddressFlag, 1) ? " 2047 2048"		: "",
						DK_BitChk(m_dwSISAddressFlag, 2) ? " 2057 2058 2059": "",
						DK_BitChk(m_dwSISAddressFlag, 3) ? " 2067 2068"		: "",
						DK_BitChk(m_dwSISAddressFlag, 2) ? " 2077 2078 2079": "",
						m_dwPop3Flag					 ? " 2095"			: "",	// POP3サーバ接続
						m_dwSmtpFlag					 ? " 2096"			: "",	// SMTPサーバ接続
						m_sRouter.sLAN3.fVirusUse		 ? " 2444"			: "",
						m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed ? " 2450": "",
						m_sRouter.m_nFqdnState != eFqdnNon ? " 101406 101407 101408 101409" : "");	// クラウドIP電話
		
		if (!_Put((CStoS)strPut, FALSE))
			return FALSE;
	}	
	if (!_Put(""))
		return FALSE;
	
	// ---------------- LAN3 Filter ----------------
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon || m_sRouter.m_nFqdnState == eFqdnLAN3 )
	{
		strPut.Format("ip filter set dk_lan3_call in 1401 1406%s", (m_sRouter.m_nFqdnState == eFqdnLAN3) ? " 1410" : "");
		if (!_Put((CStoS)strPut))
			return FALSE;
		
		if (!_Put("ip filter set dk_lan3_call out 1401 1406"))
			return FALSE;
	}
//===== ↑クラウドIP電話対応 ============================================================
	
	// ＊＊＊ ip filter set dk_lan3 ＊＊＊
	if (!_Put("ip filter set dk_lan3 in 3 4 3010 3420 3421", FALSE))
		return FALSE;
	
	if ( m_sRouter.sLAN3.fVirusUse )
	{
		if (!_Put(" 104 105 3442 3444 3446 3448 3422 3501 3521 3561 3581", FALSE))
			return FALSE;
	}
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed	|| m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed ||
		 m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed		|| m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed		|| m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed  )
	{
		wsprintf(szPut, "%s%s%s%s%s%s%s%s",
						m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed		? " 3450" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	? " 3500" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	? " 3520" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed		? " 3530" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed		? " 3550" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed			? " 3560" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed			? " 3580" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed		? " 3590" : "");
		if (!_Put(szPut, FALSE))
			return FALSE;
	}
	
	for ( nFilNo = 3600; nFilNo < 3930; nFilNo += 100 )
	{
		if ( nFilNo == 3800 ) {
			if (!_Put(" 3801 3802 3803 3804", FALSE))
				return FALSE;
			continue;
		}
		
		for ( i = 0; i < m_nConnectMax; i++ )
		{
			if ( m_sRouter.con[i].no == 0 )
				continue;
			if ( m_sRouter.ncon[i].type == eKobetuVPN || m_sRouter.ncon[i].type == eKobetuVPN_DialUp )
			{
				strPut.Format(" %d", nFilNo + m_sRouter.con[i].no);
				if (!_Put((CStoS)strPut, FALSE))
					return FALSE;
			}
		}
	}

	if ( m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed )
	{
		if (!_Put(" 10650 10660 10670", FALSE))
			return FALSE;
	}
	
	// ＊＊＊ ip filter set dk_lan3 dynamic ＊＊＊
	if ( m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed /*|| m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed*/)
	{
		wsprintf(szPut, " dynamic%s%s%s%s%s",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55FTP].fNeed	? " 3530" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55DB].fNeed	? " 3550" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3Mobile].fNeed	? " 3590" : "",
						m_sRouter.sLAN3.fVirusUse ?					  " 3444" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed ? ""      : "");	// "3450"削除　2021年秋版対応
		if (!_Put(szPut, FALSE))
			return FALSE;
	}
	else
	{
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			if (!_Put(" dynamic 3444", FALSE))
				return FALSE;
		}
	}
	if (!_Put(""))
		return FALSE;
	
	// ＊＊＊ ip filter set dk_lan3 out ＊＊＊
	if (!_Put("ip filter set dk_lan3 out 3 4 104 105 3015", FALSE))
		return FALSE;
	
	if ( m_sRouter.sLAN3.fVirusUse )
	{
		if (!_Put(" 3026 3427 3442 3443 3447 3449 3506 3526 3566 3586", FALSE))
			return FALSE;
	}

	if ( (m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed)
		 && m_dwSISAddressFlag != 0 )
	{
		wsprintf(szPut, "%s%s%s%s%s",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed		&& DK_BitChk(m_dwSISAddressFlag, 0) ? " 3037 3038"		: "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed	&& DK_BitChk(m_dwSISAddressFlag, 1) ? " 3047 3048"		: "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed		&& DK_BitChk(m_dwSISAddressFlag, 2) ? " 3057 3058 3059" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed	&& DK_BitChk(m_dwSISAddressFlag, 3) ? " 3067 3068"		: "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed		&& DK_BitChk(m_dwSISAddressFlag, 2) ? " 3077 3078 3079" : "");
		if (!_Put(szPut, FALSE))
			return FALSE;
	}
	
	if (!_Put(" 3425 3426", FALSE))
		return FALSE;
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed )
	{
		wsprintf(szPut, "%s%s%s%s",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	? " 3505" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3MS55Folder].fNeed	? " 3525" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed			? " 3565" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3CSV].fNeed			? " 3585" : "");
		if (!_Put(szPut, FALSE))
			return FALSE;
	}
	
	for ( nFilNo = 3650; nFilNo < 3980; nFilNo += 100 )
	{
		if ( nFilNo == 3850 ) {
			if (!_Put(" 3806 3807 3808 3809", FALSE))
				return FALSE;
			continue;
		}
		
		for ( i = 0; i < m_nConnectMax; i++ )
		{
			if ( m_sRouter.con[i].no == 0 )
				continue;
			if ( m_sRouter.ncon[i].type == eKobetuVPN || m_sRouter.ncon[i].type == eKobetuVPN_DialUp )
			{
				strPut.Format(" %d", nFilNo + m_sRouter.con[i].no);
				if (!_Put((CStoS)strPut, FALSE))
					return FALSE;
			}
		}
	}

	if ( m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed )
	{
		if (!_Put(" 3451 10655 10665 10675", FALSE))	// "3451"追加　2021年秋版対応
			return FALSE;
	}
	
	// ＊＊＊ ip filter set dk_lan3 out dynamic ＊＊＊
	if ( (m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed || m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed) && m_dwSISAddressFlag != 0 /*|| m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed*/ )
	{
		wsprintf(szPut, " dynamic%s%s%s%s%s%s%s",
						m_sRouter.sLAN3.fVirusUse ? " 3026"	: "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed		&& DK_BitChk(m_dwSISAddressFlag, 0) ? " 3037 3038"		: "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed	&& DK_BitChk(m_dwSISAddressFlag, 1) ? " 3047 3048"		: "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed		&& DK_BitChk(m_dwSISAddressFlag, 2) ? " 3057 3058 3059" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLaboSign].fNeed	&& DK_BitChk(m_dwSISAddressFlag, 3) ? " 3067 3068"		: "",
						m_sRouter.sLAN3.sConNaiyo[eL3SisLabo].fNeed		&& DK_BitChk(m_dwSISAddressFlag, 2) ? " 3077 3078 3079" : "",
						m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed ? "" : "");	// "3450"削除　2021年秋版対応
		if (!_Put(szPut, FALSE))
			return FALSE;
	}
	else
	{
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			if (!_Put(" dynamic 3026", FALSE))
				return FALSE;
		}
	}
	
	if (!_Put(""))
		return FALSE;
	
	if (!_Put("ip filter set dk_pp_lab in 9"))
		return FALSE;
	
	// ④DKネットワーク（172.16.0.0/16）側PPインタフェースで適用する定型フィルタ
	// ⑧syslogを取得するためのフルパス定型フィルタ	
	if ( !_Put("ip filter set dk_pp_lab out 1 2 3 4 5 6 104 105 15", FALSE) )
		return FALSE;
	
	strPut.Format("%s%s%s%s%s%s dynamic%s%s%s%s%s%s",
					(m_sRouter.sLAN3.fVirusUse)		 ? " 26"		: "",
					DK_BitChk(m_dwSISAddressFlag, 0) ? " 37 38"		: "",
					DK_BitChk(m_dwSISAddressFlag, 1) ? " 47 48"		: "",
					DK_BitChk(m_dwSISAddressFlag, 2) ? " 57 58 59"  : "",
					DK_BitChk(m_dwSISAddressFlag, 3) ? " 67 68"		: "",
					DK_BitChk(m_dwSISAddressFlag, 2) ? " 77 78 79"  : "",
					(m_sRouter.sLAN3.fVirusUse)		 ? " 26"		: "",
					DK_BitChk(m_dwSISAddressFlag, 0) ? " 37 38"		: "",
					DK_BitChk(m_dwSISAddressFlag, 1) ? " 47 48"		: "",
					DK_BitChk(m_dwSISAddressFlag, 2) ? " 57 58 59"  : "",
					DK_BitChk(m_dwSISAddressFlag, 3) ? " 67 68"		: "",
					DK_BitChk(m_dwSISAddressFlag, 2) ? " 77 78 79"  : "");
	if (!_Put((CStoS)strPut, FALSE))
		return FALSE;
	
	if ( !_Put("") )
		return FALSE;
	
	const char *pCmd13[] = {
		"ip filter set getlog in 1 2 3 4 5 6 100",
		"ip filter set getlog out 1 2 3 4 5 6 100"
	};
	if (!_Put(pCmd13, DK_Number(pCmd13)))
		return FALSE;
	
	if ( m_sRouter.sLAN3.fVirusUse )// # URL filter configuration
	{
		if (!_Put("# URL filter configuration"))
			return FALSE;
		
		const char *pCmd14[] = {
			"url filter 1 pass download.windowsupdate.com",
			"url filter 2 pass update.microsoft.com",
			"url filter 3 pass windowsupdate.com",
			"url filter 4 pass download.microsoft.com",
			"url filter 5 pass ntservicepack.microsoft.com",
			"url filter 6 pass wustat.windows.com",
			"url filter 7 pass trendmicro.co.jp",
			"url filter 8 pass trendmicro.co.com",
		};
		if (!_Put(pCmd14, DK_Number(pCmd14)))
			return FALSE;
		
		wsprintf(szPut, "url filter 9 reject * *");
		if ( !_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 10 pass * %s", GetIpB2C(bMaskedIP, m_sRouter.mask));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 21 pass download.windowsupdate.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 22 pass update.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 23 pass windowsupdate.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 24 pass download.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 25 pass ntservicepack.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 26 pass wustat.windows.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 27 pass trendmicro.co.jp %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 28 pass trendmicro.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szPut) )
			return FALSE;
		wsprintf(szPut, "url filter 100 pass * *");
		if (!_Put(szPut) )
			return FALSE;
	}
	
	return TRUE;
}

//	LAN/IP configuration
BOOL CDkRouter1200::PutLanIp(char *tit)
{
	char	szBuf[256];
	
	if (!_PutTit(tit))
		return FALSE;
	
	//===== ↓LAN1の設定 ==================================================================
	//	ip lan1 address
	wsprintf(szBuf, "ip lan1 address %s", GetIpB2C(m_sRouter.ip, m_sRouter.mask));
	if (!_Put(szBuf))
		return FALSE;
	// IPアドレス②が設定されている場合
	if ( m_sRouter.fSetIpTelFlg == eIpTelLAN1 )
	{
		wsprintf(szBuf, "ip lan1 secondary address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
		if (!_Put(szBuf))
			return FALSE;
	}
	const char *pCmd01[] = {
		"rip use on",
		"ip lan1 rip send on version 2",
		"ip lan1 rip receive on version 2",
		"ip lan1 proxyarp on",
	};
	if ( !_Put(pCmd01,DK_Number(pCmd01)) )
		return FALSE;
	
	//	ip lan1 secure filter
	wsprintf(szBuf, "ip lan1 secure filter name %s", m_fOA_LAN ? "oa_lan1" : "dk_lan1");
	if (!_PutFilterName(szBuf))
		return FALSE;
	if (!_Put("ip lan1 intrusion detection in on reject=on"))
		return FALSE;
	
	if ( m_sRouter.sLAN3.fVirusUse )
	{
		if (!_Put("ip lan1 nat descriptor 1000"))
			return FALSE;
	}
	
	if ( m_sRouter.m_nMSS > 0 )
	{
		wsprintf(szBuf, "ip lan1 tcp mss limit %d", m_sRouter.m_nMSS);
		if (!_Put(szBuf))
			return FALSE;
	}
	//===== ↑LAN1の設定 ==================================================================
	
	//===== ↓LAN2の設定 ==================================================================
	const char *pCmd02[] = {
		"ip lan2 rip send off",
		"ip lan2 rip receive off",
		"ip lan2 proxyarp off",
		"ip lan2 intrusion detection in on reject=on",	
	};
	if ( !_Put(pCmd02,DK_Number(pCmd02)) )
		return FALSE;
//===== ↓ケーブル光対応 ==================================================================
	if ( m_sRouter.port[1].kaisen == eKaisenCableHikari  )
	{
		if (!_Put("ip lan2 nat descriptor 2000"))
			return FALSE;
		if (!_Put("ip lan2 secure filter name dk_pp_lan2"))
			return FALSE;
		
//===== ↓DHCP対応 ====================================================================
		if ( m_sRouter.fUseLan2DHCP )
		{
			if (!_Put("ip lan2 address dhcp"))
				return FALSE;
		}
//===== ↑DHCP対応 ====================================================================
		else
		{
			SDkConnectInfoNew *pInfo = GetAdslInfo();
			wsprintf(szBuf, "ip lan2 address %s", GetIpB2C(pInfo->fixIp, pInfo->yobi1));
			if (!_Put(szBuf))
				return FALSE;
		}
	}
//===== ↑ケーブル光対応 ==================================================================
	//===== ↑LAN2の設定 ==================================================================
	
	//===== ↓LAN3の設定 ==================================================================
	// ルータがRTX1200限定でLAN3接続する場合
	if ( m_sRouter.kind == eRTX1200 && (m_sRouter.port[2].kaisen > eKaisenBlank || m_sRouter.m_nFqdnState == eFqdnLAN3) )
	{
		if (!_Put("lan type lan3 10-fdx"))
			return FALSE;
	}
	
	// 回線種別に個別LANが指定されている場合
	if ( m_sRouter.port[2].kaisen == eKaisenLAN )
	{
		wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask));
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "ip lan3 secure filter name %s", m_fOA_LAN ? "oa_lan3" : "dk_lan3");
		if (!_PutFilterName(szBuf))
			return FALSE;
		wsprintf(szBuf, "ip lan3 nat descriptor %d", m_fOA_LAN ? 8000 : 3000);
		if (!_Put(szBuf))
			return FALSE;
	}
	// 回線種別にIP電話が指定されている場合
	else if ( (m_sRouter.port[2].kaisen == eKaisenIP_TEL && m_sRouter.fSetIpTelFlg == eIpTelLAN3) || m_sRouter.m_nFqdnState == eFqdnLAN3 )
	{
		if ( m_sRouter.port[2].kaisen == eKaisenIP_TEL && m_sRouter.fSetIpTelFlg == eIpTelLAN3 )	// 旧IP電話(LAN3)で設定されたIPアドレスが優先となります
		{
			wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
			if (!_Put(szBuf))
				return FALSE;
		}
		else
		{
			wsprintf(szBuf, "ip lan3 address %s/29", GetIpB2C(m_sRouter.ip_2_cloud));
			if (!_Put(szBuf))
				return FALSE;
		}		
		
		wsprintf(szBuf, "ip lan3 secure filter name dk_lan3_call");
		if (!_PutFilterName(szBuf))
			return FALSE;
	}
	
	const char *pCmd03[] = {
		"ip lan3 rip send off",
		"ip lan3 rip receive off",
		"ip lan3 proxyarp off",
		"ip lan3 intrusion detection in on reject=on"
	};
	if ( !_Put(pCmd03,DK_Number(pCmd03)) )
		return FALSE;
	
	//===== ↑LAN3の設定 ==================================================================
	
	return TRUE;
}

//	ip-route Put
BOOL CDkRouter1200::PutIpRoute()
{
	// メンテナンス設定でリモートVPNが選択されている場合はDefaultRootFlgは強制的に１になる(06/06/05 DK柳川様の提案)
	DWORD dwDefRootFlg = m_dwDefRootFlag;
	if ( m_sRouter.fMent == eMentRemoteVpn )
		dwDefRootFlg = 1;
	else
		dwDefRootFlg = m_dwDefRootFlag;
	
	// ip route default gateway二重書き込み防止フラグ
	// (書き込みが2つあると最後に書いた方のコマンドで上書きされるため)
	// ※ip route default gateway句は、MIRAIGATE閲覧BBかINS用が最優先で書き込むこと 15/05/07 根木さん
	BOOL	fWriteDefault			= FALSE;
	// 各種固定IPの二重書き込み防止フラグ
	BOOL	fWriteFixPHSaddress		= FALSE;
	BOOL	fWriteFixMainRTaddress	= FALSE;
	BOOL	fWriteFixBkRTaddress	= FALSE;
	
	char	szBuf[256];
	
	if (!_Put(""))
		return FALSE;
	
	if (!_Put("ip routing process normal"))
		return FALSE;
	
	if (!_Put("ip route 172.18.0.0/16 gateway 172.16.5.11"))
		return FALSE;
	
//===== ↓ クラウドへのルーティングの設定追加(リフト移行後環境が前提) ==============================================================================
	if ( m_sRouter.sSIS05.no > 0 && m_sIniFileInf.nStatus == SDkKanshiSrvInfo::eNonMode )
	{
		CString str, strPut;
		CString	sCr = "\r\n";
		strPut.Empty();
		str.Format("ip route 10.142.65.0/24 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
		str.Format("ip route 10.142.165.0/24 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
		str.Format("ip route 10.145.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
		str.Format("ip route 10.141.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str;
		
		if (!_Put((CStoS)strPut))
			return FALSE;
	}
//===== ↑ クラウドへのルーティングの設定追加(リフト移行後環境が前提) ==============================================================================
	
	char *pCmd[] = {
		"61.114.182.53",
		"61.114.182.54",
		"61.114.186.0/25",
		"61.114.177.148",
	};
//===== ↓ケーブル光対応 ==================================================================
	if ( m_sRouter.port[1].kaisen == eKaisenCableHikari )	// L2にケーブル光が設定された場合は、最優先でip route default gateway [ゲートウェイIPアドレス]の書き込みをお願いします。
	{
		wsprintf(szBuf, "ip route default gateway pp %d", m_sRouter.m_nAdslNo);
		if (!_Put(szBuf))
			return FALSE;
		fWriteDefault = TRUE;
	}
//===== ↑ケーブル光対応 ==================================================================

	switch ( m_sRouter.fBbUse )	// MIRAIGATE閲覧-BBについて
	{
	case eLaboBB_NoUse:	// なにも選択していない
		{
			int nLaboNo = m_sRouter.m_nVWNo;
			
			if ( m_sRouter.m_nVWNo )  // INS接続のみ選択されてる
			{
				for ( int nBit = 0; nBit < DK_Number(pCmd); nBit++ )
				{
					if ( DK_BitChk(m_dwSISAddressFlag, nBit) ) 
					{
						wsprintf(szBuf, "ip route %s gateway pp %d metric 2", pCmd[nBit], nLaboNo);
						if (!_Put(szBuf))
							return FALSE;
					}
				}
				
				if ( dwDefRootFlg == 1 && !fWriteDefault )
				{
					wsprintf(szBuf, "ip route default gateway pp %d metric 2", nLaboNo);
					if (!_Put(szBuf))
						return FALSE;
					fWriteDefault = TRUE;
				}
				
				if ( DK_BitChk(m_dwSISAddressFlag, 2) ) 
				{
					wsprintf(szBuf, "ip route %s gateway pp %d metric 2", KEIHIN_MOLE, nLaboNo);
					if (!_Put(szBuf))
						return FALSE;
				}
			}
			
			break;
		}
	case eLaboBB_Adsl:	// ADSL
		{
			int nLaboNo = m_sRouter.m_nAdslNo;
			
			for ( int nBit = 0; nBit < DK_Number(pCmd); nBit++ )
			{
				if ( DK_BitChk(m_dwSISAddressFlag, nBit) ) 
				{
					wsprintf(szBuf, "ip route %s gateway pp %d metric 2", pCmd[nBit], nLaboNo);
					if (!_Put(szBuf))
						return FALSE;
				}
			}
			
			if ( dwDefRootFlg == 1 && !fWriteDefault )
			{
				wsprintf(szBuf, "ip route default gateway pp %d metric 2", nLaboNo);
				if (!_Put(szBuf))
					return FALSE;
				fWriteDefault = TRUE;
			}
			
			if ( DK_BitChk(m_dwSISAddressFlag, 2) ) 
			{
				wsprintf(szBuf, "ip route %s gateway pp %d metric 2", KEIHIN_MOLE, nLaboNo);
				if (!_Put(szBuf))
					return FALSE;
			}
			
			break;
		}
	case eLaboBB_FixAdsl:	// ADSL固定
		{
			//===== ↓ケーブル光対応 ==================================================================
			if ( m_sRouter.port[1].kaisen == eKaisenCableHikari )	// L2にケーブル光が設定された場合は、最優先でip route default gateway [ゲートウェイIPアドレス]の書き込みをお願いします。
			{
				for ( int nBit = 0; nBit < DK_Number(pCmd); nBit++ )
				{
					if ( DK_BitChk(m_dwSISAddressFlag, nBit) ) 
					{
						wsprintf(szBuf, "ip route %s gateway pp %d", pCmd[nBit], m_sRouter.m_nAdslNo);
						if (!_Put(szBuf))
							return FALSE;
					}
				}
				
				if ( DK_BitChk(m_dwSISAddressFlag, 2) ) 
				{
					wsprintf(szBuf, "ip route %s gateway pp %d", KEIHIN_MOLE, m_sRouter.m_nAdslNo);
					if (!_Put(szBuf))
						return FALSE;
				}
			}
			//===== ↑ケーブル光対応 ==================================================================
			else
			{
				if ( m_sRouter.m_nVWNo )  // ADSL固定が選択されていて、INS接続先も選択されている
				{
					for ( int nBit = 0; nBit < DK_Number(pCmd); nBit++ )
					{
						if ( DK_BitChk(m_dwSISAddressFlag, nBit) ) 
						{
							wsprintf(szBuf, "ip route %s gateway pp %d weight 999999999 hide gateway pp %d weight 1",  pCmd[nBit], m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo);
							
							if (!_Put(szBuf))
								return FALSE;
						}
					}
				} else {
					for ( int nBit = 0; nBit < DK_Number(pCmd); nBit++ )
					{
						if ( DK_BitChk(m_dwSISAddressFlag, nBit) ) 
						{
							wsprintf(szBuf,"ip route %s gateway pp %d metric 2", pCmd[nBit], m_sRouter.m_nAdslNo);
							if (!_Put(szBuf))
								return FALSE;
						}
					}
				}
				
				if ( dwDefRootFlg == 1 && !fWriteDefault )
				{
					wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
					if (!_Put(szBuf))
						return FALSE;
					fWriteDefault = TRUE;
				}
				
				if ( DK_BitChk(m_dwSISAddressFlag, 2) ) 
				{
					wsprintf(szBuf, "ip route %s gateway pp %d metric 2", KEIHIN_MOLE, m_sRouter.m_nAdslNo);
					if (!_Put(szBuf))
						return FALSE;
				}
			}
			break;
		}
	case eLaboBB_KobetuLan:	// 個別LAN
		{
			if ( m_sRouter.m_nVWNo )  // 個別LANが選択されていて、INS接続先も選択されている
			{
				for ( int nBit = 0; nBit < DK_Number(pCmd); nBit++ )
				{
					if ( DK_BitChk(m_dwSISAddressFlag, nBit) ) 
					{
						wsprintf(szBuf,"ip route %s gateway %s weight 999999999 hide gateway pp %d weight 1", 
										pCmd[nBit], GetIpB2C(m_sRouter.sLAN3.ipGateWay), m_sRouter.m_nVWNo);						
						if (!_Put(szBuf))
							return FALSE;
					}
				}
			} else {
				for ( int nBit = 0; nBit < DK_Number(pCmd); nBit++ )
				{
					if ( DK_BitChk(m_dwSISAddressFlag, nBit) ) 
					{
						wsprintf(szBuf,"ip route %s gateway %s", pCmd[nBit], GetIpB2C(m_sRouter.sLAN3.ipGateWay));						
						if (!_Put(szBuf))
							return FALSE;
					}
				}
			}
			
			if ( dwDefRootFlg == 1 && !fWriteDefault )
			{
				wsprintf(szBuf, "ip route default gateway %s", GetIpB2C(m_sRouter.sLAN3.ipGateWay));
				if (!_Put(szBuf))
					return FALSE;
				fWriteDefault = TRUE;
			}
			
			if ( DK_BitChk(m_dwSISAddressFlag, 2) ) 
			{
				wsprintf(szBuf, "ip route %s gateway %s", KEIHIN_MOLE, GetIpB2C(m_sRouter.sLAN3.ipGateWay));
				if (!_Put(szBuf))
					return FALSE;
			}
			
			break;
		}
	}
	
	//	ip route put
	for ( int n = 0; n < SETU_MAX; n++ )
	{
//===== ↓クラウドリニューアル対応 ========================================================
		if ( m_sRouter.con[n].no == 0 || m_sRouter.ncon[n].type == eNoConnect || m_sRouter.ncon[n].type == eNoConnectSub )
//===== ↑クラウドリニューアル対応 ========================================================
			continue;
		
		// IP電話の設定がある場合は書き込む
		if ( m_sRouter.m_nIpTelNo == m_sRouter.con[n].no && m_sRouter.fSetIpTelFlg > eIpTelNon ) 
		{
			if ( m_sRouter.m_nIpTelNoBk > 0 )
				wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d  keepalive %d gateway tunnel %d weight 0", m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNoBk);
			else
				wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d", m_sRouter.m_nIpTelNo);
			
			if (!_Put(szBuf))
				return FALSE;
			
			wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.m_ptcSvIp), m_sRouter.m_nIpTelNo);
			if (!_Put(szBuf))
				return FALSE;
			wsprintf(szBuf, "ip keepalive %d icmp-echo 60 3 %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.m_ptcSvIp));
			if (!_Put(szBuf))
				return FALSE;
			
//===== ↓2015/05/07 9:34:29   ==============================================================================
			if ( !fWriteFixMainRTaddress )
			{
				wsprintf(szBuf, "ip route 210.227.65.146 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				fWriteFixMainRTaddress = TRUE;
			}
			if ( !fWriteFixBkRTaddress )
			{
				wsprintf(szBuf, "ip route 122.103.88.162 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				fWriteFixBkRTaddress = TRUE;
			}
//===== ↑2015/05/07 9:34:29   ==============================================================================

			DebDisp("IP電話のtunnel番号=%dで書き込み", m_sRouter.m_nIpTelNo);
		}
		if ( m_sRouter.m_nIpTelNoBk == m_sRouter.con[n].no && m_sRouter.fSetIpTelFlg > eIpTelNon && m_sRouter.m_nIpTelNo == 0 ) 
		{
			wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d", m_sRouter.m_nIpTelNoBk);
			if (!_Put(szBuf))
				return FALSE;
			
//===== ↓2015/05/07 9:34:29   ==============================================================================
			if ( !fWriteFixBkRTaddress )
			{
				wsprintf(szBuf, "ip route 122.103.88.162 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				fWriteFixBkRTaddress = TRUE;
			}
//===== ↑2015/05/07 9:34:29   ==============================================================================
			
			DebDisp("IP電話BKのtunnel番号=%dで書き込み", m_sRouter.m_nIpTelNoBk);
		}
		// 車両管理メンテがある場合は書き込む
		if ( m_sRouter.m_nCarMentNo == m_sRouter.con[n].no ) 
		{
			wsprintf(szBuf, "ip route 10.0.240.0/24 gateway tunnel %d", m_sRouter.m_nCarMentNo);
			if (!_Put(szBuf))
				return FALSE;

			DebDisp("車両管理メンテのtunnel番号=%dで書き込み", m_sRouter.m_nCarMentNo);
//===== ↓2015/05/07 9:15:27 　 ==============================================================================
			// ADSL固定かつ「ファン動向Viewer-PCメンテナンス用設定」で接続先番号が指定された場合、以下の設定を追加する。
			if ( ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL && !fWriteDefault )
			{	// ※レジストリ[DefaultRootFlag]に関係なく、強制的にデフォルトルートを[ADSL固定IP接続の接続先番号]へ向ける。
				wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				fWriteDefault = TRUE;
			}
//===== ↑2015/05/07 9:15:27 　 ==============================================================================
		}
		if ( m_sRouter.m_nSISNo == m_sRouter.con[n].no ) 											//	SISの時
		{
			wsprintf(szBuf, "ip route %s gateway %s %d", SIS_IP, 
				(m_sRouter.con[n].port != ePortB1) ? "tunnel" : "pp", m_sRouter.con[n].no);
			if (!_Put(szBuf))	// VPNの接続先が選択されている場合があるのでここで一旦書き込んでおく
				return FALSE;
		}
		
		if ( m_sRouter.m_nRoboNo == m_sRouter.con[n].no )											//	Roboの時
		{
			wsprintf(szBuf, "ip route %s gateway pp %d metric 2", ROBO_IP, m_sRouter.con[n].no);
			if (!_Put(szBuf))
				return FALSE;
		}
		
		if ( m_sRouter.m_nDeltaBkNo == m_sRouter.con[n].no )										// DELTANETバックアップの時
		{
			wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.con[n].ip), m_sRouter.m_nAdslNo);
			if (!_Put(szBuf))
				return FALSE;
		}
		
		if ( m_sRouter.sSIS05.no == m_sRouter.con[n].no )											//  SIS05の時
		{
			if ( m_sRouter.con[n].port == ePortL2 || m_sRouter.con[n].port == ePortL2B1 ||
				 m_sRouter.con[n].port == ePortL3 || m_sRouter.con[n].port == ePortL3B1 )
			{
				wsprintf(szBuf, "ip route %s gateway tunnel %d", SIS05_IP, m_sRouter.sSIS05.no);
				if (!_Put(szBuf))
					return FALSE;
				
				if ( m_sRouter.con[n].port == ePortL2 || m_sRouter.con[n].port == ePortL2B1 ) {
					wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.sSIS05.accessIp), m_sRouter.m_nAdslNo);
					if (!_Put(szBuf))	// VPNの接続先が選択されている場合があるのでここで一旦書き込んでおく
						return FALSE;	
				}
				
				wsprintf(szBuf, "ip route %s gateway tunnel %d", VUP_IP, m_sRouter.sSIS05.no);
				if (!_Put(szBuf))
					return FALSE;
			}
			else
			{				
				wsprintf(szBuf, "ip route %s gateway pp %d metric 2", SIS05_IP, m_sRouter.sSIS05.no);
				if (!_Put(szBuf))
					return FALSE;
				
				wsprintf(szBuf, "ip route %s gateway pp %d metric 2", VUP_IP, m_sRouter.sSIS05.no);
				if (!_Put(szBuf))
					return FALSE;
			}
		}
		
		if ( m_sRouter.ncon[n].type == eDKVPN || m_sRouter.ncon[n].type == eDKVPN_DialUp )			//	DKVPN
		{
			if ( m_fSendFile )
			{
				wsprintf(szBuf, "ip route %s gateway tunnel %d",GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
				if (!_Put(szBuf))
					return FALSE;
				wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
			} else {
				wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				wsprintf(szBuf, "ip route %s gateway tunnel %d",GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
				if (!_Put(szBuf))
					return FALSE;
			}
		}
		
		if ( m_sRouter.ncon[n].type == eKobetuVPN || m_sRouter.ncon[n].type == eKobetuVPN_DialUp ) //	個別VPN			
		{	
			if ( m_fSendFile )
			{
				wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
				if (!_Put(szBuf))
					return FALSE;
				wsprintf(szBuf, "ip route %s gateway %s", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));				
				if (!_Put(szBuf))
					return FALSE;
			} else {
				wsprintf(szBuf, "ip route %s gateway %s", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));				
				if (!_Put(szBuf))
					return FALSE;
				wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
				if (!_Put(szBuf))
					return FALSE;
			}
		}
		
		if ( m_sRouter.ncon[n].type == eJnetMain ) // J-NET
		{
//===== ↓2015/05/07 9:34:29   ==============================================================================
			if ( !fWriteFixMainRTaddress )
			{
				wsprintf(szBuf, "ip route 210.227.65.146 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				fWriteFixMainRTaddress = TRUE;
			}

			if ( !fWriteFixBkRTaddress )
			{
				wsprintf(szBuf, "ip route 122.103.88.162 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				fWriteFixBkRTaddress = TRUE;
			}
//===== ↑2015/05/07 9:34:29   ==============================================================================
			// 192.168.210.3　も　192.168.210.4　も　tunnel13　の先にあります。
			// tunnel13、tunnel23が切替わった際にルーティングも切替わります。
			wsprintf(szBuf, "ip route 192.168.210.3 gateway tunnel %d", m_sRouter.m_nJnetNo);
			if (!_Put(szBuf))
				return FALSE;
			wsprintf(szBuf, "ip route 192.168.210.4 gateway tunnel %d", m_sRouter.m_nJnetNo);
			if (!_Put(szBuf))
				return FALSE;
		}

//===== ↓2015/05/07 9:21:07   ==============================================================================
		// 接続形態として「PHSVPN」が指定された場合、以下の設定を追加する。（固定IPアドレス=26.26.26.26）
		if ( ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL && m_sRouter.ncon[n].type == ePHSVPN && !fWriteFixPHSaddress )
		{
			wsprintf(szBuf, "ip route 26.26.26.26 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
			if (!_Put(szBuf))
				return FALSE;
			
			fWriteFixPHSaddress = TRUE;
		}
//===== ↑2015/05/07 9:21:07   ==============================================================================
//===== ↓2015/05/07 11:24:25   ↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
		if ( m_sRouter.sLAN3.fVirusUse && !fWriteDefault )
		{	// ※レジストリ[DefaultRootFlag]に関係なく、強制的にデフォルトルートを[ADSL固定IP接続の接続先番号]へ向ける。
			wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
			if (!_Put(szBuf))
				return FALSE;
			fWriteDefault = TRUE;
		}
//===== ↑2015/05/07 11:24:25   ↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑		
		//  その他
		// 普通の接続先(PHSは除外)
		if ( m_sRouter.ncon[n].type >= ePHS1 && m_sRouter.ncon[n].type <= ePHSfree ||
			 m_sRouter.m_nAdslNo == m_sRouter.con[n].no		|| 
			 m_sRouter.m_nVWNo	 == m_sRouter.con[n].no		||
			 m_sRouter.m_nSISNo  == m_sRouter.con[n].no		||
			 m_sRouter.m_nRoboNo == m_sRouter.con[n].no		||
			 m_sRouter.sSIS05.no == m_sRouter.con[n].no		||
			 m_sRouter.m_nDeltaBkNo == m_sRouter.con[n].no	||
			 m_sRouter.m_nIpTelNo == m_sRouter.con[n].no	||
			 m_sRouter.m_nIpTelNoBk == m_sRouter.con[n].no	||
			 m_sRouter.m_nCarMentNo == m_sRouter.con[n].no	||
			 m_sRouter.ncon[n].type == ePHSVPN				||
			 m_sRouter.ncon[n].type == eDKVPN				|| m_sRouter.ncon[n].type == eDKVPN_DialUp || 
			 m_sRouter.ncon[n].type == eKobetuVPN			|| m_sRouter.ncon[n].type == eKobetuVPN_DialUp ||
			 m_sRouter.ncon[n].type == eJnetMain			|| m_sRouter.ncon[n].type == eJnetSub)
		{
			
		} else {
			wsprintf(szBuf, "ip route %s gateway pp %d metric 2",
							GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.ncon[n].no);
			if (!_Put(szBuf))
				return FALSE;
		}
	}	// End For
	
	// LAN3信頼できるネットワーク
	for ( int i = 0; i < DK_Number(m_sRouter.sLAN3.sSinraiCon); i++ )
	{
		if ( m_sRouter.sLAN3.sSinraiCon[i].no == 0 )
			continue;
		
		wsprintf(szBuf, "ip route %s gateway %s", 
						GetIpB2C(m_sRouter.sLAN3.sSinraiCon[i].ip, m_sRouter.sLAN3.sSinraiCon[i].mask), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));
		if ( !_Put(szBuf) )
			return FALSE;
	}
	
	if ( m_fOA_LAN )
	{
		for ( int nIp = m_sPrm.nLuaTunnelMin; nIp <= m_sPrm.nLuaTunnelMax; nIp++ )
		{
			wsprintf(szBuf, "ip route %d.%d.%d.%d gateway pp anonymous metric 2", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], nIp);
			if (!_Put(szBuf))
				return FALSE;
		}
		
		if ( !_Put("ip icmp parameter-problem send off") )
			return FALSE;
		
		return TRUE;	// RTX1000-OAの場合はこれでおわり
	}
	
	// ---------- これ以降はDK_LANのみの設定 ------------------------------------------------------------------
	switch ( m_sRouter.fMent )  // メンテ用設定が何か設定してあるばあいの時だけ
	{
	case eMentAnonymous:	// anonymous
		{
			const char *pCmd[] = {
				"ip route 10.0.192.0/20 gateway pp anonymous metric 2",	// リモートメンテナンス時は、本ネットワークアドレスで接続する。
				"ip route 10.0.208.0/20 gateway pp anonymous metric 2"
			};
			if ( !_Put(pCmd, DK_Number(pCmd)) )
				return FALSE;
			break;
		}
		break;
	case eMentFixADSL:		// ADSL固定
	case eMentRemoteVpn:	// リモートVPN
		{
			// リモートメンテナンス時は、本ネットワークアドレスで接続する。
			if ( !_Put("ip route 10.0.192.0/20 gateway pp anonymous metric 2") )
				return FALSE;
			wsprintf(szBuf, "ip route 10.0.208.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
			if ( !_Put(szBuf) )
				return FALSE;
			wsprintf(szBuf, "ip route 10.0.224.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
			if ( !_Put(szBuf) )
				return FALSE;
			
			if ( m_fMxRMAddRead )	// ちゃんと入ってるなら
			{
				wsprintf(szBuf, "ip route %s gateway pp %d", m_szRMAdd, m_sRouter.m_nAdslNo);
				if ( !_Put(szBuf) )
					return FALSE;
			}
		}
		break;
	case eMentAuto:	// auto
		{
			// ADSL固定IP接続時、[接続先番号]＝autoの場合は、[DefaultRootFlag]に関係なく、強制的にデフォルトルートを[ADSL接続先番号]へ向ける。
			if ( ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL )
			{
				if ( !fWriteDefault )
				{
					wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
					if (!_Put(szBuf))
						return FALSE;
					fWriteDefault = TRUE;
				}
				
				if (!_Put("ip route 10.0.192.0/24 gateway pp anonymous metric 2"))
					return FALSE;
				
				wsprintf(szBuf, "ip route 10.0.208.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
				wsprintf(szBuf, "ip route 10.0.224.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo);
				if (!_Put(szBuf))
					return FALSE;
			}
			else
			{
				const char *pCmd[] = {
					"ip route 10.0.192.0/24 gateway pp anonymous metric 2",	// リモートメンテナンス時は、本ネットワークアドレスで接続する。
					"ip route 10.0.208.0/24 gateway pp anonymous metric 2"
				};
				if ( !_Put(pCmd, DK_Number(pCmd)) )
					return FALSE;
			}
			break;
		}
		break;
	default: 
		if ( /*m_sRouter.fMent == eMentFixADSL || m_sRouter.fMent == eMentRemoteVpn ||*/
			 m_sRouter.fMent < 0 &&	 // 接続先番号
			(m_nMentType == eDKVPN			 || m_nMentType == eDKVPN_DialUp || 
			 m_nMentType == eKobetuVPN		 || m_nMentType == eKobetuVPN_DialUp) )	
		{
			const char *pCmd[] = {
				"ip route 10.0.192.0/20 gateway pp anonymous metric 2",	// リモートメンテナンス時は、本ネットワークアドレスで接続する。
			};
			if ( !_Put(pCmd, DK_Number(pCmd)) )
				return FALSE;
			wsprintf(szBuf, "ip route 10.0.208.0/20 gateway tunnel %d", abs(m_sRouter.fMent));
			if ( !_Put(szBuf) )
				return FALSE;
			
			if ( m_sRouter.m_nAdslNo )
			{
				if ( m_nMentType == eDKVPN || m_nMentType == eDKVPN_DialUp )
				{
					wsprintf(szBuf, "ip route 10.0.224.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
					if ( !_Put(szBuf) )
						return FALSE;
					
					if ( m_fMxRMAddRead )	// ちゃんと入ってるなら
					{
						wsprintf(szBuf, "ip route %s gateway pp %d", m_szRMAdd, m_sRouter.m_nAdslNo);
						if ( !_Put(szBuf) )
							return FALSE;
					}
				}
			} 
		}
	}
	
	if ( !_Put("ip icmp parameter-problem send off") )
		return FALSE;
	
	// IP電話の設定がある場合は書き込む
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon )
	{
		if ( !_Put("ip tos supersede 1 normal precedence=0 200") )
			return FALSE;
	}

	//===== ↓クラウドIP電話対応  ==============================================================================
	if ( m_sRouter.m_nFqdnState != eFqdnNon || m_sRouter.fSetIpTelFlg > eIpTelNon)
	{
		wsprintf(szBuf, "ip icmp echo-reply send-only-linkup on");
		if (!_Put(szBuf))
			return FALSE;
	}
	//===== ↑クラウドIP電話対応  ==============================================================================

	return TRUE;
}

BOOL CDkRouter1200::PutNatOther(char *tit)
{
	char	szBuf[256];
	
	BYTE bMaskedIP[IP_BYTE];		// マスクをかけた後のIP(ネットワークアドレス)
	BYTE bBroadMaskedIP[IP_BYTE];
	
	CopyMemory(bMaskedIP,		m_sRouter.ip, sizeof(bMaskedIP));
	CopyMemory(bBroadMaskedIP,	m_sRouter.ip, sizeof(bMaskedIP));
	for ( int i = 0; i < IP_BYTE; i++ ) {
		bMaskedIP[i]		= m_sRouter.ip[i] & m_sRouter.mask[i];
		bBroadMaskedIP[i]	= m_sRouter.ip[i] | (~m_sRouter.mask[i]);
	}
	
	// ************ OA_LAN時の処理 ***********************	
	if ( m_fOA_LAN )	// L2に"ADSL"が選択されている場合で"OA_LAN"の時
	{
		if (!_PutTit(tit))
			return FALSE;
		
		if (!_Put("nat descriptor log on"))
			return FALSE;
		
		if (!_Put("nat descriptor masquerade ttl hold all"))
			return FALSE;
		
		if ( (m_sRouter.port[1].kaisen == eKaisenADSL || m_sRouter.m_nVWNo) && m_fOA_LAN )  // ADSL固定IP接続
		{
			if ( !_Put("nat descriptor type 5000 masquerade") )
				return FALSE;
			
			if ( !_Put("nat descriptor masquerade incoming 5000 discard") )
				return FALSE;
		}
		
		// L3に"個別LAN"が選択されている場合
		if ( m_sRouter.port[2].kaisen == eKaisenLAN )  // 個別LAN
		{
			wsprintf(szBuf, "nat descriptor type 8000 masquerade");
			if (!_Put(szBuf))
				return FALSE;
			
			wsprintf(szBuf, "nat descriptor address outer 8000 %s", GetIpB2C(m_sRouter.sLAN3.ip));
			if (!_Put(szBuf))
				return FALSE;
			
			wsprintf(szBuf, "nat descriptor address inner 8000 %d.%d.%d.%d-%d.%d.%d.%d",
 							bMaskedIP[0],		bMaskedIP[1],		bMaskedIP[2],		bMaskedIP[3]+1,
 							bBroadMaskedIP[0],	bBroadMaskedIP[1],	bBroadMaskedIP[2],	bBroadMaskedIP[3]-1 );
			if (!_Put(szBuf))
				return FALSE;
			
			if ( !m_fMS55IpBrank )
			{
				wsprintf(szBuf, "nat descriptor static 8000 60 %s=%d.%d.%d.%d %d", 
								GetIpB2C(m_sRouter.sLAN3.ipMS55), bMaskedIP[0], bMaskedIP[1], bMaskedIP[2], bMaskedIP[3]+2, m_sRouter.sLAN3.kosuu);
				if (!_Put(szBuf))
					return FALSE;
			}
		}
		// PPTPを通すための設定
		wsprintf(szBuf, "nat descriptor masquerade static 2000 3 %s tcp 1723", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "nat descriptor masquerade static 2000 4 %s gre", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))
			return FALSE;
		return TRUE;
	}
	
	// ************ DK_LAN時の処理 ***********************
	if ( m_sRouter.m_nSISNo )	// SIS接続先設定があるとき
	{
// 		if (!_Put("nat descriptor type 2 masquerade"))
// 			return FALSE;
	}
	
	if ( m_sRouter.m_nVWNo || m_sRouter.port[1].kaisen == eKaisenADSL )
	{
		const char *pCmd[] = {
			"nat descriptor type 3 masquerade",
			"nat descriptor masquerade incoming 3 discard"
		};
		if ( !_Put(pCmd,DK_Number(pCmd)) )
			return FALSE;
	}
	
	if ( m_sRouter.m_nRoboNo )	// Roboメール設定があるとき
	{
		if (!_Put("nat descriptor type 4 nat"))
			return FALSE;
	}
	
	if ( m_sRouter.sLAN3.fVirusUse )
	{
		if (!_Put("nat descriptor type 1000 nat"))
			return FALSE;
		if (!_Put("nat descriptor address outer 1000 172.16.252.1"))
			return FALSE;
		wsprintf(szBuf, "nat descriptor address inner 1000 %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "nat descriptor static 1000 252 172.16.252.1=%s 1", GetIpB2C(m_sRouter.sLAN3.ipVirus));
		if (!_Put(szBuf))
			return FALSE;
	}

	// L2に"ADSL固定IP"が選択されている場合
	if ( ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL && m_sRouter.m_nAdslNo )  // ADSL固定IP接続
	{
		for ( int n = 0; n < SETU_MAX; n++ )
		{
			if ( m_sRouter.ncon[n].type == eFixADSL ) 
				break;
		}
		if ( n == SETU_MAX )
			return FALSE;
		
		if (!_Put("nat descriptor type 2000 masquerade"))
			return FALSE;
		
		wsprintf(szBuf, "nat descriptor address outer 2000 %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
		if (!_Put(szBuf))
			return FALSE;
		
		wsprintf(szBuf, "nat descriptor masquerade static 2000 1 %s udp 500", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))
			return FALSE;
		
		wsprintf(szBuf, "nat descriptor masquerade static 2000 2 %s esp", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))
			return FALSE;
		
		if ( m_sRouter.nLuaPort )
		{
			wsprintf(szBuf, "nat descriptor masquerade static 2000 6 %s udp 4500", GetIpB2C(m_sRouter.ip));
			if (!_Put(szBuf))
				return FALSE;			
			wsprintf(szBuf, "nat descriptor masquerade static 2000 7 %s tcp %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort);
			if (!_Put(szBuf))
				return FALSE;
		}
	}
	
	// L3に"個別LAN"が選択されている場合
	if ( m_sRouter.port[2].kaisen == eKaisenLAN )  // 個別LAN
	{
		wsprintf(szBuf, "nat descriptor type 3000 masquerade");
		if (!_Put(szBuf))
			return FALSE;
		
		wsprintf(szBuf, "nat descriptor address outer 3000 %s", GetIpB2C(m_sRouter.sLAN3.ip));
		if (!_Put(szBuf))
			return FALSE;
		
		wsprintf(szBuf, "nat descriptor address inner 3000 %d.%d.%d.%d-%d.%d.%d.%d",
 						bMaskedIP[0],		bMaskedIP[1],		bMaskedIP[2],		bMaskedIP[3]+1,
 						bBroadMaskedIP[0],	bBroadMaskedIP[1],	bBroadMaskedIP[2],	bBroadMaskedIP[3]-1 );
		if (!_Put(szBuf))
			return FALSE;

		if ( !m_fMS55IpBrank )
		{
			wsprintf(szBuf, "nat descriptor static 3000 60 %s=172.16.60.1 %d", 
							GetIpB2C(m_sRouter.sLAN3.ipMS55), m_sRouter.sLAN3.kosuu);
			if (!_Put(szBuf))
				return FALSE;
		}
		
		wsprintf(szBuf, "nat descriptor masquerade static 3000 1 %s udp 500", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))
			return FALSE;
		
		wsprintf(szBuf, "nat descriptor masquerade static 3000 2 %s esp", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))
			return FALSE;

		if ( m_sRouter.sLAN3.fVirusUse )
		{
			wsprintf(szBuf, "nat descriptor masquerade static 3000 3 %s udp domain", GetIpB2C(m_sRouter.ip));
			if (!_Put(szBuf))
				return FALSE;
			
			wsprintf(szBuf, "nat descriptor masquerade static 3000 4 %s tcp www", GetIpB2C(m_sRouter.ip));
			if (!_Put(szBuf))
				return FALSE;
			
			wsprintf(szBuf, "nat descriptor masquerade static 3000 5 %s tcp http", GetIpB2C(m_sRouter.ip));
			if (!_Put(szBuf))
				return FALSE;
		}
	}
	
	if ( m_sRouter.sSIS05.no )  // SIS05通信先設定がある
	{
		for ( int i = 0; i < m_nConnectMax; i++ )	// 探す
		{
			if ( m_sRouter.sSIS05.no == m_sRouter.ncon[i].no )
				break;
		}
		if ( i >= m_nConnectMax )	// 見つからない
			return FALSE;
		
		if ( m_sRouter.ncon[i].port == ePortL2 || m_sRouter.ncon[i].port == ePortL2B1 )	// L2接続
		{
			int nMtGokiNo = (m_sRouter.m_nMtGokiNo == 0) ? 1 : m_sRouter.m_nMtGokiNo;	// 0=デフォルト値=1
			
			BYTE bVPNadd[IP_BYTE];
			CopyMemory(bVPNadd, m_sRouter.sSIS05.vpn_passIp, sizeof(BYTE) * IP_BYTE);
			
			wsprintf(szBuf, "nat descriptor type 2012 masquerade");
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+1
			wsprintf(szBuf, "nat descriptor address outer 2012 %s", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "nat descriptor address inner 2012 auto");
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+2
			wsprintf(szBuf, "nat descriptor static 2012 2 %s=%s 1", GetIpB2C(bVPNadd), GetIpB2C_Second(m_sRouter.ip));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+3
			wsprintf(szBuf, "nat descriptor static 2012 3 %s=172.16.60.1 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+4
			wsprintf(szBuf, "nat descriptor static 2012 4 %s=172.16.1.1 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+5
			wsprintf(szBuf, "nat descriptor static 2012 5 %s=172.16.21.1 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+6
			wsprintf(szBuf, "nat descriptor static 2012 6 %s=172.16.80.1 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+7
			wsprintf(szBuf, "nat descriptor static 2012 7 %s=172.16.15.%d 1", GetIpB2C(bVPNadd), nMtGokiNo);
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+8
			wsprintf(szBuf, "nat descriptor static 2012 8 %s=172.16.52.8 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+9
			wsprintf(szBuf, "nat descriptor static 2012 9 %s=172.16.50.2 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+10
			wsprintf(szBuf, "nat descriptor static 2012 10 %s=172.16.20.1 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+11
			wsprintf(szBuf, "nat descriptor static 2012 11 %s=172.16.20.2 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+12
			wsprintf(szBuf, "nat descriptor static 2012 12 %s=172.16.110.1 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+13
			wsprintf(szBuf, "nat descriptor static 2012 13 %s=172.16.110.2 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+14
			wsprintf(szBuf, "nat descriptor static 2012 14 %s=172.16.122.1 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
			bVPNadd[3] = (bVPNadd[3] == 255) ? 255 : bVPNadd[3]+1;	// aaa.bbb.ccc.ddd+15
			wsprintf(szBuf, "nat descriptor static 2012 15 %s=172.16.15.100 1", GetIpB2C(bVPNadd));
			if (!_Put(szBuf))	return FALSE;
		}
		
		if (!_Put("nat descriptor type 12 masquerade"))
			return FALSE;
	}
		
	if ( m_sRouter.m_nJnetNo > 0 && (ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL && m_sRouter.m_nAdslNo) )
	{
		for ( int n = 0; n < SETU_MAX; n++ )
		{
			if ( m_sRouter.ncon[n].type == eFixADSL ) 
				break;
		}
		
		wsprintf(szBuf, "nat descriptor type 3013 masquerade");
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "nat descriptor address outer 3013 %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "nat descriptor address inner 3013 auto");
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "nat descriptor masquerade static 3013 1 %s udp 500", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "nat descriptor masquerade static 3013 2 %s tcp 4500", GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))	return FALSE;
	}
	
	// PPTPを通すための設定
	wsprintf(szBuf, "nat descriptor masquerade static 2000 3 %s tcp 1723", GetIpB2C(m_sRouter.ip));
	if (!_Put(szBuf))
		return FALSE;
	wsprintf(szBuf, "nat descriptor masquerade static 2000 4 %s gre", GetIpB2C(m_sRouter.ip));
	if (!_Put(szBuf))
		return FALSE;
	// VPN（Nat-Traversal機能を使用時）のパケットを追加させる設定を追加する。
	wsprintf(szBuf, "nat descriptor masquerade static 2000 5 %s udp 4500", GetIpB2C(m_sRouter.ip));
	if (!_Put(szBuf))
		return FALSE;

//===== ↓クラウドIP電話機能  ==============================================================================
	if ( m_sRouter.m_nFqdnState != eFqdnNon )
	{
		wsprintf(szBuf, "nat descriptor sip 2000 off");
		if (!_Put(szBuf))
			return FALSE;
	}
//===== ↑クラウドIP電話機能  ==============================================================================

	return TRUE;
}

// GetNetMaskedIp()
// {
// 	
// 	static char szWkIp[20];
// 	
// 	//	ip set
// 	wsprintf(szWkIp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
// 	if (mask == NULL || *mask == 0)
// 		return szWkIp;
// 	//	mask set
// 	int	nMask = 0;
// 	//	"x.x.x.x" -> byte[4]
// 	for (int i = 0; i < 4; i++, mask++)
// 	{
// 		for (int n = 0; n < 8; n++)
// 			if ((256 - (1<<n)) == *mask)
// 				break;
// 			nMask += (8 - n);
// 	}
// 	wsprintf(szWkIp + lstrlen(szWkIp), "/%d", nMask);
// 	
// 	return szWkIp;
// }

//	NAT put(Ｃ店舗用)
BOOL CDkRouter1200::PutNat_C(char *tit)
{
	char	szBuf[256];
	
	if (!_PutTit(tit))
		return FALSE;
	
	if (!_Put("nat descriptor log on"))
		return FALSE;
	
	if (!_Put("nat descriptor masquerade ttl hold all"))
		return FALSE;
	
	if (!_Put("nat descriptor type 1 nat"))
		return FALSE;
	
	// 上位接続IPのネットマスクは 16bit と決めつける。(下位２バイトはＩＰアドレスを出力)
	wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.ip[2], m_sRouter.ip[3]);
	if (!_Put(szBuf))
		return FALSE;
	wsprintf(szBuf, "nat descriptor address inner 1 %s", GetIpB2C(m_sRouter.ip));
	if (!_Put(szBuf))
		return FALSE;
	
	SDkNat	nat[] = {
		{ 1, 1,  1}, { 2, 1,  2},			   { 4, 1,  8}, { 5, 1, 10}, {10, 1,  2}, {11, 1,128},
		{12, 1,128}, {13, 1,  8}, {14, 1, 12}, {15, 1,  8}, {20, 1,  2}, {21, 1,  1},
															{30, 1,  1}, {31, 1,  8},
					 {40, 1, 32}, {50, 1,  3}, {52, 1,  8},
		{60, 1, 127},{121,20,40}, {80, 1,  1}, {81, 1, 16}, {82, 1, 16}, {83, 1, 16},
		{110,1,  2}, {100,1,  1}, {122,1,  1}
	};
	//	real nat table put
	for (int i = 0; i < DK_Number(nat); i++)
	{
		wsprintf(szBuf, "nat descriptor static 1 %d %d.%d.%d.%d=172.16.%d.%d %d",
 						nat[i].ip1, m_sRouter.uip[0], m_sRouter.uip[1], nat[i].ip1, nat[i].ip2,
 						nat[i].ip1, nat[i].ip2, nat[i].rep);

		if (!_Put(szBuf))
			return FALSE;
	}
	
	if ( m_sRouter.sLAN3.sConNaiyo[eL3XaiTact].fNeed )	// Χai-TACT接続に「○」を設定した場合のみ
	{
		BYTE ip[4] = {0};
		CopyMemory(ip, m_sRouter.sLAN3.ip, sizeof(m_sRouter.sLAN3.ip));
		for ( int i = 0; i < 4; i++)
		{
			ip[i] = ip[i] & m_sRouter.sLAN3.mask[i];
		}
		ip[3] = (ip[3] == 255) ? 255 : ip[3]+1;
		
		wsprintf(szBuf, "nat descriptor static 1 249 %d.%d.249.1=%s 255", m_sRouter.uip[0], m_sRouter.uip[1], GetIpB2C(ip));
		
		if (!_Put(szBuf))
			return FALSE;
	}
	
	return TRUE;
}

//	NAT put(リモート店舗用)
BOOL CDkRouter1200::PutNat_R(char *tit)
{
	char	szBuf[256];
	
	if (!_PutTit(tit))
		return FALSE;
	
	if (!_Put("nat descriptor log on"))
		return FALSE;
	
	if (!_Put("nat descriptor masquerade ttl hold all"))
		return FALSE;
	
 	if (!_Put("nat descriptor type 1 nat"))
 		return FALSE;
	
	// 上位接続IPのネットマスクは 28bit と決めつける。(最後のアドレスを15でAND)
	wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.uip[2], m_sRouter.ip[3] & 15);
	if (!_Put(szBuf))
		return FALSE;
	wsprintf(szBuf, "nat descriptor address inner 1 %s", GetIpB2C(m_sRouter.ip));
	if (!_Put(szBuf))
		return FALSE;
	
	SDkNat	nat[] = {
		{ 5, 1,  1}, {15, 1,  7}, {121, 20,  1}, {121, 30,  1}, {60, 1,1}, {121, 50,  1}, {121, 55, 2},
	};
	
	int nOutIp[] = {1,2,9,10,11,12,13};	// 10.0.1.0+α
	
	//	real nat table put
	BYTE	ip4 = m_sRouter.uip[3] + 1;
	for (int i = 0; i < DK_Number(nat); i++)
	{
		wsprintf(szBuf, "nat descriptor static 1 %d 10.0.%d.%d=172.16.%d.%d %d",
						i + 1,  m_sRouter.uip[2], m_sRouter.uip[3]+nOutIp[i], nat[i].ip1, nat[i].ip2, nat[i].rep);
		if (!_Put(szBuf))
			return FALSE;
		ip4 += nat[i].rep;
	}
		
	return TRUE;
}

//	NAT put(本部店舗用)
BOOL CDkRouter1200::PutNat_H(char *tit)
{
	if (!_PutTit(tit))
		return FALSE;
	
	if (!_Put("nat descriptor log on"))
		return FALSE;
	
	if (!_Put("nat descriptor masquerade ttl hold all"))
		return FALSE;

	return TRUE;
}

//	PP の前処理
BOOL CDkRouter1200::PutPpHead(char *tit)
{
	if (!_PutTit(tit))
		return FALSE;
	
	if ( RT57or58 ) {
		if (!_Put("rip use on"))
			return FALSE;
	}
	
	const char *pCmd01[] = {
		"pp disable all",
	};
	
	if (!_Put(pCmd01, DK_Number(pCmd01)))
		return FALSE;
	
	return TRUE;
}

BOOL CDkRouter1200::PutPp()
{
	char	szBuf[256];
	
	m_fNoDHCP = FALSE;	// ＤＨＣＰサーバ機能を有効にしておくか	
	
	// ip route put
	for ( int n = 0; n < SETU_MAX; n++ )
	{
		if ( m_sRouter.con[n].no == 0 || m_sRouter.ncon[n].type == eDKVPN || m_sRouter.ncon[n].type == eKobetuVPN ||
			 m_sRouter.ncon[n].type == eDeltaNetBk	|| 
			 m_sRouter.ncon[n].type == eIpTel		|| m_sRouter.ncon[n].type == eIpTelBk ||
			 m_sRouter.ncon[n].type == eCarMent		||
			 m_sRouter.ncon[n].type == eJnetMain	|| m_sRouter.ncon[n].type == eJnetSub ||
			(m_sRouter.ncon[n].type == eSIS05 && (m_sRouter.ncon[n].port == ePortL2 ||  m_sRouter.ncon[n].port == ePortL3)) ||
//===== ↓クラウドリニューアル対応 ========================================================
			 m_sRouter.ncon[n].type == eNoConnect   || m_sRouter.ncon[n].type == eNoConnectSub ||
//===== ↑クラウドリニューアル対応 ========================================================
//===== ↓J-NET顧客代行対応 ==========================================================
			 m_sRouter.ncon[n].type == eJnetKokyakuMent )
//===== ↑J-NET顧客代行対応 ==========================================================
			continue;
		
		//	title
		wsprintf(szBuf, "### PP %d ###", m_sRouter.con[n].no);
		if (!_PutTit(szBuf))			return FALSE;
		
		//	pp select
		wsprintf(szBuf, "pp select %d", m_sRouter.con[n].no);
		if (!_Put(szBuf))				return FALSE;
		
		if ( m_sRouter.con[n].no != m_sRouter.m_nAdslNo )	// 接続先はADSLではない場合
		{
			if ( m_sRouter.ncon[n].type == ePHSVPN )
			{
				wsprintf(szBuf, "pp bind tunnel%d", m_sRouter.con[n].no);
				if (!_Put(szBuf))				return FALSE;				
			}
			else
			{
				if ( !_Put("pp bind bri1") )
					return FALSE;
			}
		}
		
		//	!SIS, !VW, !Robo, !PHSの時
		if ( m_sRouter.ncon[n].type == eDialUp			 || m_sRouter.ncon[n].type == eOther || 
			 m_sRouter.ncon[n].type == eKobetuVPN_DialUp || m_sRouter.ncon[n].type == eDKVPN_DialUp )
			//  &&m_sRouter.ncon[n].no == m_sRouter.m_nSISNo )
		{
			//	isdn remote address call
			wsprintf(szBuf, "isdn remote address call %s", m_sRouter.con[n].Tel);
			if (!_Put(szBuf))				return FALSE;
			//	isdn remote address arrive
			wsprintf(szBuf, "isdn remote address arrive %s", m_sRouter.con[n].Tel);
			if (!_Put(szBuf))				return FALSE;
		}
		
		//	SISの時
		if ( m_sRouter.ncon[n].type == eSIS && m_sRouter.con[n].no == m_sRouter.m_nSISNo )
		{
			//	isdn remote address call
			wsprintf(szBuf, "isdn remote address call %s", m_sRouter.con[n].Tel);
			if (!_Put(szBuf))				return FALSE;
			const char *pCmd[] = {
				"isdn disconnect time 15",
				"isdn disconnect input time 30",
				"isdn disconnect output time 30",
				"pp auth accept pap chap",
				"pp auth myname tao daikoku",
				"ppp ipcp ipaddress on",
				"ip pp nat descriptor 2",
			};
			
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
		} else		//	when ロボメール
		if ( m_sRouter.ncon[n].type == eRobo )
		{
			//	isdn remote address call
			wsprintf(szBuf, "isdn remote address call %s", ROBO_TEL);
			if (!_Put(szBuf))				return FALSE;
			const char *pCmd[] = {
				"isdn disconnect time 15",
				"isdn disconnect input time 30",
				"isdn disconnect output time 30",
				"ip pp nat descriptor 4",
				"pp auth accept pap chap",
				"ppp ipcp ipaddress on",
			};
			
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
			
			wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szRoboID, m_sRouter.szRoboPass);
			if (!_Put(szBuf))				return FALSE;
		} else	// when m_nVWNo
		if ( m_sRouter.ncon[n].type == eVw )
		{
			//	isdn remote address call
			wsprintf(szBuf, "isdn remote address call %s", m_sRouter.con[n].Tel);
			if (!_Put(szBuf))				return FALSE;
			const char *pCmd[] = {
				"isdn disconnect time 480",
				"isdn disconnect input time 720",
				"isdn disconnect output time 720",
			};
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
			
			if ( m_sRouter.shOverTimer == 0 )  // 0=off
				strncpy0(szBuf, "isdn forced disconnect time off", sizeof(szBuf));				// 接続超過タイマ
			else
				wsprintf(szBuf, "isdn forced disconnect time %d", m_sRouter.shOverTimer * 60);	// 接続超過タイマ
			
			if (!_Put(szBuf))
				return FALSE;
			
			wsprintf(szBuf, "ip pp secure filter name %s", m_fOA_LAN ? "oa_pp_lab" : "dk_pp_lab");
			if (!_PutFilterName(szBuf))
				return FALSE;
			
			if (!_Put("ip pp intrusion detection in on reject=on"))
				return FALSE;
			
			wsprintf(szBuf, "ip pp nat descriptor %d", m_fOA_LAN ? 5000 : 3);
			if (!_Put(szBuf))				return FALSE;
			
			const char *pCmd2[] = {
				"pp auth accept pap chap",
				"ppp ipcp ipaddress on"
			};
			if (!_Put(pCmd2, DK_Number(pCmd2)))
				return FALSE;
			
			wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szVwID, m_sRouter.szVwPass);
			if (!_Put(szBuf))
				return FALSE;
			if (!_Put("ppp ipcp msext off"))
				return FALSE;
		} else	// when SIS05
		if ( m_sRouter.ncon[n].type == eSIS05 )
		{
			if ( m_sRouter.ncon[n].port == ePortB1 || m_sRouter.ncon[n].port == ePortL2B1 )
			{
				//	isdn remote address call
				wsprintf(szBuf, "isdn remote address call 0422799015");
				if (!_Put(szBuf))				return FALSE;
				
				const char *pCmd[] = {
					"isdn disconnect time 15",
					"isdn disconnect input time 30",
					"isdn disconnect output time 30",
					"ip pp nat descriptor 12",
					"pp auth accept chap",
					"ppp ipcp ipaddress on",
				};
				if (!_Put(pCmd, DK_Number(pCmd)))
					return FALSE;
				
				wsprintf(szBuf, "pp auth myname %s %s", (lstrlen(m_sRouter.sSIS05.szHoleCode) == 0) ? "0.0.0.0" : m_sRouter.sSIS05.szHoleCode, GetIpB2C(m_sRouter.sSIS05.passIp));
				if (!_Put(szBuf))
					return FALSE;
			}

		} else	//	when ADSL
		if ( m_sRouter.ncon[n].type == eADSL )
		{
			const char *pCmd[] = {
				"pp backup none",
				"pp always-on off",
				"pppoe use lan2",
				"pppoe auto connect on",
				"pppoe auto disconnect on",
				"pppoe disconnect time 30",
			};
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
			
			// MTU値
//===== ↓LTE通信対応 ==================================================================
			wsprintf(szBuf, "ip pp mtu %d", (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nMTU : m_sRouter.nMTU);
//===== ↑LTE通信対応 ==================================================================
			if (!_Put(szBuf))
				return FALSE;
			
			const char *pCmd1[] = {		
				"ip pp intrusion detection in on reject=on",
				"pp auth accept pap chap",
			};
			if (!_Put(pCmd1, DK_Number(pCmd1)))
				return FALSE;
			
			wsprintf(szBuf, "ip pp secure filter name %s", m_fOA_LAN ? "oa_pp_lab" : "dk_pp_lab");
			if (!_PutFilterName(szBuf))				return FALSE;
			
			wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass);
			if (!_Put(szBuf))				return FALSE;
			
			wsprintf(szBuf, "ip pp nat descriptor %d", m_fOA_LAN ? 5000 : 3);
			if (!_Put(szBuf))				return FALSE;
			
			const char *pCmd2[] = {
				"ppp lcp mru on 1454",
				"ppp ccp type none",
			};
			if (!_Put(pCmd2, DK_Number(pCmd2)))
				return FALSE;
			
			if (!_Put("ppp ipcp msext off"))
				return FALSE;
			
			if ( m_sRouter.m_nAdslNo && m_sRouter.port[1].kaisen == eKaisenADSL ) { 
				if (!_Put("ppp ipcp ipaddress on"))	// Ｌ２がADSLの設定の時のみ現れる
					return FALSE;
			}
			if ( m_sRouter.sLAN3.fVirusUse )
			{
 				if (!_Put("url pp filter out 10 21 22 23 24 25 26 27 28"))
 					return FALSE;
			}
		} else	//	when ADSL固定IP接続
		if ( m_sRouter.ncon[n].type == eFixADSL )
		{
			const char *pCmd[] = {
				"pp backup none",
				"pppoe use lan2",
				"pppoe auto connect on",
				"pppoe auto disconnect on",
				"pppoe disconnect time off"
			};
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
			
			// MTU値
//===== ↓LTE通信対応 ==================================================================
			wsprintf(szBuf, "ip pp mtu %d", (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nMTU : m_sRouter.nMTU);
//===== ↑LTE通信対応 ==================================================================
			if (!_Put(szBuf))
				return FALSE;
			
			const char *pCmd1[] = {
				"ip pp intrusion detection in on reject=on",
				"ip pp nat descriptor 2000",
				"pp auth accept pap chap",
			};
			if (!_Put(pCmd1, DK_Number(pCmd1)))
				return FALSE;
			
			// L2ポートの回線種別がADSL固定の場合、絶対にonになる
			wsprintf(szBuf, "pp always-on %s", (ADSL_KIND(m_sRouter.port[1].kaisen)== eKaisenFixADSL) ? "on" : "off");
			if (!_Put(szBuf))				return FALSE;
			
			wsprintf(szBuf, "ip pp secure filter name dk_pp_lan2");
			if (!_PutFilterName(szBuf))				return FALSE;
			
//===== ↓LTE通信対応 ==================================================================
			if ( m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE )
			{
				BYTE *ip = m_sRouter.sLte.ip;
				wsprintf(szBuf, "ip pp address %d.%d.%d.%d/32", ip[0], ip[1], ip[2], ip[3]);
				if (!_Put(szBuf))				return FALSE;
				
				wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
				if (!_Put(szBuf))				return FALSE;
			}
			else
			{
				wsprintf(szBuf, "ip pp address %d.%d.%d.%d/32", 
					m_sRouter.ncon[n].fixIp[0], m_sRouter.ncon[n].fixIp[1], m_sRouter.ncon[n].fixIp[2], m_sRouter.ncon[n].fixIp[3]);
				if (!_Put(szBuf))				return FALSE;
				
				wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass);
				if (!_Put(szBuf))				return FALSE;
			}
//===== ↑LTE通信対応 ==================================================================
			
			const char *pCmd2[] = {
				"ppp lcp mru on 1454",
				"ppp ccp type none",
			};
			if (!_Put(pCmd2, DK_Number(pCmd2)))
				return FALSE;
			if ( m_sRouter.sLAN3.fVirusUse )
			{
 				if (!_Put("url pp filter out 10 21 22 23 24 25 26 27 28"))
 					return FALSE;
			}
		}
		else //	when PHS
		if ( m_sRouter.ncon[n].type >= ePHS1 && m_sRouter.ncon[n].type <= ePHSfree )	// PHS接続
		{
			//	isdn remote address arrive
			wsprintf(szBuf, "isdn remote address arrive %s", m_sRouter.con[n].Tel);
			if (!_Put(szBuf))				return FALSE;
			const char *pCmd[] = {
				"pp auth request chap-pap",
				"pp auth username openuser openuser",
			};
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
			
			if ( m_sRouter.ncon[n].type != ePHSfree )  // PHSフリーではない
			{
				if ( m_sRouter.ip[0] == 172 && m_sRouter.ip[1] == 16  &&  m_sRouter.ip[2] == 5  ||
					 m_sRouter.ip[0] == 192 && m_sRouter.ip[1] == 168 && (m_sRouter.ip[2] == 60 || m_sRouter.ip[2] == 61) && m_sRouter.ip[3] == 1 )
				{
					if ( m_sRouter.ip[0] == 192 )	// 172.16.60.[n+47]
					{
						wsprintf(szBuf, "ip pp remote address 192.168.%d.%d", m_sRouter.ip[2], m_sRouter.ncon[n].type + 47);
						m_fNoDHCP = (m_sRouter.ip[3] == 1) ? TRUE : FALSE;	// ＤＨＣＰサーバ機能OFFにするか？
						if (!_Put(szBuf)) return FALSE;
					} else {  						// 172.16.60.[n+47]
						wsprintf(szBuf, "ip pp remote address 172.16.60.%d", m_sRouter.ncon[n].type + 47);
						m_fNoDHCP = (m_sRouter.ip[3] == 1) ? TRUE : FALSE;	// ＤＨＣＰサーバ機能OFFにするか？
						if (!_Put(szBuf)) return FALSE;
					}
				}
			}
		}
		else //	when PHSVPN
		if ( m_sRouter.ncon[n].type == ePHSVPN )	// PHSVPN
		{
			const char *pCmd[] = {
				"pp always-on on",
				"pp auth request mschap-v2",
				"pp auth username openuser openuser",
				"ppp ccp type mppe-any",
				"ip pp mtu 1280",
				"pptp service type server",
			};
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
		} else {	// 普通の接続先設定
			
			if ( m_nUseType != eUseHonbu ) { // 本部店舗の場合は、ここでは何も書き込まない
				const char *pCmd[] = {
					"ip pp nat descriptor 1",
				};
				if (!_Put(pCmd, DK_Number(pCmd)))
					return FALSE;
			} 
		}
		
		// if (!_Put("ip pp rip listen off"))	return FALSE;
		if ( m_sRouter.ncon[n].type != ePHSVPN )	// PHSVPN
		{
			const char *pCmd1[] = 
			{
				"ip pp rip send off",
				"ip pp rip receive off"
			};	
			if (!_Put(pCmd1, DK_Number(pCmd1)))
				return FALSE;
		}
		
		//	pp enable
//===== ↓ケーブル光対応 ==================================================================
		if ( m_sRouter.con[n].no == m_sRouter.m_nAdslNo && m_sRouter.port[1].kaisen == eKaisenCableHikari )
			wsprintf(szBuf, "pp disable %d", m_sRouter.con[n].no);
		else
			wsprintf(szBuf, "pp enable %d", m_sRouter.con[n].no);
		if (!_Put(szBuf))				return FALSE;
//===== ↑ケーブル光対応 ==================================================================
	}
	
	return TRUE;
}

BOOL CDkRouter1200::PutFmt(LPCSTR lpszFmt, ...)
{
	CString			strMsg;
	LPSTR			*lppParam;
	char			szBuff[1024];
	
	lppParam = ((LPSTR*)&lpszFmt) + 1;
    vsprintf(szBuff, lpszFmt, (LPSTR)lppParam);
	strMsg = szBuff;
	
	return _Put(strMsg);
}

//	PP anonymous
BOOL CDkRouter1200::PutPpAnonymous(char *tit)
{
	if ( m_sRouter.nLuaPort )
	{
		if (!_PutTit(tit))
			return FALSE;
		
		const char *pCmd01[] = {
			"no ip route 10.0.192.0/20",
			"pp select anonymous",
			"no ip pp rip send",
			"no ip pp rip receive",
			"no ip pp nat descriptor"
		};
		if (!_Put(pCmd01, DK_Number(pCmd01)))
			return FALSE;
		if (!PutFmt("no pp auth username %s", m_sRouter.szAuthId))	return FALSE;
		
		if (!PutFmt("pp bind tunnel%d-tunnel%d", m_sPrm.nLuaTunnelMin, m_sPrm.nLuaTunnelMax))	return FALSE;
		if (!PutFmt("pp auth request mschap-v2"))	return FALSE;
		if (!PutFmt("pp auth username %s %s", m_sRouter.szVPN_User, m_sRouter.szVPN_Pass))	return FALSE;
		if (!PutFmt("ppp ipcp ipaddress on"))	return FALSE;
		if (!PutFmt("ppp ipcp msext on"))	return FALSE;
		if (!PutFmt("ip pp remote address pool 172.16.60.%d-172.16.60.%d", m_sPrm.nLuaTunnelMin, m_sPrm.nLuaTunnelMax))	return FALSE;
		if (!PutFmt("ip pp mtu 1258"))	return FALSE;
		if (!PutFmt("pp enable anonymous"))	return FALSE;
	}
	else
	{
		if ( m_fOA_LAN )	// OA_LANの場合は絶対に書き込み
		{	
			if (!_PutTit(tit))
				return FALSE;
			
			const char *pCmd01[] = {
				"pp select anonymous",
				"pp bind bri1",
				"pp auth request pap",
				"pp auth username * *",
				"ip pp rip send off",
				"ip pp rip receive off",
				"pp enable anonymous"
			};	
			
			if (!_Put(pCmd01, DK_Number(pCmd01)))
				return FALSE;
			
			return TRUE;
		}

		if ( m_sRouter.fMent == eMentNoUse )	// 接続先番号に何らかの値が選択されている場合（リモートメンテナンス用設定が有効な場合）は、以下の設定も追加する。
			return TRUE;
		
		if (!_PutTit(tit))
			return FALSE;
		
		const char *pCmd01[] = {
			"pp select anonymous",
		};
		
		if (!_Put(pCmd01, DK_Number(pCmd01)))
			return FALSE;
		
		const char *pCmd2[] = {
			"ip pp rip send off",
			"ip pp rip receive off",
			"pp bind bri1"
		};
		if (!_Put(pCmd2, DK_Number(pCmd2)))
			return FALSE;
		
		if ( m_sRouter.fMent != eMentAuto )
		{	
			if ( m_nUseType != eUseHonbu )  // 本部以外は全て"ip pp nat descriptor 1"を書き込む
			{
				_Put("ip pp nat descriptor 1");
			}
			
			const char *pCmd02[] = {
				"pp auth request pap",
				"pp auth username * *",
			};
			if (!_Put(pCmd02, DK_Number(pCmd02)))
				return FALSE;
		}
		else	// m_sRouter.fMent == eMentAuto
		{
			if (!_Put("pp auth request chap"))
				return FALSE;
			
			CString strAuthPass;
			strAuthPass.Format("pp auth username %s %s", m_strAUTH_ID, m_strAUTH_PASS);
			if (!_Put(strAuthPass))
				return FALSE;
		}
		
		if (!_Put("pp enable anonymous"))
			return FALSE;
	}
	
	return TRUE;
}

BOOL CDkRouter1200::PutTunnel()
{	
	if ( !_Put("\ntunnel disable all") )
		return FALSE;

	char szBuf[256];
	int nSetuzokuNo = 0;
	
	for ( int n = 0; n <= SETU_MAX; n++ )
	{
		nSetuzokuNo = m_sRouter.con[n].no;
		
		// IP電話は後でセット
		if ( m_sRouter.ncon[n].no == 0 || m_sRouter.ncon[n].type == eIpTel || m_sRouter.ncon[n].type == eIpTelBk ||
			m_sRouter.ncon[n].type == eCarMent ||	// 来店管理メンテは後でセット
			m_sRouter.ncon[n].type != ePHSVPN &&	
			m_sRouter.ncon[n].type != eDKVPN		 && m_sRouter.ncon[n].type != eKobetuVPN && 
			m_sRouter.ncon[n].type != eDKVPN_DialUp && m_sRouter.ncon[n].type != eKobetuVPN_DialUp &&
			m_sRouter.ncon[n].type != eJnetMain && m_sRouter.ncon[n].type != eJnetSub &&
			!(m_sRouter.ncon[n].type == eSIS05 && (m_sRouter.con[n].port == ePortL2 || m_sRouter.con[n].port == ePortL2B1)) ||
//===== ↓クラウドリニューアル対応 ========================================================
			 m_sRouter.ncon[n].type == eNoConnect || m_sRouter.ncon[n].type == eNoConnectSub || m_sRouter.ncon[n].type == eDeltaNetBk ||	
//===== ↑クラウドリニューアル対応 ========================================================
//===== ↓J-NET顧客代行対応 ==========================================================
			m_sRouter.ncon[n].type == eJnetKokyakuMent )
			continue;
//===== ↑J-NET顧客代行対応 ==========================================================
		if ( m_sRouter.con[n].no == abs(m_sRouter.fMent) && m_sRouter.fMent < 0 )	// メンテ接続先について
		{
			m_nMentType = m_sRouter.ncon[n].type;
		}
		
		//	title
		wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo);
		if (!_PutTit(szBuf))			return FALSE;
		
		wsprintf(szBuf, "tunnel select %d", nSetuzokuNo);
		if (!_Put(szBuf))				return FALSE;
		
		if ( m_sRouter.ncon[n].type == ePHSVPN )
		{			
			if ( !_Put("tunnel encapsulation pptp") )
				return FALSE;
			
			wsprintf(szBuf, "tunnel endpoint address %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
			if (!_Put(szBuf))				return FALSE;
			
			wsprintf(szBuf, "tunnel enable %d", m_sRouter.con[n].no);
			if (!_Put(szBuf))				return FALSE;		
			// PHSVPNは これだけ書いたら終わり
			continue;
		}
		
		if ( m_sRouter.ncon[n].type == eSIS05 && (m_sRouter.ncon[n].port == ePortL2 || m_sRouter.ncon[n].port == ePortL2B1) )
		{
			wsprintf(szBuf, "tunnel encapsulation ipsec");
			if (!_Put(szBuf))				return FALSE;
		}
		
		wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo + 100);
		if (!_Put(szBuf))				return FALSE;
		
		//===== ↓IPSEC ======================================================================
		if ( (m_sRouter.ncon[n].no == m_sRouter.m_nJnetNo || m_sRouter.ncon[n].no == m_sRouter.m_nJnetNo2) && 
			 (m_sRouter.ncon[n].port == ePortL2 || m_sRouter.ncon[n].port == ePortL2B1) )
		{
			wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc md5-hmac", nSetuzokuNo+100, nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike duration isakmp-sa %d 28800", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike keepalive log %d off", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE; 
			wsprintf(szBuf,"ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
			if (!_Put(szBuf))	return FALSE; 
			wsprintf(szBuf,"ipsec ike local name %d %s key-id", nSetuzokuNo, m_sRouter.m_szJnetVpnName);
			if (!_Put(szBuf))	return FALSE; 
			wsprintf(szBuf,"ipsec ike pre-shared-key %d text %sDN%s", nSetuzokuNo, m_sRouter.m_szJnetHoujin, m_sRouter.m_szJnetHall);
			if (!_Put(szBuf))	return FALSE; 
			wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, (m_sRouter.ncon[n].no == m_sRouter.m_nJnetNo) ? IP_JNET_MAIN_IP : IP_JNET_SUB_IP);
			if (!_Put(szBuf))	return FALSE;
			
			if ( m_sRouter.ncon[n].no == m_sRouter.m_nJnetNo )	// メイン番号時のみ
			{
				wsprintf(szBuf,"tunnel backup tunnel %d", m_sRouter.m_nJnetNo2);
				if (!_Put(szBuf))	return FALSE;
			}
			
			wsprintf(szBuf,"ip tunnel nat descriptor 3013");
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ip tunnel tcp mss limit auto");
			if (!_Put(szBuf))	return FALSE;			
			wsprintf(szBuf,"ip tunnel secure filter in 9");
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ip tunnel secure filter out 10210 10211 dynamic 10210 10211");
			if (!_Put(szBuf))	return FALSE;
		}
		else if ( m_sRouter.ncon[n].no == m_sRouter.sSIS05.no && (m_sRouter.ncon[n].port == ePortL2 || m_sRouter.ncon[n].port == ePortL2B1) )
		{
			wsprintf(szBuf,"ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike encryption %d 3des-cbc", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike group %d modp1024", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike hash %d sha", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike pfs %d on", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			
			for ( int i = 0; i < SETU_MAX; i++ )
			{
				if ( m_sRouter.m_nAdslNo == m_sRouter.con[i].no )
				{
					break;
				}
			}
			
			wsprintf(szBuf,"ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.sSIS05.accessIp));
			if (!_Put(szBuf))	return FALSE;
		}
		else
		{
			wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, (m_sRouter.ncon[n].type == eDKVPN || m_sRouter.ncon[n].type == eKobetuVPN) ? "off" : "on");
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike pre-shared-key %d text xxxxxx", nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp));
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask));
			if (!_Put(szBuf))	return FALSE;
			wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo);
			if (!_Put(szBuf))	return FALSE;		
		}
		//===== ↑IPSEC ======================================================================
		
		if ( m_sRouter.ncon[n].type == eJnetMain || m_sRouter.ncon[n].type == eJnetSub )
		{
			
		} 
		else
		{
			if ( m_sRouter.ncon[n].type == eDKVPN_DialUp || m_sRouter.ncon[n].type == eKobetuVPN_DialUp )
			{
				wsprintf(szBuf, "tunnel backup pp %d", m_sRouter.con[n].no);
				if (!_Put(szBuf))
					return FALSE;
			} else 
			if ( m_sRouter.ncon[n].type == eSIS05 && m_sRouter.ncon[n].port == ePortL2 && m_sRouter.m_nDeltaBkNo )
			{			
				wsprintf(szBuf, "tunnel backup tunnel %d switch-interface=on", m_sRouter.m_nDeltaBkNo);
				if (!_Put(szBuf))
					return FALSE;
			} else
			if ( m_sRouter.ncon[n].type == eSIS05 && m_sRouter.ncon[n].port == ePortL2B1 )	// L2B1はDELTANETバックアップ指定してあっても意味なし
			{
				wsprintf(szBuf, "tunnel backup pp %d switch-router=on", m_sRouter.ncon[n].no);
				if (!_Put(szBuf))
					return FALSE;
			} else
			{
				wsprintf(szBuf, "tunnel backup none");
				if (!_Put(szBuf))
					return FALSE;
			}
			
			const char *pCmd[] = {
				"ip tunnel rip send off",
				"ip tunnel rip receive off"
			};
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
			
			if ( m_sRouter.ncon[n].no == m_sRouter.sSIS05.no && 
				(m_sRouter.ncon[n].port == ePortL2 || m_sRouter.ncon[n].port == ePortL2B1) )
			{
				if (!_Put("ip tunnel nat descriptor 2012"))
					return FALSE;				
			} else {
				if ( m_nUseType != eUseHonbu ) 
				{ // 本部店舗の場合は、ここでは何も書き込まない
					if (!_Put("ip tunnel nat descriptor 1"))
						return FALSE;
				}
			}
		}
		
		wsprintf(szBuf, "tunnel enable %d", m_sRouter.con[n].no);
		if (!_Put(szBuf))				return FALSE;		
	}// end for
	
	// IP電話のtunnel定義が必要な場合
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon && m_sRouter.m_nIpTelNo > 0 )
	{	
		// ADSL固定ＩＰが選択されていなければここにはこないはず
		for ( int n = 0; n < SETU_MAX; n++ )
		{
			if ( m_sRouter.ncon[n].type == eFixADSL ) 
				break;
		}
		if ( n == SETU_MAX )
			return FALSE;
		
		wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nIpTelNo);
		if (!_PutTit(szBuf))			return FALSE;
		wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf, "tunnel encapsulation ipsec");
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nIpTelNo+100);
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc sha-hmac", m_sRouter.m_nIpTelNo+100, m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike encryption %d aes-cbc", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike group %d modp1024", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike hash %d sha", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike keepalive log %d off", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 60 10", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.ncon[n].fixIp));
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike pfs %d on", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;		
		wsprintf(szBuf,"ipsec ike pre-shared-key %d text CallCentre", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike remote address %d %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.ptc_ip, NULL));
		if (!_Put(szBuf))	return FALSE;
		
		wsprintf(szBuf,"ip tunnel address %s", GetIpB2C(m_sRouter.ip_2, NULL));
		if (!_Put(szBuf))	return FALSE;
		
		const char *pCmd[] = {
			"ip tunnel rip send off",
			"ip tunnel rip receive off"
		};
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
		
		wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nIpTelNo);
		if (!_Put(szBuf))				return FALSE;
	}
	
	// IP電話のtunnel定義が必要な場合
	if ( m_sRouter.fSetIpTelFlg > eIpTelNon && m_sRouter.m_nIpTelNoBk > 0 )
	{	
		// ADSL固定ＩＰが選択されていなければここにはこないはず
		for ( int n = 0; n < SETU_MAX; n++ )
		{
			if ( m_sRouter.ncon[n].type == eFixADSL ) 
				break;
		}
		if ( n == SETU_MAX )
			return FALSE;
		
		wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nIpTelNoBk);
		if (!_PutTit(szBuf))			return FALSE;
		wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf, "tunnel encapsulation ipsec");
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nIpTelNoBk+100);
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc sha-hmac", m_sRouter.m_nIpTelNoBk+100, m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike encryption %d aes-cbc", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike group %d modp1024", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike hash %d sha", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike keepalive log %d off", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 60 10", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.ncon[n].fixIp));
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike pfs %d on", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;		
		wsprintf(szBuf,"ipsec ike pre-shared-key %d text CallCentre", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike remote address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.center_ip, NULL));
		if (!_Put(szBuf))	return FALSE;
		
// 		wsprintf(szBuf,"ip tunnel address %s", GetIpB2C(m_sRouter.center_ip, NULL));
// 		if (!_Put(szBuf))	return FALSE;
		
		const char *pCmd[] = {
			"ip tunnel rip send off",
			"ip tunnel rip receive off"
		};
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
		
		wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nIpTelNoBk);
		if (!_Put(szBuf))				return FALSE;
	}

	// 車両管理メンテのtunnel定義が必要な場合
	if ( m_sRouter.m_nCarMentNo > 0 )
	{
		wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nCarMentNo);
		if (!_PutTit(szBuf))			return FALSE;
		wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf, "tunnel encapsulation ipsec");
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nCarMentNo+100);
		if (!_Put(szBuf))				return FALSE;
		wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc sha-hmac", m_sRouter.m_nCarMentNo+100, m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike encryption %d aes-cbc", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike group %d modp1024", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike hash %d sha", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 10 2", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nCarMentNo, GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike pfs %d on", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;		
		wsprintf(szBuf,"ipsec ike pre-shared-key %d text *", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike remote address %d any", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf,"ipsec ike remote name %d *", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
		
		const char *pCmd[] =
		{
			"ip tunnel secure filter in 10100 10110 10120 10130",
			"ip tunnel secure filter out 10125 10135",
			"ip tunnel rip send off",
			"ip tunnel rip receive off"
		};
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
		
		wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nCarMentNo);
		if (!_Put(szBuf))	return FALSE;
	}
	
	// メンテ設定がリモートVPNかADSL固定か接続番号(つまりポートがL2 or L2+B1)
	if ( m_sRouter.fMent == eMentFixADSL || m_sRouter.fMent == eMentRemoteVpn || m_sRouter.fMent < 0 && (m_nMentType == eDKVPN || m_nMentType == eDKVPN_DialUp))
	{
		//	title
		wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nAdslNo);
		if (!_PutTit(szBuf))			return FALSE;
		
		wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nAdslNo);
		if (!_Put(szBuf))				return FALSE;
		
		wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nAdslNo + 100);
		if (!_Put(szBuf))				return FALSE;		
		
		if ( m_sRouter.fMent == eMentRemoteVpn )	// リモートVPNの場合
		{		
			// RMAddressの読み込みに失敗しても成功しても書き込む
			wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc md5-hmac", m_sRouter.m_nAdslNo + 100, m_sRouter.m_nAdslNo);
			if (!_Put(szBuf))				return FALSE;
			wsprintf(szBuf, "ipsec ike pre-shared-key %d text *", m_sRouter.m_nAdslNo);
			if (!_Put(szBuf))				return FALSE;
			wsprintf(szBuf, "ipsec ike remote address %d any", m_sRouter.m_nAdslNo);
			if (!_Put(szBuf))				return FALSE;
			wsprintf(szBuf, "ipsec ike remote name %d *", m_sRouter.m_nAdslNo);
			if (!_Put(szBuf))				return FALSE;
		}
		else
		{
			//===== ↓IPSEC ======================================================================
			if ( m_fMxRMAddRead )	// RMAddressを読み込んでいなければIPSEC自体書き込まない
			{
				nSetuzokuNo = m_sRouter.m_nAdslNo;
				
				wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike keepalive use %d off", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike pre-shared-key %d text *", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, m_szRMAdd);	// GetIpB2C(m_bRMAdd));
				if (!_Put(szBuf))	return FALSE;		
				wsprintf(szBuf, "ipsec ike remote id %d 10.0.224.0/20", nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
				wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo);
				if (!_Put(szBuf))	return FALSE;
			}
			//===== ↑IPSEC ======================================================================
			
			if (!_Put("tunnel backup none"))
				return FALSE;
			
			const char *pCmd[] = {
				"ip tunnel rip send off",
				"ip tunnel rip receive off",
			};
			
			if (!_Put(pCmd, DK_Number(pCmd)))
				return FALSE;
		}
		
		if ( m_nUseType != eUseHonbu )  // 本部店舗の場合は、ここでは何も書き込まない
		{
			if (!_Put("ip tunnel nat descriptor 1"))
				return FALSE;
		}
		
		wsprintf(szBuf, "tunnel enable %d",m_sRouter.m_nAdslNo);
		if (!_Put(szBuf))				return FALSE;
	}
	
	// メンテ設定がautoでL2の回線種別がADSL固定の場合(=メンテナンス接続先ポートL2)
	if ( m_sRouter.fMent == eMentAuto && ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL )
	{
		nSetuzokuNo = m_sRouter.m_nAdslNo;
		
		//	title
		wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo);
		if (!_PutTit(szBuf))			return FALSE;
		
		wsprintf(szBuf, "tunnel select %d", nSetuzokuNo);
		if (!_Put(szBuf))				return FALSE;
		
		if (!_Put("tunnel encapsulation ipsec"))	return FALSE;
		
		wsprintf(szBuf, "ipsec tunnel %d",nSetuzokuNo + 100);
		if (!_Put(szBuf))	return FALSE;
		
		wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac",nSetuzokuNo + 100, nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		
		for ( int i = 0; i < SETU_MAX; i++ )
		{
			if ( m_sRouter.ncon[i].no == m_sRouter.m_nAdslNo && m_sRouter.ncon[i].no != 0 )
				break;
		}
		
		wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, m_strPRE_SHARED_KEY);
		if (!_Put(szBuf))	return FALSE;
		
		wsprintf(szBuf, "ipsec ike remote address %d any", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike remote name %d *", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 10 2", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;	
		
		const char *pCmd[] = {
			"ip tunnel rip send off",
			"ip tunnel rip receive off",
		};		
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
		
		if ( m_sRouter.sLAN3.fVirusUse )
		{
			if (!_Put("ip tunnel nat descriptor 1000"))	
				return FALSE;
		}
		
		wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
	}
	
	if ( m_sRouter.m_nDeltaBkNo )	// DELTANETバックアップ用設定
	{
		nSetuzokuNo = m_sRouter.m_nDeltaBkNo;
		
		//	title
		wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo);
		if (!_PutTit(szBuf))return FALSE;
		
		wsprintf(szBuf, "tunnel select %d", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		
		if (!_Put("tunnel encapsulation ipsec"))	return FALSE;
		
		wsprintf(szBuf, "ipsec tunnel %d",nSetuzokuNo + 100);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac",nSetuzokuNo + 100, nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		
		for ( int i = 0; i < SETU_MAX; i++ )
		{
			if ( m_sRouter.ncon[i].no == m_sRouter.sSIS05.no && m_sRouter.ncon[i].no != 0 && m_sRouter.ncon[i].type != eNoConnect )
				break;
		}
		wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo);	// (m_sRouter.ncon[i].port == ePortL2) ? "10 6" : "60 10");
		if (!_Put(szBuf))	return FALSE;
		
		wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
		if (!_Put(szBuf))	return FALSE;
		wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
		
		for ( i = 0; i < SETU_MAX; i++ )
		{
			if ( m_sRouter.ncon[i].no == m_sRouter.m_nAdslNo && m_sRouter.ncon[i].no != 0 )
				break;
		}
		wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
		if (!_Put(szBuf))	return FALSE;
		
		for ( i = 0; i < SETU_MAX; i++ )
		{
			if ( m_sRouter.ncon[i].no == m_sRouter.m_nDeltaBkNo && m_sRouter.ncon[i].no != 0 && m_sRouter.ncon[i].type != eNoConnectSub )
				break;
		}	
		wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].ip));
		if (!_Put(szBuf))	return FALSE;
		
		const char *pCmd[] = {
			"ip tunnel rip send off",
			"ip tunnel rip receive off",
			"ip tunnel nat descriptor 2012"
		};
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
		
		wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo);
		if (!_Put(szBuf))	return FALSE;
	}
	
	if ( m_sRouter.nLuaPort )
	{
		for ( int nTunnelNo = m_sPrm.nLuaTunnelMin; nTunnelNo <= m_sPrm.nLuaTunnelMax; nTunnelNo++ )
		{
			if (!PutFmt("tunnel select %d", nTunnelNo))	return FALSE;
			if (!PutFmt("tunnel encapsulation l2tp"))	return FALSE;
			if (!PutFmt("ipsec tunnel 1%d", nTunnelNo))	return FALSE;
			if (!PutFmt("ipsec sa policy 1%d %d esp aes-cbc sha-hmac", nTunnelNo, nTunnelNo))	return FALSE;
			if (!PutFmt("ipsec ike keepalive use %d off", nTunnelNo))							return FALSE;
			if (!PutFmt("ipsec ike nat-traversal %d on", nTunnelNo))						return FALSE;
			if (!PutFmt("ipsec ike pre-shared-key %d text %s", nTunnelNo, m_sRouter.szVPN_Key))	return FALSE;
			if (!PutFmt("ipsec ike remote address %d any", nTunnelNo))							return FALSE;
			if (!PutFmt("l2tp tunnel auth off"))			return FALSE;
			if (!PutFmt("l2tp tunnel disconnect time off"))	return FALSE;
			if (!PutFmt("l2tp keepalive use on"))			return FALSE;
			if (!PutFmt("ip tunnel tcp mss limit auto"))	return FALSE;
			if (!PutFmt("ip tunnel secure filter name xai_tunnel_lan2"))	return FALSE;
			if (!PutFmt("tunnel enable %d", nTunnelNo))		return FALSE;	
		}
	}
	
	return TRUE;
}

BOOL CDkRouter1200::PutIpsec()
{	
	if ( m_sRouter.nLuaPort )
	{
		//	title
		if (!_PutTit("### IPSEC ###"))
			return FALSE;
		
		for ( int nTunnelNo = m_sPrm.nLuaTunnelMin; nTunnelNo <= m_sPrm.nLuaTunnelMax; nTunnelNo++ ) 
		{
			if (!PutFmt("ipsec transport %d 1%d udp 1701", nTunnelNo, nTunnelNo))
				return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CDkRouter1200::PutOther(char *tit)
{
	char	szBuf[256];
	
	if (!_PutTit(tit))
		return FALSE;	
	
// 	if (!_Put("nat descriptor log on"))
// 		return FALSE;
	
	const char *pCmd01[] = {
		"syslog info on",
		"syslog notice on",
		"syslog debug off",
		"tftp host any",
	};
	if (!_Put(pCmd01, DK_Number(pCmd01)))
		return FALSE;
	
	if ( !m_fOA_LAN ) {	// OALAN時は書かない
		if (!_Put("ipsec auto refresh on"))
			return FALSE;
	}
	
	if ( !_Put("dhcp server rfc2131 compliant except remain-silent") )
		return FALSE;
	
	if ( m_sRouter.fBbUse == eLaboBB_KobetuLan ) 
	{
		wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip), GetIpB2C_Second(m_sRouter.sLAN3.sPriSecIp.mask));
		if (!_Put(szBuf))
			return FALSE;
		
		if (!_Put("no dns server pp"))
			return FALSE;
		
	} else {
		
		//	dns server pp ?
		if ( m_sRouter.m_nVWNo != 0 || m_sRouter.fBbUse )	// VWの設定がないなら何も書きこまない
		{
			for (int n = 0; n < SETU_MAX; n++)
			{
				if ( m_sRouter.con[n].no == m_sRouter.m_nVWNo && m_sRouter.fBbUse == eLaboBB_NoUse ) {	//	VWの時でADSLの設定が無いとき
					wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask));
					if (!_Put(szBuf))
						return FALSE;
					break;
				}
				if ( m_sRouter.con[n].no == m_sRouter.m_nAdslNo && m_sRouter.fBbUse ) {	//	ADSLの設定があるとき
					wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask));
					if (!_Put(szBuf))
						return FALSE;
					break;
				}
			}
//===== ↓ケーブル光対応 ==================================================================	
			if ( m_sRouter.port[1].kaisen != eKaisenCableHikari )
			{
				wsprintf(szBuf, "dns server pp %d", (m_sRouter.m_nAdslNo && m_sRouter.fBbUse) ? m_sRouter.m_nAdslNo : m_sRouter.m_nVWNo);
				if (!_Put(szBuf))
					return FALSE;
			}
		}
	}
	if ( m_sRouter.port[1].kaisen != eKaisenCableHikari )
	{
		if (!_Put("dns private address spoof on"))
			return FALSE;
	}
//===== ↑ケーブル光対応 ==================================================================
	
	if ( m_sRouter.sSIS05.no )  // SIS05通信先設定がある
	{
		for ( int i = 0; i < m_nConnectMax; i++ )
		{
			if ( m_sRouter.sSIS05.no == m_sRouter.ncon[i].no )
			{
				if ( m_sRouter.ncon[i].port == ePortL2 || m_sRouter.ncon[i].port == ePortL2B1 ||
					 m_sRouter.ncon[i].port == ePortL3 || m_sRouter.ncon[i].port == ePortL3B1 )
				{
					if (!_Put("schedule at 1 */* 6:00 * ntpdate 172.31.191.62"))
						return FALSE;
				}
				break;
			}
		}
	}

	if (!_Put("dns service recursive"))
		return FALSE;
//===== ↓クラウドIP電話対応  ==============================================================================
	// DHCPサーバ機能は、複数のルータに設けることはできないので、１台目のルータ（172.16.5.1）のみ設定可能とする。
	// 現時点ではDHCPサーバ機能は無効とする。
	if ( m_sRouter.m_fDHCP && lstrlen(m_szFirstAdd) > 0 && lstrlen(m_szLastAdd) > 0 )
	{
		if (!_Put("dhcp service server"))
			return FALSE;
		
		wsprintf(szBuf, "dhcp scope 1 %s-%s/16", m_szFirstAdd, m_szLastAdd);
		if (!_Put(szBuf))
			return FALSE;
	}
	else
	{
		const char *pCmd[] = {
			"no dhcp service",
			"no dhcp scope 1"
		};
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
	}
	
	if ( m_sRouter.m_nFqdnState == eFqdnLAN1 )
	{
		wsprintf(szBuf, "dhcp scope option 1 dns=172.16.5.1");
		if (!_Put(szBuf))
			return FALSE;
	}
	
	if ( m_sRouter.m_nFqdnState == eFqdnLAN3 )
	{
		if (!_Put("dhcp service server"))
			return FALSE;

		wsprintf(szBuf, "dhcp scope 2 %s-%s/29", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5));
		if (!_Put(szBuf))
			return FALSE;	
		
		wsprintf(szBuf, "dhcp scope option 2 dns=%s", GetIpB2C(m_sRouter.ip_2_cloud));		
		if (!_Put(szBuf))
			return FALSE;
	}
//===== ↑クラウドIP電話対応  ==============================================================================

	// 外部からのICMP攻撃を防ぐため
	if (!_Put("pptp service on"))
		return FALSE;
	
	if ( ADSL_KIND(m_sRouter.port[1].kaisen) == eKaisenFixADSL )
	{
		wsprintf(szBuf, "ip stealth pp %d", m_sRouter.m_nAdslNo);
		if (!_Put(szBuf))
			return FALSE;		
	}
	
	if ( m_sRouter.nLuaPort )
	{
		const char *pCmd[] = {
			"l2tp service on",
			"schedule at 2 startup * lua /xai_check.lua",
			"schedule at 3 */* 04:00:00 * terminate lua all",
			"schedule at 4 */* 04:00:10 * lua /xai_check.lua",
		};
		
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
	}
	
//===== ↓クラウドリニューアル対応 ========================================================
	wsprintf(szBuf, "# LIFT_PHASE=%d", m_sIniFileInf.nStatus);	// コメントはルータへ送信すると消える
	if (!_Put(szBuf))
		return FALSE;
//===== ↑クラウドリニューアル対応 ========================================================
	
//===== ↓LTE通信対応 ==================================================================
	SDkConnectInfoNew *pInfo = GetAdslInfo();
	if ( pInfo && m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE )
	{
		const char *pCmd[] = {
			"ip wan1 address dhcp",
			"ip wan1 secure filter name dk_pp_lan2",
			"ip wan1 intrusion detection in on reject=on",
			"ip wan1 nat descriptor 2000",
			"wan1 bind usb1",
			"wan1 always-on on",
			"wan1 auto connect on",
			"wan1 disconnect time off",
			"wan1 disconnect input time off",
			"wan1 disconnect output time off",
			"wan1 access limit length off",
			"wan1 access limit time off",
			"mobile use usb1 on",
		};
		if (!_Put(pCmd, DK_Number(pCmd)))
			return FALSE;
		
		wsprintf(szBuf, "ip wan1 mtu 1454");	// 固定値
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "wan1 auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "wan1 access-point name %s", m_szWanApiName);	// mobiledata.ntt.com
		if (!_Put(szBuf))
			return FALSE;
		
		SDkConnectInfoNew *pInfo = GetAdslInfo(&m_sPutPreRouter);	// ADSLレコード LTE有効なら必ず設定されているはず
		
		// LTE設定さた場合、L2の記憶用としてDescriptorを使う
		int nKind = 0;
		switch ( m_sPutPreRouter.port[ePortIdxL2].kaisen )
		{
		case eKaisenADSL:
			nKind = 1;
			break;
		case eKaisenFixADSL:
			nKind = 2;
			break;
		case eKaisenCableHikari:
			nKind = 3;
			break;
		}
		
		wsprintf(szBuf, "description 10 \"No=%d Kind=%d Ip=%s MTU=%d Gateway=%s\"", pInfo->no, nKind, GetIpB2C(pInfo->fixIp, pInfo->yobi1), m_sPutPreRouter.nMTU, GetIpB2C_Second(pInfo->yobi2));
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "description 12 \"XaiTactP=%d PH=%s\"", m_sPutPreRouter.nLuaPort, m_sPutPreRouter.szPH_CodeA2910);
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "description 13 \"UserID=%s\"", m_sPutPreRouter.szAdslID);
		if (!_Put(szBuf))
			return FALSE;
		wsprintf(szBuf, "description 14 \"Pass=%s\"", m_sPutPreRouter.szAdslPass);
		if (!_Put(szBuf))
			return FALSE;
	}
//===== ↑LTE通信対応 ==================================================================

//===== ↓DHCP対応 ====================================================================
// すでに書いている
// 	if ( m_sRouter.fUseLan2DHCP ) 
// 	{
// 		const char *pCmd[] = {
// 			"ip lan2 address dhcp",
// 			"ip lan2 secure filter name dk_pp_lan2",
// 			"ip lan2 nat descriptor 2000",
// 		};
// 		if (!_Put(pCmd, DK_Number(pCmd)))
// 			return FALSE;
// 	}
//===== ↑DHCP対応 ====================================================================
	
	const char *pCmd02[] = {
		"description 1 \"configversion=5.0\"",// RTX1000以外は書く
		"save",
	};
	if (!_Put(pCmd02, DK_Number(pCmd02)))
		return FALSE;
	
	return TRUE;
}

// LAN3ポートが設定されているか?
BOOL CDkRouter1200::IsLAN3IpEnable(SDkLan3 *pRtLan3)
{
	if ( pRtLan3->ip[0] == 0 && pRtLan3->ip[1] == 0 && pRtLan3->ip[2] == 0 && pRtLan3->ip[3] == 0 )
	{
		return FALSE;
	}
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	出力すべきフィルターネームを取得する
//	<引数>	char *cmd = 
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter1200::_PutFilterName(char *cmd)
{
	CString strGetLog;
	strGetLog = cmd;
	
	if ( m_dwSecFilOffFlag )
	{	
		strGetLog.TrimRight();
		strGetLog = strGetLog.Left(strGetLog.ReverseFind(' '));
		strGetLog += " getlog";
	} 
	
	return _Put(strGetLog);
}

//----------------------------------------------------------------------------
//	<概要>	リビジョンアッププログラム(.binファイル)をルータへ送信する
//	<引数>	BYTE *pSendIp = 送信先IPアドレス
//          char *szPass  = 指名パスワード
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter1200::PutRevUpFile(BYTE *pSendIp, char *szPass/*=NULL*/)
{
	char szReDir[MAX_PATH];
	char szCmd[512];
	
	//	bin filepath 作成
	char szRevUpFilePath[MAX_PATH];
	wsprintf(szRevUpFilePath, "%s\\%s\\%s", pMxBas->GetBaseDir(), ROUTER_CONFIG_DIR, CDkRouter::GetBinFileName(m_sRouter.kind));
	
	// その".bin"ファイルは存在するのか？
	WIN32_FIND_DATA sWinFind;
	if ( FindFirstFile( szRevUpFilePath, &sWinFind ) == INVALID_HANDLE_VALUE )
	{
		CString str;
		str.Format("%sが存在しないためRevUpできません", szRevUpFilePath);
		DebDisp(str);
		return FALSE;
	}
	
	for (int cnt = 0; cnt < 1 * 10; cnt++, Sleep(100))	// 1秒Wait
		DoIdle(3);
	
	// リダイレクト用ファイル名作成
	wsprintf(szReDir, "%s%s\\%s", pMxBas->GetBaseDir(), ROUTER_TMP_DIR, RT_REDIRECT_FILE);
	DeleteFile(szReDir);	// リダイレクトファイルを消しとく
	
	if ( TRUE ) {//  Configで設定が反映されてるから".bin"の送り先のIPアドレスをかえる
		CopyMemory(pSendIp, m_sRouter.ip, sizeof(BYTE) * IP_BYTE);
	}
	
	// Program send(.binは送れたかどうかのチェックはしない)
	wsprintf(szCmd, "cmd /c tftp -i %s put %s exec > %s", GetIpB2C(pSendIp), szRevUpFilePath, szReDir);
	_Run(szCmd);
	
	int nWait = (m_sRouter.kind == eRT103i) ? 100 : 60;
	for (cnt = 0; cnt < (nWait * 10); cnt++, Sleep(100))	// 60~100秒Wait
		DoIdle(3);
	
	return TRUE;
}

//-----------------------------------------------------------------------------
//	<概要>	TFTPでルータに書き出す
//	<引数>	BYTE *pSendIp		= 送信先IPアドレス
//			char *szSendFilePath= 送信ファイル
//			char *szPass		= 指名パスワード
//	<戻値>	BOOL 1:成功
// 				 0:TFTPコマンド設定ファイル送信エラー
//				-1:TFTPコマンドリビジョンアップエラー
//				-2:送り先のルータと送るデータのルータの機種が異なる
//-----------------------------------------------------------------------------
int CDkRouter1200::PutTftp(BYTE *pSendIp, char *szSendFilePath, char *szPass/*=NULL*/, int nRouterNo/*=0*/)
{
 	if ( m_sRouter.kind == eRTX1000 || m_sRouter.kind == eRTX1000_OA || m_sRouter.kind == eRTX1100 || m_sRouter.kind == eRTX1100_OA )
 	{
 		return CDkRouter1000::PutTftp(pSendIp, szSendFilePath, szPass);
 	}
	
	DebDisp("※ルータへ設定送信処理開始");
	
	CString strMsg;
	BOOL fResult = FALSE;
	
// 	pMxBas->DispMsg("しばらくお待ち下さい");
	
	int nSelRtNo = nRouterNo;
	
	char szIp[20], szSeikouPass[256] = {0};
	char szReDir[MAX_PATH];	// リダイレクトファイル
	const char *pCmdChar = "successful";	// 成功時にリダイレクトファイルに書き込まれる文字列
	
	wsprintf(szIp, "%d.%d.%d.%d", pSendIp[0], pSendIp[1], pSendIp[2], pSendIp[3]);
	
	if ( !IsRtTypeEditable(m_sRouter.kind) ) {
		::MessageBox(m_hwndView, "ルータの種類が不正です！！", "ルータ設定エラー", MB_OK);
		return 0;
	}
	
	if ( m_fSendBin )	// .binファイルも送る？
	{
		//	bin filepath 作成
		char szRevUpFilePath[MAX_PATH];
		wsprintf(szRevUpFilePath, "%s\\%s\\%s", pMxBas->GetBaseDir(), ROUTER_CONFIG_DIR, CDkRouter::GetBinFileName(m_sRouter.kind));
		
		// その".bin"ファイルは存在するのか？
		WIN32_FIND_DATA sWinFind;
		if ( FindFirstFile( szRevUpFilePath, &sWinFind ) == INVALID_HANDLE_VALUE )
		{
			// pMxBas->MessageBox("対象リビジョンアップファイルが存在しません", pMxBas->m_sProgJName, MB_ICONEXCLAMATION);
			return -3;
		}
	}
	
	// リダイレクト用ファイル名作成
	wsprintf(szReDir, "%s%s\\%s", pMxBas->GetBaseDir(), ROUTER_TMP_DIR, RT_REDIRECT_FILE);
	DeleteFile(szReDir);	// リダイレクトファイルを消しとく
	
	//===== ↓追加 ============================================================================
	// →Lan2ポートのポート詳細設定でΧai-TACT認証Pに有効なポート番号が設定されていた場合、”実行可能”な「LUAバイトコード＋パラメータファイル」
	// 　Lan2ポートのポート詳細設定でΧai-TACT認証Pに有効なポート番号が設定されていなかった場合、”無効”な「LUAバイトコード＋パラメータファイル」
	ZeroMemory(m_szSendLuaFile, sizeof(m_szSendLuaFile));
	ZeroMemory(m_szSendLuaChkFile, sizeof(m_szSendLuaChkFile));
	ZeroMemory(m_szSendPrmFile, sizeof(m_szSendPrmFile));
	
	// ”無効”な「LUAバイトコード＋パラメータファイル」ファイル名の作成
	wsprintf(m_szSendLuaFile,	"%s\\%s\\%s\\router\\routerMukou.lua",		CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);	// lua無効ファイルパス
	wsprintf(m_szSendLuaChkFile,"%s\\%s\\%s\\router\\routerChkMukou.lua",	CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);	// luachk無効ファイルパス	
	wsprintf(m_szSendPrmFile,	"%s\\%s\\%s\\router\\routerMukou.prm",		CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);	// prm無効ファイルパス	
	
	if ( m_sRouter.nLuaPort > 0 )
	{	// ”実行可能”な「LUAバイトコード＋パラメータファイル」ファイル名の作成
		char szRouterLuaFile[MAX_PATH];
		char szRouterLuaChkFile[MAX_PATH];
		char szRouterPrmFile[MAX_PATH];
		wsprintf(szRouterLuaFile,	"%s\\%s\\%s\\router\\router.lua",		CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);
		wsprintf(szRouterLuaChkFile, "%s\\%s\\%s\\router\\routerChk.lua",	CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);
		wsprintf(szRouterPrmFile,	 "%s\\%s\\%s\\router\\router%02dW.prm",	CDkMxBas::GetBaseDir(), MX_DIR_DATA, MX_DIR_CM, nRouterNo);
		
		if ( m_cRtPassEx.isExist(szRouterLuaFile) )
			lstrcpy(m_szSendLuaFile, szRouterLuaFile);
		if ( m_cRtPassEx.isExist(szRouterLuaChkFile) )
			lstrcpy(m_szSendLuaChkFile, szRouterLuaChkFile);
		if ( m_cRtPassEx.isExist(szRouterPrmFile) )
			lstrcpy(m_szSendPrmFile, szRouterPrmFile);
	}
	
	while ( TRUE )
	{
		// 存在しないIPを指定すると５分ぐらい固まってしまうためあらかじめpingで確かめる
		CString sMac;
		DWORD dwRet = CDkRoutePass::getMacAdress(szIp, sMac);
		if ( dwRet != ERROR_SUCCESS )
		{
			CString strErrMsg;
			if ( dwRet == ERROR_BAD_NET_NAME )
				strErrMsg = "指定されたIPアドレスの機器が見つかりませんでした";
			else
				strErrMsg.Format("getMacAdress errorcode=%d", dwRet);
			
			DebDisp(strErrMsg);
			break;	//  失敗
		}
#if 1
		
		BOOL fSendLua = TRUE;
		
#if A2910_PH_INP_SHOW
		CString str;
		str.Format("●nLuaPort=%d IsXai=%d", m_sRouter.nLuaPort, IsXai() );
		OutputDebugString(str);
		
		if ( GetReleaseVersion() < A3000 || !IsXai() )
		{
//  		if ( m_sRouter.nLuaPort <= 0 )
//  		{
			fSendLua = FALSE;	// C2はLUAを絶対に送らない
//  		}
		}
#endif
		if ( m_sRouter.kind >= eRTX1200 && fSendLua )
		{
 			// configファイルを先に送るとパスワードが変わる可能性があるため、Lua,prmを先に送る(config送信後すぐにSWCへWM_802が飛んでRouterInfo.inf更新される)
 			DebDisp("%s送信処理開始", m_szSendLuaFile);
 			if ( !m_cRtPassEx.RouterSendRecv(TRUE, szIp, "/xai.lua", m_szSendLuaFile, NULL, szSeikouPass) )	// 成功した場合szSeikouPassに記憶し、そのパスワードで他のファイルも送る
 			{
 				strMsg.Format("%sの送信に失敗しました", m_szSendLuaFile);
 				break;	//  失敗
 			}
 			DoIdle(2000, TRUE);	// sleepつき	1000=10秒

 			DebDisp("%s送信処理開始", m_szSendLuaChkFile);
 			if ( !m_cRtPassEx.RouterSendRecv(TRUE, szIp, "/xai_check.lua", m_szSendLuaChkFile, szSeikouPass) )
			{
 				strMsg.Format("%sの送信に失敗しました", m_szSendLuaChkFile);
 				break;	//  失敗
 			}
 			DoIdle(2000, TRUE);	// sleepつき

 			DebDisp("%s送信処理開始", m_szSendPrmFile);
 			if ( !m_cRtPassEx.RouterSendRecv(TRUE, szIp, "/xai.prm", m_szSendPrmFile, szSeikouPass) )
 			{
 				strMsg.Format("%sの送信に失敗しました", m_szSendPrmFile);
 				break;	//  失敗
 			}
 			DoIdle(2000, TRUE);	// sleepつき
 		}	
#endif		
		DebDisp("%s送信処理開始", szSendFilePath);
		if ( !m_cRtPassEx.RouterSendRecv(TRUE, szIp, "config", szSendFilePath, NULL, szSeikouPass) )
		{
			strMsg.Format("%sの送信に失敗しました", szSendFilePath);
			break;	//  失敗
		}	
		DoIdle(2000, TRUE);	// sleepつき
		
		if ( !m_cRtPassEx.MakePassInfoFile(CIntStr("%d.%d.%d.%d", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], m_sRouter.ip[3]), m_strNewLoginPass, m_strNewAdminPass) )
		{
			strMsg = "暗号化パスワード情報格納ファイルの更新に失敗しました";
			break;	//  失敗
		}
		
		DoIdle(30);
		if ( noAngoPass < 2 && m_sRouter.kind >= eRTX1200 && m_nRegRdpPort > 0 && (m_cRtPassEx.m_nMcnType == eDkMachineMS55 || m_cRtPassEx.m_nMcnType == eDkMachineMS77) )
		{
			if ( !m_cRtPassEx.WriteRegRdpPort(m_nRegRdpPort) )	// HKEY_LOCAL_MACHINE\SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp\PortNumber
			{
				strMsg = "ＲＤＰのポート番号のレジストリに書き込みに失敗しました";
				break;	//  失敗
			}
			DebDisp("ＲＤＰのポート番号のレジストリに書き込みに成功(%d)", m_nRegRdpPort);
		}
		
		DoIdle(30);
		fResult = TRUE;
		break;
	}
	
	if ( !fResult )
	{
		DebDisp("ルータ設定送信に失敗しました(%s)", strMsg);
		return 0;
	}
	//===== ↑追加 ============================================================================
	
	if ( m_fSendBin )	// .binファイルも送る？
	{	
		if ( !PutRevUpFile(pSendIp, szSeikouPass) )
			return -1;
	} else {
		//	telnet で restart set
		CString szIP;
		szIP.Format("%d.%d.%d.%d", pSendIp[0], pSendIp[1], pSendIp[2], pSendIp[3]);
		
		DebDisp("Telnetでルータ再起動コマンド発行");
		for (int wait = 0; wait < 150; wait++, Sleep(100))	// wait秒
			DoIdle(10);	// sleep100
		TelnetRestart(szIP, szSeikouPass);	// ルータ再起動
	}
	
	DebDisp("ルータ設定送信処理完了");
	
	return 1;	// 成功
}

//----------------------------------------------------------------------------
//	<概要>	TFTPでルータから取得する
//	<引数>	int  &no      = ルータ種類番号
//          char *ip	  = 取得先ルータ端末のIPアドレス
//          BOOL fKindGet = ルータの機種の種類だけが知りたい場合はTRUE
//          char *szPass  = パスワード
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter1200::GetTftp(int &no, char *ip, BOOL fKindGet/*=FALSE*/, char *szPass/*=NULL*/)
{	
	DebDisp("※ルータから設定受信処理開始");
	
	BOOL fResult = FALSE;
	char szSeikouPass[256];
	char szIp[50];	// IPアドレスの文字列作成
	char szBuf[2048];	
	CString strRecvFile[10][2];
	
	int nSelRtNo = no;
	
	if ( !fKindGet )
	{
		ZeroMemory(&m_sRouter, sizeof(m_sRouter));	// ルータ構造体を初期化
		m_sRouter.m_nVWNo	= 0;	// 初期化
		m_sRouter.m_nSISNo	= 0;
		m_sRouter.m_nRoboNo	= 0;
	}
	
	// configファイル
	int n = 0;
	strRecvFile[n++][0].Format("%s\\%s\\%s\\%s", CDkMxBas::GetBaseDir(), MX_DIR_TMP, MX_DIR_CM, RT_RECV_FILE);
// 	strRecvFile[n++][0].Format("%s\\%s\\router%02dW.lua",		CDkMxBas::GetBaseDir(), ROUTER_FILE_DIR, nSelRtNo);	//config\cm\routerの決めうちファイルを送信する
// 	strRecvFile[n++][0].Format("%s\\%s\\router_chk%02dW.lua",	CDkMxBas::GetBaseDir(), ROUTER_FILE_DIR, nSelRtNo);	//config\cm\routerの決めうちファイルを送信する
// 	strRecvFile[n++][0].Format("%s\\%s\\router%02dW.prm",		CDkMxBas::GetBaseDir(), ROUTER_FILE_DIR, nSelRtNo);	// <-送信時つくる
	n = 0;
	strRecvFile[n++][1] = "config";
// 	strRecvFile[n++][1] = "/xai.lua";
// 	strRecvFile[n++][1] = "/xai_check.lua";
// 	strRecvFile[n++][1] = "/xai.prm";
	
	wsprintf(szIp, "%s", ip);
	
	while ( TRUE )
	{
		// 存在しないIPを指定すると５分ぐらい固まってしまうためあらかじめpingで確かめる
		CString sMac;
		DWORD dwRet = CDkRoutePass::getMacAdress(szIp, sMac);
		if ( dwRet != ERROR_SUCCESS )
		{
			CString strErrMsg;
			if ( dwRet == ERROR_BAD_NET_NAME )
				strErrMsg = "指定されたIPアドレスの機器が見つかりませんでした";
			else
				strErrMsg.Format("getMacAdress errorcode=%d", dwRet);
			
			DebDisp(strErrMsg);
			break;	//  失敗
		}
		
		for ( int i = 0 ; i < DK_Number(strRecvFile[i][0]); i++ ) 
		{
			if ( strRecvFile[i][1].IsEmpty() || strRecvFile[i][0].IsEmpty() )
				continue;
			DWORD dwAttri = GetFileAttributes(strRecvFile[i][0]);
			if ( dwAttri != 0xFFFFFFFF ) {	// 読み取り専用でなくする
				dwAttri &= 0xFFFFFFFE;
				SetFileAttributes(strRecvFile[i][0], dwAttri);
			}
		}
		
		// 数パターンのパスワード試行する
		if ( !m_cRtPassEx.RouterSendRecv(FALSE, szIp, "config", (CStoS)strRecvFile[0][0], NULL, szSeikouPass) )
		{
			DebDisp("ルータからの受信に失敗しました");
			break;
		}
		
		fResult = TRUE;	// 成功
		break;
	}
	
	if ( !fResult )
	{
		DebDisp("ルータからの受信に失敗しました");
		return FALSE;
	}
	
	CString str;
	if ( isMentMod() )
	{
		str.Format("CDkRouter::GetTftp pass=%s", szPass);
		DebDisp(str);
	}
	
	if ( !fKindGet )
	{
		ZeroMemory(&m_sRouter, sizeof(m_sRouter));	// ルータ構造体を初期化
		m_sRouter.m_nVWNo	= 0;	// 初期化
		m_sRouter.m_nSISNo	= 0;
		m_sRouter.m_nRoboNo	= 0;
	}
	
	lstrcpy(m_szFileName, (CStoS)strRecvFile[0][0]);
	
	CFileStatus	cStat;
	CFile::GetStatus(m_szFileName, cStat);  // ファイルステータス取得
	SYSTEMTIME	st;
	GetLocalTime(&st);						// システム時刻をゲット
	CTime cSysTime(st);
	CTimeSpan sp(0,0,0,10);
	
	SYSTEMTIME stt;
	cStat.m_mtime.GetAsSystemTime(stt);
	
	// 現時刻より１０秒以内に作成されたのであればＯＫ
	if ( cStat.m_mtime > (cSysTime - sp) && cStat.m_mtime < (cSysTime + sp) )
	{
		if ( fKindGet ) // ルータの機種の種類だけが知りたいとき
		{
			CDkFileRap cFile(eReportRead | eReportWrite);
			if ( !cFile.Open(m_szFileName, CFile::modeRead) )
				return FALSE;
			
			cFile.Read(szBuf, sizeof(szBuf));	// 最初の一行を読む
			for ( int i = 0; i < DK_Number(szRtType); i++ )
			{
				if ( strstr(szBuf, szRtType[i]) != NULL )
				{
					no = i+1;	// ルータの種類をセット
					break;
				}
			}
			if ( i == DK_Number(szRtType) )
			{
				return FALSE;	// 検索失敗
			}
		}
		else
		{
			return ReadTftpFile(no, m_szFileName);
		}
	}
	else
	{
// 		if ( isMentMod() )
// 		{
// 			str.Format("pass=%sでの設定受信に失敗しました", szPass);
// 			DebDisp(str);
// 		}
// 		
// 		// デフォルトパスワードでの取得失敗 or 指名パスワードの場合は即エラー
// 		if ( lstrcmp(pRouterPass, RT_PASS) == 0 )
// 			return FALSE;
// 		
// 		int no = 0;
// 		if ( !GetTftp(no, ip, fKindGet, RT_PASS) )	// デフォルトパスでＧＥＴ処理をしてみる
// 			return FALSE;
// 		
// 		if ( szPass != NULL ) {
// 			lstrcpy(pRouterPass, RT_PASS);	// デフォルトでいけるみたい
// 			return TRUE;
// 		}
		
		return FALSE;
	}
	
	return TRUE;
}

//	TFTPでルータから取得する
BOOL CDkRouter1200::GetTftp(int &no, BYTE *ip, BOOL fKindGet/*=FALSE*/, char *szPass/*=NULL*/)
{
	char szIp[32];
	wsprintf(szIp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	return GetTftp(no, szIp, fKindGet, szPass);
}

//----------------------------------------------------------------------------
//	<概要>	レジストリデータ読み込み
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter1200::ReadRegstryData()
{
	// リモートアドレス
	ZeroMemory(m_szRMAdd, sizeof(m_szRMAdd));
	ZeroMemory(m_bRMAdd, sizeof(m_bRMAdd));
	m_fMxRMAddRead = FALSE;	// レジストリ"RMAddress"をちゃんと読み込んでいるのか？
	if ( CDkMxBas::MxRegRead("RMAddress", (BYTE*)m_szRMAdd, sizeof(m_szRMAdd), "") )
	{
		if ( lstrlen(m_szRMAdd) > 0 )
			m_fMxRMAddRead = TRUE;	// 不定ではない
		else
			m_fMxRMAddRead = FALSE;	// ブランク＝不定
		ConvertIpDelmit(m_szRMAdd, m_bRMAdd);
	}
	
	// セキュリティフィルタ解除			セキュリティフィルタを無効とし、全ての通信を通すようにします。
	m_dwSecFilOffFlag = 0;
	DWORD dwSecFilOffFlag;
	if ( CDkMxBas::MxRegRead("FilterOffFlag", (BYTE*)&dwSecFilOffFlag, sizeof(dwSecFilOffFlag), "") )
	{
		m_dwSecFilOffFlag = dwSecFilOffFlag;
	}
	
	// デフォルトルート				デフォルトルート（不定IPアドレスに対するルーティング）を有効にします。
	m_dwDefRootFlag = 0;
	DWORD dwDefRootFlag;
	if ( CDkMxBas::MxRegRead("DefaultRootFlag", (BYTE*)&dwDefRootFlag, sizeof(dwDefRootFlag), "") )
	{
		m_dwDefRootFlag = dwDefRootFlag;
	}
	
	// 2拠点間の橋渡し			直接通信できない2拠点間（10.0.0.0/8〜10.0.0.0/8）を本ルータで橋渡しします。
	m_dwViaRootFlag = 0;
	DWORD dwViaRootFlag;
	if ( CDkMxBas::MxRegRead("ViaRootFlag", (BYTE*)&dwViaRootFlag, sizeof(dwViaRootFlag), "") )
	{
		m_dwViaRootFlag = dwViaRootFlag;
	}
	
	// 旧SISLabo閲覧			旧SISLabo閲覧に対応します。
	// 旧SISLaboの証明書更新	旧SISLaboの証明書更新に対応します。
	// DELTANET閲覧				DELTANET閲覧に対応します。
	// DELTANETの証明書更新		DELTANETの証明書更新に対応します。
	m_dwSISAddressFlag = 3;	// デフォルト値(読み込めなかった場合)は　3（下1bit目=1、下2bit目=1、下3bit目以上=0）
	DWORD dwSISAddressFlag;
	if ( CDkMxBas::MxRegRead("SISAddressFlag", (BYTE*)&dwSISAddressFlag, sizeof(dwSISAddressFlag), "") )
	{
		m_dwSISAddressFlag = dwSISAddressFlag;
	}
	
	m_dwDnsFlag	 = 0;	// DNSサーバ接続
	m_dwPop3Flag = 0;	// POP3サーバ接続
	m_dwSmtpFlag = 0;	// SMTPサーバ接続
	
	// DNSサーバ接続
	DWORD dwDnsFlag;
	if ( CDkMxBas::MxRegRead("DnsFlag", (BYTE*)&dwDnsFlag, sizeof(dwDnsFlag), "") )
	{
		m_dwDnsFlag = dwDnsFlag;
	}
	
	// POP3サーバ接続
	DWORD dwPop3Flag;
	if ( CDkMxBas::MxRegRead("Pop3Flag", (BYTE*)&dwPop3Flag, sizeof(dwPop3Flag), "") )
	{
		m_dwPop3Flag = dwPop3Flag;
	}
	
	// SMTPサーバ接続
	DWORD dwSmtpFlag;
	if ( CDkMxBas::MxRegRead("SmtpFlag", (BYTE*)&dwSmtpFlag, sizeof(dwSmtpFlag), "") )
	{
		m_dwSmtpFlag = dwSmtpFlag;
	}
	
	// Dhcpアドレス
	ZeroMemory(m_szFirstAdd, sizeof(m_szFirstAdd));
	ZeroMemory(m_szLastAdd, sizeof(m_szLastAdd));
	CDkMxBas::MxRegRead("DhcpFirstAddress", (BYTE*)m_szFirstAdd, sizeof(m_szFirstAdd), "");
	CDkMxBas::MxRegRead("DhcpLastAddress",  (BYTE*)m_szLastAdd,  sizeof(m_szLastAdd),  "");
	
	DebDisp("ReadReg  RMAddress=%s  FilterOffFlag=%d  DefaultRootFlag=%d  ViaRootFlag=%d  SISAddressFlag=%d  DnsFlag=%d  Pop3Flag=%d  SmtpFlag=%d DhcpFirstAddress=%s DhcpLastAddress=%s", 
					  m_szRMAdd,	m_dwSecFilOffFlag,m_dwDefRootFlag,    m_dwViaRootFlag,m_dwSISAddressFlag,m_dwDnsFlag,m_dwPop3Flag,m_dwSmtpFlag, m_szFirstAdd, m_szLastAdd);
	return TRUE;
}

//----------------------------------------------------------------------------
//	<概要>	レジストリデータ書き込み
//	<戻値>	TRUE = 成功    FALSE = 失敗
//----------------------------------------------------------------------------
BOOL CDkRouter1200::WriteRegstryData(BOOL fRMAddBrank/*=FALSE*/)
{
	// リモートアドレス
// 	if ( m_fMxRMAddRead )	// レジストリ"RMAddress"をちゃんと読み込んでいるのか？
// 	{
	if ( fRMAddBrank ) {
		ZeroMemory(m_szRMAdd, sizeof(m_szRMAdd));
		m_fMxRMAddRead = FALSE;	// 不定
	} else {
		wsprintf(m_szRMAdd, "%d.%d.%d.%d", m_bRMAdd[0], m_bRMAdd[1], m_bRMAdd[2], m_bRMAdd[3]);
		m_fMxRMAddRead = TRUE;	// ちゃんと入っている
	}
	
	if ( !CDkMxBas::MxRegWrite("RMAddress", REG_SZ, (BYTE*)m_szRMAdd, lstrlen(m_szRMAdd), NULL) ) {
		m_fMxRMAddRead = FALSE;
		return FALSE;
	}
// 	}
	
	// セキュリティフィルタ解除			セキュリティフィルタを無効とし、全ての通信を通すようにします。
	if ( !CDkMxBas::MxRegWrite("FilterOffFlag", REG_DWORD, (BYTE*)&m_dwSecFilOffFlag, sizeof(DWORD), NULL) )
		return FALSE;
	
	// デフォルトルート				デフォルトルート（不定IPアドレスに対するルーティング）を有効にします。
	if ( !CDkMxBas::MxRegWrite("DefaultRootFlag", REG_DWORD, (BYTE*)&m_dwDefRootFlag, sizeof(DWORD), NULL) )
		return FALSE;
	
	// 2拠点間の橋渡し			直接通信できない2拠点間（10.0.0.0/8〜10.0.0.0/8）を本ルータで橋渡しします。
	if ( !CDkMxBas::MxRegWrite("ViaRootFlag", REG_DWORD, (BYTE*)&m_dwViaRootFlag, sizeof(DWORD), NULL) )
		return FALSE;
	
	// 旧SISLabo閲覧			旧SISLabo閲覧に対応します。
	// 旧SISLaboの証明書更新	旧SISLaboの証明書更新に対応します。
	// DELTANET閲覧				DELTANET閲覧に対応します。
	// DELTANETの証明書更新		DELTANETの証明書更新に対応します。
	if ( !CDkMxBas::MxRegWrite("SISAddressFlag", REG_DWORD, (BYTE*)&m_dwSISAddressFlag, sizeof(DWORD), NULL) )
		return FALSE;
	
	// DNSサーバ接続
	if ( !CDkMxBas::MxRegWrite("DnsFlag", REG_DWORD, (BYTE*)&m_dwDnsFlag, sizeof(DWORD), NULL) )
		return FALSE;
	
	// POP3サーバ接続
	if ( !CDkMxBas::MxRegWrite("Pop3Flag", REG_DWORD, (BYTE*)&m_dwPop3Flag, sizeof(DWORD), NULL) )
		return FALSE;
	
	// SMTPサーバ接続
	if ( !CDkMxBas::MxRegWrite("SmtpFlag", REG_DWORD, (BYTE*)&m_dwSmtpFlag, sizeof(DWORD), NULL) )
		return FALSE;
	
	DebDisp("ReadReg  RMAddress=%s  FilterOffFlag=%d  DefaultRootFlag=%d  ViaRootFlag=%d  SISAddressFlag=%d  DnsFlag=%d  Pop3Flag=%d  SmtpFlag=%d", 
					  m_szRMAdd,	m_dwSecFilOffFlag,m_dwDefRootFlag,    m_dwViaRootFlag,m_dwSISAddressFlag,m_dwDnsFlag,m_dwPop3Flag,m_dwSmtpFlag);
	
	return TRUE;
}

