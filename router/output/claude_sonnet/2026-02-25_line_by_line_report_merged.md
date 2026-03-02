# router01W.dat 行別書き込み解析レポート（パート1: 行1〜200）

## 凡例
- **書込関数**: Router1200.cpp内の関数名
- **SDkRouterメンバ**: 書き込みに使用されたメンバ変数（固定文字列の場合は「固定」）
- **根拠**: ソース上のフォーマット文字列

---

## ヘッダー・基本設定セクション（行1〜15）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 1 | `#	DK-ROUTER-CONFIGURATION Ver A1.000` | `PutConfig` | 固定文字列 | `_Put("#\tDK-ROUTER-CONFIGURATION Ver A1.000")` (Router1200.cpp:2578) | ReadTftpFile (対象外) | なし |
| 2 | `#	RTX830 Rev.15.02.31` | `PutConfig` | `m_sRouter.kind`, `m_sRouter.szRev` | `wsprintf(szBuf, "#\t%s Rev.%s", szRtType[m_sRouter.kind-1], m_sRouter.szRev)` (Router1200.cpp:2582) | AnalizeRouterType | m_sRouter.kind, m_sRouter.szRev |
| 3 | （空行） | `PutConfig` | − | セクション区切り空行 | − | − |
| 4 | `# System configuration` | `PutConfig` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:2585) | − | セクション区切り |
| 5 | （空行） | `PutConfig` | − | セクション区切り空行 | − | − |
| 6 | `clear configuration` | `PutConfig` | 固定文字列 | `_Put("clear configuration")` (Router1200.cpp:2588) | CDkRouter(1200)::Analize | なし (スキップ等) |
| 7 | `no schedule at 1` | `PutConfig` | 固定文字列 | `_Put(pCmd01, DK_Number(pCmd01))` pCmd01配列要素 (Router1200.cpp:2597) | CDkRouter(1200)::Analize | なし (スキップ等) |
| 8 | `no schedule at 2` | `PutConfig` | 固定文字列 | 同上 pCmd01配列要素 | CDkRouter(1200)::Analize | なし (スキップ等) |
| 9 | `no schedule at 3` | `PutConfig` | 固定文字列 | 同上 pCmd01配列要素 | CDkRouter(1200)::Analize | なし (スキップ等) |
| 10 | `no schedule at 4` | `PutConfig` | 固定文字列 | 同上 pCmd01配列要素 | CDkRouter(1200)::Analize | なし (スキップ等) |
| 11 | `login password ccsystem` | `PutConfig` | `m_strADMIN_PASS` | `str.Format("login password %s", m_strADMIN_PASS)` → `_Put(str)` (Router1200.cpp:2644) | PreReadFile / Analize | m_strADMIN_PASS 等 |
| 12 | `administrator password ccsystem` | `PutConfig` | `m_strADMIN_PASS` | `str.Format("administrator password %s", m_strADMIN_PASS)` → `_Put(str)` (Router1200.cpp:2648) | PreReadFile / Analize | m_strADMIN_PASS 等 |
| 13 | `snmp syslocation "Hall 23999185.5123"` | `PutConfig` | `m_cRtPassEx.m_nPH_Code` | `PutFmt("snmp syslocation \"Hall %d.%d\"", m_cRtPassEx.m_nPH_Code, nHint)` (Router1200.cpp:2684) | CDkRouter::Analize | m_cRtPassEx.m_nPH_Code |
| 14 | `security class 1 on on` | `PutConfig` | 固定文字列 | `_Put(pCmd02, DK_Number(pCmd02))` pCmd02配列要素 (Router1200.cpp:2693) | Analize / ProcOther | 固定 |
| 15 | `console info off` | `PutConfig` | 固定文字列 | 同上 pCmd02配列要素 | Analize / ProcOther | 固定 |

---

## IPルーティング設定セクション（行16〜44）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 16 | （空行） | `PutConfig` | − | セクション区切り空行 | − | − |
| 17 | `ip routing process normal` | `PutConfig` | 固定文字列 | pCmd02配列要素 (Router1200.cpp:2693) | ProcIP (ip routing ...) | 固定 |
| 18 | `ip route 10.142.65.0/24 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.142.65.0/24 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 19 | `ip route 10.142.165.0/24 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.142.165.0/24 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 20 | `ip route 10.145.0.0/16 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.145.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 21 | `ip route 10.141.0.0/16 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.141.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 22 | （空行） | `PutIpRoute` | − | セクション区切り空行 | − | − |
| 23 | `ip route 172.18.0.0/16 gateway 172.16.5.11` | `PutIpRoute` | 固定文字列（推論） | 固定ルート。pCmd配列の固定要素と推論。具体的なフォーマット行不明 | ProcIP (ip route ...) | m_sRouter.con[n].ip 等 |
| 24 | `ip route 61.114.182.53 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route %s gateway pp %d metric 2", pCmd[nBit], m_sRouter.m_nAdslNo)` (Router1200.cpp付近) | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 25 | `ip route 61.114.182.54 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 26 | `ip route 61.114.186.0/25 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 27 | `ip route 61.114.177.148 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 28 | `ip route 202.229.106.177 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 29 | `ip route 172.31.191.0/24 gateway tunnel 1` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | `wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no)` | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 30 | `ip route 61.114.186.111 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo)` | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 31 | `ip route 172.31.65.0/24 gateway tunnel 1` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 32 | `ip route 52.197.151.142 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 33 | `ip route 0.0.0.0/32 gateway tunnel 3` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 34 | `ip route 61.114.186.110 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 35 | `ip route 153.142.244.126 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 36 | `ip route 10.2.0.0/16 gateway tunnel 6` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 37 | `ip route 153.142.244.125 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 38 | `ip route 10.1.0.0/16 gateway tunnel 7` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 39 | `ip route default gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo)` | ProcIP (ip route ...) | m_sRouter.con[n].no 等 (PP向け) |
| 40 | `ip route 10.0.192.0/24 gateway pp anonymous metric 2` | `PutIpRoute` | `m_sRouter.ip[0]`, `m_sRouter.ip[1]`, `m_sRouter.ip[2]` | `wsprintf(szBuf, "ip route %d.%d.%d.%d gateway pp anonymous metric 2", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], nIp)` | ProcIP (ip route ...) | m_sRouter.ip など関連変数 |
| 41 | `ip route 10.0.208.0/24 gateway tunnel 5` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route 10.0.208.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo)` | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 42 | `ip route 10.0.224.0/24 gateway tunnel 5` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route 10.0.224.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo)` | ProcIP (ip route ...) | m_sRouter.sSIS05.no 等 (トンネル向け) |
| 43 | `ip icmp parameter-problem send off` | `PutConfig` | 固定文字列 | pCmd02配列要素 (Router1200.cpp:2693) | ProcIP (ip icmp ...) | 固定 |
| 44 | `ip icmp echo-reply send-only-linkup on` | `PutConfig` | 固定文字列 | 同上 pCmd02配列要素 | ProcIP (ip icmp ...) | 固定 |

---

## LAN/IP設定セクション（行45〜58）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 45 | （空行） | − | − | セクション区切り空行 | − | − |
| 46 | `# LAN/IP configuration` | `PutLanIp` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:2747) | − | セクション区切り |
| 47 | （空行） | − | − | セクション区切り空行 | − | − |
| 48 | `ip lan1 address 172.16.5.1/16` | `PutLanIp` | `m_sRouter.ip`, `m_sRouter.mask` | `wsprintf(szBuf, "ip lan1 address %s", GetIpB2C(m_sRouter.ip, m_sRouter.mask))` | ProcIP (ip lan1 address ...) | m_sRouter.ip, m_sRouter.mask |
| 49 | `rip use on` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (rip ...) | 固定 |
| 50 | `ip lan1 rip send on version 2` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |
| 51 | `ip lan1 rip receive on version 2` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |
| 52 | `ip lan1 proxyarp on` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |
| 53 | `ip lan1 secure filter name dk_lan1` | `PutFilter` | 固定文字列 | 固定コマンド（フィルターセット名） | ProcFilter | 固定 (フィルターセット名) |
| 54 | `ip lan1 intrusion detection in on reject=on` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |
| 55 | `ip lan2 rip send off` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |
| 56 | `ip lan2 rip receive off` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |
| 57 | `ip lan2 proxyarp off` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |
| 58 | `ip lan2 intrusion detection in on reject=on` | `PutLanIp` | 固定文字列 | 固定コマンド | ProcIP (ip lan ...) | 固定 |

---

