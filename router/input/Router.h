/******************************************************************************/
//	ファイル名			Router.h
//	システム名			ＤＫ−Ｍｘ （共通）
//	プログラム種別		ルータ操作クラス
//	概要				ルータ操作クラス定義
//	作成者				林エンジニアリング
//	バージョン				02/01/30				R 1.000
/*******************************************************************************
	改版履歴
*/
#include "MxServer.h"
#include "MxCommon.h"
#include "MfcRap.h"

#ifndef	_ROUTER_H
#define	_ROUTER_H

#define	ROUTER_FILE_DIR		"\\data\\cm\\router"
#define	ROUTER_TMP_DIR		"\\tmp\\cm"
#define	ROUTER_CONFIG_DIR	"\\config\\CM\\router"

#define	MX_RT_INFO			"RouterInfo.inf"
#define	MX_RT_LIST			"RouterList.inf"

#define RT_REDIRECT_FILE	"CMS0330.txt"			// ルータ送信時リダイレクトファイル
#define RT_SEND_FILE		"routerSEND.dat"		// ルータへ設定送信するためのファイル
#define RT_RECV_FILE		"routerRECV.dat"		// ルータから設定受信するためのファイル

#define	REOMOTE_MENT_IP		"10.0.192.0/20"
#define	REOMOTE_MENT_IP2	"10.0.208.0/20"
#define	REOMOTE_MENT_IP_AT	"10.0.192.0/24"
#define	REOMOTE_MENT_IP2_AT	"10.0.208.0/24"

#define	SIS_IP				"172.30.1.110"
#define SIS05_IP			"172.31.191.0/24"
#define KEIHIN_MOLE			"202.229.106.177"	// Faith（DELTANET）のインタネット公開Webサーバ「景品モール」のIP
#define SIS_TEL				"0422703570"		// "0522290150"
#define SIS05_TEL			"0422799015"		// "0522290150"
#define	RT_PASS				"********"			// デフォルトパスワード
#define	ROBO_IP				"172.30.47.254"
#define	ROBO_TEL			"0422799015"		// ロボ認識用電話番号
#define	ROBO_ACNT			"dk******"			// ロボユーザID
#define	ROBO_PASS			"dk******"			// ロボパスワード
#define	VUP_IP				"172.31.65.0/24"	// Verupプログラム配信サーバ
#define IP_TEL_IP			"172.21.0.0/16"		// IP電話のＩＰ
#define IP_CAR_MENT_IP		"10.0.240.0/24"		// 車両管理システムメンテナンス用設定のＩＰ
#define IP_JNET_MAIN_IP		"153.150.112.121"	// J-NETのメインFTPサーバ
#define IP_JNET_SUB_IP		"153.150.112.122"	// J-NETのバックアップFTPサーバ
#define IP_MG_WEB_IP		"113.52.158.128/25"	// MGWEBのＩＰ 旧"113.52.158.167"
#define IP_JNET_KOKYAKU_IP	"10.0.241.0/24"		// J-NETメンテのＩＰ

#define ROUTERNUM_MAX		10		// 選択できるルータ数

#define PORT_MAX			4		// ルータのポート数
#define REV_LEN				10+1	// REVの文字長
#define IP_BYTE				4		// ＩＰアドレスのバイト数
#define	TEL_SUBADR			26		// 電話番号/サブアドレスの文字長
#define SETU_MAX			100		// 接続数
#define	LPLIN_MAX			26
// 電話番号のEDITBOXについては電話番号１１桁以上、リモートホスト名１０桁以内となるようにする
#define TEL_LEN_PHS			11		// 電話番号が成立する最小文字数(PHSの電話番号は11桁以上)
#define TEL_LEN_DIAL		10		// 電話番号が成立する最小文字数(普通の電話番号は10桁以上)
#define TELADD_MAX			22		// 接続先電話番号に入力できる最大文字列数
#define HALL_LEN_MAX1		8		// ホールコードに入力できる最大文字列数　その１
#define HALL_LEN_MAX2		9		// ホールコードに入力できる最大文字列数　その２
#define PASS_LEN_MAX		12		// パスワード、VPNパスワードの最大文字列数
#define MTU_MIN				64		// MTU値の最小
#define MTU_MAX				1500	// MTU値の最大
#define JNET_HOUJIN_LEN_MAX	6		// JNET会社コード文字数最大
#define JNET_HALL_LEN_MAX	4		// JNETホールコード文字数最大
#define JNET_VPN_LOCAL_MAX	16		// JNET vpn local name文字数最大
#define INPUT_USER_MAX		(50+1)	// USERID最大文字長
#define INPUT_PASS_MAX		(50+1)	// パスワード最大文字長
#define TUNNEL_DEF_MAX		(200)	// ルータ本体のトンネル定義数最大

#define DELIMIT				','		// ファイルへ書き込む時のデリミタ
#define	RT100i				"RT100i"
#define	RT103i				"RT103i"
#define	RT52p				"RT52pro"
#define	RTA54i				"RTA54i"
#define	RTA55i				"RTA55i"
#define	RTA52i				"RTA52i"
#define RT57i				"RT57i"
#define RT58i				"RT58i"
#define	RT200i				"RT200i"
#define RTX1000				"RTX1000"
#define RTX1000_OA			"RTX1000-OA"
#define RTX1100				"RTX1100"
#define RTX1100_OA			"RTX1100-OA"
#define RTX1200				"RTX1200"
#define RTX1200_OA			"RTX1200-OA"
#define RTX1210				"RTX1210"
#define RTX1210_OA			"RTX1210-OA"
#define RTX830				"RTX830"
#define RTX830_OA			"RTX830_OA"
#define RTX1220				"RTX1220"
#define RTX1220_OA			"RTX1220_OA"

#define RT57or58			((m_sRouter.kind == eRT57i || m_sRouter.kind == eRT58i))
#define	A3000				3000	//  A3.000
#define	SEARCH_CHR			" \t\n"

#define RT_CMD_LINE_MAX		4096

#define CMS0334_INI_PATH	"\\data\\CM\\router\\LiftSwitch\\CMS0334.ini"
#define CMB0490_INI_PATH	"\\data\\CM\\router\\LiftSwitch\\CMB0490.ini"
#define FQDN_DAT_PMS_PATH	 "D:\\pms\\config\\dms\\InsNumberInf2.dat"
#define FQDN_DAT_REF_PATH	"\\data\\CM\\router\\LiftSwitch\\InsNumberInf2.dat"

