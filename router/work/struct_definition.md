# SDkRouter 定義完全抽出

## 定義ファイル
- [Router.h](file:///c:/_Develop/Doc/Skills/router/input/src/CMS0330Passmask/Router.h)

## 関連定数
```cpp
#define IP_BYTE             4
#define PORT_MAX            4
#define SETU_MAX            100 // Router.h:58
#define REV_LEN             11  // Router.h:55
#define TEL_SUBADR          26  // Router.h:57
#define INPUT_USER_MAX      51  // Router.h:72 (50+1)
#define INPUT_PASS_MAX      51  // Router.h:73 (50+1)
#define HALL_LEN_MAX2       9   // Router.h:65
```

## 全メンバー（SDkRouter）
| 型 | メンバー名 | 説明 |
|:---|:---|:---|
| BYTE | kind | ルータ種別 |
| char[11] | szRev | Rev |
| BYTE[4] | ip | ルータのIPアドレス |
| BYTE[4] | mask | ルータのNetmask |
| BYTE[4] | uip | ルータの上位接続IP |
| SDkPortInfo[4] | port | ポート設定 |
| SDkConnectInfo[101] | con | 従来の接続先設定 |
| char[51] | szVwID | VWユーザID |
| char[51] | szVwPass | VWパスワード |
| BYTE | provider | プロバイダ種類 |
| short | shOverTimer | 接続超過タイマ |
| BOOL | fMent | メンテナンス用設定 |
| int | m_nSISNo | SIS処理番号 |
| int | m_nVWNo | VW処理番号 |
| int | m_nRoboNo | ロボ処理番号 |
| char[21] | etc | その他情報 |
| char[51] | szAdslID | ADSLユーザID |
| char[51] | szAdslPass | ADSLパスワード |
| int | m_nAdslNo | ADSL処理番号 |
| BOOL | fBbUse | Labo閲覧ＢＢ接続 |
| SDkConnectInfoNew[101] | ncon | 新規対応接続先設定 |
| SDkSIS05Info | sSIS05 | SIS05通信用設定 |
| SDkLan3 | sLAN3 | LAN3詳細 |
| char[51] | szRoboID | ROBOユーザID |
| char[51] | szRoboPass | ROBOパスワード |
| int | m_nMtGokiNo | MT号機番号 |
| int | m_nDeltaBkNo | DELTANETバックアップ番号 |
| int | nMTU | MTU値 |
| BYTE[4] | ip_2 | IPアドレス② |
| BYTE[4] | mask_2 | IPアドレス②のNetmask |
| BYTE[4] | m_ptcSvIp | PTCサーバー |
| BYTE[4] | ptc_ip | PTC接続用設定 |
| BYTE[4] | center_ip | PTCバックアップ用設定 |
| SDkLteKanriInfo | sLte | LTE管理情報 |
| SDkLteKanriInfo | sTmp | LTE管理情報（一時） |
| BOOL | fSetIpTelFlg | IP電話セットフラグ |
| int | m_nIpTelNo | IP電話Tunnel番号 |
| int | m_nIpTelNoBk | IP電話BK Tunnel番号 |
| int | m_nCarMentNo | 車両管理メンテ番号 |
| int | m_nMSS | MSS制限データ長 |
| int | m_fDHCP | DHCPサーバ機能 |
| int | m_nJnetNo | JNET通信接続先番号メイン |
| int | m_nJnetNo2 | JNET通信接続先番号サブ |
| char[7] | m_szJnetHoujin | JNET通信会社番号 |
| char[5] | m_szJnetHall | JNET通信ホール番号 |
| char[17] | m_szJnetVpnName | JNET通信 vpn local name |
| char[65] | szAuthId | リモートメンテナンス接続ユーザ名 |
| char[17] | szVPN_User | VPNユーザ名 |
| char[17] | szVPN_Pass | VPNパスワード |
| char[33] | szVPN_Key | VPN PRE-SHARED-KEY |
| int | nLuaPort | LUA待受ポート |
| int | nFreePcPort | フリーPC待受ポート |
| int | nRdpOpenPort | RDP開放ポート |
| int | nRdpRecvPort | RDP待受ポート |
| char[9] | szPH_CodeA2910 | PH会員番号 |
| int | m_nFqdnState | クラウドIP電話選択状態 |
| BYTE[4] | ip_2_cloud | IPアドレス②(クラウド用) |
| BYTE[4] | mask_2_cloud | IPアドレス②のNetmask(クラウド用) |
| int | m_nNonConnect | 未接続定義 |
| int | m_nNonConnectBk | 未接続定義BK |
| int | m_nMemNonConnect | 未接続定義削除後用 |
| int | m_nMemNonConnectBk | 未接続定義BK削除後用 |
| int | m_nJnetMent | JNETメンテ |
| BOOL | fUseLan2DHCP | LAN2 DHCP使用 |

## pack指定有無
- 明示的な `#pragma pack` は [Router.h](file:///c:/_Develop/Doc/Skills/router/input/src/CMS0330Passmask/Router.h) および周辺には見当たらない。

## 継承有無
- 構造体としての継承はない。

## sizeof利用箇所
- [SetSisrouta.cpp](file:///c:/_Develop/Doc/Skills/router/input/src/CMS0330Passmask/SetSisrouta.cpp): メンバー間のバックアップコピー (`m_sBkRouter` への退避・復元)
- [Router1200.cpp](file:///c:/_Develop/Doc/Skills/router/input/src/CMS0330Passmask/Router1200.cpp): 処理前後の状態保存 (`m_sPutPreRouter` への退避・復元)
- [MainFrm.cpp](file:///c:/_Develop/Doc/Skills/router/input/src/CMS0330Passmask/MainFrm.cpp): UIダイアログからの反映
- [CMS0330View.cpp](file:///c:/_Develop/Doc/Skills/router/input/src/CMS0330Passmask/CMS0330View.cpp): ビューの状態管理
- [CMS0330Doc.cpp](file:///c:/_Develop/Doc/Skills/router/input/src/CMS0330Passmask/CMS0330Doc.cpp): ドキュメントクラスのメンバ (`m_Router`) と `m_pRt->m_sRouter` 間の同期や初期化 (`ZeroMemory`)