## PP設定セクション（行59〜102）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 59 | （空行） | − | − | セクション区切り空行 | − | − |
| 60 | `# PP configuration` | `PutPp` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:2955) | − | セクション区切り |
| 61 | （空行） | − | − | セクション区切り空行 | − | − |
| 62 | `pp disable all` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (pp disable ...) | 固定 |
| 63 | （空行） | − | − | セクション区切り空行 | − | − |
| 64 | `### PP 5 ###` | `PutPp` | `m_sRouter.con[n].no` | `wsprintf(szBuf, "### PP %d ###", m_sRouter.con[n].no)` | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 65 | （空行） | − | − | セクション区切り空行 | − | − |
| 66 | `pp select 5` | `PutPp` | `m_sRouter.con[n].no` | `wsprintf(szBuf, "pp select %d", m_sRouter.con[n].no)` | ProcPP (pp select ...) | m_sRouter.con[n].no |
| 67 | `pp backup none` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 68 | `pppoe use lan2` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (pppoe ...) | 固定 |
| 69 | `pppoe auto connect on` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (pppoe ...) | 固定 |
| 70 | `pppoe auto disconnect on` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (pppoe ...) | 固定 |
| 71 | `pppoe disconnect time off` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (pppoe ...) | 固定 |
| 72 | `ip pp mtu 1454` | `PutPp` | `m_sRouter.nMTU` | `wsprintf(szBuf, "ip pp mtu %d", m_sRouter.nMTU)` | ProcPP / ProcMTU | m_sRouter.nMTU |
| 73 | `ip pp intrusion detection in on reject=on` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 74 | `ip pp nat descriptor 2000` | `PutPp` | 固定文字列 | 固定コマンド | ProcNAT (ip pp nat descriptor ...) | m_sRouter.m_nVWNo 等 (NATディスクリプタ番号) |
| 75 | `pp auth accept pap chap` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 76 | `pp always-on on` | `PutPp` | `m_sRouter.port[1].kaisen` | `wsprintf(szBuf, "pp always-on %s", (ADSL_KIND(m_sRouter.port[1].kaisen)==eKaisenFixADSL) ? "on" : "off")` | ProcPP | m_sRouter.port[1].kaisen |
| 77 | `ip pp secure filter name dk_pp_lan2` | `PutFilter` | 固定文字列 | 固定コマンド（フィルターセット名） | ProcFilter (ip pp secure filter ...) | 固定 (フィルターセット名) |
| 78 | `ip pp address 153.142.244.124/32` | `PutPp` | `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "nat descriptor address outer 2000 %s", GetIpB2C(m_sRouter.ncon[n].fixIp))` に対応 ※PPアドレス行は固定文字列または推論 | ProcPP (ip pp address ...) | m_sRouter.ncon[n].fixIp 等 |
| 79 | `pp auth myname w3102kwz@bizf.ocn.ne.jp sdjLSa76` | `PutPp` | `m_sRouter.szAdslID`, `m_sRouter.szAdslPass` | `wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass)` | ProcPP (pp auth myname ...) | m_sRouter.szVwID, m_sRouter.szAdslID 等 |
| 80 | `ppp lcp mru on 1454` | `PutPp` | `m_sRouter.nMTU` | `wsprintf(szBuf, "ppp lcp mru on %d", m_sRouter.nMTU)` ※推論 | ProcPP / ProcMTU | m_sRouter.nMTU |
| 81 | `ppp ccp type none` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (ppp ...) | 固定 |
| 82 | `ip pp rip send off` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 83 | `ip pp rip receive off` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 84 | `pp enable 5` | `PutPp` | `m_sRouter.con[n].no` | `wsprintf(szBuf, "pp enable %d", m_sRouter.con[n].no)` | ProcPP (pp enable ...) | m_sRouter.con[n].no |
| 85 | （空行） | − | − | セクション区切り空行 | − | − |
| 86 | `### PP anonymous ###` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 87 | （空行） | − | − | − | − | − |
| 88 | `no ip route 10.0.192.0/20` | `PutPp` | 固定文字列 | 固定コマンド | ProcIP (no ip route ...) | 固定 |
| 89 | `pp select anonymous` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (pp select ...) | 固定 (anonymous) |
| 90 | `no ip pp rip send` | `PutPp` | 固定文字列 | 固定コマンド | Analize / ProcXxx (no ...) | 固定 |
| 91 | `no ip pp rip receive` | `PutPp` | 固定文字列 | 固定コマンド | Analize / ProcXxx (no ...) | 固定 |
| 92 | `no ip pp nat descriptor` | `PutPp` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor...) | NAT変換関連IP等 |
| 93 | `no pp auth username ccsupport` | `PutPp` | 固定文字列 | 固定コマンド | Analize / ProcXxx (no ...) | 固定 |
| 94 | `pp bind tunnel16-tunnel20` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 95 | `pp auth request mschap-v2` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 96 | `pp auth username w8LeUYeGaZgstZXs 00393792` | `PutPp` | `m_sRouter.szVPN_User`, `m_sRouter.szVPN_Pass` | `PutFmt("pp auth username %s %s", m_sRouter.szVPN_User, m_sRouter.szVPN_Pass)` | ProcPP (pp auth username ...) | m_sRouter.szVPN_User, m_sRouter.szVPN_Pass |
| 97 | `ppp ipcp ipaddress on` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (ppp ...) | 固定 |
| 98 | `ppp ipcp msext on` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (ppp ...) | 固定 |
| 99 | `ip pp remote address pool 172.16.60.16-172.16.60.20` | `PutPp` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 100 | `ip pp mtu 1258` | `PutPp` | 固定文字列 | 匿名PP専用固定値（推論） | ProcPP / ProcMTU | m_sRouter.nMTU |
| 101 | `pp enable anonymous` | `PutPp` | 固定文字列 | 固定コマンド | ProcPP (pp enable ...) | m_sRouter.con[n].no |
| 102 | `tunnel disable all` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (固定) | 固定 |

---

## トンネル設定セクション - TUNNEL 1（行103〜123）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 103 | （空行） | − | − | − | − | − |
| 104 | `### TUNNEL 1 ###` | `PutTunnel` | `nSetuzokuNo`(=1) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 105 | （空行） | − | − | − | − | − |
| 106 | `tunnel select 1` | `PutTunnel` | `nSetuzokuNo`(=1) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 107 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 108 | `ipsec tunnel 101` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 109 | `ipsec sa policy 101 1 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 110 | `ipsec ike duration ipsec-sa 1 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 111 | `ipsec ike encryption 1 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 112 | `ipsec ike group 1 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 113 | `ipsec ike hash 1 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 114 | `ipsec ike keepalive use 1 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 115 | `ipsec ike local address 1 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 116 | `ipsec ike pfs 1 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 117 | `ipsec ike pre-shared-key 1 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 118 | `ipsec ike remote address 1 61.114.186.111` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 119 | `tunnel backup tunnel 4 switch-interface=on` | `PutTunnel` | `m_sRouter.m_nDeltaBkNo` | `wsprintf(szBuf, "tunnel backup tunnel %d switch-interface=on", m_sRouter.m_nDeltaBkNo)` | ProcTunnel (tunnel backup ...) | m_sRouter.m_nDeltaBkNo / 固定 |
| 120 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 121 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 122 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcNAT (ip tunnel nat descriptor ...) | m_sRouter.con[n].no等 (NATディスクリプタ番号) |
| 123 | `tunnel enable 1` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## TUNNEL 3（行124〜146）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 124 | （空行） | − | − | − | − | − |
| 125 | `### TUNNEL 3 ###` | `PutTunnel` | `nSetuzokuNo`(=3) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 126 | （空行） | − | − | − | − | − |
| 127 | `tunnel select 3` | `PutTunnel` | `nSetuzokuNo`(=3) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 128 | `ipsec tunnel 103` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 129 | `ipsec ike duration ipsec-sa 3 10800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 130 | `ipsec ike duration isakmp-sa 3 3600` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 131 | `ipsec ike encryption 3 aes-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 132 | `ipsec ike group 3 modp768 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 133 | `ipsec ike hash 3 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 134 | `ipsec ike keepalive use 3 off` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].type` | `wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, (m_sRouter.ncon[n].type==eDKVPN...) ? "off" : "on")` | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 135 | `ipsec ike local address 3 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 136 | `ipsec ike local id 3 10.6.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.uip` | `wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask)` | ProcIPSEC (ipsec ike id ...) | m_sRouter.uip / m_sRouter.con[n].ip 等 |
| 137 | `ipsec ike pfs 3 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 138 | `ipsec ike pre-shared-key 3 text wh1+eb0@rd` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo)` 固定パスフレーズ | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 139 | `ipsec ike remote address 3 52.197.151.142` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 140 | `ipsec ike remote id 3 0.0.0.0/32` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask))` | ProcIPSEC (ipsec ike id ...) | m_sRouter.uip / m_sRouter.con[n].ip 等 |
| 141 | `ipsec sa policy 103 3 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 142 | `tunnel backup none` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (tunnel backup ...) | m_sRouter.m_nDeltaBkNo / 固定 |
| 143 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 144 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 145 | `ip tunnel nat descriptor 1` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcNAT (ip tunnel nat descriptor ...) | m_sRouter.con[n].no等 (NATディスクリプタ番号) |
| 146 | `tunnel enable 3` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## TUNNEL 6（行147〜169）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 147 | （空行） | − | − | − | − | − |
| 148 | `### TUNNEL 6 ###` | `PutTunnel` | `nSetuzokuNo`(=6) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 149 | （空行） | − | − | − | − | − |
| 150 | `tunnel select 6` | `PutTunnel` | `nSetuzokuNo`(=6) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 151 | `ipsec tunnel 106` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 152 | `ipsec ike duration ipsec-sa 6 10800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 153 | `ipsec ike duration isakmp-sa 6 3600` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 154 | `ipsec ike encryption 6 aes-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 155 | `ipsec ike group 6 modp768 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 156 | `ipsec ike hash 6 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 157 | `ipsec ike keepalive use 6 off` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].type` | `wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, ...)` 値がoffのためtypeはeDKVPN等 | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 158 | `ipsec ike local address 6 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 159 | `ipsec ike local id 6 10.6.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.uip` | `wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask)` | ProcIPSEC (ipsec ike id ...) | m_sRouter.uip / m_sRouter.con[n].ip 等 |
| 160 | `ipsec ike pfs 6 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 161 | `ipsec ike pre-shared-key 6 text wh1+eb0@rd` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo)` 固定パスフレーズ | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 162 | `ipsec ike remote address 6 153.142.244.126` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 163 | `ipsec ike remote id 6 10.2.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask))` | ProcIPSEC (ipsec ike id ...) | m_sRouter.uip / m_sRouter.con[n].ip 等 |
| 164 | `ipsec sa policy 106 6 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 165 | `tunnel backup none` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (tunnel backup ...) | m_sRouter.m_nDeltaBkNo / 固定 |
| 166 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 167 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 168 | `ip tunnel nat descriptor 1` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcNAT (ip tunnel nat descriptor ...) | m_sRouter.con[n].no等 (NATディスクリプタ番号) |
| 169 | `tunnel enable 6` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## TUNNEL 7（行170〜192）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 170 | （空行） | − | − | − | − | − |
| 171 | `### TUNNEL 7 ###` | `PutTunnel` | `nSetuzokuNo`(=7) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 172 | （空行） | − | − | − | − | − |
| 173 | `tunnel select 7` | `PutTunnel` | `nSetuzokuNo`(=7) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 174 | `ipsec tunnel 107` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 175 | `ipsec ike duration ipsec-sa 7 10800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 176 | `ipsec ike duration isakmp-sa 7 3600` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 177 | `ipsec ike encryption 7 aes-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 178 | `ipsec ike group 7 modp768 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 179 | `ipsec ike hash 7 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 180 | `ipsec ike keepalive use 7 off` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].type` | `wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, ...)` 値offのためtypeはeDKVPN等 | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 181 | `ipsec ike local address 7 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 182 | `ipsec ike local id 7 10.6.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.uip` | `wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask)` | ProcIPSEC (ipsec ike id ...) | m_sRouter.uip / m_sRouter.con[n].ip 等 |
| 183 | `ipsec ike pfs 7 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 184 | `ipsec ike pre-shared-key 7 text wh1+eb0@rd` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo)` 固定パスフレーズ | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 185 | `ipsec ike remote address 7 153.142.244.125` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 186 | `ipsec ike remote id 7 10.1.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask))` | ProcIPSEC (ipsec ike id ...) | m_sRouter.uip / m_sRouter.con[n].ip 等 |
| 187 | `ipsec sa policy 107 7 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 188 | `tunnel backup none` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (tunnel backup ...) | m_sRouter.m_nDeltaBkNo / 固定 |
| 189 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 190 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 191 | `ip tunnel nat descriptor 1` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcNAT (ip tunnel nat descriptor ...) | m_sRouter.con[n].no等 (NATディスクリプタ番号) |
| 192 | `tunnel enable 7` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## TUNNEL 5（行193〜212） ※メンテナンス用VPN

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 193 | （空行） | − | − | − | − | − |
| 194 | `### TUNNEL 5 ###` | `PutTunnel` | `m_sRouter.m_nAdslNo`(=5) | `wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nAdslNo)` | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 195 | （空行） | − | − | − | − | − |
| 196 | `tunnel select 5` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nAdslNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 197 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 198 | `ipsec tunnel 105` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nAdslNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 199 | `ipsec sa policy 105 5 esp aes-cbc sha-hmac` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", ...+100, ...)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 200 | `ipsec ike duration ipsec-sa 5 28800` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nAdslNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |

---

## TUNNEL 5 続き（行201〜212） ※メンテナンス用VPN

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 201 | `ipsec ike encryption 5 aes-cbc` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", m_sRouter.m_nAdslNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 202 | `ipsec ike group 5 modp1024` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", m_sRouter.m_nAdslNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 203 | `ipsec ike hash 5 sha` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", m_sRouter.m_nAdslNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 204 | `ipsec ike local address 5 172.16.5.1` | `PutTunnel` | `m_sRouter.m_nAdslNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", m_sRouter.m_nAdslNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 205 | `ipsec ike pfs 5 on` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", m_sRouter.m_nAdslNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 206 | `ipsec ike pre-shared-key 5 text dkccsupport` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text *", m_sRouter.m_nAdslNo)` ※実出力値は"dkccsupport"（固定パスフレーズ） | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 207 | `ipsec ike remote address 5 any` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike remote address %d any", m_sRouter.m_nAdslNo)` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 208 | `ipsec ike remote name 5 ccsupport` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike remote name %d *", m_sRouter.m_nAdslNo)` ※実出力値は"ccsupport" | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 209 | `ipsec ike keepalive use 5 on heartbeat 10 2` | `PutTunnel` | `m_sRouter.m_nAdslNo` or `m_sRouter.m_nCarMentNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 10 2", ...)` | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 210 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 211 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 212 | `tunnel enable 5` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nAdslNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## TUNNEL 4（行213〜232） ※バックアップトンネル

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 213 | （空行） | − | − | − | − | − |
| 214 | `### TUNNEL 4 ###` | `PutTunnel` | `nSetuzokuNo`(=4) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 215 | （空行） | − | − | − | − | − |
| 216 | `tunnel select 4` | `PutTunnel` | `nSetuzokuNo`(=4) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 217 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 218 | `ipsec tunnel 104` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 219 | `ipsec sa policy 104 4 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 220 | `ipsec ike duration ipsec-sa 4 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 221 | `ipsec ike encryption 4 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 222 | `ipsec ike group 4 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 223 | `ipsec ike hash 4 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 224 | `ipsec ike keepalive use 4 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 225 | `ipsec ike local address 4 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 226 | `ipsec ike pfs 4 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 227 | `ipsec ike pre-shared-key 4 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 228 | `ipsec ike remote address 4 61.114.186.110` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 229 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 230 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 231 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcNAT (ip tunnel nat descriptor ...) | m_sRouter.con[n].no等 (NATディスクリプタ番号) |
| 232 | `tunnel enable 4` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## TUNNEL 16〜20（行233〜302） ※L2TP/IPsec VPN（5本同パターン）

> **注**: TUNNEL 16〜20 は同一パターンの繰り返し。番号のみ異なる（nSetuzokuNo）。  
> 使用メンバ変数は `nSetuzokuNo`（ループ変数）、`m_sRouter.szVPN_Key`（PSK値）。

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 233 | `tunnel select 16` | `PutTunnel` | `nSetuzokuNo`(=16) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 234 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 235 | `ipsec tunnel 116` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 236 | `ipsec sa policy 116 16 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 237 | `ipsec ike keepalive use 16 off` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d off", nSetuzokuNo)` | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 238 | `ipsec ike nat-traversal 16 on` | `PutTunnel` | `nSetuzokuNo` | 固定文字列にnSetuzokuNo埋め込み | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 239 | `ipsec ike pre-shared-key 16 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | `PutFmt("ipsec ike pre-shared-key %d text %s", nTunnelNo, m_sRouter.szVPN_Key)` | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 240 | `ipsec ike remote address 16 any` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike remote address %d any", nSetuzokuNo)` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 241 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 242 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 243 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 244 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 245 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 246 | `tunnel enable 16` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |
| 247 | `tunnel select 17` | `PutTunnel` | `nSetuzokuNo`(=17) | 同上パターン | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 248 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 249 | `ipsec tunnel 117` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 250 | `ipsec sa policy 117 17 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 251 | `ipsec ike keepalive use 17 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 252 | `ipsec ike nat-traversal 17 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 253 | `ipsec ike pre-shared-key 17 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 254 | `ipsec ike remote address 17 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 255 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 256 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 257 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 258 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 259 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 260 | `tunnel enable 17` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |
| 261 | `tunnel select 18` | `PutTunnel` | `nSetuzokuNo`(=18) | 同上パターン | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 262 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 263 | `ipsec tunnel 118` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 264 | `ipsec sa policy 118 18 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 265 | `ipsec ike keepalive use 18 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 266 | `ipsec ike nat-traversal 18 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 267 | `ipsec ike pre-shared-key 18 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 268 | `ipsec ike remote address 18 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 269 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 270 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 271 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 272 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 273 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 274 | `tunnel enable 18` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |
| 275 | `tunnel select 19` | `PutTunnel` | `nSetuzokuNo`(=19) | 同上パターン | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 276 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 277 | `ipsec tunnel 119` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 278 | `ipsec sa policy 119 19 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 279 | `ipsec ike keepalive use 19 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 280 | `ipsec ike nat-traversal 19 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 281 | `ipsec ike pre-shared-key 19 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 282 | `ipsec ike remote address 19 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 283 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 284 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 285 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 286 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 287 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 288 | `tunnel enable 19` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |
| 289 | `tunnel select 20` | `PutTunnel` | `nSetuzokuNo`(=20) | 同上パターン | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 290 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 291 | `ipsec tunnel 120` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 292 | `ipsec sa policy 120 20 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 293 | `ipsec ike keepalive use 20 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 294 | `ipsec ike nat-traversal 20 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 295 | `ipsec ike pre-shared-key 20 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 296 | `ipsec ike remote address 20 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 297 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 298 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 299 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (l2tp ...) | 固定 |
| 300 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 301 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel | m_sRouter.con[n].type 等 |
| 302 | `tunnel enable 20` | `PutTunnel` | `nSetuzokuNo` | 同上パターン | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## IPSec transport設定（行303〜310）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 303 | （空行） | − | − | − | − | − |
| 304 | `### IPSEC ###` | `PutTunnel` | 固定文字列 | `_PutTit(tit)` セクションタイトル | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 305 | （空行） | − | − | − | − | − |
| 306 | `ipsec transport 16 116 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=16) | 固定文字列にnSetuzokuNo埋め込み（pCmd03または固定配列） | ProcIPSEC (ipsec transport ...) | nSetuzokuNo |
| 307 | `ipsec transport 17 117 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=17) | 同上パターン | ProcIPSEC (ipsec transport ...) | nSetuzokuNo |
| 308 | `ipsec transport 18 118 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=18) | 同上パターン | ProcIPSEC (ipsec transport ...) | nSetuzokuNo |
| 309 | `ipsec transport 19 119 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=19) | 同上パターン | ProcIPSEC (ipsec transport ...) | nSetuzokuNo |
| 310 | `ipsec transport 20 120 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=20) | 同上パターン | ProcIPSEC (ipsec transport ...) | nSetuzokuNo |

---

## IPフィルター設定ヘッダー（行311〜315）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 311 | （空行） | − | − | − | − | − |
| 312 | `# IP filter configuration` | `PutFilter` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:3338) | − | セクション区切り |
| 313 | （空行） | − | − | − | − | − |
| 314 | `ip filter source-route on` | `PutFilter` | 固定文字列 | pCmd02配列要素 (Router1200.cpp:3365) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 315 | `ip filter directed-broadcast on` | `PutFilter` | 固定文字列 | 同上 pCmd02配列要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## TUNNEL 2（行316〜333） ※本線VPNトンネル

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 316 | `tunnel select 2` | `PutTunnel` | `nSetuzokuNo`(=2) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 317 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 318 | `ipsec tunnel 102` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 319 | `ipsec sa policy 102 2 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 320 | `ipsec ike duration ipsec-sa 2 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 321 | `ipsec ike encryption 2 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 322 | `ipsec ike group 2 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 323 | `ipsec ike hash 2 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 324 | `ipsec ike keepalive use 2 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 325 | `ipsec ike local address 2 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 326 | `ipsec ike pfs 2 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 327 | `ipsec ike pre-shared-key 2 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 328 | `ipsec ike remote address 2 192.0.2.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 329 | `tunnel backup tunnel 8 switch-interface=on` | `PutTunnel` | `m_sRouter.m_nDeltaBkNo`(=8) | `wsprintf(szBuf, "tunnel backup tunnel %d switch-interface=on", m_sRouter.m_nDeltaBkNo)` | ProcTunnel (tunnel backup ...) | m_sRouter.m_nDeltaBkNo / 固定 |
| 330 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcNAT (ip tunnel nat descriptor ...) | m_sRouter.con[n].no等 (NATディスクリプタ番号) |
| 331 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 332 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 333 | `tunnel enable 2` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## TUNNEL 8（行334〜350） ※バックアップ本線VPN

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 334 | `tunnel select 8` | `PutTunnel` | `nSetuzokuNo`(=8) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` | ProcTunnel (tunnel select ...) | m_nCurrentTunnel など |
| 335 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 336 | `ipsec tunnel 108` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` | ProcIPSEC (ipsec tunnel ...) | m_sRouter.con[n].type 等 紐付け |
| 337 | `ipsec sa policy 108 8 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` | ProcIPSEC (ipsec sa ...) | m_sRouter.con[n].type 等 |
| 338 | `ipsec ike duration ipsec-sa 8 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 339 | `ipsec ike encryption 8 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 340 | `ipsec ike group 8 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 341 | `ipsec ike hash 8 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 342 | `ipsec ike keepalive use 8 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` | ProcIPSEC (ipsec ike keepalive ...) | m_sRouter.con[n].type / nSetuzokuNo |
| 343 | `ipsec ike local address 8 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` | ProcIPSEC (ipsec ike local address ...) | m_sRouter.ip (ローカルIP格納先) |
| 344 | `ipsec ike pfs 8 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` | ProcIPSEC (ipsec ike ...) | m_sRouter.con[n].type 等 接続情報 |
| 345 | `ipsec ike pre-shared-key 8 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` | ProcIPSEC (ipsec ike pre-shared-key ...) | m_sRouter.szVPN_Key / m_sRouter.ncon[i].fixIp 等 |
| 346 | `ipsec ike remote address 8 192.0.2.254` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcIPSEC (ipsec ike remote address ...) | m_sRouter.ncon[n].fixIp 等 |
| 347 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcNAT (ip tunnel nat descriptor ...) | m_sRouter.con[n].no等 (NATディスクリプタ番号) |
| 348 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 349 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド | ProcTunnel (ip tunnel rip ...) | 固定 |
| 350 | `tunnel enable 8` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` | ProcTunnel (tunnel enable ...) | m_sRouter.con[n].no / nSetuzokuNo |

---

## IPフィルター個別エントリ（行351〜400）

> **注**: `ip filter` 系エントリの多くは `PutFilter` 関数内でフォーマット文字列を生成。  
> IPアドレス部に `m_sRouter.ip`（=172.16.5.1）や `m_sRouter.mask`（=172.16.0.0/16相当）が使われる場合と、 pCmd配列の固定文字列の場合がある。

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 351 | `ip filter 2602 pass 192.0.2.1 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | `wsprintf(szPut, "ip filter 2602 pass %s %s udp * 500", ...)` ncon[n].fixIp + m_sRouter.ip | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 352 | `ip filter 2608 pass 192.0.2.254 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | 同上（tunnel 8用） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 353 | `ip filter 2652 pass 172.16.5.1 192.0.2.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | 同上（逆方向） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 354 | `ip filter 2658 pass 172.16.5.1 192.0.2.254 udp * 500` | `PutFilter` | `m_sRouter.ip` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 355 | `ip filter 2702 pass 192.0.2.1 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上（ESP） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 356 | `ip filter 2708 pass 192.0.2.254 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 357 | `ip filter 2752 pass 172.16.5.1 192.0.2.1 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上（逆方向） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 358 | `ip filter 2758 pass 172.16.5.1 192.0.2.254 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 359 | （空行） | − | − | − | − | − |
| 360 | `ip filter 1 reject * * udp,tcp netbios_ns-netbios_ssn *` | `PutFilter` | 固定文字列 | pCmd01配列要素 (Router1200.cpp:3365) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 361 | `ip filter 2 reject * * udp,tcp * netbios_ns-netbios_ssn` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 362 | `ip filter 3 reject * * udp,tcp 135 *` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 363 | `ip filter 4 reject * * udp,tcp * 135` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 364 | `ip filter 5 reject * * udp,tcp 445 *` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 365 | `ip filter 6 reject * * udp,tcp * 445` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 366 | `ip filter 9 reject * * * * *` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 367 | `ip filter 10 reject 172.16.0.0/16 * * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 10 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 368 | `ip filter 15 reject * 172.16.0.0/16 * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 15 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 369 | `ip filter 20 pass 211.129.14.134,211.129.12.43 172.16.0.0/16 udp domain *` | `PutFilter` | `m_sRouter.mask` | pCmd02配列: 固定IP + `GetIpB2C(bMaskedIP, m_sRouter.mask)` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 370 | `ip filter 21 pass 211.129.14.134,211.129.12.43 172.16.5.0/24 udp domain *` | `PutFilter` | `m_sRouter.ip` | 固定IPと m_sRouter.ip から生成 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 371 | `ip filter 22 pass 211.129.14.134,211.129.12.43 172.16.15.0/24 udp domain *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 372 | `ip filter 23 pass 211.129.14.134,211.129.12.43 172.16.60.0/24 udp domain *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 373 | `ip filter 25 pass 172.16.0.0/16 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 374 | `ip filter 26 pass 172.16.5.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip 派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 375 | `ip filter 27 pass 172.16.15.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 376 | `ip filter 28 pass 172.16.60.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 377 | `ip filter 30 pass 61.114.182.53 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 378 | `ip filter 31 pass 61.114.182.53 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | 固定IP + m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 379 | `ip filter 32 pass 61.114.182.53 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 380 | `ip filter 33 pass 61.114.182.53 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 381 | `ip filter 35 pass 172.16.0.0/16 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 382 | `ip filter 36 pass 172.16.5.0/24 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 383 | `ip filter 37 pass 172.16.15.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 384 | `ip filter 38 pass 172.16.60.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 385 | `ip filter 40 pass 61.114.182.54 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 386 | `ip filter 41 pass 61.114.182.54 172.16.5.0/24 tcp www *` | `PutFilter` | `m_sRouter.ip` | 同上 m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 387 | `ip filter 42 pass 61.114.182.54 172.16.15.0/24 tcp www *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 388 | `ip filter 43 pass 61.114.182.54 172.16.60.0/24 tcp www *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 389 | `ip filter 45 pass 172.16.0.0/16 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 390 | `ip filter 46 pass 172.16.5.0/24 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 391 | `ip filter 47 pass 172.16.15.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 392 | `ip filter 48 pass 172.16.60.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 393 | `ip filter 50 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 394 | `ip filter 51 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | 同上 m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 395 | `ip filter 52 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 396 | `ip filter 53 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 397 | `ip filter 54 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 398 | `ip filter 55 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 399 | `ip filter 56 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 400 | `ip filter 57 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## IPフィルター 個別エントリ続き（行401〜530）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 401 | `ip filter 58 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 402 | `ip filter 59 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 403 | `ip filter 60 pass 61.114.177.148 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 404 | `ip filter 61 pass 61.114.177.148 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 405 | `ip filter 62 pass 61.114.177.148 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 406 | `ip filter 63 pass 61.114.177.148 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 407 | `ip filter 65 pass 172.16.0.0/16 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 408 | `ip filter 66 pass 172.16.5.0/24 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 409 | `ip filter 67 pass 172.16.15.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 410 | `ip filter 68 pass 172.16.60.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 411 | `ip filter 70 pass 202.229.106.177 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 412 | `ip filter 71 pass 202.229.106.177 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 413 | `ip filter 72 pass 202.229.106.177 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 414 | `ip filter 73 pass 202.229.106.177 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 415 | `ip filter 74 pass 202.229.106.177 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 416 | `ip filter 75 pass 172.16.0.0/16 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 417 | `ip filter 76 pass 172.16.5.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 418 | `ip filter 77 pass 172.16.15.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 419 | `ip filter 78 pass 172.16.60.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 420 | `ip filter 79 pass 172.16.80.0/22 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 421 | `ip filter 80 pass 202.226.76.11 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 80 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3370) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 422 | `ip filter 81 pass 202.226.76.11 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 81 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3372) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 423 | `ip filter 82 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 424 | `ip filter 83 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | `PutFilter` | 固定文字列 | `_Put("ip filter 83 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *")` (Router1200.cpp:3477) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 425 | `ip filter 84 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | `PutFilter` | 固定文字列 | `_Put("ip filter 84 pass * 172.16.121.90-...")` (Router1200.cpp:3480) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 426 | `ip filter 85 pass 172.16.0.0/16 202.226.76.11 tcp * https` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 85 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3484) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 427 | `ip filter 86 pass 172.16.0.0/16 202.226.76.11 tcp * www` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 86 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3486) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 428 | `ip filter 87 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 (Router1200.cpp:3488) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 429 | `ip filter 88 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | `PutFilter` | 固定文字列 | `_Put("ip filter 88 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https")` (Router1200.cpp:3490) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 430 | `ip filter 89 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | `PutFilter` | 固定文字列 | `_Put("ip filter 89 pass ...")` (Router1200.cpp:3493) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 431 | `ip filter 92 pass * 172.16.5.0/24 tcp https,5222,5223 *` | `PutFilter` | `m_sRouter.ip` | `wsprintf(szPut, "ip filter 92 pass * %s tcp https,5222,5223 *",...)` m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 432 | `ip filter 93 pass * 172.16.5.0/24 udp domain,ntp *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 433 | `ip filter 94 pass * 172.16.5.0/24 icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 434 | `ip filter 97 pass 172.16.5.0/24 * tcp * https,5222,5223` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 435 | `ip filter 98 pass 172.16.5.0/24 * udp * domain,ntp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 436 | `ip filter 99 pass 172.16.5.0/24 * icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 437 | `ip filter 100 pass-log * * * * *` | `PutFilter` | 固定文字列 | pCmd01配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 438 | `ip filter 101 reject * * established * netbios_ns-netbios_ssn` | `PutFilter` | 固定文字列 | pCmd01配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 439 | `ip filter 102 reject * * established * 135` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 440 | `ip filter 103 reject * * established * 445` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 441 | `ip filter 104 reject * * established * https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 442 | `ip filter 105 reject * * established * www` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 443 | `ip filter 106 reject * * established * pop3` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 444 | `ip filter 107 reject * * established * smtp` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 445 | `ip filter 1400 pass 172.16.0.0/16 * * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 1400 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 446 | `ip filter 1401 pass 172.21.0.0/16 * * * *` | `PutFilter` | 固定文字列 | `_Put("ip filter 1401 pass 172.21.0.0/16 * * * *")` (Router1200.cpp:3548) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 447 | `ip filter 1405 pass * 172.16.0.0/16 * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 1405 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 448 | `ip filter 1406 pass * 172.21.0.0/16 * * *` | `PutFilter` | 固定文字列 | `_Put("ip filter 1406 pass * 172.21.0.0/16 * * *")` (Router1200.cpp:3558) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 449 | `ip filter 1410 pass * * udp dhcpc dhcps` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 (Router1200.cpp:3553) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 450 | `ip filter 1430 pass 10.0.0.0/8 10.0.208.0/20 * * *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 451 | `ip filter 1431 pass 10.0.208.0/20 10.0.0.0/8 * * *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 452 | `ip filter 1432 pass 10.0.0.0/8 172.30.1.110 tcp * 21` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 453 | `ip filter 1433 pass 172.30.1.110 10.0.0.0/8 tcp 21 *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 454 | `ip filter 1434 pass 10.0.0.0/8 172.30.1.110 tcp * ftpdata` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 455 | `ip filter 1435 pass 172.30.1.110 10.0.0.0/8 tcp ftpdata *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 456 | `ip filter 1436 pass 10.0.0.0/8 172.31.191.0/24 * * *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 457 | `ip filter 1437 pass 172.31.191.0/24 10.0.0.0/8 * * *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 458 | `ip filter 1438 pass 10.0.0.0/8 10.0.0.0/8 * * *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 459 | `ip filter 1440 pass 172.18.0.0/24 10.0.192.0/18 * * *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 460 | `ip filter 1441 pass 10.0.192.0/18 172.18.0.0/24 * * *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 461 | `ip filter 2010 reject 172.16.0.0/16 * * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 462 | `ip filter 2015 reject * 172.16.0.0/16 * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 463 | `ip filter 2025 pass 172.16.0.0/16 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 464 | `ip filter 2026 pass * 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 465 | `ip filter 2027 pass 172.16.15.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 466 | `ip filter 2028 pass 172.16.60.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 467 | `ip filter 2030 pass 61.114.182.53 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 468 | `ip filter 2031 pass 61.114.182.53 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 469 | `ip filter 2032 pass 61.114.182.53 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 470 | `ip filter 2033 pass 61.114.182.53 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 471 | `ip filter 2035 pass 172.16.0.0/16 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 472 | `ip filter 2036 pass 172.16.5.0/24 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 473 | `ip filter 2037 pass 172.16.15.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 474 | `ip filter 2038 pass 172.16.60.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 475 | `ip filter 2040 pass 61.114.182.54 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 476 | `ip filter 2041 pass 61.114.182.54 172.16.5.0/24 tcp www *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 477 | `ip filter 2042 pass 61.114.182.54 172.16.15.0/24 tcp www *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 478 | `ip filter 2043 pass 61.114.182.54 172.16.60.0/24 tcp www *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 479 | `ip filter 2045 pass 172.16.0.0/16 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 480 | `ip filter 2046 pass 172.16.5.0/24 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 481 | `ip filter 2047 pass 172.16.15.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 482 | `ip filter 2048 pass 172.16.60.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 483 | `ip filter 2050 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 484 | `ip filter 2051 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 485 | `ip filter 2052 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 486 | `ip filter 2053 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 487 | `ip filter 2054 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 488 | `ip filter 2055 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 489 | `ip filter 2056 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 490 | `ip filter 2057 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 491 | `ip filter 2058 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 492 | `ip filter 2059 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 493 | `ip filter 2060 pass 61.114.177.148 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 494 | `ip filter 2061 pass 61.114.177.148 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 495 | `ip filter 2062 pass 61.114.177.148 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 496 | `ip filter 2063 pass 61.114.177.148 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 497 | `ip filter 2065 pass 172.16.0.0/16 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 498 | `ip filter 2066 pass 172.16.5.0/24 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 499 | `ip filter 2067 pass 172.16.15.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 500 | `ip filter 2068 pass 172.16.60.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 501 | `ip filter 2070 pass 202.229.106.177 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 502 | `ip filter 2071 pass 202.229.106.177 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 503 | `ip filter 2072 pass 202.229.106.177 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 504 | `ip filter 2073 pass 202.229.106.177 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 505 | `ip filter 2074 pass 202.229.106.177 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 506 | `ip filter 2075 pass 172.16.0.0/16 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 507 | `ip filter 2076 pass 172.16.5.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 508 | `ip filter 2077 pass 172.16.15.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 509 | `ip filter 2078 pass 172.16.60.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 510 | `ip filter 2079 pass 172.16.80.0/22 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 511 | `ip filter 2080 pass 202.226.76.11 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 512 | `ip filter 2081 pass 202.226.76.11 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 513 | `ip filter 2082 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 514 | `ip filter 2083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 515 | `ip filter 2084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | `PutFilter` | 固定文字列 | 固定文字列 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 516 | `ip filter 2085 pass 172.16.0.0/16 202.226.76.11 tcp * https` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 517 | `ip filter 2086 pass 172.16.0.0/16 202.226.76.11 tcp * www` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 518 | `ip filter 2087 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 519 | `ip filter 2088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 520 | `ip filter 2089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 521 | `ip filter 2092 pass * 172.16.5.0/24 tcp https,5222,5223 *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3499) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 522 | `ip filter 2093 pass * 172.16.5.0/24 udp domain,ntp *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3501) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 523 | `ip filter 2094 pass * 172.16.5.0/24 icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3506) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 524 | `ip filter 2097 pass 172.16.5.0/24 * tcp * https,5222,5223` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3508) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 525 | `ip filter 2098 pass 172.16.5.0/24 * udp * domain,ntp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 526 | `ip filter 2099 pass 172.16.5.0/24 * icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 527 | `ip filter 2200 pass * 172.16.5.1 tcp * 1723` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2200 pass * %s tcp * 1723", GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 528 | `ip filter 2250 pass 172.16.5.1 * tcp 1723 *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2250 pass %s * tcp 1723 *", GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 529 | `ip filter 2300 pass * 172.16.5.1 gre * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2300 pass * %s gre * *", GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 530 | `ip filter 2350 pass 172.16.5.1 * gre * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2350 pass %s * gre * *", GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## IPフィルター VPN/IPsec用（行531〜554）