enum {
	// 構文解析結果
	eSyntaxErr	= -1,	//  構文エラー
	eNon		=  0,	//	データ無し	0

	eRead = 0,
	eWrite,

	// ルータ種類
	eRT100i		=  1,	//	RT100i		1
	eRT103i,			//	RT103i		2
	eRT52pro,			//	RT52pro		3
	eRTA54i,			//	RTA54i		4
	eRTA55i,			//  RTA55i		5	
	eRTA52i,			//  RTA52i		6
	eRT57i,				//  RT57i		7
	eRT200i,			//	RT200i		8
	eRTX1000,			//  RTX1000		9
	eRTX1000_OA,		//  RTX1000-OA	10
	eRTX1100,			//  RTX1100		11
	eRTX1100_OA,		//  RTX1100-OA	12
	eRT58i,				//  RT58i		13
	eRTX1200,			//  RTX1200		14
	eRTX1200_OA,		//  RTX1200-OA	15
	eRTX1210,			//  RTX1200		16
	eRTX1210_OA,		//  RTX1200-OA	17
	eRTX830,			//  RTX830		18
	eRTX830_OA,			//  RTX830-OA	19
	eRTX1220,			//  RTX1220		20
	eRTX1220_OA,		//  RTX1220-OA	21
	
	eRTmax = eRTX1220_OA,
	
	eRtTypeISDN = 0,	// ＩＳＤＮルータ
	eRtTypeBB,			// ブロードバンドルータ
	eRtType1000,		// IPSEC対応ルータ
	eRtType1000_OA,		// IPSEC対応ルータ_OA
	eRtType1200,		// IPSEC対応ルータ
	eRtType1200_OA,		// IPSEC対応ルータ_OA
	eRtType830,			// IPSEC対応ルータ
	eRtType830_OA,		// IPSEC対応ルータ_OA
// 	eRtType1220,		// IPSEC対応ルータ
// 	eRtType1220_OA,		// IPSEC対応ルータ_OA

	eUseC		= 0,	//	Ｃ店
	eUseRemote	= 1,	//	リモート
	eUseHonbu	= 2,	//	本部
	
	// 回線の種類
	eKaisenNoEdit	=99,// 使用不可
	eKaisenBlank	= 0,// ブランク
	eKaisenINS64,		// INS64
	eKaisenADSL,		// ADSL
	eKaisenFixADSL,		// ADSL固定IP
	eKaisenLAN,			// 個別LAN
	eKaisenIP_TEL,		// IP電話
 	eKaisenCableHikari,	// ケーブル光
	eKaisenLTE,			// LTE
	
	// 回線ポート情報格納位置
	ePorIdxL1 = 0,
	ePorIdxL2,
	ePorIdxL3,
	ePorIdxUSB1,

	// 接続形態
	eOther	= -1,		// その他
	eDialUp = 0,		// DialUp					0
	ePHS1,				// PHS1						1
	ePHS2,				// PHS2						2
	ePHS3,				// PHS3						3
	ePHS4,				// PHS4						4
	ePHS5,				// PHS5						5
	ePHS6,				// PHS6						6
	ePHS7,				// PHS7						7
	ePHS8,				// PHS8						8
	ePHS9,				// PHS9						9
	ePHS10,				// PHS10					10
	ePHS11,				// PHS11					11
	ePHS12,				// PHS12					12
	ePHS13,				// PHS13					13
	ePHS14,				// PHS14					14
	ePHS15,				// PHS15					15
	ePHS16,				// PHS16					16
	ePHSfree,			// PHSフリー					17
	eSIS,				// SIS通信					18
	eVw,				// Labo閲覧-INS				19
	eRobo,				// ロボメール送信				20
	eADSL,				// ADSL接続					21
	eFixADSL,			// ADSL固定IP接続			22
	eDKVPN,				// DKVPN					23
	eDKVPN_DialUp,		// DKVPN+Dialup				24
	eKobetuVPN,			// 個別VPN					25
	eKobetuVPN_DialUp,	// 個別VPN+Dialup			26
	eSIS05,				// SIS05通信				27
	eDeltaNetBk,		// DELTANETバックアップ		28
	eIpTel,				// PTC接続(IP電話通信)		29
	eCarMent,			// 車両管理システムメンテナンス		30
	ePHSVPN,			// PHSVPN					31
	eIpTelBk,			// PTCバックアップ(IP電話通信BK)32
	eJnetMain,			// JNETメイン				33
	eJnetSub,			// JNETサブ					34
	eCableHikari,		// ケーブル光				35
	eNoConnect,			// 未接続定義				36
	eNoConnectSub,		// 未接続定義バックアップ		37
	eJnetKokyakuMent,	// JNETメンテ				38

	// MIRAIGATE接続先設定のコンボ
	eCmbRobo = 0,		// ロボ処理番号
	eCmbSis,			// SIS処理番号
	eCmbDeltaBk,		// DELTANETバックアップ処理番号
	eCmbVw,				// VW処理番号
	eCmbAdsl,			// ADSL処理番号
	eCmbSis05,			// SIS05処理番号
	eCmbContDL,			// MT号機番号処理番号
	eCmbIpTel,			// IP電話通信処理番号
	eCmbCarMent,		// 車両管理メンテナンス
	eCmbJnetMain,		// JNET接続先メイン
	eCmbJnetSub,		// JNET接続先サブ

	// ポート
	ePortB1 = 1,
	ePortL2,
	ePortL2B1,
	ePortL3,
	ePortL3B1,
	ePortUSB1,
	
	ePortIdxL1 = 0,
	ePortIdxL2,
	ePortIdxL3,
	ePortIdxUSB1,

	// LAN3通信内容
	eL3MS55Folder = 0,	// MS-55の共有フォルダ公開
	eL3MS55FTP,			// MS-55へのFTPアクセス
	eL3MS55DB,			// MS-55へのDBアクセス
	eL3CSV,				// CSV出力
	eL3KobetuVPN,		// 個別VPN
	eL3Domain,			// ドメイン名の解決
	eL3SisLabo,			// SISLabo閲覧
	eL3SisLaboSign,		// SISLaboの証明書更新
	eL3Mobile,			// モバイル通信
	eL3XaiTact,			// Χai-TACT接続
	eL3NaiyoMax,
	
