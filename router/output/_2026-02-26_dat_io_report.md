# router01W.dat 1行毎の入出力元解析レポート

`router01W.dat`の各行がどの関数でパース(格納)され、どの関数で出力(書き込み)されるか、および対応する`SDkRouter`のメンバ変数を解析した結果です。


| 行番号 | 出力内容 | 書き込み関数 | 使用メンバ変数 | 格納処理関数(パース) | 格納先メンバ変数 | 備考/根拠 |

|---|---|---|---|---|---|---|

| 1 | `#	DK-ROUTER-CONFIGURATION Ver A1.000` | PutConfig または 類似のヘッダ出力 | なし | ReadTftpFile (対象外) | なし | ヘッダー固定文字 |
| 2 | `#	RTX830 Rev.15.02.31` | PutConfig | m_sRouter.kind, m_sRouter.szRev | AnalizeRouterType | m_sRouter.kind, m_sRouter.szRev | ルータ型番とリビジョン |
| 4 | `# System configuration` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 6 | `clear configuration` | _Put | なし | CDkRouter(1200)::Analize | なし (スキップ等) | 初期化用固定コマンド |
| 7 | `no schedule at 1` | _Put | なし | CDkRouter(1200)::Analize | なし (スキップ等) | 初期化用固定コマンド |
| 8 | `no schedule at 2` | _Put | なし | CDkRouter(1200)::Analize | なし (スキップ等) | 初期化用固定コマンド |
| 9 | `no schedule at 3` | _Put | なし | CDkRouter(1200)::Analize | なし (スキップ等) | 初期化用固定コマンド |
| 10 | `no schedule at 4` | _Put | なし | CDkRouter(1200)::Analize | なし (スキップ等) | 初期化用固定コマンド |
| 11 | `login password ccsystem` | PutConfig | m_strADMIN_PASS または m_strNewLoginPass | PreReadFile または Analize | m_strADMIN_PASS 等 | パスワード設定 |
| 12 | `administrator password ccsystem` | PutConfig | m_strADMIN_PASS または m_strNewAdminPass | PreReadFile または Analize | m_strADMIN_PASS 等 | 管理者パスワード |
| 13 | `snmp syslocation "Hall 23999185.5123"` | PutConfig | m_cRtPassEx.m_nPH_Code | CDkRouter::Analize | m_cRtPassEx.m_nPH_Code | ロケーションにPHコードを埋め込み |
| 14 | `security class 1 on on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 15 | `console info off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 17 | `ip routing process normal` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 18 | `ip route 10.142.65.0/24 gateway tunnel 2` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 19 | `ip route 10.142.165.0/24 gateway tunnel 2` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 20 | `ip route 10.145.0.0/16 gateway tunnel 2` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 21 | `ip route 10.141.0.0/16 gateway tunnel 2` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 23 | `ip route 172.18.0.0/16 gateway 172.16.5.11` | PutIpRoute | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipGateWayなどIP類 | ProcIP (ip route ...) | 各LAN・接続先向けIP | ルーティング設定群 |
| 24 | `ip route 61.114.182.53 gateway pp 5 metric 2` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 25 | `ip route 61.114.182.54 gateway pp 5 metric 2` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 26 | `ip route 61.114.186.0/25 gateway pp 5 metric 2` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 27 | `ip route 61.114.177.148 gateway pp 5 metric 2` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 28 | `ip route 202.229.106.177 gateway pp 5 metric 2` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 29 | `ip route 172.31.191.0/24 gateway tunnel 1` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 30 | `ip route 61.114.186.111 gateway pp 5` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 31 | `ip route 172.31.65.0/24 gateway tunnel 1` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 32 | `ip route 52.197.151.142 gateway pp 5` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 33 | `ip route 0.0.0.0/32 gateway tunnel 3` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 34 | `ip route 61.114.186.110 gateway pp 5` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 35 | `ip route 153.142.244.126 gateway pp 5` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 36 | `ip route 10.2.0.0/16 gateway tunnel 6` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 37 | `ip route 153.142.244.125 gateway pp 5` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 38 | `ip route 10.1.0.0/16 gateway tunnel 7` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 39 | `ip route default gateway pp 5 metric 2` | PutIpRoute | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo 等 | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向けルーティング) | ルーティング設定群 |
| 40 | `ip route 10.0.192.0/24 gateway pp anonymous metric 2` | PutIpRoute | m_sRouter.ip | ProcIP (ip route ...) | m_sRouter.ipなど関連変数 | ルーティング設定群 |
| 41 | `ip route 10.0.208.0/24 gateway tunnel 5` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 42 | `ip route 10.0.224.0/24 gateway tunnel 5` | PutIpRoute | m_sRouter.sSIS05.no, m_sRouter.m_nIpTelNo 等の対象接続先番号 | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向けルーティング) | ルーティング設定群 |
| 43 | `ip icmp parameter-problem send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 44 | `ip icmp echo-reply send-only-linkup on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 46 | `# LAN/IP configuration` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 48 | `ip lan1 address 172.16.5.1/16` | PutLanIp | m_sRouter.ip, m_sRouter.mask | ProcIP (ip lan1 address ...) | m_sRouter.ip, m_sRouter.mask | LAN1のIP設定 |
| 49 | `rip use on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 50 | `ip lan1 rip send on version 2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 51 | `ip lan1 rip receive on version 2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 52 | `ip lan1 proxyarp on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 53 | `ip lan1 secure filter name dk_lan1` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 54 | `ip lan1 intrusion detection in on reject=on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 55 | `ip lan2 rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 56 | `ip lan2 rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 57 | `ip lan2 proxyarp off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 58 | `ip lan2 intrusion detection in on reject=on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 60 | `# PP configuration` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 62 | `pp disable all` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 64 | `### PP 5 ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 66 | `pp select 5` | PutPp | m_sRouter.con[n].no または anonymous | ProcPP (pp select ...) | m_nCurrentPP など現在の接続対象状態変数 | PP選択 |
| 67 | `pp backup none` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 68 | `pppoe use lan2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 69 | `pppoe auto connect on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 70 | `pppoe auto disconnect on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 71 | `pppoe disconnect time off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 72 | `ip pp mtu 1454` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 73 | `ip pp intrusion detection in on reject=on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 74 | `ip pp nat descriptor 2000` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 75 | `pp auth accept pap chap` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 76 | `pp always-on on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 77 | `ip pp secure filter name dk_pp_lan2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 78 | `ip pp address 153.142.244.124/32` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 79 | `pp auth myname w3102kwz@bizf.ocn.ne.jp sdjLSa76` | PutPp | m_sRouter.szVwID, m_sRouter.szVwPass 等 (対象毎) | ProcPP (pp auth myname ...) | m_sRouter.szVwID, m_sRouter.szAdslID 等 | PP認証情報 |
| 80 | `ppp lcp mru on 1454` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 81 | `ppp ccp type none` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 82 | `ip pp rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 83 | `ip pp rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 84 | `pp enable 5` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 86 | `### PP anonymous ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 88 | `no ip route 10.0.192.0/20` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 89 | `pp select anonymous` | PutPp | m_sRouter.con[n].no または anonymous | ProcPP (pp select ...) | m_nCurrentPP など現在の接続対象状態変数 | PP選択 |
| 90 | `no ip pp rip send` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 91 | `no ip pp rip receive` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 92 | `no ip pp nat descriptor` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 93 | `no pp auth username ccsupport` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 94 | `pp bind tunnel16-tunnel20` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 95 | `pp auth request mschap-v2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 96 | `pp auth username w8LeUYeGaZgstZXs 00393792` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 97 | `ppp ipcp ipaddress on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 98 | `ppp ipcp msext on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 99 | `ip pp remote address pool 172.16.60.16-172.16.60.20` | PutPp | m_sRouter.ip, m_sRouter.ncon[n].type | ProcIP (ip pp remote address ...) | m_sRouter.con[n].ip, m_sRouter.ncon[n].type等 | PPリモートIP |
| 100 | `ip pp mtu 1258` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 101 | `pp enable anonymous` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 102 | `tunnel disable all` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 104 | `### TUNNEL 1 ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 106 | `tunnel select 1` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 107 | `tunnel encapsulation ipsec` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 108 | `ipsec tunnel 101` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 109 | `ipsec sa policy 101 1 esp 3des-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 110 | `ipsec ike duration ipsec-sa 1 28800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 111 | `ipsec ike encryption 1 3des-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 112 | `ipsec ike group 1 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 113 | `ipsec ike hash 1 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 114 | `ipsec ike keepalive use 1 on heartbeat 60 10` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 115 | `ipsec ike local address 1 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 116 | `ipsec ike pfs 1 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 117 | `ipsec ike pre-shared-key 1 text 153.142.244.124` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 118 | `ipsec ike remote address 1 61.114.186.111` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 119 | `tunnel backup tunnel 4 switch-interface=on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 120 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 121 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 122 | `ip tunnel nat descriptor 2012` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 123 | `tunnel enable 1` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 125 | `### TUNNEL 3 ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 127 | `tunnel select 3` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 128 | `ipsec tunnel 103` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 129 | `ipsec ike duration ipsec-sa 3 10800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 130 | `ipsec ike duration isakmp-sa 3 3600` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 131 | `ipsec ike encryption 3 aes-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 132 | `ipsec ike group 3 modp768 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 133 | `ipsec ike hash 3 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 134 | `ipsec ike keepalive use 3 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 135 | `ipsec ike local address 3 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 136 | `ipsec ike local id 3 10.6.0.0/16` | PutTunnel | m_sRouter.uip, m_sRouter.con[n].ipなど | ProcIPSEC (ipsec ike ...) | m_sRouter.uip 等 | IKE設定 |
| 137 | `ipsec ike pfs 3 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 138 | `ipsec ike pre-shared-key 3 text wh1+eb0@rd` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 139 | `ipsec ike remote address 3 52.197.151.142` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 140 | `ipsec ike remote id 3 0.0.0.0/32` | PutTunnel | m_sRouter.uip, m_sRouter.con[n].ipなど | ProcIPSEC (ipsec ike ...) | m_sRouter.uip 等 | IKE設定 |
| 141 | `ipsec sa policy 103 3 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 142 | `tunnel backup none` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 143 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 144 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 145 | `ip tunnel nat descriptor 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 146 | `tunnel enable 3` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 148 | `### TUNNEL 6 ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 150 | `tunnel select 6` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 151 | `ipsec tunnel 106` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 152 | `ipsec ike duration ipsec-sa 6 10800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 153 | `ipsec ike duration isakmp-sa 6 3600` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 154 | `ipsec ike encryption 6 aes-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 155 | `ipsec ike group 6 modp768 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 156 | `ipsec ike hash 6 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 157 | `ipsec ike keepalive use 6 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 158 | `ipsec ike local address 6 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 159 | `ipsec ike local id 6 10.6.0.0/16` | PutTunnel | m_sRouter.uip, m_sRouter.con[n].ipなど | ProcIPSEC (ipsec ike ...) | m_sRouter.uip 等 | IKE設定 |
| 160 | `ipsec ike pfs 6 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 161 | `ipsec ike pre-shared-key 6 text wh1+eb0@rd` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 162 | `ipsec ike remote address 6 153.142.244.126` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 163 | `ipsec ike remote id 6 10.2.0.0/16` | PutTunnel | m_sRouter.uip, m_sRouter.con[n].ipなど | ProcIPSEC (ipsec ike ...) | m_sRouter.uip 等 | IKE設定 |
| 164 | `ipsec sa policy 106 6 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 165 | `tunnel backup none` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 166 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 167 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 168 | `ip tunnel nat descriptor 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 169 | `tunnel enable 6` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 171 | `### TUNNEL 7 ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 173 | `tunnel select 7` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 174 | `ipsec tunnel 107` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 175 | `ipsec ike duration ipsec-sa 7 10800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 176 | `ipsec ike duration isakmp-sa 7 3600` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 177 | `ipsec ike encryption 7 aes-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 178 | `ipsec ike group 7 modp768 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 179 | `ipsec ike hash 7 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 180 | `ipsec ike keepalive use 7 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 181 | `ipsec ike local address 7 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 182 | `ipsec ike local id 7 10.6.0.0/16` | PutTunnel | m_sRouter.uip, m_sRouter.con[n].ipなど | ProcIPSEC (ipsec ike ...) | m_sRouter.uip 等 | IKE設定 |
| 183 | `ipsec ike pfs 7 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 184 | `ipsec ike pre-shared-key 7 text wh1+eb0@rd` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 185 | `ipsec ike remote address 7 153.142.244.125` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 186 | `ipsec ike remote id 7 10.1.0.0/16` | PutTunnel | m_sRouter.uip, m_sRouter.con[n].ipなど | ProcIPSEC (ipsec ike ...) | m_sRouter.uip 等 | IKE設定 |
| 187 | `ipsec sa policy 107 7 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 188 | `tunnel backup none` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 189 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 190 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 191 | `ip tunnel nat descriptor 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 192 | `tunnel enable 7` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 194 | `### TUNNEL 5 ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 196 | `tunnel select 5` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 197 | `tunnel encapsulation ipsec` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 198 | `ipsec tunnel 105` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 199 | `ipsec sa policy 105 5 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 200 | `ipsec ike duration ipsec-sa 5 28800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 201 | `ipsec ike encryption 5 aes-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 202 | `ipsec ike group 5 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 203 | `ipsec ike hash 5 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 204 | `ipsec ike local address 5 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 205 | `ipsec ike pfs 5 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 206 | `ipsec ike pre-shared-key 5 text dkccsupport` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 207 | `ipsec ike remote address 5 any` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 208 | `ipsec ike remote name 5 ccsupport` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 209 | `ipsec ike keepalive use 5 on heartbeat 10 2` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 210 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 211 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 212 | `tunnel enable 5` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 214 | `### TUNNEL 4 ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 216 | `tunnel select 4` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 217 | `tunnel encapsulation ipsec` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 218 | `ipsec tunnel 104` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 219 | `ipsec sa policy 104 4 esp 3des-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 220 | `ipsec ike duration ipsec-sa 4 28800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 221 | `ipsec ike encryption 4 3des-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 222 | `ipsec ike group 4 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 223 | `ipsec ike hash 4 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 224 | `ipsec ike keepalive use 4 on heartbeat 60 10` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 225 | `ipsec ike local address 4 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 226 | `ipsec ike pfs 4 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 227 | `ipsec ike pre-shared-key 4 text 153.142.244.124` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 228 | `ipsec ike remote address 4 61.114.186.110` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 229 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 230 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 231 | `ip tunnel nat descriptor 2012` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 232 | `tunnel enable 4` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 233 | `tunnel select 16` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 234 | `tunnel encapsulation l2tp` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 235 | `ipsec tunnel 116` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 236 | `ipsec sa policy 116 16 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 237 | `ipsec ike keepalive use 16 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 238 | `ipsec ike nat-traversal 16 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 239 | `ipsec ike pre-shared-key 16 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 240 | `ipsec ike remote address 16 any` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 241 | `l2tp tunnel auth off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 242 | `l2tp tunnel disconnect time off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 243 | `l2tp keepalive use on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 244 | `ip tunnel tcp mss limit auto` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 245 | `ip tunnel secure filter name xai_tunnel_lan2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 246 | `tunnel enable 16` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 247 | `tunnel select 17` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 248 | `tunnel encapsulation l2tp` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 249 | `ipsec tunnel 117` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 250 | `ipsec sa policy 117 17 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 251 | `ipsec ike keepalive use 17 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 252 | `ipsec ike nat-traversal 17 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 253 | `ipsec ike pre-shared-key 17 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 254 | `ipsec ike remote address 17 any` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 255 | `l2tp tunnel auth off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 256 | `l2tp tunnel disconnect time off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 257 | `l2tp keepalive use on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 258 | `ip tunnel tcp mss limit auto` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 259 | `ip tunnel secure filter name xai_tunnel_lan2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 260 | `tunnel enable 17` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 261 | `tunnel select 18` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 262 | `tunnel encapsulation l2tp` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 263 | `ipsec tunnel 118` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 264 | `ipsec sa policy 118 18 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 265 | `ipsec ike keepalive use 18 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 266 | `ipsec ike nat-traversal 18 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 267 | `ipsec ike pre-shared-key 18 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 268 | `ipsec ike remote address 18 any` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 269 | `l2tp tunnel auth off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 270 | `l2tp tunnel disconnect time off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 271 | `l2tp keepalive use on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 272 | `ip tunnel tcp mss limit auto` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 273 | `ip tunnel secure filter name xai_tunnel_lan2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 274 | `tunnel enable 18` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 275 | `tunnel select 19` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 276 | `tunnel encapsulation l2tp` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 277 | `ipsec tunnel 119` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 278 | `ipsec sa policy 119 19 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 279 | `ipsec ike keepalive use 19 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 280 | `ipsec ike nat-traversal 19 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 281 | `ipsec ike pre-shared-key 19 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 282 | `ipsec ike remote address 19 any` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 283 | `l2tp tunnel auth off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 284 | `l2tp tunnel disconnect time off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 285 | `l2tp keepalive use on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 286 | `ip tunnel tcp mss limit auto` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 287 | `ip tunnel secure filter name xai_tunnel_lan2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 288 | `tunnel enable 19` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 289 | `tunnel select 20` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 290 | `tunnel encapsulation l2tp` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 291 | `ipsec tunnel 120` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 292 | `ipsec sa policy 120 20 esp aes-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 293 | `ipsec ike keepalive use 20 off` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 294 | `ipsec ike nat-traversal 20 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 295 | `ipsec ike pre-shared-key 20 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 296 | `ipsec ike remote address 20 any` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 297 | `l2tp tunnel auth off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 298 | `l2tp tunnel disconnect time off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 299 | `l2tp keepalive use on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 300 | `ip tunnel tcp mss limit auto` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 301 | `ip tunnel secure filter name xai_tunnel_lan2` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 302 | `tunnel enable 20` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 304 | `### IPSEC ###` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 306 | `ipsec transport 16 116 udp 1701` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 307 | `ipsec transport 17 117 udp 1701` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 308 | `ipsec transport 18 118 udp 1701` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 309 | `ipsec transport 19 119 udp 1701` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 310 | `ipsec transport 20 120 udp 1701` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 312 | `# IP filter configuration` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 314 | `ip filter source-route on` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 315 | `ip filter directed-broadcast on` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 316 | `tunnel select 2` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 317 | `tunnel encapsulation ipsec` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 318 | `ipsec tunnel 102` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 319 | `ipsec sa policy 102 2 esp 3des-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 320 | `ipsec ike duration ipsec-sa 2 28800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 321 | `ipsec ike encryption 2 3des-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 322 | `ipsec ike group 2 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 323 | `ipsec ike hash 2 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 324 | `ipsec ike keepalive use 2 on heartbeat 60 10` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 325 | `ipsec ike local address 2 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 326 | `ipsec ike pfs 2 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 327 | `ipsec ike pre-shared-key 2 text 153.142.244.124` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 328 | `ipsec ike remote address 2 192.0.2.1` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 329 | `tunnel backup tunnel 8 switch-interface=on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 330 | `ip tunnel nat descriptor 2012` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 331 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 332 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 333 | `tunnel enable 2` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 334 | `tunnel select 8` | PutTunnel | m_sRouter.con[n].no または nSetuzokuNo | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など現在選択中のトンネル変数 | Tunnel選択 |
| 335 | `tunnel encapsulation ipsec` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 336 | `ipsec tunnel 108` | PutTunnel | 設定先のnSetuzokuNo | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type等への紐付け | IPSec Tunnel宣言 |
| 337 | `ipsec sa policy 108 8 esp 3des-cbc sha-hmac` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 338 | `ipsec ike duration ipsec-sa 8 28800` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 339 | `ipsec ike encryption 8 3des-cbc` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 340 | `ipsec ike group 8 modp1024` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 341 | `ipsec ike hash 8 sha` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 342 | `ipsec ike keepalive use 8 on heartbeat 60 10` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 343 | `ipsec ike local address 8 172.16.5.1` | PutTunnel | m_sRouter.ip または m_sRouter.ncon[n].fixIp | ProcIPSEC (ipsec ike ...) | m_sRouter.ip等 ローカルIP格納先 | IKE設定 |
| 344 | `ipsec ike pfs 8 on` | PutTunnel | ルータ設定に紐づく接続情報(m_sRouter.con[n].type等) | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].X (IPsec各種属性メンバ) | IKE設定 |
| 345 | `ipsec ike pre-shared-key 8 text 153.142.244.124` | PutTunnel | m_sRouter.m_szJnetHoujin, m_sRouter.szVPN_Key等 | ProcIPSEC (ipsec ike ...) | m_sRouter.szVPN_Key 等 事前共有鍵格納先 | IKE設定 |
| 346 | `ipsec ike remote address 8 192.0.2.254` | PutTunnel | m_sRouter.ncon[n].fixIp, m_sRouter.sSIS05.accessIp等 | ProcIPSEC (ipsec ike ...) | m_sRouter.ncon[n].fixIp 等 | IKE設定 |
| 347 | `ip tunnel nat descriptor 2012` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 348 | `ip tunnel rip send off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 349 | `ip tunnel rip receive off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 350 | `tunnel enable 8` | PutTunnel / PutPp | m_sRouter.con[n].no 等の番号 | ProcTunnel / ProcPP | m_sRouter.con[n].type 有効化フラグ等 | 定義有効化 |
| 351 | `ip filter 2602 pass 192.0.2.1 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 352 | `ip filter 2608 pass 192.0.2.254 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 353 | `ip filter 2652 pass 172.16.5.1 192.0.2.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 354 | `ip filter 2658 pass 172.16.5.1 192.0.2.254 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 355 | `ip filter 2702 pass 192.0.2.1 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 356 | `ip filter 2708 pass 192.0.2.254 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 357 | `ip filter 2752 pass 172.16.5.1 192.0.2.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 358 | `ip filter 2758 pass 172.16.5.1 192.0.2.254 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 360 | `ip filter 1 reject * * udp,tcp netbios_ns-netbios_ssn *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 361 | `ip filter 2 reject * * udp,tcp * netbios_ns-netbios_ssn` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 362 | `ip filter 3 reject * * udp,tcp 135 *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 363 | `ip filter 4 reject * * udp,tcp * 135` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 364 | `ip filter 5 reject * * udp,tcp 445 *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 365 | `ip filter 6 reject * * udp,tcp * 445` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 366 | `ip filter 9 reject * * * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 367 | `ip filter 10 reject 172.16.0.0/16 * * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 368 | `ip filter 15 reject * 172.16.0.0/16 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 369 | `ip filter 20 pass 211.129.14.134,211.129.12.43 172.16.0.0/16 udp domain *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 370 | `ip filter 21 pass 211.129.14.134,211.129.12.43 172.16.5.0/24 udp domain *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 371 | `ip filter 22 pass 211.129.14.134,211.129.12.43 172.16.15.0/24 udp domain *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 372 | `ip filter 23 pass 211.129.14.134,211.129.12.43 172.16.60.0/24 udp domain *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 373 | `ip filter 25 pass 172.16.0.0/16 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 374 | `ip filter 26 pass 172.16.5.0/24 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 375 | `ip filter 27 pass 172.16.15.0/24 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 376 | `ip filter 28 pass 172.16.60.0/24 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 377 | `ip filter 30 pass 61.114.182.53 172.16.0.0/16 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 378 | `ip filter 31 pass 61.114.182.53 172.16.5.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 379 | `ip filter 32 pass 61.114.182.53 172.16.15.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 380 | `ip filter 33 pass 61.114.182.53 172.16.60.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 381 | `ip filter 35 pass 172.16.0.0/16 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 382 | `ip filter 36 pass 172.16.5.0/24 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 383 | `ip filter 37 pass 172.16.15.0/24 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 384 | `ip filter 38 pass 172.16.60.0/24 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 385 | `ip filter 40 pass 61.114.182.54 172.16.0.0/16 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 386 | `ip filter 41 pass 61.114.182.54 172.16.5.0/24 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 387 | `ip filter 42 pass 61.114.182.54 172.16.15.0/24 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 388 | `ip filter 43 pass 61.114.182.54 172.16.60.0/24 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 389 | `ip filter 45 pass 172.16.0.0/16 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 390 | `ip filter 46 pass 172.16.5.0/24 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 391 | `ip filter 47 pass 172.16.15.0/24 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 392 | `ip filter 48 pass 172.16.60.0/24 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 393 | `ip filter 50 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 394 | `ip filter 51 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 395 | `ip filter 52 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 396 | `ip filter 53 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 397 | `ip filter 54 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 398 | `ip filter 55 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 399 | `ip filter 56 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 400 | `ip filter 57 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 401 | `ip filter 58 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 402 | `ip filter 59 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 403 | `ip filter 60 pass 61.114.177.148 172.16.0.0/16 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 404 | `ip filter 61 pass 61.114.177.148 172.16.5.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 405 | `ip filter 62 pass 61.114.177.148 172.16.15.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 406 | `ip filter 63 pass 61.114.177.148 172.16.60.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 407 | `ip filter 65 pass 172.16.0.0/16 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 408 | `ip filter 66 pass 172.16.5.0/24 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 409 | `ip filter 67 pass 172.16.15.0/24 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 410 | `ip filter 68 pass 172.16.60.0/24 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 411 | `ip filter 70 pass 202.229.106.177 172.16.0.0/16 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 412 | `ip filter 71 pass 202.229.106.177 172.16.5.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 413 | `ip filter 72 pass 202.229.106.177 172.16.15.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 414 | `ip filter 73 pass 202.229.106.177 172.16.60.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 415 | `ip filter 74 pass 202.229.106.177 172.16.80.0/22 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 416 | `ip filter 75 pass 172.16.0.0/16 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 417 | `ip filter 76 pass 172.16.5.0/24 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 418 | `ip filter 77 pass 172.16.15.0/24 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 419 | `ip filter 78 pass 172.16.60.0/24 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 420 | `ip filter 79 pass 172.16.80.0/22 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 421 | `ip filter 80 pass 202.226.76.11 172.16.0.0/16 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 422 | `ip filter 81 pass 202.226.76.11 172.16.0.0/16 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 423 | `ip filter 82 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 424 | `ip filter 83 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 425 | `ip filter 84 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 426 | `ip filter 85 pass 172.16.0.0/16 202.226.76.11 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 427 | `ip filter 86 pass 172.16.0.0/16 202.226.76.11 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 428 | `ip filter 87 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 429 | `ip filter 88 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 430 | `ip filter 89 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 431 | `ip filter 92 pass * 172.16.5.0/24 tcp https,5222,5223 *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 432 | `ip filter 93 pass * 172.16.5.0/24 udp domain,ntp *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 433 | `ip filter 94 pass * 172.16.5.0/24 icmp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 434 | `ip filter 97 pass 172.16.5.0/24 * tcp * https,5222,5223` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 435 | `ip filter 98 pass 172.16.5.0/24 * udp * domain,ntp` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 436 | `ip filter 99 pass 172.16.5.0/24 * icmp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 437 | `ip filter 100 pass-log * * * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 438 | `ip filter 101 reject * * established * netbios_ns-netbios_ssn` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 439 | `ip filter 102 reject * * established * 135` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 440 | `ip filter 103 reject * * established * 445` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 441 | `ip filter 104 reject * * established * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 442 | `ip filter 105 reject * * established * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 443 | `ip filter 106 reject * * established * pop3` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 444 | `ip filter 107 reject * * established * smtp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 445 | `ip filter 1400 pass 172.16.0.0/16 * * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 446 | `ip filter 1401 pass 172.21.0.0/16 * * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 447 | `ip filter 1405 pass * 172.16.0.0/16 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 448 | `ip filter 1406 pass * 172.21.0.0/16 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 449 | `ip filter 1410 pass * * udp dhcpc dhcps` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 450 | `ip filter 1430 pass 10.0.0.0/8 10.0.208.0/20 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 451 | `ip filter 1431 pass 10.0.208.0/20 10.0.0.0/8 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 452 | `ip filter 1432 pass 10.0.0.0/8 172.30.1.110 tcp * 21` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 453 | `ip filter 1433 pass 172.30.1.110 10.0.0.0/8 tcp 21 *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 454 | `ip filter 1434 pass 10.0.0.0/8 172.30.1.110 tcp * ftpdata` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 455 | `ip filter 1435 pass 172.30.1.110 10.0.0.0/8 tcp ftpdata *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 456 | `ip filter 1436 pass 10.0.0.0/8 172.31.191.0/24 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 457 | `ip filter 1437 pass 172.31.191.0/24 10.0.0.0/8 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 458 | `ip filter 1438 pass 10.0.0.0/8 10.0.0.0/8 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 459 | `ip filter 1440 pass 172.18.0.0/24 10.0.192.0/18 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 460 | `ip filter 1441 pass 10.0.192.0/18 172.18.0.0/24 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 461 | `ip filter 2010 reject 172.16.0.0/16 * * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 462 | `ip filter 2015 reject * 172.16.0.0/16 * * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 463 | `ip filter 2025 pass 172.16.0.0/16 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 464 | `ip filter 2026 pass * 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 465 | `ip filter 2027 pass 172.16.15.0/24 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 466 | `ip filter 2028 pass 172.16.60.0/24 211.129.14.134,211.129.12.43 udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 467 | `ip filter 2030 pass 61.114.182.53 172.16.0.0/16 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 468 | `ip filter 2031 pass 61.114.182.53 172.16.5.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 469 | `ip filter 2032 pass 61.114.182.53 172.16.15.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 470 | `ip filter 2033 pass 61.114.182.53 172.16.60.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 471 | `ip filter 2035 pass 172.16.0.0/16 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 472 | `ip filter 2036 pass 172.16.5.0/24 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 473 | `ip filter 2037 pass 172.16.15.0/24 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 474 | `ip filter 2038 pass 172.16.60.0/24 61.114.182.53 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 475 | `ip filter 2040 pass 61.114.182.54 172.16.0.0/16 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 476 | `ip filter 2041 pass 61.114.182.54 172.16.5.0/24 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 477 | `ip filter 2042 pass 61.114.182.54 172.16.15.0/24 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 478 | `ip filter 2043 pass 61.114.182.54 172.16.60.0/24 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 479 | `ip filter 2045 pass 172.16.0.0/16 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 480 | `ip filter 2046 pass 172.16.5.0/24 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 481 | `ip filter 2047 pass 172.16.15.0/24 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 482 | `ip filter 2048 pass 172.16.60.0/24 61.114.182.54 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 483 | `ip filter 2050 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 484 | `ip filter 2051 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 485 | `ip filter 2052 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 486 | `ip filter 2053 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 487 | `ip filter 2054 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 488 | `ip filter 2055 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 489 | `ip filter 2056 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 490 | `ip filter 2057 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 491 | `ip filter 2058 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 492 | `ip filter 2059 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 493 | `ip filter 2060 pass 61.114.177.148 172.16.0.0/16 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 494 | `ip filter 2061 pass 61.114.177.148 172.16.5.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 495 | `ip filter 2062 pass 61.114.177.148 172.16.15.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 496 | `ip filter 2063 pass 61.114.177.148 172.16.60.0/24 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 497 | `ip filter 2065 pass 172.16.0.0/16 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 498 | `ip filter 2066 pass 172.16.5.0/24 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 499 | `ip filter 2067 pass 172.16.15.0/24 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 500 | `ip filter 2068 pass 172.16.60.0/24 61.114.177.148 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 501 | `ip filter 2070 pass 202.229.106.177 172.16.0.0/16 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 502 | `ip filter 2071 pass 202.229.106.177 172.16.5.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 503 | `ip filter 2072 pass 202.229.106.177 172.16.15.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 504 | `ip filter 2073 pass 202.229.106.177 172.16.60.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 505 | `ip filter 2074 pass 202.229.106.177 172.16.80.0/22 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 506 | `ip filter 2075 pass 172.16.0.0/16 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 507 | `ip filter 2076 pass 172.16.5.0/24 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 508 | `ip filter 2077 pass 172.16.15.0/24 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 509 | `ip filter 2078 pass 172.16.60.0/24 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 510 | `ip filter 2079 pass 172.16.80.0/22 202.229.106.177 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 511 | `ip filter 2080 pass 202.226.76.11 172.16.0.0/16 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 512 | `ip filter 2081 pass 202.226.76.11 172.16.0.0/16 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 513 | `ip filter 2082 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 514 | `ip filter 2083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 515 | `ip filter 2084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 516 | `ip filter 2085 pass 172.16.0.0/16 202.226.76.11 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 517 | `ip filter 2086 pass 172.16.0.0/16 202.226.76.11 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 518 | `ip filter 2087 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 519 | `ip filter 2088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 520 | `ip filter 2089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 521 | `ip filter 2092 pass * 172.16.5.0/24 tcp https,5222,5223 *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 522 | `ip filter 2093 pass * 172.16.5.0/24 udp domain,ntp *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 523 | `ip filter 2094 pass * 172.16.5.0/24 icmp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 524 | `ip filter 2097 pass 172.16.5.0/24 * tcp * https,5222,5223` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 525 | `ip filter 2098 pass 172.16.5.0/24 * udp * domain,ntp` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 526 | `ip filter 2099 pass 172.16.5.0/24 * icmp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 527 | `ip filter 2200 pass * 172.16.5.1 tcp * 1723` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 528 | `ip filter 2250 pass 172.16.5.1 * tcp 1723 *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 529 | `ip filter 2300 pass * 172.16.5.1 gre * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 530 | `ip filter 2350 pass 172.16.5.1 * gre * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 531 | `ip filter 2600 pass 180.42.57.138,180.42.57.139,180.42.57.140,180.42.57.141,180.42.57.142,60.32.143.115,202.41.213.169,222.151.236.147,125.206.255.43,202.124.220.210,202.124.220.211,203.129.125.175,180.42.70.99,202.124.220.212,202.124.220.213,202.124.220.214,202.124.220.215,202.124.220.216,202.124.220.217,202.124.220.218,202.124.220.219,202.124.220.220,202.124.220.221,202.124.220.222,203.129.125.172,203.129.125.173,203.129.125.174,203.129.125.171,14.192.44.211,14.192.44.212,14.192.44.213,14.192.44.214,14.192.44.215,14.192.44.216,14.192.44.217,14.192.44.218,14.192.44.219,14.192.44.220,14.192.44.221,14.192.44.222,180.42.70.98,180.42.70.101,114.179.30.27,114.179.105.106,114.179.105.107,118.22.14.107,118.22.14.106,124.110.99.116,124.110.99.117,124.110.99.118,124.110.99.119,124.110.99.120,124.110.99.121,124.110.99.122,124.110.99.123,210.162.143.68,210.162.143.69,210.162.143.70,210.162.143.71,210.227.65.146,210.227.65.147,210.227.65.148,210.227.65.149,122.103.88.162,122.103.88.163,122.103.88.164,122.103.88.165,122.20.254.42,153.150.112.121,153.150.112.122 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 532 | `ip filter 2601 pass 61.114.186.111 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 533 | `ip filter 2603 pass 52.197.151.142 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 534 | `ip filter 2604 pass 61.114.186.110 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 535 | `ip filter 2606 pass 153.142.244.126 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 536 | `ip filter 2607 pass 153.142.244.125 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 537 | `ip filter 2650 pass 172.16.5.1 180.42.57.138,180.42.57.139,180.42.57.140,180.42.57.141,180.42.57.142,60.32.143.115,202.41.213.169,222.151.236.147,125.206.255.43,202.124.220.210,202.124.220.211,203.129.125.175,180.42.70.99,202.124.220.212,202.124.220.213,202.124.220.214,202.124.220.215,202.124.220.216,202.124.220.217,202.124.220.218,202.124.220.219,202.124.220.220,202.124.220.221,202.124.220.222,203.129.125.172,203.129.125.173,203.129.125.174,203.129.125.171,14.192.44.211,14.192.44.212,14.192.44.213,14.192.44.214,14.192.44.215,14.192.44.216,14.192.44.217,14.192.44.218,14.192.44.219,14.192.44.220,14.192.44.221,14.192.44.222,180.42.70.98,180.42.70.101,114.179.30.27,114.179.105.106,114.179.105.107,118.22.14.107,118.22.14.106,124.110.99.116,124.110.99.117,124.110.99.118,124.110.99.119,124.110.99.120,124.110.99.121,124.110.99.122,124.110.99.123,210.162.143.68,210.162.143.69,210.162.143.70,210.162.143.71,210.227.65.146,210.227.65.147,210.227.65.148,210.227.65.149,122.103.88.162,122.103.88.163,122.103.88.164,122.103.88.165,122.20.254.42,153.150.112.121,153.150.112.122 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 538 | `ip filter 2651 pass 172.16.5.1 61.114.186.111 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 539 | `ip filter 2653 pass 172.16.5.1 52.197.151.142 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 540 | `ip filter 2654 pass 172.16.5.1 61.114.186.110 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 541 | `ip filter 2656 pass 172.16.5.1 153.142.244.126 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 542 | `ip filter 2657 pass 172.16.5.1 153.142.244.125 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 543 | `ip filter 2700 pass 180.42.57.138,180.42.57.139,180.42.57.140,180.42.57.141,180.42.57.142,60.32.143.115,202.41.213.169,222.151.236.147,125.206.255.43,202.124.220.210,202.124.220.211,203.129.125.175,180.42.70.99,202.124.220.212,202.124.220.213,202.124.220.214,202.124.220.215,202.124.220.216,202.124.220.217,202.124.220.218,202.124.220.219,202.124.220.220,202.124.220.221,202.124.220.222,203.129.125.172,203.129.125.173,203.129.125.174,203.129.125.171,14.192.44.211,14.192.44.212,14.192.44.213,14.192.44.214,14.192.44.215,14.192.44.216,14.192.44.217,14.192.44.218,14.192.44.219,14.192.44.220,14.192.44.221,14.192.44.222,180.42.70.98,180.42.70.101,114.179.30.27,114.179.105.106,114.179.105.107,118.22.14.107,118.22.14.106,124.110.99.116,124.110.99.117,124.110.99.118,124.110.99.119,124.110.99.120,124.110.99.121,124.110.99.122,124.110.99.123,210.162.143.68,210.162.143.69,210.162.143.70,210.162.143.71,210.227.65.146,210.227.65.147,210.227.65.148,210.227.65.149,122.103.88.162,122.103.88.163,122.103.88.164,122.103.88.165,122.20.254.42,153.150.112.121,153.150.112.122 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 544 | `ip filter 2701 pass 61.114.186.111 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 545 | `ip filter 2703 pass 52.197.151.142 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 546 | `ip filter 2704 pass 61.114.186.110 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 547 | `ip filter 2706 pass 153.142.244.126 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 548 | `ip filter 2707 pass 153.142.244.125 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 549 | `ip filter 2750 pass 172.16.5.1 180.42.57.138,180.42.57.139,180.42.57.140,180.42.57.141,180.42.57.142,60.32.143.115,202.41.213.169,222.151.236.147,125.206.255.43,202.124.220.210,202.124.220.211,203.129.125.175,180.42.70.99,202.124.220.212,202.124.220.213,202.124.220.214,202.124.220.215,202.124.220.216,202.124.220.217,202.124.220.218,202.124.220.219,202.124.220.220,202.124.220.221,202.124.220.222,203.129.125.172,203.129.125.173,203.129.125.174,203.129.125.171,14.192.44.211,14.192.44.212,14.192.44.213,14.192.44.214,14.192.44.215,14.192.44.216,14.192.44.217,14.192.44.218,14.192.44.219,14.192.44.220,14.192.44.221,14.192.44.222,180.42.70.98,180.42.70.101,114.179.30.27,114.179.105.106,114.179.105.107,118.22.14.107,118.22.14.106,124.110.99.116,124.110.99.117,124.110.99.118,124.110.99.119,124.110.99.120,124.110.99.121,124.110.99.122,124.110.99.123,210.162.143.68,210.162.143.69,210.162.143.70,210.162.143.71,210.227.65.146,210.227.65.147,210.227.65.148,210.227.65.149,122.103.88.162,122.103.88.163,122.103.88.164,122.103.88.165,122.20.254.42,153.150.112.121,153.150.112.122 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 550 | `ip filter 2751 pass 172.16.5.1 61.114.186.111 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 551 | `ip filter 2753 pass 172.16.5.1 52.197.151.142 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 552 | `ip filter 2754 pass 172.16.5.1 61.114.186.110 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 553 | `ip filter 2756 pass 172.16.5.1 153.142.244.126 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 554 | `ip filter 2757 pass 172.16.5.1 153.142.244.125 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 555 | `ip filter 2900 pass * 172.16.0.0/16 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 556 | `ip filter 2901 pass 61.114.186.111 172.16.0.0/16 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 557 | `ip filter 2903 pass 52.197.151.142 172.16.0.0/16 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 558 | `ip filter 2904 pass 61.114.186.110 172.16.0.0/16 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 559 | `ip filter 2906 pass 153.142.244.126 172.16.0.0/16 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 560 | `ip filter 2907 pass 153.142.244.125 172.16.0.0/16 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 561 | `ip filter 2950 pass 172.16.0.0/16 * icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 562 | `ip filter 2951 pass 172.16.0.0/16 61.114.186.111 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 563 | `ip filter 2953 pass 172.16.0.0/16 52.197.151.142 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 564 | `ip filter 2954 pass 172.16.0.0/16 61.114.186.110 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 565 | `ip filter 2956 pass 172.16.0.0/16 153.142.244.126 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 566 | `ip filter 2957 pass 172.16.0.0/16 153.142.244.125 icmp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 567 | `ip filter 3080 pass 202.226.76.11 172.16.0.0/16 tcp https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 568 | `ip filter 3081 pass 202.226.76.11 172.16.0.0/16 tcp www *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 569 | `ip filter 3082 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 570 | `ip filter 3083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 571 | `ip filter 3084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 572 | `ip filter 3085 pass 172.16.0.0/16 202.226.76.11 tcp * https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 573 | `ip filter 3086 pass 172.16.0.0/16 202.226.76.11 tcp * www` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 574 | `ip filter 3087 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 575 | `ip filter 3088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 576 | `ip filter 3089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 577 | `ip filter 3092 pass * 172.16.5.0/24 tcp https,5222,5223 *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 578 | `ip filter 3093 pass * 172.16.5.0/24 udp domain,ntp *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 579 | `ip filter 3094 pass * 172.16.5.0/24 icmp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 580 | `ip filter 3097 pass 172.16.5.0/24 * tcp * https,5222,5223` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 581 | `ip filter 3098 pass 172.16.5.0/24 * udp * domain,ntp` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 582 | `ip filter 3099 pass 172.16.5.0/24 * icmp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 583 | `ip filter 101402 pass * - udp ntp *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 584 | `ip filter 101404 pass * - tcp www,https *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 585 | `ip filter 101406 pass - 180.55.241.146 udp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 586 | `ip filter 101407 pass - * udp * ntp` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 587 | `ip filter 101408 pass - * udp * domain` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 588 | `ip filter 101409 pass - * tcp * www,https` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 589 | `ip filter 10800 pass 18.179.137.187,3.115.131.124 172.16.15.0/24 tcp www,https,8080 *` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 590 | `ip filter 10801 pass 172.16.15.0/24 18.179.137.187,3.115.131.124 tcp * www,https,8080` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 591 | `ip filter 10500 pass * 172.16.5.1 tcp * 53196` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 592 | `ip filter 10505 pass 172.16.5.1 * tcp 53196` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 593 | `ip filter 10510 pass 172.16.5.1 172.16.5.1 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 594 | `ip filter 10520 pass 172.16.5.1 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 595 | `ip filter 10525 pass 172.16.5.1 172.16.5.1 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 596 | `ip filter 10550 pass 172.16.5.1 172.16.5.1 udp * 1701` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 597 | `ip filter 10560 pass 172.16.5.1 172.16.5.1 udp * 4500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 598 | `ip filter 10575 pass 172.16.5.1 172.16.5.1 udp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 599 | `ip filter 10600 pass 172.16.60.0/24 172.16.60.0/24 tcp * 58300` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 600 | `ip filter 10605 pass 172.16.60.0/24 172.16.60.0/24 tcp 58300 *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 601 | `ip filter 10610 pass 172.16.60.0/24 172.16.60.0/24 udp 58301 *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 602 | `ip filter 10615 pass 172.16.60.0/24 172.16.60.0/24 udp * 58301` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 603 | `ip filter 10620 pass 172.16.60.0/24 172.16.60.0/24 tcp * 58378` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 604 | `ip filter 10625 pass 172.16.60.0/24 172.16.60.0/24 tcp 58378 *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 605 | `ip filter 12600 pass 221.186.163.29,180.42.107.1,153.139.247.107,222.151.227.83,221.186.163.27,221.186.163.28,220.110.177.59,220.110.177.60,122.1.9.170,122.1.9.171,122.1.9.172,122.1.9.173,122.1.9.174,125.206.196.139,125.206.210.19,60.32.159.235,60.32.143.116,60.32.143.117,202.41.213.170,202.41.213.171,202.41.213.172,202.41.213.173,221.186.110.83,60.32.158.219,114.156.254.18,222.151.236.148,222.151.236.149,222.151.236.146,153.231.254.66,153.231.254.67,60.32.188.115,60.32.188.116,60.32.123.123,60.32.123.124,125.206.255.44,125.206.255.45,60.32.122.123,60.32.122.124,180.42.70.100,180.42.70.102,180.42.70.103,180.42.70.104,180.42.70.105,118.22.14.108,118.22.14.109,153.150.21.247 172.16.5.0/24 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 606 | `ip filter 12650 pass 172.16.5.0/24 221.186.163.29,180.42.107.1,153.139.247.107,222.151.227.83,221.186.163.27,221.186.163.28,220.110.177.59,220.110.177.60,122.1.9.170,122.1.9.171,122.1.9.172,122.1.9.173,122.1.9.174,125.206.196.139,125.206.210.19,60.32.159.235,60.32.143.116,60.32.143.117,202.41.213.170,202.41.213.171,202.41.213.172,202.41.213.173,221.186.110.83,60.32.158.219,114.156.254.18,222.151.236.148,222.151.236.149,222.151.236.146,153.231.254.66,153.231.254.67,60.32.188.115,60.32.188.116,60.32.123.123,60.32.123.124,125.206.255.44,125.206.255.45,60.32.122.123,60.32.122.124,180.42.70.100,180.42.70.102,180.42.70.103,180.42.70.104,180.42.70.105,118.22.14.108,118.22.14.109,153.150.21.247 udp * 500` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 607 | `ip filter 12700 pass 221.186.163.29,180.42.107.1,153.139.247.107,222.151.227.83,221.186.163.27,221.186.163.28,220.110.177.59,220.110.177.60,122.1.9.170,122.1.9.171,122.1.9.172,122.1.9.173,122.1.9.174,125.206.196.139,125.206.210.19,60.32.159.235,60.32.143.116,60.32.143.117,202.41.213.170,202.41.213.171,202.41.213.172,202.41.213.173,221.186.110.83,60.32.158.219,114.156.254.18,222.151.236.148,222.151.236.149,222.151.236.146,153.231.254.66,153.231.254.67,60.32.188.115,60.32.188.116,60.32.123.123,60.32.123.124,125.206.255.44,125.206.255.45,60.32.122.123,60.32.122.124,180.42.70.100,180.42.70.102,180.42.70.103,180.42.70.104,180.42.70.105,118.22.14.108,118.22.14.109,153.150.21.247 172.16.5.0/24 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 608 | `ip filter 12750 pass 172.16.5.0/24 221.186.163.29,180.42.107.1,153.139.247.107,222.151.227.83,221.186.163.27,221.186.163.28,220.110.177.59,220.110.177.60,122.1.9.170,122.1.9.171,122.1.9.172,122.1.9.173,122.1.9.174,125.206.196.139,125.206.210.19,60.32.159.235,60.32.143.116,60.32.143.117,202.41.213.170,202.41.213.171,202.41.213.172,202.41.213.173,221.186.110.83,60.32.158.219,114.156.254.18,222.151.236.148,222.151.236.149,222.151.236.146,153.231.254.66,153.231.254.67,60.32.188.115,60.32.188.116,60.32.123.123,60.32.123.124,125.206.255.44,125.206.255.45,60.32.122.123,60.32.122.124,180.42.70.100,180.42.70.102,180.42.70.103,180.42.70.104,180.42.70.105,118.22.14.108,118.22.14.109,153.150.21.247 esp * *` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 609 | `ip filter dynamic 25 172.16.0.0/16 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 610 | `ip filter dynamic 26 172.16.5.0/24 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 611 | `ip filter dynamic 27 172.16.15.0/24 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 612 | `ip filter dynamic 28 172.16.60.0/24 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 613 | `ip filter dynamic 35 172.16.0.0/16 61.114.182.53 filter 35 in 30` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 614 | `ip filter dynamic 36 172.16.5.0/24 61.114.182.53 filter 36 in 31` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 615 | `ip filter dynamic 37 172.16.15.0/24 61.114.182.53 filter 37 in 32` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 616 | `ip filter dynamic 38 172.16.60.0/24 61.114.182.53 filter 38 in 33` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 617 | `ip filter dynamic 45 172.16.0.0/16 61.114.182.54 filter 45 in 40` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 618 | `ip filter dynamic 46 172.16.5.0/24 61.114.182.54 filter 46 in 41` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 619 | `ip filter dynamic 47 172.16.15.0/24 61.114.182.54 filter 47 in 42` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 620 | `ip filter dynamic 48 172.16.60.0/24 61.114.182.54 filter 48 in 43` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 621 | `ip filter dynamic 55 172.16.0.0/16 61.114.186.0/25 filter 55 in 50` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 622 | `ip filter dynamic 56 172.16.5.0/24 61.114.186.0/25 filter 56 in 51` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 623 | `ip filter dynamic 57 172.16.15.0/24 61.114.186.0/25 filter 57 in 52` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 624 | `ip filter dynamic 58 172.16.60.0/24 61.114.186.0/25 filter 58 in 53` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 625 | `ip filter dynamic 59 172.16.80.0/22 61.114.186.0/25 filter 59 in 54` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 626 | `ip filter dynamic 65 172.16.0.0/16 61.114.177.148 filter 65 in 60` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 627 | `ip filter dynamic 66 172.16.5.0/24 61.114.177.148 filter 66 in 61` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 628 | `ip filter dynamic 67 172.16.15.0/24 61.114.177.148 filter 67 in 62` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 629 | `ip filter dynamic 68 172.16.60.0/24 61.114.177.148 filter 68 in 63` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 630 | `ip filter dynamic 75 172.16.0.0/16 202.229.106.177 filter 75 in 70` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 631 | `ip filter dynamic 76 172.16.5.0/24 202.229.106.177 filter 76 in 71` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 632 | `ip filter dynamic 77 172.16.15.0/24 202.229.106.177 filter 77 in 72` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 633 | `ip filter dynamic 78 172.16.60.0/24 202.229.106.177 filter 78 in 73` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 634 | `ip filter dynamic 79 172.16.80.0/22 202.229.106.177 filter 79 in 74` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 635 | `ip filter dynamic 85 172.16.0.0/16 * filter 85 in 80` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 636 | `ip filter dynamic 86 172.16.0.0/16 * filter 86 in 81` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 637 | `ip filter dynamic 87 172.16.15.0/24 * filter 87 in 82` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 638 | `ip filter dynamic 88 172.16.0.0/16 * filter 88 in 83` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 639 | `ip filter dynamic 89 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 89 in 84` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 640 | `ip filter dynamic 97 172.16.5.0/24 * filter 97 in 92` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 641 | `ip filter dynamic 98 172.16.5.0/24 * filter 98 in 93` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 642 | `ip filter dynamic 99 172.16.5.0/24 * filter 99 in 94` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 643 | `ip filter dynamic 2097 172.16.5.0/24 * filter 2097 in 2092` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 644 | `ip filter dynamic 2098 172.16.5.0/24 * filter 2098 in 2093` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 645 | `ip filter dynamic 2099 172.16.5.0/24 * filter 2099 in 2094` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 646 | `ip filter dynamic 3097 172.16.5.0/24 * filter 3097 in 3092` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 647 | `ip filter dynamic 3098 172.16.5.0/24 * filter 3098 in 3093` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 648 | `ip filter dynamic 3099 172.16.5.0/24 * filter 3099 in 3094` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 649 | `ip filter dynamic 2025 172.16.0.0/16 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 650 | `ip filter dynamic 2026 * 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 651 | `ip filter dynamic 2027 172.16.15.0/24 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 652 | `ip filter dynamic 2028 172.16.60.0/24 211.129.14.134,211.129.12.43 domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 653 | `ip filter dynamic 2035 172.16.0.0/16 61.114.182.53 filter 2035 in 2030` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 654 | `ip filter dynamic 2036 172.16.5.0/24 61.114.182.53 filter 2036 in 2031` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 655 | `ip filter dynamic 2037 172.16.15.0/24 61.114.182.53 filter 2037 in 2032` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 656 | `ip filter dynamic 2038 172.16.60.0/24 61.114.182.53 filter 2038 in 2033` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 657 | `ip filter dynamic 2045 172.16.0.0/16 61.114.182.54 filter 2045 in 2040` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 658 | `ip filter dynamic 2046 172.16.5.0/24 61.114.182.54 filter 2046 in 2041` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 659 | `ip filter dynamic 2047 172.16.15.0/24 61.114.182.54 filter 2047 in 2042` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 660 | `ip filter dynamic 2048 172.16.60.0/24 61.114.182.54 filter 2048 in 2043` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 661 | `ip filter dynamic 2055 172.16.0.0/16 61.114.186.0/25 filter 2055 in 2050` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 662 | `ip filter dynamic 2056 172.16.5.0/24 61.114.186.0/25 filter 2056 in 2051` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 663 | `ip filter dynamic 2057 172.16.15.0/24 61.114.186.0/25 filter 2057 in 2052` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 664 | `ip filter dynamic 2058 172.16.60.0/24 61.114.186.0/25 filter 2058 in 2053` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 665 | `ip filter dynamic 2059 172.16.80.0/22 61.114.186.0/25 filter 2059 in 2054` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 666 | `ip filter dynamic 2065 172.16.0.0/16 61.114.177.148 filter 2065 in 2060` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 667 | `ip filter dynamic 2066 172.16.5.0/24 61.114.177.148 filter 2066 in 2061` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 668 | `ip filter dynamic 2067 172.16.15.0/24 61.114.177.148 filter 2067 in 2062` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 669 | `ip filter dynamic 2068 172.16.60.0/24 61.114.177.148 filter 2068 in 2063` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 670 | `ip filter dynamic 2075 172.16.0.0/16 202.229.106.177 filter 2075 in 2070` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 671 | `ip filter dynamic 2076 172.16.5.0/24 202.229.106.177 filter 2076 in 2071` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 672 | `ip filter dynamic 2077 172.16.15.0/24 202.229.106.177 filter 2077 in 2072` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 673 | `ip filter dynamic 2078 172.16.60.0/24 202.229.106.177 filter 2078 in 2073` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 674 | `ip filter dynamic 2079 172.16.80.0/22 202.229.106.177 filter 2079 in 2074` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 675 | `ip filter dynamic 2085 172.16.0.0/16 * filter 2085 in 2080` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 676 | `ip filter dynamic 2086 172.16.0.0/16 * filter 2086 in 2081` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 677 | `ip filter dynamic 2087 172.16.15.0/24 * filter 2087 in 2082` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 678 | `ip filter dynamic 2088 172.16.0.0/16 * filter 2088 in 2083` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 679 | `ip filter dynamic 2089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 2089 in 2084` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 680 | `ip filter dynamic 101406 - 180.55.241.146 udp` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 681 | `ip filter dynamic 101407 - * filter 101407 in 101402` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 682 | `ip filter dynamic 101408 - * domain` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 683 | `ip filter dynamic 101409 - * filter 101409 in 101404` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 684 | `ip filter dynamic 3085 172.16.0.0/16 * filter 3085 in 3080` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 685 | `ip filter dynamic 3086 172.16.0.0/16 * filter 3086 in 3081` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 686 | `ip filter dynamic 3087 172.16.15.0/24 * filter 3087 in 3082` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 687 | `ip filter dynamic 3088 172.16.0.0/16 * filter 3088 in 3083` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 688 | `ip filter dynamic 3089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 3089 in 3084` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 689 | `ip filter dynamic 10615 172.16.60.0/24 172.16.60.0/24 filter 10615 in 10610 10620 out 10625` | PutFilter | m_sRouter.mask, m_sRouter.sLAN3.mask, m_sRouter.sLAN3.ipVirus 等 | ProcFilter (ip filter dynamic ...) | m_sRouterのIPおよび接続先設定フラグ等 | 動的フィルタ |
| 690 | `ip filter set xai_tunnel_lan2 out 10605 10615 dynamic 10615` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 691 | `ip filter set dk_lan1 in 1400 1405 1410 1430 1431 1432 1433 1434 1435 1436 1437 1440 1441` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 692 | `ip filter set dk_lan1 out 1400 1405 1430 1431 1432 1433 1434 1435 1436 1437 1440 1441` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 693 | `ip filter set dk_pp_lan2 in 1 2 3 4 5 6 2010 10800 12600 12700 2200 2300 2600 2601 2603 2604 2606 2607 2700 2701 2703 2704 2706 2707 2901 2903 2904 2906 2907 10500 10510 10520 10550 10560 2602 2608 2702 2708` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 694 | `ip filter set dk_pp_lan2 out 1 2 3 4 5 6 104 105 106 107 2015 2026 2037 2038 2047 2048 2057 2058 2059 2067 2068 2077 2078 2079 2085 2086 2087 2088 2089 2097 2098 2099 101406 101407 101408 101409 10801 12650 12750 2250 2350 2650 2651 2653 2654 2656 2657 2750 2751 2753 2754 2756 2757 2951 2953 2954 2956 2957 10505 10525 10575 2652 2658 2752 2758 dynamic 2026 2037 2038 2047 2048 2057 2058 2059 2067 2068 2077 2078 2079 2085 2086 2087 2088 2089 2097 2098 2099 101406 101407 101408 101409` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 695 | `ip filter set dk_lan3 in 3 4 3010 3420 3421 3500 3520 3530 3550 3560 3580 3590 3801 3802 3803 3804 dynamic 3530 3550 3590` | PutFilter | m_sRouter.con[n].ip, m_sRouter.sLAN3.ipVirus, m_sRouter.ip 等 | ProcFilter (ip filter ...) | m_sRouter.con[n].ip 等 | 静的フィルタ設定 |
| 696 | `ip filter set dk_lan3 out 3 4 104 105 3015 3425 3426 3505 3525 3565 3585 3806 3807 3808 3809` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 697 | `ip filter set dk_pp_lab in 9` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 698 | `ip filter set dk_pp_lab out 1 2 3 4 5 6 104 105 15 37 38 47 48 57 58 59 67 68 77 78 79 dynamic 37 38 47 48 57 58 59 67 68 77 78 79` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 699 | `ip filter set getlog in 1 2 3 4 5 6 100` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 700 | `ip filter set getlog out 1 2 3 4 5 6 100` | PutFilter | m_sRouter.ip または m_sRouter.mask 等 | ProcFilter (ip filter ...) | m_sRouter.ip, m_sRouter.mask等 | 静的フィルタ設定 |
| 702 | `# NAT Descriptor configuration` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 704 | `nat descriptor log on` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 705 | `nat descriptor masquerade ttl hold all` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 706 | `nat descriptor type 1 nat` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 707 | `nat descriptor address outer 1 10.6.5.1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | m_sRouter.uip または m_sRouter.sLAN3.ip 等 | ProcNAT (nat descriptor ...) | m_sRouter.uip 等 (外側アドレス定義) | NAT設定 |
| 708 | `nat descriptor address inner 1 172.16.5.1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | m_sRouter.ip または m_sRouter.sLAN3.ipVirus 等 | ProcNAT (nat descriptor ...) | m_sRouter.ip 等 (内側アドレス定義) | NAT設定 |
| 709 | `nat descriptor static 1 1 10.6.1.1=172.16.1.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 710 | `nat descriptor static 1 2 10.6.2.1=172.16.2.1 2` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 711 | `nat descriptor static 1 4 10.6.4.1=172.16.4.1 8` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 712 | `nat descriptor static 1 5 10.6.5.1=172.16.5.1 10` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 713 | `nat descriptor static 1 10 10.6.10.1=172.16.10.1 2` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 714 | `nat descriptor static 1 11 10.6.11.1=172.16.11.1 128` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 715 | `nat descriptor static 1 12 10.6.12.1=172.16.12.1 128` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 716 | `nat descriptor static 1 13 10.6.13.1=172.16.13.1 8` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 717 | `nat descriptor static 1 14 10.6.14.1=172.16.14.1 12` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 718 | `nat descriptor static 1 15 10.6.15.1=172.16.15.1 8` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 719 | `nat descriptor static 1 20 10.6.20.1=172.16.20.1 2` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 720 | `nat descriptor static 1 21 10.6.21.1=172.16.21.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 721 | `nat descriptor static 1 30 10.6.30.1=172.16.30.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 722 | `nat descriptor static 1 31 10.6.31.1=172.16.31.1 8` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 723 | `nat descriptor static 1 40 10.6.40.1=172.16.40.1 32` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 724 | `nat descriptor static 1 50 10.6.50.1=172.16.50.1 3` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 725 | `nat descriptor static 1 52 10.6.52.1=172.16.52.1 8` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 726 | `nat descriptor static 1 60 10.6.60.1=172.16.60.1 127` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 727 | `nat descriptor static 1 121 10.6.121.20=172.16.121.20 40` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 728 | `nat descriptor static 1 80 10.6.80.1=172.16.80.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 729 | `nat descriptor static 1 81 10.6.81.1=172.16.81.1 16` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 730 | `nat descriptor static 1 82 10.6.82.1=172.16.82.1 16` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 731 | `nat descriptor static 1 83 10.6.83.1=172.16.83.1 16` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 732 | `nat descriptor static 1 110 10.6.110.1=172.16.110.1 2` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 733 | `nat descriptor static 1 100 10.6.100.1=172.16.100.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 734 | `nat descriptor static 1 122 10.6.122.1=172.16.122.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 735 | `nat descriptor type 2000 masquerade` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 736 | `nat descriptor address outer 2000 153.142.244.124` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | m_sRouter.uip または m_sRouter.sLAN3.ip 等 | ProcNAT (nat descriptor ...) | m_sRouter.uip 等 (外側アドレス定義) | NAT設定 |
| 737 | `nat descriptor masquerade static 2000 1 172.16.5.1 udp 500` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 738 | `nat descriptor masquerade static 2000 2 172.16.5.1 esp` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 739 | `nat descriptor masquerade static 2000 6 172.16.5.1 udp 4500` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 740 | `nat descriptor masquerade static 2000 7 172.16.5.1 tcp 53196` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 741 | `nat descriptor type 2012 masquerade` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 742 | `nat descriptor address outer 2012 172.23.176.129` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | m_sRouter.uip または m_sRouter.sLAN3.ip 等 | ProcNAT (nat descriptor ...) | m_sRouter.uip 等 (外側アドレス定義) | NAT設定 |
| 743 | `nat descriptor address inner 2012 auto` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | m_sRouter.ip または m_sRouter.sLAN3.ipVirus 等 | ProcNAT (nat descriptor ...) | m_sRouter.ip 等 (内側アドレス定義) | NAT設定 |
| 744 | `nat descriptor static 2012 2 172.23.176.130=172.16.5.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 745 | `nat descriptor static 2012 3 172.23.176.131=172.16.60.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 746 | `nat descriptor static 2012 4 172.23.176.132=172.16.1.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 747 | `nat descriptor static 2012 5 172.23.176.133=172.16.21.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 748 | `nat descriptor static 2012 6 172.23.176.134=172.16.80.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 749 | `nat descriptor static 2012 7 172.23.176.135=172.16.15.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 750 | `nat descriptor static 2012 8 172.23.176.136=172.16.52.8 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 751 | `nat descriptor static 2012 9 172.23.176.137=172.16.50.2 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 752 | `nat descriptor static 2012 10 172.23.176.138=172.16.20.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 753 | `nat descriptor static 2012 11 172.23.176.139=172.16.20.2 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 754 | `nat descriptor static 2012 12 172.23.176.140=172.16.110.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 755 | `nat descriptor static 2012 13 172.23.176.141=172.16.110.2 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 756 | `nat descriptor static 2012 14 172.23.176.142=172.16.122.1 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 757 | `nat descriptor static 2012 15 172.23.176.143=172.16.15.100 1` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 758 | `nat descriptor type 12 masquerade` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 759 | `nat descriptor masquerade static 2000 3 172.16.5.1 tcp 1723` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 760 | `nat descriptor masquerade static 2000 4 172.16.5.1 gre` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 761 | `nat descriptor masquerade static 2000 5 172.16.5.1 udp 4500` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 762 | `nat descriptor sip 2000 off` | PutNat_C / PutNat_R / PutNat_H / PutNatOther / _Put | NAT変換関連IP (m_sRouter.ip等) | ProcNAT (nat descriptor ...) | 各種NATポート・IP関連フラグ | NAT設定 |
| 764 | `# Other configuration` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 766 | `syslog info on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 767 | `syslog notice on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 768 | `syslog debug off` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 769 | `tftp host any` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 770 | `ipsec auto refresh on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 771 | `dhcp server rfc2131 compliant except remain-silent` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 772 | `dns server 211.129.14.134 211.129.12.43` | _Put (通常出力) または PutPPP/PutTunnel時のインライン判定出力 | m_sRouter.con[n].ip, m_sRouter.m_nAdslNo 等 | ProcDNS | m_sRouter.con[n].ip 等(指定された対象先) | DNS設定 |
| 773 | `dns server pp 5` | _Put (通常出力) または PutPPP/PutTunnel時のインライン判定出力 | m_sRouter.con[n].ip, m_sRouter.m_nAdslNo 等 | ProcDNS | m_sRouter.con[n].ip 等(指定された対象先) | DNS設定 |
| 774 | `dns private address spoof on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 775 | `schedule at 1 */* 6:00 * ntpdate 172.31.191.62` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 776 | `dns service recursive` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 777 | `no dhcp service` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 778 | `no dhcp scope 1` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 779 | `dhcp scope option 1 dns=172.16.5.1` | _Put (通常出力) | m_sRouter.ip_2_cloud など対象IP | ProcDHCP | m_szFirstAdd, m_szLastAdd, m_sRouter.m_fDHCP等 | DHCPスコープ |
| 780 | `pptp service on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 781 | `ip stealth pp 5` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 782 | `l2tp service on` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 783 | `schedule at 2 startup * lua /xai_check.lua` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 784 | `schedule at 3 */* 04:00:00 * terminate lua all` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 785 | `schedule at 4 */* 04:00:10 * lua /xai_check.lua` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 786 | `# LIFT_PHASE=1` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 787 | `description 1 "configversion=5.0"` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 788 | `save` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 789 | `description 1 "configversion=5.1"` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |
| 790 | `save` | _Put (その他) | なし (固定文字列が主) | 各要素のProc処理 または 読込スキップ | なし または 簡易フラグ | その他固定設定・コマンド |