> **注**: 行531,537,543,549 は非常に長いIPアドレスリスト（XAI/DKVPNサーバー群）を含む。  
> これらは `pCmd02_1`/`pCmd02_2` 配列の固定文字列 (Router1200.cpp:3519,3532) に `m_sRouter.ip` を合成している。

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 531 | `ip filter 2600 pass [長IPリスト] 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | `_Put(pCmd02_1, ...)` pCmd02_1配列要素 + m_sRouter.ip (Router1200.cpp:3519) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 532 | `ip filter 2601 pass 61.114.186.111 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szPut, "ip filter 2601 pass %s %s udp * 500", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 533 | `ip filter 2603 pass 52.197.151.142 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 3用） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 534 | `ip filter 2604 pass 61.114.186.110 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 4用） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 535 | `ip filter 2606 pass 153.142.244.126 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 6用） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 536 | `ip filter 2607 pass 153.142.244.125 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 7用） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 537 | `ip filter 2650 pass 172.16.5.1 [長IPリスト] udp * 500` | `PutFilter` | `m_sRouter.ip` | `_Put(pCmd02_2, ...)` pCmd02_2配列要素 + m_sRouter.ip (Router1200.cpp:3532) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 538 | `ip filter 2651 pass 172.16.5.1 61.114.186.111 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 逆方向（PuFilterでm_sRouter.ip + ncon[n].fixIp） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 539 | `ip filter 2653 pass 172.16.5.1 52.197.151.142 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 540 | `ip filter 2654 pass 172.16.5.1 61.114.186.110 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 541 | `ip filter 2656 pass 172.16.5.1 153.142.244.126 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 542 | `ip filter 2657 pass 172.16.5.1 153.142.244.125 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 543 | `ip filter 2700 pass [長IPリスト] 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd02_1配列 + m_sRouter.ip (ESP版) (Router1200.cpp:3519) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 544 | `ip filter 2701 pass 61.114.186.111 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同様パターン（ESP) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 545 | `ip filter 2703 pass 52.197.151.142 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 546 | `ip filter 2704 pass 61.114.186.110 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 547 | `ip filter 2706 pass 153.142.244.126 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 548 | `ip filter 2707 pass 153.142.244.125 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 549 | `ip filter 2750 pass 172.16.5.1 [長IPリスト] esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd02_2配列 + m_sRouter.ip (ESP逆方向) (Router1200.cpp:3532) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 550 | `ip filter 2751 pass 172.16.5.1 61.114.186.111 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上（逆方向ESP） | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 551 | `ip filter 2753 pass 172.16.5.1 52.197.151.142 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 552 | `ip filter 2754 pass 172.16.5.1 61.114.186.110 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 553 | `ip filter 2756 pass 172.16.5.1 153.142.244.126 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 554 | `ip filter 2757 pass 172.16.5.1 153.142.244.125 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## IPフィルター ICMP/XAI/LAN系（行555〜600）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 555 | `ip filter 2900 pass * 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2900 pass * %s icmp * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3543) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 556 | `ip filter 2901 pass 61.114.186.111 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 557 | `ip filter 2903 pass 52.197.151.142 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 558 | `ip filter 2904 pass 61.114.186.110 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 559 | `ip filter 2906 pass 153.142.244.126 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 560 | `ip filter 2907 pass 153.142.244.125 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 561 | `ip filter 2950 pass 172.16.0.0/16 * icmp * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2950 pass %s * icmp * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 562 | `ip filter 2951 pass 172.16.0.0/16 61.114.186.111 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 563 | `ip filter 2953 pass 172.16.0.0/16 52.197.151.142 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 564 | `ip filter 2954 pass 172.16.0.0/16 61.114.186.110 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 565 | `ip filter 2956 pass 172.16.0.0/16 153.142.244.126 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 566 | `ip filter 2957 pass 172.16.0.0/16 153.142.244.125 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 567 | `ip filter 3080 pass 202.226.76.11 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 568 | `ip filter 3081 pass 202.226.76.11 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 569 | `ip filter 3082 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3577) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 570 | `ip filter 3083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 571 | `ip filter 3084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 572 | `ip filter 3085 pass 172.16.0.0/16 202.226.76.11 tcp * https` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 573 | `ip filter 3086 pass 172.16.0.0/16 202.226.76.11 tcp * www` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 574 | `ip filter 3087 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3586) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 575 | `ip filter 3088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 576 | `ip filter 3089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 577 | `ip filter 3092 pass * 172.16.5.0/24 tcp https,5222,5223 *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3594) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 578 | `ip filter 3093 pass * 172.16.5.0/24 udp domain,ntp *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3596) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 579 | `ip filter 3094 pass * 172.16.5.0/24 icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3601) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 580 | `ip filter 3097 pass 172.16.5.0/24 * tcp * https,5222,5223` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 581 | `ip filter 3098 pass 172.16.5.0/24 * udp * domain,ntp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 582 | `ip filter 3099 pass 172.16.5.0/24 * icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 583 | `ip filter 101402 pass * - udp ntp *` | `PutFilter` | 固定文字列 | pCmd配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 584 | `ip filter 101404 pass * - tcp www,https *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 585 | `ip filter 101406 pass - 180.55.241.146 udp * *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 586 | `ip filter 101407 pass - * udp * ntp` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 587 | `ip filter 101408 pass - * udp * domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 588 | `ip filter 101409 pass - * tcp * www,https` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 589 | `ip filter 10800 pass 18.179.137.187,3.115.131.124 172.16.15.0/24 tcp www,https,8080 *` | `PutFilter` | 固定文字列 | pCmd配列固定要素 (Router1200.cpp:3577付近) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 590 | `ip filter 10801 pass 172.16.15.0/24 18.179.137.187,3.115.131.124 tcp * www,https,8080` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 591 | `ip filter 10500 pass * 172.16.5.1 tcp * 53196` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.nLuaPort` | `PutFmt("ip filter 10500 pass * %s tcp * %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort)` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 592 | `ip filter 10505 pass 172.16.5.1 * tcp 53196` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.nLuaPort` | `PutFmt("ip filter 10505 pass %s * tcp %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort)` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 593 | `ip filter 10510 pass 172.16.5.1 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10510 pass %s %s udp * 500", GetLuaIP(), GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 594 | `ip filter 10520 pass 172.16.5.1 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10520 pass %s %s esp * *", GetLuaIP(), GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 595 | `ip filter 10525 pass 172.16.5.1 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10525 pass %s %s esp * *", GetIpB2C(m_sRouter.ip), GetLuaIP())` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 596 | `ip filter 10550 pass 172.16.5.1 172.16.5.1 udp * 1701` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10550 pass %s %s udp * 1701", GetLuaIP(), GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 597 | `ip filter 10560 pass 172.16.5.1 172.16.5.1 udp * 4500` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10560 pass %s %s udp * 4500", GetLuaIP(), GetIpB2C(m_sRouter.ip))` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 598 | `ip filter 10575 pass 172.16.5.1 172.16.5.1 udp * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10575 pass %s %s udp * *", GetIpB2C(m_sRouter.ip), GetLuaIP())` | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 599 | `ip filter 10600 pass 172.16.60.0/24 172.16.60.0/24 tcp * 58300` | `PutFilter` | 固定文字列 | pCmd配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 600 | `ip filter 10605 pass 172.16.60.0/24 172.16.60.0/24 tcp 58300 *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## IPフィルター 続き（行601〜615）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 601 | `ip filter 10610 pass 172.16.60.0/24 172.16.60.0/24 udp 58301 *` | `PutFilter` | 固定文字列 | pCmd配列固定要素 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 602 | `ip filter 10615 pass 172.16.60.0/24 172.16.60.0/24 udp * 58301` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 603 | `ip filter 10620 pass 172.16.60.0/24 172.16.60.0/24 tcp * 58378` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 604 | `ip filter 10625 pass 172.16.60.0/24 172.16.60.0/24 tcp 58378 *` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 605 | `ip filter 12600 pass [長IPリスト] 172.16.5.0/24 udp * 500` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（XAIサーバーIPリスト + m_sRouter.ip派生）(Router1200.cpp:3577) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 606 | `ip filter 12650 pass 172.16.5.0/24 [長IPリスト] udp * 500` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（逆方向）(Router1200.cpp:3586) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 607 | `ip filter 12700 pass [長IPリスト] 172.16.5.0/24 esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（ESP）(Router1200.cpp:3577) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 608 | `ip filter 12750 pass 172.16.5.0/24 [長IPリスト] esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（ESP逆方向）(Router1200.cpp:3586) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## ip filter dynamic エントリ（行609〜689）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 609 | `ip filter dynamic 25 172.16.0.0/16 211.129.14.134,211.129.12.43 domain` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 25 %s 211.129.14.134,211.129.12.43 domain", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 610 | `ip filter dynamic 26 172.16.5.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 611 | `ip filter dynamic 27 172.16.15.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 612 | `ip filter dynamic 28 172.16.60.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 613 | `ip filter dynamic 35 172.16.0.0/16 61.114.182.53 filter 35 in 30` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 614 | `ip filter dynamic 36 172.16.5.0/24 61.114.182.53 filter 36 in 31` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 615 | `ip filter dynamic 37 172.16.15.0/24 61.114.182.53 filter 37 in 32` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 616 | `ip filter dynamic 38 172.16.60.0/24 61.114.182.53 filter 38 in 33` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 617 | `ip filter dynamic 45 172.16.0.0/16 61.114.182.54 filter 45 in 40` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 618 | `ip filter dynamic 46 172.16.5.0/24 61.114.182.54 filter 46 in 41` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 619 | `ip filter dynamic 47 172.16.15.0/24 61.114.182.54 filter 47 in 42` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 620 | `ip filter dynamic 48 172.16.60.0/24 61.114.182.54 filter 48 in 43` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 621 | `ip filter dynamic 55 172.16.0.0/16 61.114.186.0/25 filter 55 in 50` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 622 | `ip filter dynamic 56 172.16.5.0/24 61.114.186.0/25 filter 56 in 51` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 623 | `ip filter dynamic 57 172.16.15.0/24 61.114.186.0/25 filter 57 in 52` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 624 | `ip filter dynamic 58 172.16.60.0/24 61.114.186.0/25 filter 58 in 53` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 625 | `ip filter dynamic 59 172.16.80.0/22 61.114.186.0/25 filter 59 in 54` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 626 | `ip filter dynamic 65 172.16.0.0/16 61.114.177.148 filter 65 in 60` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 627 | `ip filter dynamic 66 172.16.5.0/24 61.114.177.148 filter 66 in 61` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 628 | `ip filter dynamic 67 172.16.15.0/24 61.114.177.148 filter 67 in 62` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 629 | `ip filter dynamic 68 172.16.60.0/24 61.114.177.148 filter 68 in 63` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 630 | `ip filter dynamic 75 172.16.0.0/16 202.229.106.177 filter 75 in 70` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 631 | `ip filter dynamic 76 172.16.5.0/24 202.229.106.177 filter 76 in 71` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 632 | `ip filter dynamic 77 172.16.15.0/24 202.229.106.177 filter 77 in 72` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 633 | `ip filter dynamic 78 172.16.60.0/24 202.229.106.177 filter 78 in 73` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 634 | `ip filter dynamic 79 172.16.80.0/22 202.229.106.177 filter 79 in 74` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 635 | `ip filter dynamic 85 172.16.0.0/16 * filter 85 in 80` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 85 %s * filter 85 in 80", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 636 | `ip filter dynamic 86 172.16.0.0/16 * filter 86 in 81` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 86 %s * filter 86 in 81", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 637 | `ip filter dynamic 87 172.16.15.0/24 * filter 87 in 82` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 638 | `ip filter dynamic 88 172.16.0.0/16 * filter 88 in 83` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 639 | `ip filter dynamic 89 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 89 in 84` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 640 | `ip filter dynamic 97 172.16.5.0/24 * filter 97 in 92` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 641 | `ip filter dynamic 98 172.16.5.0/24 * filter 98 in 93` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 642 | `ip filter dynamic 99 172.16.5.0/24 * filter 99 in 94` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 643 | `ip filter dynamic 2097 172.16.5.0/24 * filter 2097 in 2092` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 644 | `ip filter dynamic 2098 172.16.5.0/24 * filter 2098 in 2093` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 645 | `ip filter dynamic 2099 172.16.5.0/24 * filter 2099 in 2094` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 646 | `ip filter dynamic 3097 172.16.5.0/24 * filter 3097 in 3092` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 647 | `ip filter dynamic 3098 172.16.5.0/24 * filter 3098 in 3093` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 648 | `ip filter dynamic 3099 172.16.5.0/24 * filter 3099 in 3094` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 649 | `ip filter dynamic 2025 172.16.0.0/16 211.129.14.134,211.129.12.43 domain` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 650 | `ip filter dynamic 2026 * 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 651 | `ip filter dynamic 2027 172.16.15.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 652 | `ip filter dynamic 2028 172.16.60.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 653 | `ip filter dynamic 2035 172.16.0.0/16 61.114.182.53 filter 2035 in 2030` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 654 | `ip filter dynamic 2036 172.16.5.0/24 61.114.182.53 filter 2036 in 2031` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 655 | `ip filter dynamic 2037 172.16.15.0/24 61.114.182.53 filter 2037 in 2032` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 656 | `ip filter dynamic 2038 172.16.60.0/24 61.114.182.53 filter 2038 in 2033` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 657 | `ip filter dynamic 2045 172.16.0.0/16 61.114.182.54 filter 2045 in 2040` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 658 | `ip filter dynamic 2046 172.16.5.0/24 61.114.182.54 filter 2046 in 2041` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 659 | `ip filter dynamic 2047 172.16.15.0/24 61.114.182.54 filter 2047 in 2042` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 660 | `ip filter dynamic 2048 172.16.60.0/24 61.114.182.54 filter 2048 in 2043` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 661 | `ip filter dynamic 2055 172.16.0.0/16 61.114.186.0/25 filter 2055 in 2050` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 662 | `ip filter dynamic 2056 172.16.5.0/24 61.114.186.0/25 filter 2056 in 2051` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 663 | `ip filter dynamic 2057 172.16.15.0/24 61.114.186.0/25 filter 2057 in 2052` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 664 | `ip filter dynamic 2058 172.16.60.0/24 61.114.186.0/25 filter 2058 in 2053` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 665 | `ip filter dynamic 2059 172.16.80.0/22 61.114.186.0/25 filter 2059 in 2054` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 666 | `ip filter dynamic 2065 172.16.0.0/16 61.114.177.148 filter 2065 in 2060` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 667 | `ip filter dynamic 2066 172.16.5.0/24 61.114.177.148 filter 2066 in 2061` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 668 | `ip filter dynamic 2067 172.16.15.0/24 61.114.177.148 filter 2067 in 2062` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 669 | `ip filter dynamic 2068 172.16.60.0/24 61.114.177.148 filter 2068 in 2063` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 670 | `ip filter dynamic 2075 172.16.0.0/16 202.229.106.177 filter 2075 in 2070` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 671 | `ip filter dynamic 2076 172.16.5.0/24 202.229.106.177 filter 2076 in 2071` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 672 | `ip filter dynamic 2077 172.16.15.0/24 202.229.106.177 filter 2077 in 2072` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 673 | `ip filter dynamic 2078 172.16.60.0/24 202.229.106.177 filter 2078 in 2073` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 674 | `ip filter dynamic 2079 172.16.80.0/22 202.229.106.177 filter 2079 in 2074` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 675 | `ip filter dynamic 2085 172.16.0.0/16 * filter 2085 in 2080` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 2085 %s * filter 2085 in 2080", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 676 | `ip filter dynamic 2086 172.16.0.0/16 * filter 2086 in 2081` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 2086 %s * filter 2086 in 2081", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 677 | `ip filter dynamic 2087 172.16.15.0/24 * filter 2087 in 2082` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 678 | `ip filter dynamic 2088 172.16.0.0/16 * filter 2088 in 2083` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 679 | `ip filter dynamic 2089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 2089 in 2084` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 680 | `ip filter dynamic 101406 - 180.55.241.146 udp` | `PutFilter` | 固定文字列 | pCmd配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 681 | `ip filter dynamic 101407 - * filter 101407 in 101402` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 682 | `ip filter dynamic 101408 - * domain` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 683 | `ip filter dynamic 101409 - * filter 101409 in 101404` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 684 | `ip filter dynamic 3085 172.16.0.0/16 * filter 3085 in 3080` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 3085 %s * filter 3085 in 3080", GetIpB2C(bMaskedIP, m_sRouter.mask))` | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 685 | `ip filter dynamic 3086 172.16.0.0/16 * filter 3086 in 3081` | `PutFilter` | `m_sRouter.mask` | 同上パターン | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 686 | `ip filter dynamic 3087 172.16.15.0/24 * filter 3087 in 3082` | `PutFilter` | 固定文字列 | pCmd03配列固定要素 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 687 | `ip filter dynamic 3088 172.16.0.0/16 * filter 3088 in 3083` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 688 | `ip filter dynamic 3089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 3089 in 3084` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 689 | `ip filter dynamic 10615 172.16.60.0/24 172.16.60.0/24 filter 10615 in 10610 10620 out 10625` | `PutFilter` | 固定文字列 | 固定コマンド | ProcFilter (ip filter dynamic ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## ip filter set エントリ（行690〜700）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 690 | `ip filter set xai_tunnel_lan2 out 10605 10615 dynamic 10615` | `PutFilter` | 固定文字列 | 固定フィルターセット名+番号 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 691 | `ip filter set dk_lan1 in 1400 1405 1410 1430 1431 1432 1433 1434 1435 1436 1437 1440 1441` | `PutFilter` | 固定文字列 | `_Put(szPut, FALSE)` フィルターセット (Router1200.cpp:3252) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 692 | `ip filter set dk_lan1 out 1400 1405 1430 1431 1432 1433 1434 1435 1436 1437 1440 1441` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 693 | `ip filter set dk_pp_lan2 in 1 2 3 4 ...` | `PutFilter` | 固定文字列（一部`m_sRouter.ip`依存ID含む） | `_Put(szPut, FALSE)` 複数フィルタIDをまとめた文字列 (Router1200.cpp:3261) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 694 | `ip filter set dk_pp_lan2 out 1 2 3 4 ...dynamic ...` | `PutFilter` | 固定文字列（一部`m_sRouter.ip`依存ID含む） | 同上（out方向）(Router1200.cpp:3275) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 695 | `ip filter set dk_lan3 in 3 4 3010 3420 ...dynamic 3530 3550 3590` | `PutFilter` | 固定文字列 | 同上 (Router1200.cpp:3284) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 696 | `ip filter set dk_lan3 out 3 4 104 105 3015 ...` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 697 | `ip filter set dk_pp_lab in 9` | `PutFilter` | 固定文字列 | `_Put("ip filter set dk_pp_lab in 9")` (Router1200.cpp:3289) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 698 | `ip filter set dk_pp_lab out 1 2 3 4 5 6 104 105 ... dynamic ...` | `PutFilter` | 固定文字列 | `_Put(szPut, FALSE)` (Router1200.cpp:3297) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 699 | `ip filter set getlog in 1 2 3 4 5 6 100` | `PutFilter` | 固定文字列 | `_Put(szPut, FALSE)` (Router1200.cpp:3306) | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |
| 700 | `ip filter set getlog out 1 2 3 4 5 6 100` | `PutFilter` | 固定文字列 | 同上 | ProcFilter (ip filter ...) | m_sRouter.ip / m_sRouter.mask 等 |