	// Labo閲覧-BB接続
	eLaboBB_NoUse = 0,	// 0=使わない(ブランク)
	eLaboBB_Adsl,		// 1=ADSL
	eLaboBB_FixAdsl,	// 2=ADSL固定
	eLaboBB_KobetuLan,	// 3=個別LAN
	
	// メンテナンス接続先設定
	eMentNoUse = 0,		// 0=使わない(ブランク)
	eMentAnonymous,		// 1=Anonymous
	eMentFixADSL,		// 2=ADSL
	eMentRemoteVpn,		// 3=リモートVPN
	eMentAuto,			// 4=auto
	
	// レジストリ内容
	eRegSecFilter = 0,	// セキュリティフィルタ解除
	eRegDefaultRoot,	// デフォルトルート
	eReg2PointBridge,	// 2拠点間の橋渡し
	eRegOldSis,			// 旧SISLabo閲覧
	eRegOldSisSign,		// 旧SISLaboの証明書更新
	eRegDELTANET,		// DELTANET閲覧
	eRegDELTANET_Sign,	// DELTANETの証明書更新
	eRegDNS_Server,		// DNSサーバ接続
	eRegPOP3_Server,	// POP3サーバ接続
	eRegSMTP_Server,	// SMTPサーバ接続
	eRegMax,
	
	// IP電話セット位置
	eIpTelNon = 0,		// どちらにもセットしてない
	eIpTelLAN1,			// LAN1にセットした
	eIpTelLAN3,			// LAN3にセットした

	eIpTelBkNon = 0,	// どちらにもセットしてない
	eIpTelBkLAN1,		// LAN1にセットした
	eIpTelBkLAN3,		// LAN3にセットした

	eFqdnNon = 0,
	eFqdnLAN1,
	eFqdnLAN3,
};

#define ADSL_KIND(x) ((x) == eKaisenCableHikari ? eKaisenFixADSL : (x))
#define CABLE_HIKARI_CODE	(99)

#include "RoutePassEx.h"
//----- ↓構造体定義 ----------------------------------------------------------------------
// ポート設定
struct SDkPortInfo {
	BYTE	kaisen;					// 回線の種類(-1:使用不可 0:ブランク 1:INS64 2:ADSL 3:ADSL固定IP 4:個別LAN)
	char	Tel[TEL_SUBADR];		// 電話番号/サブアドレス(10桁/10桁)
};

// 従来からある接続先設定
struct SDkConnectInfo {
	short	no;						// 接続先番号
	BYTE	ip[IP_BYTE];			// 接続先のIPアドレス
	BYTE	mask[IP_BYTE];			// 接続先のNetmask
	char	Tel[TEL_SUBADR];		// 接続先の電話番号/サブアドレス(10桁/10桁)
	BYTE	port;					// 接続先のポート(1〜4)
	BYTE	sakuzyo;				// 削除(0.ブランク 1.削除)
};

// 従来からある接続先設定+新規追加
struct SDkConnectInfoNew {
	//------- 従来からある接続先設定メンバ変数 -------	
	short	no;						// 接続先番号
	BYTE	ip[IP_BYTE];			// 接続先のIPアドレス
	BYTE	mask[IP_BYTE];			// 接続先のNetmask
	char	Tel[TEL_SUBADR];		// 接続先の電話番号/サブアドレス(10桁/10桁)
	BYTE	port;					// 接続先のポート(1〜4)
	BYTE	sakuzyo;				// 削除(0.ブランク 1.削除)
	
	//------- 新規追加接続先設定メンバ変数 -------
	char	type;					// 接続形態
	BYTE	fixIp[IP_BYTE];			// 接続先の固定IPアドレス

	BYTE	yobi1[IP_BYTE];
	BYTE	yobi2[IP_BYTE];
};

// LAN3通信内容
struct SDkLan3ConnectNaiyou {
	char	szNaiyou[30+1];	// 通信内容
	char	szSetumei[80+1];// 説明
	BOOL	fNeed;			// 要
};

// 信頼できるネットワーク設定
struct SDkLan3SinraiConnect {
	short	no;				// 接続先番号
	BYTE	ip[IP_BYTE];	// 接続先IPアドレス
	BYTE	mask[IP_BYTE];	// 接続先ネットマスク
	BYTE	port;			// ポート
};

// LAN3統合情報構造体
struct SDkLan3 {
	BYTE			ip[IP_BYTE];		// 固定IPアドレス
	BYTE			mask[IP_BYTE];		// ネットマスク
	BYTE			ipMS55[IP_BYTE];	// MS-55 IPアドレス
	BYTE			ipVirus[IP_BYTE];	// ウィルス対策PC
	BYTE			kosuu;				// 個数
	SDkLan3ConnectNaiyou sConNaiyo[eL3NaiyoMax];		// 通信内容
	BYTE			ipGateWay[IP_BYTE];	// ゲートウェイ
	
	SDkConnectInfoNew		sPriSecIp;		// 個別LANプライマリセカンダリ接続先IPアドレス記憶用
	
	SDkLan3SinraiConnect	sSinraiCon[4];	// 信頼できるネットワーク設定
	
	BOOL			fVirusUse;			// ウィルス対策IPが設定してあるか？
};

// SIS05通信用設定構造体
struct SDkSIS05Info {
	int		no;						// SIS05通信用設定接続先番号
	BYTE	ip[IP_BYTE];			// 接続先IPアドレス	(IPアドレス)
	char	szHoleCode[HALL_LEN_MAX2+1];		// ホールコード
	BYTE	passIp[IP_BYTE];		// パスワード		(IPアドレス)
	BYTE	vpn_passIp[IP_BYTE];	// VPNパスワード	(IPアドレス)
	BYTE	accessIp[IP_BYTE];		// アクセスポイント	(IPアドレス)
};

// ルータ設定
struct SDkRouter
{
	//------- 従来からあるルータ設定構造体メンバ変数 -------
	BYTE			kind;				// ルータ種別
	char			szRev[REV_LEN];		// Rev(ex. 4.00.000)
	BYTE			ip[IP_BYTE];		// ルータのIPアドレス
	BYTE			mask[IP_BYTE];		// ルータのNetmask
	BYTE			uip[IP_BYTE];		// ルータの上位接続IP
	SDkPortInfo		port[PORT_MAX];		// ポート設定構造体
	SDkConnectInfo	con[SETU_MAX+1];	// 従来の接続先設定構造体
	char			szVwID[INPUT_USER_MAX+1];		// VWユーザID
	char			szVwPass[INPUT_PASS_MAX+1];		// VWパスワード
	BYTE			provider;			// プロバイダ種類 0=個別契約　1=OCN関東　2=OCN関西　3.=旧東京
	short			shOverTimer;		// 接続超過タイマ
	BOOL			fMent;				// メンテナンス用設定	  0=無	1=anonymous 2=ADSL固定IP マイナス値=接続先番号
	int				m_nSISNo;			// SIS処理番号
	int				m_nVWNo;			// VW処理番号
	int				m_nRoboNo;			// ロボ処理番号	
	char			etc[20+1];			// その他情報 = DB登録用SIS番号(3)＆VW番号(3)＆ロボ番号(3)＆メンテ有無(1)＆強制切断タイマ(3)
	
