# router01W.dat 行別書き込み解析レポート（パート1: 行1〜200）

## 凡例
- **書込関数**: Router1200.cpp内の関数名
- **SDkRouterメンバ**: 書き込みに使用されたメンバ変数（固定文字列の場合は「固定」）
- **根拠**: ソース上のフォーマット文字列

---

## ヘッダー・基本設定セクション（行1〜15）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 1 | `#	DK-ROUTER-CONFIGURATION Ver A1.000` | `PutConfig` | 固定文字列 | `_Put("#\tDK-ROUTER-CONFIGURATION Ver A1.000")` (Router1200.cpp:2578) |
| 2 | `#	RTX830 Rev.15.02.31` | `PutConfig` | `m_sRouter.kind`, `m_sRouter.szRev` | `wsprintf(szBuf, "#\t%s Rev.%s", szRtType[m_sRouter.kind-1], m_sRouter.szRev)` (Router1200.cpp:2582) |
| 3 | （空行） | `PutConfig` | − | セクション区切り空行 |
| 4 | `# System configuration` | `PutConfig` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:2585) |
| 5 | （空行） | `PutConfig` | − | セクション区切り空行 |
| 6 | `clear configuration` | `PutConfig` | 固定文字列 | `_Put("clear configuration")` (Router1200.cpp:2588) |
| 7 | `no schedule at 1` | `PutConfig` | 固定文字列 | `_Put(pCmd01, DK_Number(pCmd01))` pCmd01配列要素 (Router1200.cpp:2597) |
| 8 | `no schedule at 2` | `PutConfig` | 固定文字列 | 同上 pCmd01配列要素 |
| 9 | `no schedule at 3` | `PutConfig` | 固定文字列 | 同上 pCmd01配列要素 |
| 10 | `no schedule at 4` | `PutConfig` | 固定文字列 | 同上 pCmd01配列要素 |
| 11 | `login password ccsystem` | `PutConfig` | `m_strADMIN_PASS` | `str.Format("login password %s", m_strADMIN_PASS)` → `_Put(str)` (Router1200.cpp:2644) |
| 12 | `administrator password ccsystem` | `PutConfig` | `m_strADMIN_PASS` | `str.Format("administrator password %s", m_strADMIN_PASS)` → `_Put(str)` (Router1200.cpp:2648) |
| 13 | `snmp syslocation "Hall 23999185.5123"` | `PutConfig` | `m_cRtPassEx.m_nPH_Code` | `PutFmt("snmp syslocation \"Hall %d.%d\"", m_cRtPassEx.m_nPH_Code, nHint)` (Router1200.cpp:2684) |
| 14 | `security class 1 on on` | `PutConfig` | 固定文字列 | `_Put(pCmd02, DK_Number(pCmd02))` pCmd02配列要素 (Router1200.cpp:2693) |
| 15 | `console info off` | `PutConfig` | 固定文字列 | 同上 pCmd02配列要素 |

---