---

## NATディスクリプター設定（行701〜763）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 701 | （空行） | − | − | − | − | − |
| 702 | `# NAT Descriptor configuration` | `PutNatOther` | 固定文字列 | セクションタイトル | − | セクション区切り |
| 703 | （空行） | − | − | − | − | − |
| 704 | `nat descriptor log on` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor masquerade ...) | 固定 |
| 705 | `nat descriptor masquerade ttl hold all` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor masquerade ...) | 固定 |
| 706 | `nat descriptor type 1 nat` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor type ...) | m_sRouter.m_nVWNo 等 (接続形態) |
| 707 | `nat descriptor address outer 1 10.6.5.1` | `PutNatOther` | `m_sRouter.uip`, `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.ip[2], m_sRouter.ip[3])` | ProcNAT (nat descriptor address outer ...) | m_sRouter.uip / m_sRouter.sLAN3.ip 等 |
| 708 | `nat descriptor address inner 1 172.16.5.1` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor address inner 1 %s", GetIpB2C(m_sRouter.ip))` | ProcNAT (nat descriptor address inner ...) | m_sRouter.ip / m_sRouter.sLAN3.ipVirus 等 |
| 709 | `nat descriptor static 1 1 10.6.1.1=172.16.1.1 1` | `PutNatOther` | 固定文字列（`m_sRouter.uip`派生） | 固定または uip基準の静的NATエントリ | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 710 | `nat descriptor static 1 2 10.6.2.1=172.16.2.1 2` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 711 | `nat descriptor static 1 4 10.6.4.1=172.16.4.1 8` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 712 | `nat descriptor static 1 5 10.6.5.1=172.16.5.1 10` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 713 | `nat descriptor static 1 10 10.6.10.1=172.16.10.1 2` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 714 | `nat descriptor static 1 11 10.6.11.1=172.16.11.1 128` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 715 | `nat descriptor static 1 12 10.6.12.1=172.16.12.1 128` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 716 | `nat descriptor static 1 13 10.6.13.1=172.16.13.1 8` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 717 | `nat descriptor static 1 14 10.6.14.1=172.16.14.1 12` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 718 | `nat descriptor static 1 15 10.6.15.1=172.16.15.1 8` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 719 | `nat descriptor static 1 20 10.6.20.1=172.16.20.1 2` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 720 | `nat descriptor static 1 21 10.6.21.1=172.16.21.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 721 | `nat descriptor static 1 30 10.6.30.1=172.16.30.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 722 | `nat descriptor static 1 31 10.6.31.1=172.16.31.1 8` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 723 | `nat descriptor static 1 40 10.6.40.1=172.16.40.1 32` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 724 | `nat descriptor static 1 50 10.6.50.1=172.16.50.1 3` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 725 | `nat descriptor static 1 52 10.6.52.1=172.16.52.1 8` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 726 | `nat descriptor static 1 60 10.6.60.1=172.16.60.1 127` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 727 | `nat descriptor static 1 121 10.6.121.20=172.16.121.20 40` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 728 | `nat descriptor static 1 80 10.6.80.1=172.16.80.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 729 | `nat descriptor static 1 81 10.6.81.1=172.16.81.1 16` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 730 | `nat descriptor static 1 82 10.6.82.1=172.16.82.1 16` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 731 | `nat descriptor static 1 83 10.6.83.1=172.16.83.1 16` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 732 | `nat descriptor static 1 110 10.6.110.1=172.16.110.1 2` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 733 | `nat descriptor static 1 100 10.6.100.1=172.16.100.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 734 | `nat descriptor static 1 122 10.6.122.1=172.16.122.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 735 | `nat descriptor type 2000 masquerade` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor type ...) | m_sRouter.m_nVWNo 等 (接続形態) |
| 736 | `nat descriptor address outer 2000 153.142.244.124` | `PutNatOther` | `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "nat descriptor address outer 2000 %s", GetIpB2C(m_sRouter.ncon[n].fixIp))` | ProcNAT (nat descriptor address outer ...) | m_sRouter.uip / m_sRouter.sLAN3.ip 等 |
| 737 | `nat descriptor masquerade static 2000 1 172.16.5.1 udp 500` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 1 %s udp 500", GetIpB2C(m_sRouter.ip))` | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |
| 738 | `nat descriptor masquerade static 2000 2 172.16.5.1 esp` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 2 %s esp", GetIpB2C(m_sRouter.ip))` | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |
| 739 | `nat descriptor masquerade static 2000 6 172.16.5.1 udp 4500` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 6 %s udp 4500", GetIpB2C(m_sRouter.ip))` | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |
| 740 | `nat descriptor masquerade static 2000 7 172.16.5.1 tcp 53196` | `PutNatOther` | `m_sRouter.ip`, `m_sRouter.nLuaPort` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 7 %s tcp %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort)` | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |
| 741 | `nat descriptor type 2012 masquerade` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor type ...) | m_sRouter.m_nVWNo 等 (接続形態) |
| 742 | `nat descriptor address outer 2012 172.23.176.129` | `PutNatOther` | 固定文字列 | 固定コマンド（固定VPNアドレス） | ProcNAT (nat descriptor address outer ...) | m_sRouter.uip / m_sRouter.sLAN3.ip 等 |
| 743 | `nat descriptor address inner 2012 auto` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor address inner ...) | m_sRouter.ip / m_sRouter.sLAN3.ipVirus 等 |
| 744 | `nat descriptor static 2012 2 172.23.176.130=172.16.5.1 1` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor static 2012 2 %s=%s 1", GetIpB2C(bVPNadd), GetIpB2C_Second(m_sRouter.ip))` | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 745 | `nat descriptor static 2012 3 172.23.176.131=172.16.60.1 1` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 746 | `nat descriptor static 2012 4 172.23.176.132=172.16.1.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 747 | `nat descriptor static 2012 5 172.23.176.133=172.16.21.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 748 | `nat descriptor static 2012 6 172.23.176.134=172.16.80.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 749 | `nat descriptor static 2012 7 172.23.176.135=172.16.15.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 750 | `nat descriptor static 2012 8 172.23.176.136=172.16.52.8 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 751 | `nat descriptor static 2012 9 172.23.176.137=172.16.50.2 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 752 | `nat descriptor static 2012 10 172.23.176.138=172.16.20.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 753 | `nat descriptor static 2012 11 172.23.176.139=172.16.20.2 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 754 | `nat descriptor static 2012 12 172.23.176.140=172.16.110.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 755 | `nat descriptor static 2012 13 172.23.176.141=172.16.110.2 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 756 | `nat descriptor static 2012 14 172.23.176.142=172.16.122.1 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 757 | `nat descriptor static 2012 15 172.23.176.143=172.16.15.100 1` | `PutNatOther` | 固定文字列 | 同上 | ProcNAT (nat descriptor static ...) | m_sRouter.ip 等 |
| 758 | `nat descriptor type 12 masquerade` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor type ...) | m_sRouter.m_nVWNo 等 (接続形態) |
| 759 | `nat descriptor masquerade static 2000 3 172.16.5.1 tcp 1723` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 3 %s tcp 1723", GetIpB2C(m_sRouter.ip))` | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |
| 760 | `nat descriptor masquerade static 2000 4 172.16.5.1 gre` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 4 %s gre", GetIpB2C(m_sRouter.ip))` | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |
| 761 | `nat descriptor masquerade static 2000 5 172.16.5.1 udp 4500` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 5 %s udp 4500", GetIpB2C(m_sRouter.ip))` | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |
| 762 | `nat descriptor sip 2000 off` | `PutNatOther` | 固定文字列 | 固定コマンド | ProcNAT (nat descriptor masquerade static ...) | m_sRouter.ip / m_sRouter.nLuaPort 等 |