	//------- 新規追加ルータ設定構造体メンバ変数 -------
	char			szAdslID[INPUT_USER_MAX+1];		// ADSLユーザID
	char			szAdslPass[INPUT_PASS_MAX+1];	// ADSLパスワード
	int				m_nAdslNo;			// ADSL処理番号
	BOOL			fBbUse;				// Labo閲覧ＢＢ接続を使うか？	  0=使わない 1=ADSL	2=ADSL固定 3=個別LAN
	SDkConnectInfoNew ncon[SETU_MAX+1];	// 新規対応接続先設定構造体
	
	SDkSIS05Info	sSIS05;				// SIS05通信用設定
	
	SDkLan3			sLAN3;				// LAN3詳細
	
	char			szRoboID[INPUT_USER_MAX+1];		// ROBOユーザID
	char			szRoboPass[INPUT_PASS_MAX+1];	// ROBOパスワード
	
	int				m_nMtGokiNo;		// MT号機番号
	int				m_nDeltaBkNo;		// DELTANETバックアップ番号
	
	int				nMTU;				// MTU値
	
	BYTE			ip_2[IP_BYTE];		// IPアドレス②
	BYTE			mask_2[IP_BYTE];	// IPアドレス②のNetmask
	BYTE			m_ptcSvIp[IP_BYTE];	// PTCサーバー
	BYTE			ptc_ip[IP_BYTE];	// PTC接続用設定
	BYTE			center_ip[IP_BYTE];	// PTCバックアップ用設定(旧：センター側IPアドレス)
//===== ↓LTE通信対応 ======================================================================
	struct SDkLteKanriInfo {
		int		nKaisenKind;
		// int		type;						// 
		BYTE	ip[IP_BYTE];				// IPアドレスLTE用
		char	szID[INPUT_USER_MAX+1];		// LTEユーザID
		char	szPass[INPUT_PASS_MAX+1];	// LTEパスワード
		int		nMTU;						// MTU値
		int		nLuaPort;					// LUA待受ポート
		char	szPH_CodeA2910[8+1];		// PH会員番号(A29XX以前の環境でXaiにする前にルータに送信する際に使用)
		BYTE	gateway[IP_BYTE];			// ケーブル光用
		BYTE	netmask[IP_BYTE];			// ケーブル光用
	};
	
	SDkLteKanriInfo sLte;
	SDkLteKanriInfo sTmp;
	
//===== ↑LTE通信対応 ======================================================================	
	BOOL			fSetIpTelFlg;		// どこにＩＰ電話のＩＰセットしたか
	
	int				m_nIpTelNo;			// ＩＰ電話の接続先Tunnel番号
	int				m_nIpTelNoBk;		// ＩＰ電話のバックアップ接続先Tunnel番号
	
	int				m_nCarMentNo;		// 車両管理メンテ番号
	
	int				m_nMSS;				// MSS制限データ長(しない場合は0)
	
	int				m_fDHCP;			// DHCPサーバ機能 有効無効
	
	int				m_nJnetNo;			// JNET通信接続先番号メイン
	int				m_nJnetNo2;			// JNET通信接続先番号サブ
	char			m_szJnetHoujin[6+1];// JNET通信会社番号
	char			m_szJnetHall[4+1];	// JNET通信ホール番号
	char			m_szJnetVpnName[16+1];// JNET通信 vpn local name
	
	char			szAuthId[64+1];		// ① リモートメンテナンス接続ユーザ名（RouterInfo.inf の AUTH_ID）
	char			szVPN_User[16+1];	// ② VPNユーザ名
	char			szVPN_Pass[16+1];	// ③ VPNパスワード
	char			szVPN_Key[32+1];	// ④ VPN PRE-SHARED-KEY
	int				nLuaPort;			// ⑤ LUA待受ポート
	int				nFreePcPort;		// ⑥ フリーPC待受ポート
	int				nRdpOpenPort;		// ⑦ RDP開放ポート
	int				nRdpRecvPort;		// ⑧ RDP待受ポート
										// ⑨ ルータのIPアドレス
	
	char			szPH_CodeA2910[8+1];// PH会員番号(A29XX以前の環境でXaiにする前にルータに送信する際に使用)
	
	int				m_nFqdnState;			// クラウドIP電話選択状態
	BYTE			ip_2_cloud[IP_BYTE];	// IPアドレス②(クラウドIP電話用)
	BYTE			mask_2_cloud[IP_BYTE];	// IPアドレス②のNetmask(クラウドIP電話用)
	
	int				m_nNonConnect;		// 未接続定義
	int				m_nNonConnectBk;	// 未接続定義BK
	
	int				m_nMemNonConnect;	// 未接続定義削除後用
	int				m_nMemNonConnectBk;	// 未接続定義BK削除後用

	int				m_nJnetMent;
	BOOL			fUseLan2DHCP;
};

//	NAT 定義
struct SDkNat {
	BYTE	ip1;
	BYTE	ip2;
	int		rep;
};
// マシーン設定引き渡し用
struct SDkMcnInfo {
	BYTE	ip[IP_BYTE];			// 接続先のIPアドレス
	BYTE	mask[IP_BYTE];			// 接続先のNetmask
	BYTE	gate[IP_BYTE];			// 接続先のGateWay
};

//===== ↓クラウドリニューアル対応 ========================================================
// CMS0334.ini 状態記憶ファイル
struct SDkKanshiSrvInfo
{
	int nStatus;
	int nAddTunnelNo;
	int nAddSubTunnelNo;
	int nPreTunnelNo;
	int nPreSubTunnelNo;
	char szAddIp1[15+1];
	char szAddIp2[15+1];
	char szWroteIp1[15+1];
	char szWroteIp2[15+1];
	char szPreIp1[15+1];
	char szPreIp2[15+1];
	char szProxyIp1[15+1];
	char szProxyIp2[15+1];
	char szProxyPort[15+1];
	