## IPルーティング設定セクション（行16〜44）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 16 | （空行） | `PutConfig` | − | セクション区切り空行 |
| 17 | `ip routing process normal` | `PutConfig` | 固定文字列 | pCmd02配列要素 (Router1200.cpp:2693) |
| 18 | `ip route 10.142.65.0/24 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.142.65.0/24 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) |
| 19 | `ip route 10.142.165.0/24 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.142.165.0/24 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) |
| 20 | `ip route 10.145.0.0/16 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.145.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) |
| 21 | `ip route 10.141.0.0/16 gateway tunnel 2` | `PutIpRoute` | `m_sRouter.sSIS05.no` | `str.Format("ip route 10.141.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no)` (Router1200.cpp:2813) |
| 22 | （空行） | `PutIpRoute` | − | セクション区切り空行 |
| 23 | `ip route 172.18.0.0/16 gateway 172.16.5.11` | `PutIpRoute` | 固定文字列（推論） | 固定ルート。pCmd配列の固定要素と推論。具体的なフォーマット行不明 |
| 24 | `ip route 61.114.182.53 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route %s gateway pp %d metric 2", pCmd[nBit], m_sRouter.m_nAdslNo)` (Router1200.cpp付近) |
| 25 | `ip route 61.114.182.54 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP |
| 26 | `ip route 61.114.186.0/25 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP |
| 27 | `ip route 61.114.177.148 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP |
| 28 | `ip route 202.229.106.177 gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | 同上 pCmd配列固定IP |
| 29 | `ip route 172.31.191.0/24 gateway tunnel 1` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | `wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no)` |
| 30 | `ip route 61.114.186.111 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo)` |
| 31 | `ip route 172.31.65.0/24 gateway tunnel 1` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット |
| 32 | `ip route 52.197.151.142 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット |
| 33 | `ip route 0.0.0.0/32 gateway tunnel 3` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット |
| 34 | `ip route 61.114.186.110 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット |
| 35 | `ip route 153.142.244.126 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット |
| 36 | `ip route 10.2.0.0/16 gateway tunnel 6` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット |
| 37 | `ip route 153.142.244.125 gateway pp 5` | `PutIpRoute` | `m_sRouter.ncon[n].fixIp`, `m_sRouter.m_nAdslNo` | 同行30と同フォーマット |
| 38 | `ip route 10.1.0.0/16 gateway tunnel 7` | `PutIpRoute` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask`, `m_sRouter.con[n].no` | 同行29と同フォーマット |
| 39 | `ip route default gateway pp 5 metric 2` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo)` |
| 40 | `ip route 10.0.192.0/24 gateway pp anonymous metric 2` | `PutIpRoute` | `m_sRouter.ip[0]`, `m_sRouter.ip[1]`, `m_sRouter.ip[2]` | `wsprintf(szBuf, "ip route %d.%d.%d.%d gateway pp anonymous metric 2", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], nIp)` |
| 41 | `ip route 10.0.208.0/24 gateway tunnel 5` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route 10.0.208.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo)` |
| 42 | `ip route 10.0.224.0/24 gateway tunnel 5` | `PutIpRoute` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip route 10.0.224.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo)` |
| 43 | `ip icmp parameter-problem send off` | `PutConfig` | 固定文字列 | pCmd02配列要素 (Router1200.cpp:2693) |
| 44 | `ip icmp echo-reply send-only-linkup on` | `PutConfig` | 固定文字列 | 同上 pCmd02配列要素 |

---

## LAN/IP設定セクション（行45〜58）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 45 | （空行） | − | − | セクション区切り空行 |
| 46 | `# LAN/IP configuration` | `PutLanIp` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:2747) |
| 47 | （空行） | − | − | セクション区切り空行 |
| 48 | `ip lan1 address 172.16.5.1/16` | `PutLanIp` | `m_sRouter.ip`, `m_sRouter.mask` | `wsprintf(szBuf, "ip lan1 address %s", GetIpB2C(m_sRouter.ip, m_sRouter.mask))` |
| 49 | `rip use on` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 50 | `ip lan1 rip send on version 2` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 51 | `ip lan1 rip receive on version 2` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 52 | `ip lan1 proxyarp on` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 53 | `ip lan1 secure filter name dk_lan1` | `PutFilter` | 固定文字列 | 固定コマンド（フィルターセット名） |
| 54 | `ip lan1 intrusion detection in on reject=on` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 55 | `ip lan2 rip send off` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 56 | `ip lan2 rip receive off` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 57 | `ip lan2 proxyarp off` | `PutLanIp` | 固定文字列 | 固定コマンド |
| 58 | `ip lan2 intrusion detection in on reject=on` | `PutLanIp` | 固定文字列 | 固定コマンド |

---