---

## その他設定セクション（行763〜791）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット | 格納処理関数(パース) | 格納先メンバ変数 |
|---|---|---|---|---|---|---|
| 763 | （空行） | − | − | − | − | − |
| 764 | `# Other configuration` | `PutConfig` | 固定文字列 | セクションタイトル (Router1200.cpp:2940付近) | − | セクション区切り |
| 765 | （空行） | − | − | − | − | − |
| 766 | `syslog info on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 (Router1200.cpp:2986) | ProcOther (syslog ...) | 固定 |
| 767 | `syslog notice on` | `PutConfig` | 固定文字列 | 同上 | ProcOther (syslog ...) | 固定 |
| 768 | `syslog debug off` | `PutConfig` | 固定文字列 | 同上 | ProcOther (syslog ...) | 固定 |
| 769 | `tftp host any` | `PutConfig` | 固定文字列 | 同上 | ProcOther (tftp ...) | 固定 |
| 770 | `ipsec auto refresh on` | `PutConfig` | 固定文字列 | 同上 | ProcIPSEC | 固定 |
| 771 | `dhcp server rfc2131 compliant except remain-silent` | `PutConfig` | 固定文字列 | 同上 | ProcDHCP | 固定 |
| 772 | `dns server 211.129.14.134 211.129.12.43` | `PutConfig` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask))` または固定 | ProcDNS (dns server ...) | m_sRouter.con[n].ip / m_sRouter.sLAN3.sPriSecIp.ip 等 |
| 773 | `dns server pp 5` | `PutConfig` | `m_sRouter.m_nAdslNo`, `m_sRouter.fBbUse`, `m_sRouter.m_nVWNo` | `wsprintf(szBuf, "dns server pp %d", (m_sRouter.m_nAdslNo && m_sRouter.fBbUse) ? m_sRouter.m_nAdslNo : m_sRouter.m_nVWNo)` | ProcDNS (dns server pp ...) | m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo |
| 774 | `dns private address spoof on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 | ProcDNS | 固定 |
| 775 | `schedule at 1 */* 6:00 * ntpdate 172.31.191.62` | `PutConfig` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3316) | Analize (schedule ...) | 固定 |
| 776 | `dns service recursive` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 | ProcDNS | 固定 |
| 777 | `no dhcp service` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 | ProcDHCP (dhcp service ...) | m_sRouter.m_fDHCP 等 |
| 778 | `no dhcp scope 1` | `PutConfig` | 固定文字列 | 同上 | ProcDHCP (dhcp scope ...) | m_szFirstAdd, m_szLastAdd, m_sRouter.m_fDHCP 等 |
| 779 | `dhcp scope option 1 dns=172.16.5.1` | `PutConfig` | `m_sRouter.ip` | `wsprintf(szBuf, "dhcp scope option 1 dns=%s", GetIpB2C(m_sRouter.ip))` | ProcDHCP (dhcp scope option ...) | m_sRouter.ip_2_cloud 等 |
| 780 | `pptp service on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 | ProcOther | 固定 |
| 781 | `ip stealth pp 5` | `PutConfig` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip stealth pp %d", m_sRouter.m_nAdslNo)` | ProcIP (ip stealth ...) | m_sRouter.m_nAdslNo 等 |
| 782 | `l2tp service on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 | ProcTunnel (l2tp ...) | 固定 |
| 783 | `schedule at 2 startup * lua /xai_check.lua` | `PutConfig` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3316) | Analize (schedule ...) | 固定 |
| 784 | `schedule at 3 */* 04:00:00 * terminate lua all` | `PutConfig` | 固定文字列 | 同上 | Analize (schedule ...) | 固定 |
| 785 | `schedule at 4 */* 04:00:10 * lua /xai_check.lua` | `PutConfig` | 固定文字列 | 同上 | Analize (schedule ...) | 固定 |
| 786 | `# LIFT_PHASE=1` | − | − | コメント行（書き込み不明） | − | セクション区切り |
| 787 | `description 1 "configversion=5.0"` | `PutConfig` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 788 | `save` | `PutConfig` | 固定文字列 | 固定コマンド | Analize | 固定 |
| 789 | `description 1 "configversion=5.1"` | `PutConfig` | 固定文字列 | 固定コマンド | 各要素のProc処理 または Analize | なし または 簡易フラグ |
| 790 | `save` | `PutConfig` | 固定文字列 | 固定コマンド | Analize | 固定 |
| 791 | （空行） | − | − | ファイル末尾 | − | − |

---

## 補足: 本レポートで使用した主なSDkRouterメンバ変数まとめ

| メンバ変数 | .dat上での利用例 | 主な書込関数 |
|---|---|---|
| `m_sRouter.kind` | `#\tRTX830 Rev...` ルーター種別名 | `PutConfig` |
| `m_sRouter.szRev` | `#\tRTX830 Rev.15.02.31` ファームバージョン | `PutConfig` |
| `m_sRouter.ip` | `ip lan1 address 172.16.5.1/16`, `ipsec ike local address N 172.16.5.1`, フィルター内172.16.5.1 | `PutLanIp`, `PutTunnel`, `PutFilter`, `PutNatOther` |
| `m_sRouter.mask` | `ip lan1 address .../16`, フィルター内172.16.0.0/16 | `PutLanIp`, `PutFilter` |
| `m_sRouter.uip` | `ipsec ike local id N 10.6.0.0/16`, NAT outer 1 | `PutTunnel`, `PutNatOther` |
| `m_sRouter.con[n].no` | `### PP 5 ###`, `pp select 5`, `tunnel enable 1` | `PutPp`, `PutTunnel` |
| `m_sRouter.con[n].ip/mask` | `ip route 172.31.191.0/24 gateway tunnel 1`, `ipsec ike remote id` | `PutIpRoute`, `PutTunnel` |
| `m_sRouter.ncon[n].fixIp` | `ipsec ike remote address`, `ipsec ike pre-shared-key text 153.142.244.124` | `PutTunnel`, `PutNatOther` |
| `m_sRouter.ncon[n].type` | `ipsec ike keepalive use N off` (eDKVPN判定) | `PutTunnel` |
| `m_sRouter.m_nAdslNo` | `### TUNNEL 5 ###`, `tunnel select 5`, `ip route default gateway pp 5` | `PutTunnel`, `PutIpRoute` |
| `m_sRouter.m_nDeltaBkNo` | `tunnel backup tunnel 4 switch-interface=on` | `PutTunnel` |
| `m_sRouter.sSIS05.no` | `ip route 10.142.65.0/24 gateway tunnel 2` (4行) | `PutIpRoute` |
| `m_sRouter.szAdslID/szAdslPass` | `pp auth myname w3102kwz@bizf.ocn.ne.jp sdjLSa76` | `PutPp` |
| `m_sRouter.szVPN_User/szVPN_Pass` | `pp auth username w8LeUYeGaZgstZXs 00393792` | `PutPp` |
| `m_sRouter.szVPN_Key` | `ipsec ike pre-shared-key N text e6BQyqm7...` (TUNNEL 16-20) | `PutTunnel` |
| `m_sRouter.nMTU` | `ip pp mtu 1454` | `PutPp` |
| `m_sRouter.port[1].kaisen` | `pp always-on on` | `PutPp` |
| `m_sRouter.nLuaPort` | `ip filter 10500 pass * 172.16.5.1 tcp * 53196`, NAT static 2000 7 | `PutFilter`, `PutNatOther` |
| `m_sRouter.fBbUse` | `dns server pp 5` (条件分岐) | `PutConfig` |
| `m_sRouter.m_nVWNo` | `dns server pp 5` (条件分岐) | `PutConfig` |
| `m_strADMIN_PASS` | `login password ccsystem`, `administrator password ccsystem` | `PutConfig` |
| `m_cRtPassEx.m_nPH_Code` | `snmp syslocation "Hall 23999185.5123"` | `PutConfig` |
| `nSetuzokuNo` | TUNNEL番号（ループ変数）、`### TUNNEL N ###`, `ipsec tunnel N+100` 等 | `PutTunnel` |