	char szLiftChgBackPath[2][MAX_PATH];
	BOOL fJNETment;
	int nAddJnetMentNo;
	
	char szDebugIp[15+1];
	BOOL fDebugNoCheckTunnel;
	int nDebugCmdRecvTimeOut;
	enum	// nStatus
	{
		eNonMode = 0,
		eAddTunnel,
		eLiftMode,
		eDelete,
		eProxyAdd,
	};
};

#define LIFT_CHG_PTN_MAX	(4)	// リフト切替パターン

//===== ↑クラウドリニューアル対応 ========================================================

//----- ↑構造体定義 ----------------------------------------------------------------------
extern BYTE g_bDNS1[4][4];	// プロバイダのプライマリDNS 
extern BYTE g_bDNS2[4][4];	// プロバイダのセカンダリDNS

#define A2910_PH_INP_SHOW	1		// PH会員番号欄を表示する(A29XX用特別対応版専用 A29XX用特別対応版作成時は1にすること)

// ISDNルータ専用クラス
class CDkRouter
{
public :
	int				m_nAdslProc;	// ADSL接続先と思われる接続先番号
	
	int				m_nNumConnect;			//	実接続数
	SDkRouter		m_sRouter;				//	ルータ設定
	char			m_szFileName[MAX_PATH];	//	TFTP用ファイル名
	int				m_nCurrentPP;			//	処理中のPP
	BOOL			m_fReadRtType;			//	ルータタイプ処理済み
	FILE			*m_Fp;					//	入出力用ファイルポインター
	char			m_szWkIp[64];			//	IP書き出し用
	int				m_nUseType;				//	使用するタイプ 0:C店,1:リモート,2:本部
	int				m_nArray;
	BOOL			m_fAnonymous;			//  アノニマスシ処理中
	char			m_szInterFaceDK[32];	//	NICのインターフェイス番号
	char			m_szInterFaceOA[32];	//	NICのインターフェイス番号
	SDkMcnInfo		m_pDkLan[2];			//	DK-LANとOA-LANセット
	CFile			*m_pLogFile;			//	ログファイル
	BOOL			m_fNoDHCP;				//  DHCPサーバ機能を無効にするか？
	
	int				m_nStockVwNo;			// VWの接続先番号の情報が来る前に
	char			m_szStockVwID[INPUT_USER_MAX+1];	// VWのID、Pass情報が来た時
	char			m_szStockVwPass[INPUT_PASS_MAX+1];	// としての対策用の一時的退避変数
	
	BOOL			m_fSisRoboVwM;			// SIS,Robo,Vw,PHS設定 or メンテナンスのpp selectか？
	HWND			m_hwndView;				
	BOOL			m_fSendBin;				// 設定ファイル送信時に".bin"ファイルも一緒に送るか？
	
	int				m_nRtTypeKind;			// ルータの分類(0:ＩＳＤＮルータ 1:ブロードバンドルータ 2:IPSEC対応ルータ)
	BOOL			m_fCantTypeRead;		// 本ＰＧで非対応のルータ種類を読み込もうとした
	int				m_fNoMsgbox;			// メッセージボックスを出さないか？
	
	//----- レジストリ値 --------------------------------------------------------------------
	DWORD			m_dwDefRootFlag;		// デフォルトルートをISP（DNS）　or　ゲートウェイIPへ向ける
	DWORD			m_dwSISAddressFlag;		// どのサイト（SISLabo、CA局、SISLabo05、CA局05）へのルーティングを設定するかを制御
	
	BOOL			m_fMxRMAddRead;			// レジストリ"RMAddress"がちゃんと入ってるかどうか
	
	BYTE			m_bRMAdd[IP_BYTE];		// %RMAddress%作業用
	char			m_szRMAdd[20];			// レジストリ"HKEY_LOCAL_MACHINE\SOFTWARE\Daikoku\Mx\RMAddress"
	
	DWORD			m_dwSecFilOffFlag;		// セキュリティフィルタ解除
	DWORD			m_dwViaRootFlag;		// 2拠点間の橋渡し
	
	CString			m_strPRE_SHARED_KEY;	// latestkey	
	CString			m_strADMIN_PASS;		// latest		
	CString			m_strAUTH_ID;			// latestid		
	CString			m_strAUTH_PASS;			// latestpass	
	CString			m_strADMIN_PASS_HALL;	// latest		
	DWORD			m_dwDnsFlag;			// DNSサーバ接続
	DWORD			m_dwPop3Flag;			// POP3サーバ接続
	DWORD			m_dwSmtpFlag;			// SMTPサーバ接続
	//----------------------------------------------------------------------------------------
	
	char			m_szTmpID[INPUT_USER_MAX+1];		// 現段階でどこにいれていいのかわからないＩＤとパスワード
	char			m_szTmpPass[INPUT_PASS_MAX+1];
	
	char			m_szDebug[2048];		// デバッグ用コマンド記憶
	
	char			m_szFirstAdd[15+1];
	char			m_szLastAdd[15+1];
	
	char			*szRtType[eRTmax];

	//	TFTP用ファイル名を作成
	char *GetFileName(int no, int read_write);

//===== ↓クラウドIP電話対応  ==============================================================================
	CStringArray	m_strFqdnArray;		// (DK様提供)FQDN一覧リスト
	CString			m_strNowSelFqdn;	// 現在選択されているFQDN
	CString**		m_strFqdnCmb;		// FQDNコンボ用
	int				m_nFqdnCnt;			// FQDN一覧リスト数
	
	BYTE			m_byLan3Ip_plus2[IP_BYTE];
	BYTE			m_byLan3Ip_plus5[IP_BYTE];
	BYTE			m_byLAN3IP[IP_BYTE];
	
	CStringArray	m_strIniGetIpArray;		// (HE作成)iniファイルから取得したFQDN取得情報IPアドレス(送信完了時に更新される)
	CStringArray	m_strRouterSendIpArray;	// 実際の送信に使われるIPアドレス一覧
	CStringArray	m_strRouterGetIpArray;	// ルータconfigから読み込んだFQNDの割当IPアドレス
	
	CDkRoutePassEx	m_cRtPassEx;
	BOOL GetFqdnInfo(CStringArray *pFqdnArray, int *pFqdnCnt);
	