## PP設定セクション（行59〜102）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 59 | （空行） | − | − | セクション区切り空行 |
| 60 | `# PP configuration` | `PutPp` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:2955) |
| 61 | （空行） | − | − | セクション区切り空行 |
| 62 | `pp disable all` | `PutPp` | 固定文字列 | 固定コマンド |
| 63 | （空行） | − | − | セクション区切り空行 |
| 64 | `### PP 5 ###` | `PutPp` | `m_sRouter.con[n].no` | `wsprintf(szBuf, "### PP %d ###", m_sRouter.con[n].no)` |
| 65 | （空行） | − | − | セクション区切り空行 |
| 66 | `pp select 5` | `PutPp` | `m_sRouter.con[n].no` | `wsprintf(szBuf, "pp select %d", m_sRouter.con[n].no)` |
| 67 | `pp backup none` | `PutPp` | 固定文字列 | 固定コマンド |
| 68 | `pppoe use lan2` | `PutPp` | 固定文字列 | 固定コマンド |
| 69 | `pppoe auto connect on` | `PutPp` | 固定文字列 | 固定コマンド |
| 70 | `pppoe auto disconnect on` | `PutPp` | 固定文字列 | 固定コマンド |
| 71 | `pppoe disconnect time off` | `PutPp` | 固定文字列 | 固定コマンド |
| 72 | `ip pp mtu 1454` | `PutPp` | `m_sRouter.nMTU` | `wsprintf(szBuf, "ip pp mtu %d", m_sRouter.nMTU)` |
| 73 | `ip pp intrusion detection in on reject=on` | `PutPp` | 固定文字列 | 固定コマンド |
| 74 | `ip pp nat descriptor 2000` | `PutPp` | 固定文字列 | 固定コマンド |
| 75 | `pp auth accept pap chap` | `PutPp` | 固定文字列 | 固定コマンド |
| 76 | `pp always-on on` | `PutPp` | `m_sRouter.port[1].kaisen` | `wsprintf(szBuf, "pp always-on %s", (ADSL_KIND(m_sRouter.port[1].kaisen)==eKaisenFixADSL) ? "on" : "off")` |
| 77 | `ip pp secure filter name dk_pp_lan2` | `PutFilter` | 固定文字列 | 固定コマンド（フィルターセット名） |
| 78 | `ip pp address 153.142.244.124/32` | `PutPp` | `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "nat descriptor address outer 2000 %s", GetIpB2C(m_sRouter.ncon[n].fixIp))` に対応 ※PPアドレス行は固定文字列または推論 |
| 79 | `pp auth myname w3102kwz@bizf.ocn.ne.jp sdjLSa76` | `PutPp` | `m_sRouter.szAdslID`, `m_sRouter.szAdslPass` | `wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass)` |
| 80 | `ppp lcp mru on 1454` | `PutPp` | `m_sRouter.nMTU` | `wsprintf(szBuf, "ppp lcp mru on %d", m_sRouter.nMTU)` ※推論 |
| 81 | `ppp ccp type none` | `PutPp` | 固定文字列 | 固定コマンド |
| 82 | `ip pp rip send off` | `PutPp` | 固定文字列 | 固定コマンド |
| 83 | `ip pp rip receive off` | `PutPp` | 固定文字列 | 固定コマンド |
| 84 | `pp enable 5` | `PutPp` | `m_sRouter.con[n].no` | `wsprintf(szBuf, "pp enable %d", m_sRouter.con[n].no)` |
| 85 | （空行） | − | − | セクション区切り空行 |
| 86 | `### PP anonymous ###` | `PutPp` | 固定文字列 | 固定コマンド |
| 87 | （空行） | − | − | − |
| 88 | `no ip route 10.0.192.0/20` | `PutPp` | 固定文字列 | 固定コマンド |
| 89 | `pp select anonymous` | `PutPp` | 固定文字列 | 固定コマンド |
| 90 | `no ip pp rip send` | `PutPp` | 固定文字列 | 固定コマンド |
| 91 | `no ip pp rip receive` | `PutPp` | 固定文字列 | 固定コマンド |
| 92 | `no ip pp nat descriptor` | `PutPp` | 固定文字列 | 固定コマンド |
| 93 | `no pp auth username ccsupport` | `PutPp` | 固定文字列 | 固定コマンド |
| 94 | `pp bind tunnel16-tunnel20` | `PutPp` | 固定文字列 | 固定コマンド |
| 95 | `pp auth request mschap-v2` | `PutPp` | 固定文字列 | 固定コマンド |
| 96 | `pp auth username w8LeUYeGaZgstZXs 00393792` | `PutPp` | `m_sRouter.szVPN_User`, `m_sRouter.szVPN_Pass` | `PutFmt("pp auth username %s %s", m_sRouter.szVPN_User, m_sRouter.szVPN_Pass)` |
| 97 | `ppp ipcp ipaddress on` | `PutPp` | 固定文字列 | 固定コマンド |
| 98 | `ppp ipcp msext on` | `PutPp` | 固定文字列 | 固定コマンド |
| 99 | `ip pp remote address pool 172.16.60.16-172.16.60.20` | `PutPp` | 固定文字列 | 固定コマンド |
| 100 | `ip pp mtu 1258` | `PutPp` | 固定文字列 | 匿名PP専用固定値（推論） |
| 101 | `pp enable anonymous` | `PutPp` | 固定文字列 | 固定コマンド |
| 102 | `tunnel disable all` | `PutTunnel` | 固定文字列 | 固定コマンド |