	BOOL _Put(char *buf, BOOL fKaigyo=TRUE);
	BOOL _Put(const char *buf[], int n, BOOL fKaigyo=TRUE);
	BOOL _PutTit(char *buf);
	BOOL _Put(CString strCmd);
	
	SDkConnectInfoNew m_sWkHikari;			// ケーブル光記憶用
	BOOL m_fBBsetLAN3;
//===== ↑クラウドIP電話対応 ==============================================================================
	
//===== ↓クラウドリニューアル対応 ========================================================
	SDkKanshiSrvInfo m_sIniFileInf;
	
	int AnalizeRouterType(char *buf, int *pSetuzokuMax);
	void TunnelAdd(CString *strPut, int nAddTunnelNo1, int nAddTunnelNo2, char *szTunnelIP1, char *szTunnelIP2, char *szRouterIP, char *szFixIP);
	BOOL RouterCmdEdit_TunnelDelete(CStdioFile *pGetFile, CFile *pSendFile);
	BOOL RouterCmdEdit_Change(CStdioFile *pGetFile, CFile *pSendFile);
	BOOL RouterCmdEdit_TunnelRepair(CStdioFile *pGetFile, CFile *pSendFile);
	BOOL RouterCmdEdit_TunnelAdd(CStdioFile *pGetFile, CFile *pSendFile);
	BOOL RouterCmdEdit_Recovery(CStdioFile *pGetFile, CFile *pSendFile);
	BOOL RouterCmdEdit_ProxyAdd(CStdioFile *pGetFile, CFile *pSendFile);
	BOOL RouterCmdEdit_JnerMent(CStdioFile *pGetFile, CFile *pSendFile);
	BOOL ReadServerIpFile();

	CString ConvertStrList(CStringArray *pArray);
	void GetToken(CString strText, int nPosCnt, int *pData=NULL, char *pszData=NULL);
	void CheckCmd(CString sBuf, int nKind);
	BOOL PreConfigRead(CStdioFile *pGetFile, BOOL fRtSet=FALSE, BOOL fJnetMent=FALSE);
	BOOL ConvertIP(BYTE *ip, char *szIP);
	BOOL SyncLocalFile(CString strSendFilePath);

	enum
	{
		eFilter = 0,
		eFilterDynamic,
		eFilterSetL2in,
		eFilterSetL2out,
		eFilterSetL3in,
		eFilterSetL3out,
		eServerIpFile,
	};
	
	int m_nLine;
	int m_nNowGetRouterKind;
	
	char m_szTargetRouterIp[32];
	char m_szPublicIp[32];
	
	int m_nRtSetuzokuMax;
	
	int m_fFilterCheck[4];
	int m_fFilterSetCheck[4];
	int m_fSrvIpFileCheck[4];
	int m_AddFilter[4][10];
	int m_nFileReadStatus;
	
	CStringArray	m_strPutFilter[4];
	CString			m_strPutL2in;
	CString 		m_strPutL2out;
	CString 		m_strPutL3in;
	CString 		m_strPutL3out;
	CStringArray	m_cSrvIpList[4];	// 61.114.186.*
	CStringArray	m_cSrvIpList2[4];	// 113.52.158.*
	
	BOOL CheckTunnelConnect(int nTunnelNo, char *szIP1);
	BOOL LineRead(CStdioFile *pFile, CString *pBuf);
	BOOL GetPutIniFile(BOOL fRead, SDkKanshiSrvInfo *pInfo);	// ini読込書込
	BOOL ReadOnlyBreak(CString strPath);
	BOOL RouterCloudWrite(SDkRouter *pRouter, CString strFile);
	BOOL RouterAccesCheck(char *szTargetIP);
	BYTE SearchFilterSetNo(int nFilterNo, CString strBuf);
	BOOL RouterJnetMentWrite(SDkRouter *pRouter, CString strTargetFile);
	BOOL IsXai();
	
	CStringArray m_sSvIpList;	// サーバーIPファイルのレコード
	int m_nSrvIp200CntMax1;
	int m_nSrvIp200CntMax2;
	BOOL m_fJnetMentWrote;
//===== ↑クラウドリニューアル対応 ========================================================
	
	char m_szWanApiName[256];
public :
	CDkRouter(SDkMcnInfo *pDkLan = NULL, CDkMxFile *pLog=NULL);
	~CDkRouter();
	
	int DebDisp(CString strMsg);
	int DebDisp(LPSTR lpszFmt, ...);				// .logファイルにメッセージ出力する
	BOOL IsRtTypeEditable(int nRtType);				// ルータのタイプがこのＰＧで編集可なのかを判断する
	BOOL PutRouterIpFile(BYTE *pIp);				// 現ルータＩＰ情報ファイルにルータＩＰを書き込む
	char *GetIpB2C(BYTE *ip, BYTE *mask=NULL);
	char *GetIpB2C_Second(BYTE *ip, BYTE *mask=NULL);
	//　IPアドレスの文字列をBYTE配列に変換
	BOOL ConvertIpDelmit(char *szIp, BYTE *pIp);

	// ルータタイプからルータカテゴリ(何系か?)を取得
	GetRtCategory(int nRtType);

	int GetReleaseVersion(int subsys=0) 
	{
		char	*fnam[5] = {"CM", "DA", "KE", "KO"};
		char	file[MAX_PATH];
		char	szLocal[24];
		int		ver=0;
		
		wsprintf(file, "%s\\bin\\dkVer%s.ini", CDkM200Common::GetBaseDir(), fnam[subsys]);
		GetPrivateProfileString(fnam[subsys], "VERSION", "", szLocal, sizeof(szLocal), file);
		ver = fn_aToi(szLocal, strlen(szLocal));
		
		return ver;
	};
	
	// RevUpファイル名取得
	static CString GetBinFileName(int nRtKind)
	{
		CString str;
		const char *exe[] = {"rt100i.bin", "rt103i.bin", "rt52pro.bin", "rta54i.bin", "rta55i.bin", "rta52i.bin", "rt57i.bin", "RT200i.bin", "RTX1000.bin", "RTX1000.bin", "RTX1100.bin", "RTX1100.bin", "rt58i.bin", "RTX1200.bin", "RTX1200.bin", "RTX1210.bin", "RTX1210.bin", "RTX830.bin", "RTX830.bin",  "RTX1220.bin", "RTX1220.bin"};
		str = exe[nRtKind-1];
		return str;
	}
	
	// ルータの接続先最大設定数
	static int GetConnectMax(int nRtKind)
	{
		if ( nRtKind == eRTX830 || nRtKind == eRTX830_OA )
		{
			return 16;
		}
		return (nRtKind == eRTX1200 || nRtKind == eRTX1200_OA || nRtKind == eRTX1210 || nRtKind == eRTX1210_OA || nRtKind == eRTX1220 || nRtKind == eRTX1220_OA) ? 49 : 30;
	}

	// IPアドレス加算処理
	static void IpAddressCalc(BYTE *pIp, int nPlus)
	{
		int nIpCalc[IP_BYTE] = {0};
		for ( int i = 0; i < IP_BYTE; i++ )
		{
			nIpCalc[i] = (int)pIp[i]; 
		}
		
		nIpCalc[3] += nPlus;
		if ( nIpCalc[3] > 255 )	// 繰り上げ処理
		{
			nIpCalc[2]++;
			nIpCalc[3] = nIpCalc[3] - 255;
			if ( nIpCalc[2] > 255 )
			{
				nIpCalc[1]++;
				nIpCalc[2] = nIpCalc[2] - 255;
			}
		}
		
		for ( i = 0; i < IP_BYTE; i++ )
		{
			pIp[i] = (BYTE)nIpCalc[i]; 
		}
	}
protected :
	//	構文を解析する
	int Analize(char *buf);
	//	TFTPファイルに書き出す
	BOOL WriteTftpFile(int no, char *szFile=NULL, BOOL fStatic=FALSE);
	//  プロバイダの番号取得
	int GetProviderNo(char *szDNS1, BYTE *pDNS1=NULL, BYTE *pDNS2=NULL);
		
	// RouterInfo.datからADMIN_PASSを取得
	char* GetAdminPass(char *szPass);
	//	文字列がファイルに含まれるかチェックする
	BOOL CheckResult(char *fname, CStringArray *pStrCheck=NULL);
	
	//	読込
	BOOL ProcPP(char *buf);
	BOOL ProcIP(char *buf);
	BOOL ProcISDN(char *buf);
	BOOL ProcNAT(char *buf);
	BOOL ProcDNS(char *buf);
	BOOL ProcAnalog(char *buf);
	void SetIpC2B(char *ip_mask, BYTE *ip, BYTE *mask=NULL);
	
	//	書込
	BOOL PutConfig(char *tit);
	BOOL PutIsdn(char *tit);
	BOOL PutFilter(char *tit);
	BOOL PutLanIp(char *tit);
	BOOL PutNat_C(char *tit);
	BOOL PutNat_R(char *tit);
	BOOL PutNat_H(char *tit);
	BOOL PutPpHead(char *tit);
	BOOL PutPp();
	BOOL PutPpAnonymous(char *tit);
	BOOL PutOther(char *tit);
	
	BOOL PutIpRoute();
	
	BOOL _Run(char *pg);
	
	void _Conv(char *inout);
	BOOL GetRouterInfo(int nIpIdx=0);
	
	int strncpy0(char *pszOUT, char *pszIN, int nLen);
	
	int GetRouterKind()
	{
		if ( m_sRouter.kind == eRT58i )
			return eRT57i;
		
		return m_sRouter.kind;
	}
};

struct SDkFlexPrm
{
	int nLuaTunnelMin;
	int nLuaTunnelMax;	
};

// IPSEC対応ルータクラス
// ※IPSEC対応ルータクラスは、ISDNルータ専用クラス、BBルータ専用クラスの機能を内包した
// "ルータ設定操作総合クラス"です。外部からは、必ずこのクラスを使用してください。
class CDkRouter1000 : public CDkRouter
{
public :
	int				m_nMentType;
	BOOL			m_fOA_LAN;				// OA_LAN(192.168.60.1)か？
	int				m_nUipMask;				// 上位接続IPの決め付けるネットマスク
	BOOL			m_bL3GateWay;			// GateWayアドレスがセットされた
	BYTE			m_thisIp[IP_BYTE];		// 読み込んだ時の　このルータのＩＰアドレス
	BOOL			m_fSendFile;			// ルータへ設定送信用ファイルへの書き込みか？
	BOOL			m_fMS55IpBrank;			// MS55IPアドレスをブランクにして登録しようとしたか？
	BOOL			m_fVirusIpBrank;		// MS55IPアドレスをブランクにして登録しようとしたか？
	CString			m_strReadLine;			// 読み取った一行のすべて
	int				m_nConnectMax;			// 現在のルータの最大接続設定可能数

	SDkFlexPrm		m_sPrm;

public :
	// ※※※ 外部から呼び出される関数郡はここで絶対ここで定義しろ ※※※
	CDkRouter1000(SDkMcnInfo *pDkLan = NULL, CFile *pLog=NULL);
	~CDkRouter1000(){};
	
	//  レジストリデータを読み込み
	BOOL ReadRegstryData();
	//  レジストリデータを書き込み
	BOOL WriteRegstryData(BOOL fRMAddBrank=FALSE);
	
	//	TFTPファイルを読む
	BOOL ReadTftpFile(int no, char* szFile = NULL);
	
 	//	TFTPでルータから取得する
 	BOOL GetTftp(int &no, char *ip, BOOL fKindGet=FALSE, char *szPass=NULL);
 	BOOL GetTftp(int &no, BYTE *ip, BOOL fKindGet=FALSE, char *szPass=NULL);
 	
	//	TFTPファイルに書き出す
	BOOL WriteTftpFile(int no, char *szFile=NULL, BOOL fStatic=FALSE, char *szPH_CodeA2910="", BYTE *pSendSrcIp=NULL);
	//	TFTPでルータに書き出す
	int PutTftp(BYTE *pSendIp, char *szSendFilePath, char *szPass=NULL);
	
	//  LAN3ポートが設定されているか?
	BOOL IsLAN3IpEnable(SDkLan3 *pRtLan3);
	
	//  ADSL情報の入った配列を取得
	SDkConnectInfoNew *GetAdslInfo(SDkRouter *pRouter=NULL);
	
	BOOL _Put(char *buf, BOOL fKaigyo=TRUE);
	BOOL _Put(const char *buf[], int n, BOOL fKaigyo=TRUE);
	BOOL _PutTit(char *buf);
	BOOL _Put(CString strCmd);

protected :
	//	構文を解析する
	int Analize(char *buf);
	
private :
	BOOL PutNatOther(char *tit);
	