---

## トンネル設定セクション - TUNNEL 1（行103〜123）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 103 | （空行） | − | − | − |
| 104 | `### TUNNEL 1 ###` | `PutTunnel` | `nSetuzokuNo`(=1) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` |
| 105 | （空行） | − | − | − |
| 106 | `tunnel select 1` | `PutTunnel` | `nSetuzokuNo`(=1) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 107 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 108 | `ipsec tunnel 101` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 109 | `ipsec sa policy 101 1 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 110 | `ipsec ike duration ipsec-sa 1 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` |
| 111 | `ipsec ike encryption 1 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` |
| 112 | `ipsec ike group 1 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` |
| 113 | `ipsec ike hash 1 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` |
| 114 | `ipsec ike keepalive use 1 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` |
| 115 | `ipsec ike local address 1 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` |
| 116 | `ipsec ike pfs 1 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` |
| 117 | `ipsec ike pre-shared-key 1 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` |
| 118 | `ipsec ike remote address 1 61.114.186.111` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 119 | `tunnel backup tunnel 4 switch-interface=on` | `PutTunnel` | `m_sRouter.m_nDeltaBkNo` | `wsprintf(szBuf, "tunnel backup tunnel %d switch-interface=on", m_sRouter.m_nDeltaBkNo)` |
| 120 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 121 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 122 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 123 | `tunnel enable 1` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |

---

## TUNNEL 3（行124〜146）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 124 | （空行） | − | − | − |
| 125 | `### TUNNEL 3 ###` | `PutTunnel` | `nSetuzokuNo`(=3) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` |
| 126 | （空行） | − | − | − |
| 127 | `tunnel select 3` | `PutTunnel` | `nSetuzokuNo`(=3) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 128 | `ipsec tunnel 103` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 129 | `ipsec ike duration ipsec-sa 3 10800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo)` |
| 130 | `ipsec ike duration isakmp-sa 3 3600` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo)` |
| 131 | `ipsec ike encryption 3 aes-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo)` |
| 132 | `ipsec ike group 3 modp768 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo)` |
| 133 | `ipsec ike hash 3 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` |
| 134 | `ipsec ike keepalive use 3 off` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].type` | `wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, (m_sRouter.ncon[n].type==eDKVPN...) ? "off" : "on")` |
| 135 | `ipsec ike local address 3 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` |
| 136 | `ipsec ike local id 3 10.6.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.uip` | `wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask)` |
| 137 | `ipsec ike pfs 3 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` |
| 138 | `ipsec ike pre-shared-key 3 text wh1+eb0@rd` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo)` 固定パスフレーズ |
| 139 | `ipsec ike remote address 3 52.197.151.142` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 140 | `ipsec ike remote id 3 0.0.0.0/32` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask))` |
| 141 | `ipsec sa policy 103 3 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 142 | `tunnel backup none` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 143 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 144 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 145 | `ip tunnel nat descriptor 1` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 146 | `tunnel enable 3` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |

---

## TUNNEL 6（行147〜169）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 147 | （空行） | − | − | − |
| 148 | `### TUNNEL 6 ###` | `PutTunnel` | `nSetuzokuNo`(=6) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` |
| 149 | （空行） | − | − | − |
| 150 | `tunnel select 6` | `PutTunnel` | `nSetuzokuNo`(=6) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 151 | `ipsec tunnel 106` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 152 | `ipsec ike duration ipsec-sa 6 10800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo)` |
| 153 | `ipsec ike duration isakmp-sa 6 3600` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo)` |
| 154 | `ipsec ike encryption 6 aes-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo)` |
| 155 | `ipsec ike group 6 modp768 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo)` |
| 156 | `ipsec ike hash 6 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` |
| 157 | `ipsec ike keepalive use 6 off` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].type` | `wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, ...)` 値がoffのためtypeはeDKVPN等 |
| 158 | `ipsec ike local address 6 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` |
| 159 | `ipsec ike local id 6 10.6.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.uip` | `wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask)` |
| 160 | `ipsec ike pfs 6 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` |
| 161 | `ipsec ike pre-shared-key 6 text wh1+eb0@rd` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo)` 固定パスフレーズ |
| 162 | `ipsec ike remote address 6 153.142.244.126` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 163 | `ipsec ike remote id 6 10.2.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask))` |
| 164 | `ipsec sa policy 106 6 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 165 | `tunnel backup none` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 166 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 167 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 168 | `ip tunnel nat descriptor 1` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 169 | `tunnel enable 6` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |

---

## TUNNEL 7（行170〜192）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 170 | （空行） | − | − | − |
| 171 | `### TUNNEL 7 ###` | `PutTunnel` | `nSetuzokuNo`(=7) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` |
| 172 | （空行） | − | − | − |
| 173 | `tunnel select 7` | `PutTunnel` | `nSetuzokuNo`(=7) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 174 | `ipsec tunnel 107` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 175 | `ipsec ike duration ipsec-sa 7 10800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo)` |
| 176 | `ipsec ike duration isakmp-sa 7 3600` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo)` |
| 177 | `ipsec ike encryption 7 aes-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo)` |
| 178 | `ipsec ike group 7 modp768 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo)` |
| 179 | `ipsec ike hash 7 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` |
| 180 | `ipsec ike keepalive use 7 off` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].type` | `wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, ...)` 値offのためtypeはeDKVPN等 |
| 181 | `ipsec ike local address 7 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` |
| 182 | `ipsec ike local id 7 10.6.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.uip` | `wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask)` |
| 183 | `ipsec ike pfs 7 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` |
| 184 | `ipsec ike pre-shared-key 7 text wh1+eb0@rd` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo)` 固定パスフレーズ |
| 185 | `ipsec ike remote address 7 153.142.244.125` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 186 | `ipsec ike remote id 7 10.1.0.0/16` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask))` |
| 187 | `ipsec sa policy 107 7 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 188 | `tunnel backup none` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 189 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 190 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 191 | `ip tunnel nat descriptor 1` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 192 | `tunnel enable 7` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |

---

## TUNNEL 5（行193〜212） ※メンテナンス用VPN

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 193 | （空行） | − | − | − |
| 194 | `### TUNNEL 5 ###` | `PutTunnel` | `m_sRouter.m_nAdslNo`(=5) | `wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nAdslNo)` |
| 195 | （空行） | − | − | − |
| 196 | `tunnel select 5` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nAdslNo)` |
| 197 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 198 | `ipsec tunnel 105` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nAdslNo+100)` |
| 199 | `ipsec sa policy 105 5 esp aes-cbc sha-hmac` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", ...+100, ...)` |
| 200 | `ipsec ike duration ipsec-sa 5 28800` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nAdslNo)` |