	//	読込
	BOOL ProcPP(char *buf);
	BOOL ProcIP(char *buf);
	BOOL ProcISDN(char *buf);
	BOOL ProcNAT(char *buf);
	BOOL ProcDNS(char *buf);
	BOOL ProcAnalog(char *buf);
	BOOL ProcPPPOE(char *buf);
	BOOL PutFilterPass(int nFilterOrg, int nPass, BOOL fRtIpBefor, BOOL fNetAddress=FALSE);
	BOOL ProcTunnel(char *buf);
	BOOL ProcIPSEC(char *buf);
	BOOL ProcDHCP(char *buf);
	BOOL PreReadFile(char* szFile);	
	BOOL AnalizeRouterType(char *buf);
	//	書込
	BOOL PutConfig(char *tit);
	BOOL PutIsdn(char *tit);
	BOOL PutFilter(char *tit);
	BOOL PutLanIp(char *tit);
	BOOL PutNat_C(char *tit);
	BOOL PutNat_R(char *tit);
	BOOL PutNat_H(char *tit);
	BOOL PutPpHead(char *tit);
	BOOL PutPp();
	BOOL PutTunnel();
	BOOL PutPpAnonymous(char *tit);
	BOOL PutOther(char *tit);
	BOOL PutIpsec();
	BOOL PutIpRoute();
	
	BOOL PutFilterOA_LAN(char *tit);
	
	BOOL PutRevUpFile(BYTE *pSendIp, char *szPass=NULL);
	
	BOOL _PutFilterName(char *cmd);
};

class CDkRouter1200 : public CDkRouter1000
{
public :
	int		noAngoPass;			//	mxcomm.in [DEBUG] NO_ROUTER_ANGO	
	
	// ※※※ 外部から呼び出される関数郡はここで絶対ここで定義しろ ※※※
	CDkRouter1200(SDkMcnInfo *pDkLan = NULL, CFile *pLog=NULL);
// 	~CDkRouter1200(){};
	
	//  レジストリデータを読み込み
	BOOL ReadRegstryData();
	//  レジストリデータを書き込み
	BOOL WriteRegstryData(BOOL fRMAddBrank=FALSE);
	
	//	TFTPファイルを読む
	virtual BOOL ReadTftpFile(int no, char* szFile = NULL);
	
	//	TFTPでルータから取得する
	virtual BOOL GetTftp(int &no, char *ip, BOOL fKindGet=FALSE, char *szPass=NULL);
	BOOL GetTftp(int &no, BYTE *ip, BOOL fKindGet=FALSE, char *szPass=NULL);
	
	//	TFTPファイルに書き出す
	virtual BOOL WriteTftpFile(int no, char *szFile=NULL, BOOL fStatic=FALSE, char *szPH_CodeA2910="", BYTE *pSendSrcIp=NULL);
	//	TFTPでルータに書き出す
	virtual int PutTftp(BYTE *pSendIp, char *szSendFilePath, char *szPass, int nRouterNo);
	
	//  LAN3ポートが設定されているか?
	BOOL IsLAN3IpEnable(SDkLan3 *pRtLan3);
	
	//	telnet でrestart
	BOOL TelnetRestart(LPCTSTR distAddress, LPCTSTR adminPass);
	
	//	構文を解析する
	virtual int Analize(char *buf);
	
	//	読込
	virtual BOOL ProcPP(char *buf);
	virtual BOOL ProcIP(char *buf);
// 	virtual BOOL ProcISDN(char *buf);
	virtual BOOL ProcNAT(char *buf);
	virtual BOOL ProcDNS(char *buf);
	virtual BOOL ProcAnalog(char *buf);
	virtual BOOL ProcPPPOE(char *buf);
	virtual BOOL ProcTunnel(char *buf);
	virtual BOOL ProcIPSEC(char *buf);
	virtual BOOL ProcDHCP(char *buf);
	virtual BOOL PreReadFile(char* szFile);	
	virtual BOOL AnalizeRouterType(char *buf);
	//	書込
	virtual BOOL PutConfig(char *tit);
	virtual BOOL PutIsdn(char *tit);
	virtual BOOL PutFilter(char *tit);
	virtual BOOL PutLanIp(char *tit);
	virtual BOOL PutNat_C(char *tit);
	virtual BOOL PutNat_R(char *tit);
	virtual BOOL PutNat_H(char *tit);
	virtual BOOL PutNatOther(char *tit);
	virtual BOOL PutPpHead(char *tit);
	virtual BOOL PutPp();
	virtual BOOL PutTunnel();
	virtual BOOL PutPpAnonymous(char *tit);
	virtual BOOL PutOther(char *tit);
	virtual BOOL PutIpsec();
	virtual BOOL PutIpRoute();
	virtual BOOL PutFilterOA_LAN(char *tit);
	virtual BOOL PutRevUpFile(BYTE *pSendIp, char *szPass=NULL);
	virtual BOOL _PutFilterName(char *cmd);
	virtual BOOL PutFilterPass(int nFilterOrg, int nPass, BOOL fRtIpBefor, BOOL fNetAddress=FALSE);
//===== ↓追加分 ==========================================================================
	BOOL PutFmt(LPCSTR lpszFmt, ...);
	char *GetLuaIP() { return "172.16.5.1"; }
// 	void WriteRouterPrmFile(char *fname, int nPH_Code, CString szIP, int nRecvLuaPort);
	
	char m_szSendLuaFile[MAX_PATH];
	char m_szSendLuaChkFile[MAX_PATH];
	char m_szSendPrmFile[MAX_PATH];
	
	CString m_strNewLoginPass;
	CString m_strNewAdminPass;
	
	int m_nRegRdpPort;
	void GetDescriptionParam(const char *input, CString output[]);
	SDkRouter m_sPutPreRouter;
//===== ↑追加分 ==========================================================================
};


class CDkRouter830 : public CDkRouter1200
{
public :
	CDkRouter830(SDkMcnInfo *pDkLan = NULL, CFile *pLog=NULL);
	
	//	書込
	virtual BOOL PutIsdn(char *tit);
	virtual BOOL PutLanIp(char *tit);
};

class CDkRouter1220 : public CDkRouter830
{
public :
	CDkRouter1220(SDkMcnInfo *pDkLan = NULL, CFile *pLog=NULL);
	
	//	書込
	//virtual BOOL PutIsdn(char *tit);
	virtual BOOL PutLanIp(char *tit);
};

extern BOOL g_fBackUpCmd ;
extern BOOL g_fKeepAlive;
extern int g_line;	// デバッグ用

#endif		//	_ROUTER_H

