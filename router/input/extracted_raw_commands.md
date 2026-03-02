# 関数別出力コマンド一覧 (ソースコードそのまま)

`CDkRouter1200` クラスの各設定処理関数において、ルータ設定用コマンド文字列を組み立て・出力している全コード行（`_Put`, `wsprintf`, `pCmd`配列等）を省略せずに掲載しています。

## PutConfig
```cpp
if (!_Put("#\tDK-ROUTER-CONFIGURATION Ver A1.000"))
wsprintf(szBuf, "#\t%s Rev.%s", szRtType[m_sRouter.kind - 1], m_sRouter.szRev);
if (!_Put(szBuf))
if (!_PutTit(tit))
if (!_Put("clear configuration"))
const char *pCmd01[] = {	// schedule繧ｳ繝槭Φ繝峨ｯclear configuration縺ｧ豸亥悉荳榊庄縺ｪ縺ｮ縺ｧ縺薙ｮ繧ｳ繝槭Φ繝峨〒豸医☆
"no schedule at 1",
"no schedule at 2",
"no schedule at 3",
"no schedule at 4",
if (!_Put(pCmd01, DK_Number(pCmd01)))
str.Format("login password %s", m_strADMIN_PASS);
if ( !_Put(str) )
str.Format("administrator password %s", m_strADMIN_PASS);
if ( !_Put(str) )
if ( !PutFmt("login password encrypted %s", strAngo_Login32) )
if ( !PutFmt("login password %s", m_strNewLoginPass) )
if ( !PutFmt("administrator password encrypted %s", strAngo_Admin32) )
if ( !PutFmt("administrator password %s", m_strNewAdminPass) )
if ( !PutFmt("snmp syslocation \"Hall %d.%d\"", m_cRtPassEx.m_nPH_Code, nHint) )
if ( !_Put("snmp syslocation \"Hall .\"") )
const char *pCmd02[] = {
"security class 1 on on",
"console info off",
if ( !_Put(pCmd02, DK_Number(pCmd02)) )
```

## PutLanIp
```cpp
if (!_PutTit(tit))
wsprintf(szBuf, "ip lan1 address %s", GetIpB2C(m_sRouter.ip, m_sRouter.mask));
if (!_Put(szBuf))
wsprintf(szBuf, "ip lan1 secondary address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
if (!_Put(szBuf))
const char *pCmd01[] = {
"rip use on",
"ip lan1 rip send on version 2",
"ip lan1 rip receive on version 2",
"ip lan1 proxyarp on",
if ( !_Put(pCmd01,DK_Number(pCmd01)) )
wsprintf(szBuf, "ip lan1 secure filter name %s", m_fOA_LAN ? "oa_lan1" : "dk_lan1");
if (!_Put("ip lan1 intrusion detection in on reject=on"))
if (!_Put("ip lan1 nat descriptor 1000"))
wsprintf(szBuf, "ip lan1 tcp mss limit %d", m_sRouter.m_nMSS);
if (!_Put(szBuf))
const char *pCmd02[] = {
"ip lan2 rip send off",
"ip lan2 rip receive off",
"ip lan2 proxyarp off",
"ip lan2 intrusion detection in on reject=on",
if ( !_Put(pCmd02,DK_Number(pCmd02)) )
if (!_Put("ip lan2 nat descriptor 2000"))
if (!_Put("ip lan2 secure filter name dk_pp_lan2"))
if (!_Put("ip lan2 address dhcp"))
wsprintf(szBuf, "ip lan2 address %s", GetIpB2C(pInfo->fixIp, pInfo->yobi1));
if (!_Put(szBuf))
if (!_Put("lan type lan3 10-fdx"))
wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask));
if (!_Put(szBuf))
wsprintf(szBuf, "ip lan3 secure filter name %s", m_fOA_LAN ? "oa_lan3" : "dk_lan3");
wsprintf(szBuf, "ip lan3 nat descriptor %d", m_fOA_LAN ? 8000 : 3000);
if (!_Put(szBuf))
wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
if (!_Put(szBuf))
wsprintf(szBuf, "ip lan3 address %s/29", GetIpB2C(m_sRouter.ip_2_cloud));
if (!_Put(szBuf))
wsprintf(szBuf, "ip lan3 secure filter name dk_lan3_call");
const char *pCmd03[] = {
"ip lan3 rip send off",
"ip lan3 rip receive off",
"ip lan3 proxyarp off",
"ip lan3 intrusion detection in on reject=on"
if ( !_Put(pCmd03,DK_Number(pCmd03)) )
```

## PutIsdn
```cpp
if (!_PutTit(tit))
wsprintf(szBuf, "line type bri1 isdn-ntt");
if (!_Put(szBuf))
wsprintf(szBuf, "isdn local address bri1 %s", m_sRouter.port[0].Tel);
if (!_Put(szBuf))
if (!_Put("isdn terminator bri1 on"))
```

## PutPp
```cpp
wsprintf(szBuf, "### PP %d ###", m_sRouter.con[n].no);
if (!_PutTit(szBuf))			return FALSE;
wsprintf(szBuf, "pp select %d", m_sRouter.con[n].no);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "pp bind tunnel%d", m_sRouter.con[n].no);
if (!_Put(szBuf))				return FALSE;
if ( !_Put("pp bind bri1") )
wsprintf(szBuf, "isdn remote address call %s", m_sRouter.con[n].Tel);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "isdn remote address arrive %s", m_sRouter.con[n].Tel);
if (!_Put(szBuf))				return FALSE;
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
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "isdn remote address call %s", ROBO_TEL);
if (!_Put(szBuf))				return FALSE;
const char *pCmd[] = {
"isdn disconnect time 15",
"isdn disconnect input time 30",
"isdn disconnect output time 30",
"ip pp nat descriptor 4",
"pp auth accept pap chap",
"ppp ipcp ipaddress on",
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szRoboID, m_sRouter.szRoboPass);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "isdn remote address call %s", m_sRouter.con[n].Tel);
if (!_Put(szBuf))				return FALSE;
const char *pCmd[] = {
"isdn disconnect time 480",
"isdn disconnect input time 720",
"isdn disconnect output time 720",
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "isdn forced disconnect time %d", m_sRouter.shOverTimer * 60);	// 謗･邯夊ｶ驕弱ち繧､繝
if (!_Put(szBuf))
wsprintf(szBuf, "ip pp secure filter name %s", m_fOA_LAN ? "oa_pp_lab" : "dk_pp_lab");
if (!_Put("ip pp intrusion detection in on reject=on"))
wsprintf(szBuf, "ip pp nat descriptor %d", m_fOA_LAN ? 5000 : 3);
if (!_Put(szBuf))				return FALSE;
const char *pCmd2[] = {
"pp auth accept pap chap",
"ppp ipcp ipaddress on"
if (!_Put(pCmd2, DK_Number(pCmd2)))
wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szVwID, m_sRouter.szVwPass);
if (!_Put(szBuf))
if (!_Put("ppp ipcp msext off"))
wsprintf(szBuf, "isdn remote address call 0422799015");
if (!_Put(szBuf))				return FALSE;
const char *pCmd[] = {
"isdn disconnect time 15",
"isdn disconnect input time 30",
"isdn disconnect output time 30",
"ip pp nat descriptor 12",
"pp auth accept chap",
"ppp ipcp ipaddress on",
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "pp auth myname %s %s", (lstrlen(m_sRouter.sSIS05.szHoleCode) == 0) ? "0.0.0.0" : m_sRouter.sSIS05.szHoleCode, GetIpB2C(m_sRouter.sSIS05.passIp));
if (!_Put(szBuf))
const char *pCmd[] = {
"pp backup none",
"pp always-on off",
"pppoe use lan2",
"pppoe auto connect on",
"pppoe auto disconnect on",
"pppoe disconnect time 30",
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "ip pp mtu %d", (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nMTU : m_sRouter.nMTU);
if (!_Put(szBuf))
const char *pCmd1[] = {
"ip pp intrusion detection in on reject=on",
"pp auth accept pap chap",
if (!_Put(pCmd1, DK_Number(pCmd1)))
wsprintf(szBuf, "ip pp secure filter name %s", m_fOA_LAN ? "oa_pp_lab" : "dk_pp_lab");
wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ip pp nat descriptor %d", m_fOA_LAN ? 5000 : 3);
if (!_Put(szBuf))				return FALSE;
const char *pCmd2[] = {
"ppp lcp mru on 1454",
"ppp ccp type none",
if (!_Put(pCmd2, DK_Number(pCmd2)))
if (!_Put("ppp ipcp msext off"))
if (!_Put("ppp ipcp ipaddress on"))	// ｼｬｼ偵′ADSL縺ｮ險ｭ螳壹ｮ譎ゅｮ縺ｿ迴ｾ繧後ｋ
if (!_Put("url pp filter out 10 21 22 23 24 25 26 27 28"))
const char *pCmd[] = {
"pp backup none",
"pppoe use lan2",
"pppoe auto connect on",
"pppoe auto disconnect on",
"pppoe disconnect time off"
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "ip pp mtu %d", (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nMTU : m_sRouter.nMTU);
if (!_Put(szBuf))
const char *pCmd1[] = {
"ip pp intrusion detection in on reject=on",
"ip pp nat descriptor 2000",
"pp auth accept pap chap",
if (!_Put(pCmd1, DK_Number(pCmd1)))
wsprintf(szBuf, "pp always-on %s", (ADSL_KIND(m_sRouter.port[1].kaisen)== eKaisenFixADSL) ? "on" : "off");
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ip pp secure filter name dk_pp_lan2");
wsprintf(szBuf, "ip pp address %d.%d.%d.%d/32", ip[0], ip[1], ip[2], ip[3]);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ip pp address %d.%d.%d.%d/32",
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass);
if (!_Put(szBuf))				return FALSE;
const char *pCmd2[] = {
"ppp lcp mru on 1454",
"ppp ccp type none",
if (!_Put(pCmd2, DK_Number(pCmd2)))
if (!_Put("url pp filter out 10 21 22 23 24 25 26 27 28"))
wsprintf(szBuf, "isdn remote address arrive %s", m_sRouter.con[n].Tel);
if (!_Put(szBuf))				return FALSE;
const char *pCmd[] = {
"pp auth request chap-pap",
"pp auth username openuser openuser",
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "ip pp remote address 192.168.%d.%d", m_sRouter.ip[2], m_sRouter.ncon[n].type + 47);
if (!_Put(szBuf)) return FALSE;
wsprintf(szBuf, "ip pp remote address 172.16.60.%d", m_sRouter.ncon[n].type + 47);
if (!_Put(szBuf)) return FALSE;
const char *pCmd[] = {
"pp always-on on",
"pp auth request mschap-v2",
"pp auth username openuser openuser",
"ppp ccp type mppe-any",
"ip pp mtu 1280",
"pptp service type server",
if (!_Put(pCmd, DK_Number(pCmd)))
const char *pCmd[] = {
"ip pp nat descriptor 1",
if (!_Put(pCmd, DK_Number(pCmd)))
if (!_Put(pCmd1, DK_Number(pCmd1)))
wsprintf(szBuf, "pp disable %d", m_sRouter.con[n].no);
wsprintf(szBuf, "pp enable %d", m_sRouter.con[n].no);
if (!_Put(szBuf))				return FALSE;
```

## PutTunnel
```cpp
if ( !_Put("\ntunnel disable all") )
wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo);
if (!_PutTit(szBuf))			return FALSE;
wsprintf(szBuf, "tunnel select %d", nSetuzokuNo);
if (!_Put(szBuf))				return FALSE;
if ( !_Put("tunnel encapsulation pptp") )
wsprintf(szBuf, "tunnel endpoint address %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "tunnel enable %d", m_sRouter.con[n].no);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "tunnel encapsulation ipsec");
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo + 100);
if (!_Put(szBuf))				return FALSE;
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
wsprintf(szBuf,"tunnel backup tunnel %d", m_sRouter.m_nJnetNo2);
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf,"ip tunnel nat descriptor 3013");
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf,"ip tunnel tcp mss limit auto");
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf,"ip tunnel secure filter in 9");
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf,"ip tunnel secure filter out 10210 10211 dynamic 10210 10211");
if (!_Put(szBuf))	return FALSE;
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
wsprintf(szBuf,"ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.sSIS05.accessIp));
if (!_Put(szBuf))	return FALSE;
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
wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo);
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo);
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "tunnel backup pp %d", m_sRouter.con[n].no);
if (!_Put(szBuf))
wsprintf(szBuf, "tunnel backup tunnel %d switch-interface=on", m_sRouter.m_nDeltaBkNo);
if (!_Put(szBuf))
wsprintf(szBuf, "tunnel backup pp %d switch-router=on", m_sRouter.ncon[n].no);
if (!_Put(szBuf))
wsprintf(szBuf, "tunnel backup none");
if (!_Put(szBuf))
const char *pCmd[] = {
"ip tunnel rip send off",
"ip tunnel rip receive off"
if (!_Put(pCmd, DK_Number(pCmd)))
if (!_Put("ip tunnel nat descriptor 2012"))
if (!_Put("ip tunnel nat descriptor 1"))
wsprintf(szBuf, "tunnel enable %d", m_sRouter.con[n].no);
if (!_Put(szBuf))				return FALSE;
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
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nIpTelNo);
if (!_Put(szBuf))				return FALSE;
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
const char *pCmd[] = {
"ip tunnel rip send off",
"ip tunnel rip receive off"
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nIpTelNoBk);
if (!_Put(szBuf))				return FALSE;
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
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nCarMentNo);
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nAdslNo);
if (!_PutTit(szBuf))			return FALSE;
wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nAdslNo + 100);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc md5-hmac", m_sRouter.m_nAdslNo + 100, m_sRouter.m_nAdslNo);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ipsec ike pre-shared-key %d text *", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ipsec ike remote address %d any", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))				return FALSE;
wsprintf(szBuf, "ipsec ike remote name %d *", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))				return FALSE;
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
if (!_Put("tunnel backup none"))
const char *pCmd[] = {
"ip tunnel rip send off",
"ip tunnel rip receive off",
if (!_Put(pCmd, DK_Number(pCmd)))
if (!_Put("ip tunnel nat descriptor 1"))
wsprintf(szBuf, "tunnel enable %d",m_sRouter.m_nAdslNo);
if (!_Put(szBuf))				return FALSE;
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
if (!_Put(pCmd, DK_Number(pCmd)))
if (!_Put("ip tunnel nat descriptor 1000"))
wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo);
if (!_Put(szBuf))	return FALSE;
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
wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo);	// (m_sRouter.ncon[i].port == ePortL2) ? "10 6" : "60 10");
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo);
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].ip));
if (!_Put(szBuf))	return FALSE;
const char *pCmd[] = {
"ip tunnel rip send off",
"ip tunnel rip receive off",
"ip tunnel nat descriptor 2012"
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo);
if (!_Put(szBuf))	return FALSE;
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
```

## PutFilter
```cpp
if (!_PutTit(tit))
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
if (!_Put(pCmd01, DK_Number(pCmd01)))
wsprintf(szPut, "ip filter 10 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 15 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szIp, "%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip));
wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.mask));
wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
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
wsprintf(szPut, "ip filter 29 pass * %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
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
if (!_Put(pCmd02, DK_Number(pCmd02)))
wsprintf(szPut, "ip filter 80 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 81 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 82 pass %s 172.16.15.0/24 tcp www,https *", IP_MG_WEB_IP);
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter 83 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *"))
if (!_Put("ip filter 84 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *"))
wsprintf(szPut, "ip filter 85 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 86 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 87 pass 172.16.15.0/24 %s tcp * www,https", IP_MG_WEB_IP);
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter 88 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https"))
if (!_Put("ip filter 89 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https"))
wsprintf(szPut, "ip filter 90 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 95 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 91 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 96 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
const char *pCmd02_1[] = {
"ip filter 92 pass * 172.16.5.0/24 tcp https,5222,5223 *",
"ip filter 93 pass * 172.16.5.0/24 udp domain,ntp *",
"ip filter 94 pass * 172.16.5.0/24 icmp",
"ip filter 97 pass 172.16.5.0/24 * tcp * https,5222,5223",
"ip filter 98 pass 172.16.5.0/24 * udp * domain,ntp",
"ip filter 99 pass 172.16.5.0/24 * icmp",
if (!_Put(pCmd02_1, DK_Number(pCmd02_1)))
const char *pCmd02_2[] = {
"ip filter 100 pass-log * * * * *",
"ip filter 101 reject * * established * netbios_ns-netbios_ssn",
"ip filter 102 reject * * established * 135",
"ip filter 103 reject * * established * 445",
"ip filter 104 reject * * established * https",
"ip filter 105 reject * * established * www",
"ip filter 106 reject * * established * pop3",
"ip filter 107 reject * * established * smtp",
if (!_Put(pCmd02_2, DK_Number(pCmd02_2)))
if ( !_Put("ip filter 200 pass 172.21.0.0/16 * * * *") )
wsprintf(szPut, "ip filter 1400 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))
if (!_Put("ip filter 1401 pass 172.21.0.0/16 * * * *"))
wsprintf(szPut, "ip filter 1405 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))
if (!_Put("ip filter 1406 pass * 172.21.0.0/16 * * *"))
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
if (!_Put(pCmd03, DK_Number(pCmd03)))
const char *pCmd03_1[] = {
"ip filter 1446 pass 172.16.252.1 10.0.192.0/18 tcp 3389 *",
"ip filter 1447 pass 10.0.192.0/18 172.16.252.1 tcp * 3389"
if (!_Put(pCmd03_1, DK_Number(pCmd03_1)))
wsprintf(szPut, "ip filter 2010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2025 pass 172.16.0.0/16 %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2026 pass * %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2027 pass 172.16.15.0/24 %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2028 pass 172.16.60.0/24 %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2029 pass * %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
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
if (!_Put(pCmd04, DK_Number(pCmd04)))
wsprintf(szPut, "ip filter 2080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2082 pass %s 172.16.15.0/24 tcp www,https *", IP_MG_WEB_IP);
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter 2083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *"))
if (!_Put("ip filter 2084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *"))
wsprintf(szPut, "ip filter 2085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2087 pass 172.16.15.0/24 %s tcp * www,https", IP_MG_WEB_IP);
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter 2088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https"))
if (!_Put("ip filter 2089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https"))
wsprintf(szPut, "ip filter 2090 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2095 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2091 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 2096 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
const char *pCmd04_1[] = {
"ip filter 2092 pass * 172.16.5.0/24 tcp https,5222,5223 *",
"ip filter 2093 pass * 172.16.5.0/24 udp domain,ntp *",
"ip filter 2094 pass * 172.16.5.0/24 icmp",
"ip filter 2097 pass 172.16.5.0/24 * tcp * https,5222,5223",
"ip filter 2098 pass 172.16.5.0/24 * udp * domain,ntp",
"ip filter 2099 pass 172.16.5.0/24 * icmp",
if (!_Put(pCmd04_1, DK_Number(pCmd04_1)))
wsprintf(szPut,"ip filter 2450 pass %s 61.114.186.0/25 tcp * https", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2451 pass 61.114.186.0/25 %s tcp https *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3450 pass %s 61.114.186.0/25 tcp * https", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3451 pass 61.114.186.0/25 %s tcp https *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2444 pass %s * tcp * www,https", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 2445 pass * %s tcp www,https *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3025 pass 172.16.0.0/16 %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3026 pass 172.16.5.0/24 %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3027 pass 172.16.15.0/24 %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3028 pass 172.16.60.0/24 %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3029 pass * %s,%s udp * domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
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
if (!_Put(pCmd05, DK_Number(pCmd05)))
wsprintf(szPut, "ip filter 3080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3082 pass %s 172.16.15.0/24 tcp www,https *", IP_MG_WEB_IP);
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter 3083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *"))
if (!_Put("ip filter 3084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *"))
wsprintf(szPut, "ip filter 3085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3087 pass 172.16.15.0/24 %s tcp * www,https", IP_MG_WEB_IP);
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter 3088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https"))
if (!_Put("ip filter 3089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https"))
wsprintf(szPut, "ip filter 3090 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3095 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3091 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3096 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
const char *pCmd04_2[] = {
"ip filter 3092 pass * 172.16.5.0/24 tcp https,5222,5223 *",
"ip filter 3093 pass * 172.16.5.0/24 udp domain,ntp *",
"ip filter 3094 pass * 172.16.5.0/24 icmp",
"ip filter 3097 pass 172.16.5.0/24 * tcp * https,5222,5223",
"ip filter 3098 pass 172.16.5.0/24 * udp * domain,ntp",
"ip filter 3099 pass 172.16.5.0/24 * icmp",
if (!_Put(pCmd04_2, DK_Number(pCmd04_2)))
wsprintf(szPut,"ip filter 3420 pass-log %s %s icmp * *",
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3421 pass-log %s 172.16.60.0/24 icmp * *",
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3422 pass-log %s 172.16.0.0/16 icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3427 pass-log 172.16.0.0/16 %s icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3425 pass-log %s %s icmp * *",
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3426 pass-log 172.16.60.0/24 %s icmp * *",
if (!_Put(szPut))	return FALSE;
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
wsprintf(szPut,"ip filter 3500 pass-log %s 172.16.60.0/24 tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3501 pass-log %s 172.16.0.0/16 tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3506 pass-log 172.16.0.0/16 %s tcp,udp netbios_ns-netbios_ssn * ", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3521 pass-log %s 172.16.0.0/16 tcp,udp * 445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3526 pass-log 172.16.0.0/16 %s tcp,udp 445 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
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
wsprintf(szPut, "ip filter 3561 pass-log %s 172.16.0.0/16 tcp,udp netbios_ns-netbios_ssn *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3566 pass-log 172.16.0.0/16 %s tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3581 pass-log %s 172.16.0.0/16 tcp,udp 445 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 3586 pass-log 172.16.0.0/16 %s tcp,udp * 445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
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
wsprintf(szPut,"ip filter 3800 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo, GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 3805 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].mask));
wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo, GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf( szPut, "ip filter 10000 reject %s * * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
if (!_Put(szPut))	return FALSE;
wsprintf( szPut, "ip filter 10005 reject * %s * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
if (!_Put(szPut))	return FALSE;
wsprintf(szMainRt,	"%s", GetIpB2C(m_sRouter.ptc_ip));				// 繝｡繧､繝ｳ繝ｫ繝ｼ繧ｿ
wsprintf(szBackUpRt, "%s", GetIpB2C_Second(m_sRouter.center_ip));	// 繝舌ャ繧ｯ繧｢繝繝励Ν繝ｼ繧ｿ
wsprintf(szLan2Fix,	"%s", GetIpB2C_Second(m_sRouter.sLte.ip));		// LAN縺ｮ蝗ｺ螳唔P繧｢繝峨Ξ繧ｹ
wsprintf(szLan2Fix,	"%s", GetIpB2C_Second(m_sRouter.ncon[n].fixIp));// LAN縺ｮ蝗ｺ螳唔P繧｢繝峨Ξ繧ｹ
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
if (!_Put(pCmd05_1, DK_Number(pCmd05_1)))
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
if (!_Put(pCmdChatBot, DK_Number(pCmdChatBot)))
if ( !PutFmt("ip filter 10500 pass * %s tcp * %d", GetIpB2C(m_sRouter.ip), (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nLuaPort : m_sRouter.nLuaPort) )	return FALSE;
if ( !PutFmt("ip filter 10505 pass %s * tcp %d", GetIpB2C(m_sRouter.ip), (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nLuaPort : m_sRouter.nLuaPort) )	return FALSE;
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
wsprintf(szPut, "ip filter 12600 pass %s 172.16.5.0/24 udp * 500", strSptFilter);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 12650 pass 172.16.5.0/24 %s udp * 500", strSptFilter);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 12700 pass %s 172.16.5.0/24 esp * *", strSptFilter);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 12750 pass 172.16.5.0/24 %s esp * *", strSptFilter);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 25 172.16.0.0/16 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 26 172.16.5.0/24 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 27 172.16.15.0/24 %s,%s domain",szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 28 172.16.60.0/24 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 29 * %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
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
if (!_Put(pCmd06, DK_Number(pCmd06)))
wsprintf(szPut, "ip filter dynamic 85 %s * filter 85 in 80", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 86 %s * filter 86 in 81", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter dynamic 87 172.16.15.0/24 * filter 87 in 82"))	return FALSE;
if (!_Put("ip filter dynamic 88 172.16.0.0/16 * filter 88 in 83"))	return FALSE;
if (!_Put("ip filter dynamic 89 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 89 in 84"))	return FALSE;
wsprintf(szPut, "ip filter dynamic 95 %s * filter 95 in 90", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 96 %s * filter 96 in 91", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
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
if (!_Put(pCmd07_0, DK_Number(pCmd07_0)))
wsprintf(szPut,"ip filter dynamic 2025 172.16.0.0/16 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 2026 * %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 2027 172.16.15.0/24 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 2028 172.16.60.0/24 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 2029 * %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
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
if (!_Put(pCmd07, DK_Number(pCmd07)))
wsprintf(szPut, "ip filter dynamic 2085 %s * filter 2085 in 2080", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 2086 %s * filter 2086 in 2081", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter dynamic 2087 172.16.15.0/24 * filter 2087 in 2082"))	return FALSE;
if (!_Put("ip filter dynamic 2088 172.16.0.0/16 * filter 2088 in 2083"))	return FALSE;
if (!_Put("ip filter dynamic 2089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 2089 in 2084"))	return FALSE;
wsprintf(szPut, "ip filter dynamic 2095 %s * filter 2095 in 2090", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 2096 %s * filter 2096 in 2091", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 2444 %s * filter 2444 in 2445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 3444 %s * filter 3444 out 3445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 2450 %s 61.114.186.0/25 filter 2450 in 2451", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3450 %s 61.114.186.0/25 filter 3450 out 3451", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
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
wsprintf(szPut,"ip filter dynamic 3025 172.16.0.0/16 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3026 172.16.5.0/24 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3027 172.16.15.0/24 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3028 172.16.60.0/24 %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3029 * %s,%s domain", szIp, szIp2);
if (!_Put(szPut))	return FALSE;
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
if (!_Put(pCmd08, DK_Number(pCmd08)))
wsprintf(szPut, "ip filter dynamic 3085 %s * filter 3085 in 3080", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 3086 %s * filter 3086 in 3081", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter dynamic 3087 172.16.15.0/24 * filter 3087 in 3082"))	return FALSE;
if (!_Put("ip filter dynamic 3088 172.16.0.0/16 * filter 3088 in 3083"))	return FALSE;
if (!_Put("ip filter dynamic 3089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 3089 in 3084"))	return FALSE;
wsprintf(szPut, "ip filter dynamic 3095 %s * filter 3095 in 3090", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter dynamic 3096 %s * filter 3096 in 3091", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3530 %s 172.16.60.0/24 ftp", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3550 %s 172.16.60.0/24 filter 3550 in 3555", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 3590 %s 172.16.60.0/24 filter 3590 in 3595", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
if (!_Put("ip filter dynamic 10210 172.16.21.1 192.168.210.3 ftp"))
if (!_Put("ip filter dynamic 10211 172.16.21.1 192.168.210.4 ftp"))
wsprintf(szPut,"ip filter dynamic 10665 * %s filter 10665 in 10660 10670 out 10675", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
if (!_Put(szPut))	return FALSE;
const char *pCmd01[] = {
"ip filter dynamic 10615 172.16.60.0/24 172.16.60.0/24 filter 10615 in 10610 10620 out 10625",
"ip filter set xai_tunnel_lan2 out 10605 10615 dynamic 10615",
if (!_Put(pCmd01, DK_Number(pCmd01)))
if (!_Put(strFilter))
if (!_Put(strFilter))
if (!_Put("ip filter set dk_pp_lan2 in 1 2 3 4 5 6 2010 10800 12600 12700", FALSE))	// 12600 12700ｼ昴け繝ｩ繧ｦ繝峨Μ繝九Η繝ｼ繧｢繝ｫ譎ょｯｾ蠢(繧ｵ繝晉ｫｯ譛ｫ謗･邯壹ヵ繧｣繝ｫ繧ｿ霑ｽ蜉)
if (!_Put((CStoS)strPut, FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put(" 10000 10010 10020 10030", FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put(" 10500 10510 10520 10550 10560", FALSE))
if (!_Put(""))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put(" 10005 10015 10025 10035", FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put(" 10505 10525 10575", FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put(""))
if (!_Put((CStoS)strPut))
if (!_Put("ip filter set dk_lan3_call out 1401 1406"))
if (!_Put("ip filter set dk_lan3 in 3 4 3010 3420 3421", FALSE))
if (!_Put(" 104 105 3442 3444 3446 3448 3422 3501 3521 3561 3581", FALSE))
wsprintf(szPut, "%s%s%s%s%s%s%s%s",
if (!_Put(szPut, FALSE))
if (!_Put(" 3801 3802 3803 3804", FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put(" 10650 10660 10670", FALSE))
wsprintf(szPut, " dynamic%s%s%s%s%s",
if (!_Put(szPut, FALSE))
if (!_Put(" dynamic 3444", FALSE))
if (!_Put(""))
if (!_Put("ip filter set dk_lan3 out 3 4 104 105 3015", FALSE))
if (!_Put(" 3026 3427 3442 3443 3447 3449 3506 3526 3566 3586", FALSE))
wsprintf(szPut, "%s%s%s%s%s",
if (!_Put(szPut, FALSE))
if (!_Put(" 3425 3426", FALSE))
wsprintf(szPut, "%s%s%s%s",
if (!_Put(szPut, FALSE))
if (!_Put(" 3806 3807 3808 3809", FALSE))
if (!_Put((CStoS)strPut, FALSE))
if (!_Put(" 3451 10655 10665 10675", FALSE))	// "3451"霑ｽ蜉縲2021蟷ｴ遘狗沿蟇ｾ蠢
wsprintf(szPut, " dynamic%s%s%s%s%s%s%s",
if (!_Put(szPut, FALSE))
if (!_Put(" dynamic 3026", FALSE))
if (!_Put(""))
if (!_Put("ip filter set dk_pp_lab in 9"))
if ( !_Put("ip filter set dk_pp_lab out 1 2 3 4 5 6 104 105 15", FALSE) )
if (!_Put((CStoS)strPut, FALSE))
if ( !_Put("") )
const char *pCmd13[] = {
"ip filter set getlog in 1 2 3 4 5 6 100",
"ip filter set getlog out 1 2 3 4 5 6 100"
if (!_Put(pCmd13, DK_Number(pCmd13)))
if (!_Put("# URL filter configuration"))
const char *pCmd14[] = {
"url filter 1 pass download.windowsupdate.com",
"url filter 2 pass update.microsoft.com",
"url filter 3 pass windowsupdate.com",
"url filter 4 pass download.microsoft.com",
"url filter 5 pass ntservicepack.microsoft.com",
"url filter 6 pass wustat.windows.com",
"url filter 7 pass trendmicro.co.jp",
"url filter 8 pass trendmicro.co.com",
if (!_Put(pCmd14, DK_Number(pCmd14)))
wsprintf(szPut, "url filter 9 reject * *");
if ( !_Put(szPut) )
wsprintf(szPut, "url filter 10 pass * %s", GetIpB2C(bMaskedIP, m_sRouter.mask));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 21 pass download.windowsupdate.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 22 pass update.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 23 pass windowsupdate.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 24 pass download.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 25 pass ntservicepack.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 26 pass wustat.windows.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 27 pass trendmicro.co.jp %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 28 pass trendmicro.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szPut) )
wsprintf(szPut, "url filter 100 pass * *");
if (!_Put(szPut) )
```

## PutFilterOA_LAN
```cpp
if (!_PutTit(tit))
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
if (!_Put(pCmd01, DK_Number(pCmd01)))
wsprintf(szPut, "ip filter 5010 reject 192.168.%d.0/24 * * * *", nRouterIP3);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut, "ip filter 5015 reject * 192.168.%d.0/24 * * *", nRouterIP3);
if (!_Put(szPut))	return FALSE;
wsprintf(szIp, "%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip));
wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.mask));
wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
wsprintf(szPut,"ip filter 5025 pass 192.168.%d.0/24 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 5026 pass 192.168.%d.1 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 5028 pass 192.168.%d.2-192.168.%d.254 %s,%s udp * domain", nRouterIP3, nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
if (!_Put((CStoS)str5030[i])) return FALSE;
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
wsprintf(szPut,"ip filter 8025 pass 192.168.%d.0/24 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 8026 pass 192.168.%d.1 %s,%s udp * domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 8028 pass 192.168.%d.2-192.168.%d.254 %s,%s udp * domain", nRouterIP3, nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
if (!_Put((CStoS)str8030[i])) return FALSE;
wsprintf(szPut,"ip filter 8420 pass-log %s %s icmp * *",
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter 8425 pass-log %s %s icmp * *",
if (!_Put(szPut))	return FALSE;
if (!_Put((CStoS)str8420[i])) return FALSE;
wsprintf(szPut,"ip filter dynamic 5025 192.168.%d.0/24 %s,%s domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 5026 192.168.%d.1 %s,%s domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 5028 192.168.%d.2-192.168.%d.254 %s,%s domain", nRouterIP3, nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
if (!_Put((CStoS)str5035[i])) return FALSE;
if (!_Put((CStoS)str5055[i])) return FALSE;
wsprintf(szPut,"ip filter dynamic 8025 192.168.%d.0/24 %s,%s domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 8026 192.168.%d.1 %s,%s domain", nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 8028 192.168.%d.2-192.168.%d.254 %s,%s domain", nRouterIP3, nRouterIP3, szIp, szIp2);
if (!_Put(szPut))	return FALSE;
if (!_Put((CStoS)str8035[i])) return FALSE;
wsprintf(szPut,"ip filter dynamic 8530 %s 192.168.%d.2-192.168.%d.254 ftp", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), nRouterIP3, nRouterIP3);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 8550 %s 192.168.%d.2-192.168.%d.254 filter 8550 in 8555", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask),nRouterIP3, nRouterIP3);
if (!_Put(szPut))	return FALSE;
wsprintf(szPut,"ip filter dynamic 8590 %s 192.168.%d.2-192.168.%d.254 filter 8590 in 8595", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask),nRouterIP3, nRouterIP3);
if (!_Put(szPut))	return FALSE;
const char *pCmd02[] = {
"ip filter set oa_lan1 in 6400 6410",
"ip filter set oa_lan1 out 6405"
if (!_Put(pCmd02, DK_Number(pCmd02)))
if (!_Put("ip filter set oa_lan3 in 3 4 8010 8420 8421", FALSE))
wsprintf(szPut, "%s%s%s%s%s%s%s",
if (!_Put(szPut, FALSE))	return FALSE;
wsprintf(szPut," dynamic%s%s%s",
if (!_Put(szPut, FALSE))	return FALSE;
if (!_Put(""))
wsprintf(szPut,"ip filter set oa_lan3 out 3 4 104 105 8015%s%s%s%s 8425 8426%s%s%s%s",
if (!_Put(szPut, FALSE))	return FALSE;
wsprintf(szPut," dynamic%s%s%s%s",
if (!_Put(szPut, FALSE))	return FALSE;
if (!_Put(""))
if (!_Put("ip filter set oa_pp_lab in 9"))
wsprintf(szPut, "ip filter set oa_pp_lab out 1 2 3 4 5 6 104 105 5015%s%s%s%s",
if (!_Put(szPut, FALSE))	return FALSE;
wsprintf(szPut," dynamic%s%s%s%s",
if (!_Put(szPut, FALSE))	return FALSE;
if (!_Put(""))
const char *pCmd03[] = {
"ip filter set getlog in 1 2 3 4 5 6 100",
"ip filter set getlog out 1 2 3 4 5 6 100",
if (!_Put(pCmd03, DK_Number(pCmd03)))
```

## PutIpRoute
```cpp
if (!_Put(""))
if (!_Put("ip routing process normal"))
if (!_Put("ip route 172.18.0.0/16 gateway 172.16.5.11"))
str.Format("ip route 10.142.65.0/24 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
str.Format("ip route 10.142.165.0/24 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
str.Format("ip route 10.145.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
str.Format("ip route 10.141.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str;
if (!_Put((CStoS)strPut))
wsprintf(szBuf, "ip route default gateway pp %d", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", pCmd[nBit], nLaboNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route default gateway pp %d metric 2", nLaboNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", KEIHIN_MOLE, nLaboNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", pCmd[nBit], nLaboNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route default gateway pp %d metric 2", nLaboNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", KEIHIN_MOLE, nLaboNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d", pCmd[nBit], m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d", KEIHIN_MOLE, m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d weight 999999999 hide gateway pp %d weight 1",  pCmd[nBit], m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo);
if (!_Put(szBuf))
wsprintf(szBuf,"ip route %s gateway pp %d metric 2", pCmd[nBit], m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", KEIHIN_MOLE, m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf,"ip route %s gateway %s weight 999999999 hide gateway pp %d weight 1",
if (!_Put(szBuf))
wsprintf(szBuf,"ip route %s gateway %s", pCmd[nBit], GetIpB2C(m_sRouter.sLAN3.ipGateWay));
if (!_Put(szBuf))
wsprintf(szBuf, "ip route default gateway %s", GetIpB2C(m_sRouter.sLAN3.ipGateWay));
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway %s", KEIHIN_MOLE, GetIpB2C(m_sRouter.sLAN3.ipGateWay));
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d  keepalive %d gateway tunnel %d weight 0", m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNoBk);
wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d", m_sRouter.m_nIpTelNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.m_ptcSvIp), m_sRouter.m_nIpTelNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip keepalive %d icmp-echo 60 3 %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.m_ptcSvIp));
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 210.227.65.146 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 122.103.88.162 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d", m_sRouter.m_nIpTelNoBk);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 122.103.88.162 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 10.0.240.0/24 gateway tunnel %d", m_sRouter.m_nCarMentNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway %s %d", SIS_IP,
if (!_Put(szBuf))	// VPN縺ｮ謗･邯壼医′驕ｸ謚槭＆繧後※縺繧句ｴ蜷医′縺ゅｋ縺ｮ縺ｧ縺薙％縺ｧ荳譌ｦ譖ｸ縺崎ｾｼ繧薙〒縺翫￥
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", ROBO_IP, m_sRouter.con[n].no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.con[n].ip), m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway tunnel %d", SIS05_IP, m_sRouter.sSIS05.no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.sSIS05.accessIp), m_sRouter.m_nAdslNo);
if (!_Put(szBuf))	// VPN縺ｮ謗･邯壼医′驕ｸ謚槭＆繧後※縺繧句ｴ蜷医′縺ゅｋ縺ｮ縺ｧ縺薙％縺ｧ荳譌ｦ譖ｸ縺崎ｾｼ繧薙〒縺翫￥
wsprintf(szBuf, "ip route %s gateway tunnel %d", VUP_IP, m_sRouter.sSIS05.no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", SIS05_IP, m_sRouter.sSIS05.no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2", VUP_IP, m_sRouter.sSIS05.no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway tunnel %d",GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway tunnel %d",GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway %s", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway %s", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 210.227.65.146 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 122.103.88.162 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 192.168.210.3 gateway tunnel %d", m_sRouter.m_nJnetNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 192.168.210.4 gateway tunnel %d", m_sRouter.m_nJnetNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 26.26.26.26 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway pp %d metric 2",
if (!_Put(szBuf))
wsprintf(szBuf, "ip route %s gateway %s",
if ( !_Put(szBuf) )
wsprintf(szBuf, "ip route %d.%d.%d.%d gateway pp anonymous metric 2", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], nIp);
if (!_Put(szBuf))
if ( !_Put("ip icmp parameter-problem send off") )
const char *pCmd[] = {
"ip route 10.0.192.0/20 gateway pp anonymous metric 2",	// 繝ｪ繝｢繝ｼ繝医Γ繝ｳ繝繝翫Φ繧ｹ譎ゅｯ縲∵悽繝阪ャ繝医Ρ繝ｼ繧ｯ繧｢繝峨Ξ繧ｹ縺ｧ謗･邯壹☆繧九
"ip route 10.0.208.0/20 gateway pp anonymous metric 2"
if ( !_Put(pCmd, DK_Number(pCmd)) )
if ( !_Put("ip route 10.0.192.0/20 gateway pp anonymous metric 2") )
wsprintf(szBuf, "ip route 10.0.208.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
if ( !_Put(szBuf) )
wsprintf(szBuf, "ip route 10.0.224.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
if ( !_Put(szBuf) )
wsprintf(szBuf, "ip route %s gateway pp %d", m_szRMAdd, m_sRouter.m_nAdslNo);
if ( !_Put(szBuf) )
wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
if (!_Put("ip route 10.0.192.0/24 gateway pp anonymous metric 2"))
wsprintf(szBuf, "ip route 10.0.208.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
wsprintf(szBuf, "ip route 10.0.224.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
const char *pCmd[] = {
"ip route 10.0.192.0/24 gateway pp anonymous metric 2",	// 繝ｪ繝｢繝ｼ繝医Γ繝ｳ繝繝翫Φ繧ｹ譎ゅｯ縲∵悽繝阪ャ繝医Ρ繝ｼ繧ｯ繧｢繝峨Ξ繧ｹ縺ｧ謗･邯壹☆繧九
"ip route 10.0.208.0/24 gateway pp anonymous metric 2"
if ( !_Put(pCmd, DK_Number(pCmd)) )
const char *pCmd[] = {
"ip route 10.0.192.0/20 gateway pp anonymous metric 2",	// 繝ｪ繝｢繝ｼ繝医Γ繝ｳ繝繝翫Φ繧ｹ譎ゅｯ縲∵悽繝阪ャ繝医Ρ繝ｼ繧ｯ繧｢繝峨Ξ繧ｹ縺ｧ謗･邯壹☆繧九
if ( !_Put(pCmd, DK_Number(pCmd)) )
wsprintf(szBuf, "ip route 10.0.208.0/20 gateway tunnel %d", abs(m_sRouter.fMent));
if ( !_Put(szBuf) )
wsprintf(szBuf, "ip route 10.0.224.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
if ( !_Put(szBuf) )
wsprintf(szBuf, "ip route %s gateway pp %d", m_szRMAdd, m_sRouter.m_nAdslNo);
if ( !_Put(szBuf) )
if ( !_Put("ip icmp parameter-problem send off") )
if ( !_Put("ip tos supersede 1 normal precedence=0 200") )
wsprintf(szBuf, "ip icmp echo-reply send-only-linkup on");
if (!_Put(szBuf))
```

## PutNatOther
```cpp
if (!_PutTit(tit))
if (!_Put("nat descriptor log on"))
if (!_Put("nat descriptor masquerade ttl hold all"))
if ( !_Put("nat descriptor type 5000 masquerade") )
if ( !_Put("nat descriptor masquerade incoming 5000 discard") )
wsprintf(szBuf, "nat descriptor type 8000 masquerade");
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor address outer 8000 %s", GetIpB2C(m_sRouter.sLAN3.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor address inner 8000 %d.%d.%d.%d-%d.%d.%d.%d",
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor static 8000 60 %s=%d.%d.%d.%d %d",
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 3 %s tcp 1723", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 4 %s gre", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
const char *pCmd[] = {
"nat descriptor type 3 masquerade",
"nat descriptor masquerade incoming 3 discard"
if ( !_Put(pCmd,DK_Number(pCmd)) )
if (!_Put("nat descriptor type 4 nat"))
if (!_Put("nat descriptor type 1000 nat"))
if (!_Put("nat descriptor address outer 1000 172.16.252.1"))
wsprintf(szBuf, "nat descriptor address inner 1000 %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor static 1000 252 172.16.252.1=%s 1", GetIpB2C(m_sRouter.sLAN3.ipVirus));
if (!_Put(szBuf))
if (!_Put("nat descriptor type 2000 masquerade"))
wsprintf(szBuf, "nat descriptor address outer 2000 %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 1 %s udp 500", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 2 %s esp", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 6 %s udp 4500", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 7 %s tcp %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort);
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor type 3000 masquerade");
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor address outer 3000 %s", GetIpB2C(m_sRouter.sLAN3.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor address inner 3000 %d.%d.%d.%d-%d.%d.%d.%d",
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor static 3000 60 %s=172.16.60.1 %d",
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 3000 1 %s udp 500", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 3000 2 %s esp", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 3000 3 %s udp domain", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 3000 4 %s tcp www", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 3000 5 %s tcp http", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor type 2012 masquerade");
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor address outer 2012 %s", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor address inner 2012 auto");
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 2 %s=%s 1", GetIpB2C(bVPNadd), GetIpB2C_Second(m_sRouter.ip));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 3 %s=172.16.60.1 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 4 %s=172.16.1.1 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 5 %s=172.16.21.1 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 6 %s=172.16.80.1 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 7 %s=172.16.15.%d 1", GetIpB2C(bVPNadd), nMtGokiNo);
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 8 %s=172.16.52.8 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 9 %s=172.16.50.2 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 10 %s=172.16.20.1 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 11 %s=172.16.20.2 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 12 %s=172.16.110.1 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 13 %s=172.16.110.2 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 14 %s=172.16.122.1 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
wsprintf(szBuf, "nat descriptor static 2012 15 %s=172.16.15.100 1", GetIpB2C(bVPNadd));
if (!_Put(szBuf))	return FALSE;
if (!_Put("nat descriptor type 12 masquerade"))
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
wsprintf(szBuf, "nat descriptor masquerade static 2000 3 %s tcp 1723", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 4 %s gre", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor masquerade static 2000 5 %s udp 4500", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor sip 2000 off");
if (!_Put(szBuf))
```

## その他の関数での出力処理
### Analize
```cpp
const char *szCmd01[] = {	// RTX1000繝ｫ繝ｼ繧ｿ蟆ら畑
"pp ",	"ip ",	"isdn ",	"nat ",	"dns ",	"analog ", "pppoe ", "tunnel", "ipsec", "dhcp"
```

### ProcPP
```cpp
const char *szCmd01[] = {
"auth",		"line",		"select",	"enable",	"always-on", "disable"
```

### ProcIP
```cpp
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
{
if (p && strncmp(address, p, lstrlen(address)) == 0 )
{
if (m_sRouter.kind == eRTA54i && m_sRouter.kind == eRTA55i )
{
return eSyntaxErr;	// "lan2 address"縺悟ｭ伜惠縺励◆譎らせ縺ｧ繧｢繧ｦ繝
}
else
{
p = strtok(NULL, SEARCH_CHR);
if ( p && strncmp("dhcp", p, lstrlen("dhcp")) == 0 )
{
m_sRouter.fUseLan2DHCP = TRUE;
}
else
{
SetIpC2B(p, m_sWkHikari.fixIp, m_sWkHikari.yobi1);	// 蝗ｺ螳唔P繧｢繝峨Ξ繧ｹ縺ｨ繝阪ャ繝医槭せ繧ｯ 險俶ｶ縺励※縺翫￥
}
}
}
}
break;
case 2:		// lan3
{
if (p && strncmp(address, p, lstrlen(address)) == 0 ) 	// "lan3 address"縺悟ｭ伜惠縺吶ｋｼ晏句挨LAN or ｼｩｼｰ髮ｻ隧ｱ縺ゅｊ
{
p = strtok(NULL, SEARCH_CHR);
if ( (m_sRouter.m_nIpTelNo > 0 || m_sRouter.m_nIpTelNoBk > 0) && m_sRouter.fSetIpTelFlg != eIpTelLAN1 )	// IP髮ｻ隧ｱ
{
SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);			// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
m_sRouter.port[2].kaisen = eKaisenIP_TEL;				// L3繝昴ｼ繝医↓IP髮ｻ隧ｱ繧偵そ繝繝
m_sRouter.fSetIpTelFlg = eIpTelLAN3;					// IP髮ｻ隧ｱ縺ｯｼｬｼ｡ｼｮｼ薙→縺吶ｋ
}
else	// 蛟句挨LAN
{
if ( m_sRouter.m_nFqdnState != eFqdnLAN3 )	// 繧ｯ繝ｩ繧ｦ繝迂P髮ｻ隧ｱ(LAN3)縺ｮ縺ｿ縺梧怏蜉ｹ縺ｮ蝣ｴ蜷医↓蛟句挨LAN縺瑚ｨｭ螳壹〒縺阪↑縺
{
SetIpC2B(p, m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask);	// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
m_sRouter.port[2].kaisen = eKaisenLAN;					// L3繝昴ｼ繝医↓蛟句挨LAN繧偵そ繝繝
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
m_sRouter.fSetIpTelFlg = eIpTelLAN1;	// ｼｩｼｰ髮ｻ隧ｱ縺鯉ｼｬｼ｡ｼｮｼ代〒險ｭ螳壹＆繧後※縺繧九％縺ｨ縺檎｢ｺ螳
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
p = strtok(NULL, SEARCH_CHR);
if (p && strncmp(address, p, lstrlen(address)) != 0)
break;
p = strtok(NULL, SEARCH_CHR);
ConvertIpDelmit(p, bIP);
m_sRouter.ncon[m_nCurrentPP].type = bIP[3] - 47;			// PHS逡ｪ蜿ｷ
} else if ( p && strncmp(address, p, lstrlen(address)) == 0 )	// ADSL蝗ｺ螳唔P縺ｮ逍代＞
{
p = strtok(NULL, SEARCH_CHR);	// 蝗ｺ螳唔P繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
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
m_sRouter.ncon[m_nCurrentPP].type = eADSL;	// ADSL縺勲IRAIGATE髢ｲ隕ｧBB縺ｨ險ｭ螳壹＠縺ｦ縺ゅｋ蜿ｯ閭ｽ諤ｧ譛
if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
{
m_sRouter.fBbUse = eLaboBB_Adsl;
}
} else if ( lstrcmp(p, "dk_pp_lan2") == 0 ) {	// ADSL蝗ｺ螳壹′MIRAIGATE髢ｲ隕ｧBB縺ｨ險ｭ螳壹＠縺ｦ縺ゅｋ蜿ｯ閭ｽ諤ｧ譛
m_sRouter.ncon[m_nCurrentPP].type = eFixADSL;
if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
{
m_sRouter.fBbUse = eLaboBB_FixAdsl;
}
}
} else {	// Labo-INS謗･邯壼医□縺ｨ諤昴≧
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
m_sRouter.ncon[m_nCurrentPP].type = eADSL;	// ADSL縺勲IRAIGATE髢ｲ隕ｧBB縺ｨ險ｭ螳壹＠縺ｦ縺ゅｋ蜿ｯ閭ｽ諤ｧ譛
if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
{
m_sRouter.fBbUse = eLaboBB_Adsl;
}
}
}
}
else if ( p && strncmp("mtu", p, lstrlen("mtu")) == 0 && (m_nAdslProc == m_nCurrentPP+1) )
{
p = strtok(NULL, SEARCH_CHR);	// MTU蛟､
m_sRouter.nMTU = atoi(p);
}
break;
case 5:		//	route
{
char	szIpMask[32];
strncpy0(szIpMask, p, sizeof(szIpMask));			//	IP繧｢繝峨Ξ繧ｹ遲臥ｭ
char	*gateway = "gateway",	//	gateway
*pp		 = "pp";
if ( p && lstrcmp(p, "10.0.241.0/24") == 0 )
{
p = strtok(NULL, SEARCH_CHR);	// gateway
p = strtok(NULL, SEARCH_CHR);	// tunnel
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
m_sRouter.m_nJnetMent = atoi(p);
break;
}
if ( p && strncmp("default", szIpMask, lstrlen("default")) == 0 )  // default
{
p = strtok(NULL, SEARCH_CHR);
if ( p && strncmp(gateway, p, lstrlen(gateway)) == 0 ) 	// Gateway
{
p = strtok(NULL, SEARCH_CHR);
SetIpC2B(p, m_sWkHikari.yobi2);	// Gateway 險俶ｶ縺励※縺翫￥
}
break;
}
p = strtok(NULL, SEARCH_CHR);
if ( p && strncmp(gateway, p, lstrlen(gateway)) == 0 ) 	// Gateway
{
p = strtok(NULL, SEARCH_CHR);
if ( p && isdigit(*p) )	// gateway縺ｮ蠕後ｯIP繧｢繝峨Ξ繧ｹ縺ｧ縺ゅ▲縺
{
if ( !m_fBBsetLAN3 )
{
m_sRouter.fBbUse = eLaboBB_FixAdsl;
}
else
{
if ( lstrcmp(szIpMask, "61.114.182.54"	) == 0	||
lstrcmp(szIpMask, "61.114.182.53"	) == 0	||
lstrcmp(szIpMask, "61.114.186.0/25") == 0	||
lstrcmp(szIpMask, "61.114.177.148"	) == 0	||
strncmp(_default, szIpMask, lstrlen(_default)) == 0 )
{
m_sRouter.fBbUse = eLaboBB_KobetuLan;	// BB謗･邯壹ｯ蛟句挨LAN縺ｧ豎ｺ螳
SetIpC2B(p, m_sRouter.sLAN3.ipGateWay);
m_bL3GateWay = TRUE;
break;
}
if ( strncmp(p, "172.16.", 7) != 0 )	// PV竇｢繝阪ャ繝医Ρ繝ｼ繧ｯ縺ｫ縺､縺縺ｦ縺ｮgateway繧｢繝峨Ξ繧ｹ縺ｯ髯､螟
{
SetIpC2B(p, m_sRouter.sLAN3.ipGateWay);
m_bL3GateWay = TRUE;
}
}
break;
} else if (p && strncmp("tunnel", p, lstrlen("tunnel")) == 0) {	// Tunnel
if ( strcmp(szIpMask, SIS_IP) == 0 )	// SIS謗･邯壼
{
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
m_sRouter.m_nSISNo = atoi(p);
break;
}
if ( strcmp(szIpMask, IP_TEL_IP) == 0 )	// IP髮ｻ隧ｱ縺ｮｼｩｼｰ繧｢繝峨Ξ繧ｹ
{
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
if ( m_sRouter.m_nIpTelNo != atoi(p) )	// m_nIpTelNo縺ｯpre髢｢謨ｰ縺ｧ蜈･繧
{
m_sRouter.m_nIpTelNoBk = atoi(p);
DebDisp("IP髮ｻ隧ｱBK縺ｮtunnel逡ｪ蜿ｷ=%d", m_sRouter.m_nIpTelNoBk);
}
p = strtok(NULL, SEARCH_CHR);
if ( p && strncmp("keepalive", p, lstrlen("keepalive")) == 0 )	// 邯壹″縺後≠繧具ｼ昜ｸ｡譁ｹ繧ｻ繝繝医＠縺ｦ縺ゅｋ
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
if ( strcmp(szIpMask, IP_CAR_MENT_IP) == 0 )	// 霆贋ｸ｡邂｡逅繧ｷ繧ｹ繝繝繝｡繝ｳ繝繝翫Φ繧ｹ逕ｨ險ｭ螳壹ｮｼｩｼｰ
{
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
m_sRouter.m_nCarMentNo = atoi(p);
DebDisp("霆贋ｸ｡邂｡逅繧ｷ繧ｹ繝繝繝｡繝ｳ繝繝翫Φ繧ｹ縺ｮtunnel逡ｪ蜿ｷ=%d", m_sRouter.m_nCarMentNo);
break;
}
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
m_nCurrentPP = atoi(p) - 1;
if ( strcmp(szIpMask, "10.0.208.0/20") == 0 ) // anonymous縺ｧDKVPN髢｢騾｣縺ｮ謗･邯壼医′險ｭ螳壹＠縺ｦ縺ゅｋ
{
if ( atoi(p) == m_nAdslProc ) { // ADSL蝗ｺ螳
m_sRouter.fMent = eMentFixADSL;	// 繝｡繝ｳ繝繝翫Φ繧ｹ險ｭ螳夲ｼｯｼｮ
break;
} else {
m_sRouter.fMent = 0 - atoi(p);	// ADSL蝗ｺ螳壹′驕ｸ謚槭＆繧後※縺縺溘ｏ縺代〒縺ｯ縺ｪ縺
break;
}
}
if ( strcmp(szIpMask, "10.0.224.0/20") == 0 ) // anonymous縺ｧDKVPN髢｢騾｣
{
break;
}
if ( strcmp(szIpMask, SIS05_IP) == 0 ) {
m_sRouter.sSIS05.no = atoi(p);	// pp auth蜿･縺ｮ蜑阪↓蠢縺壹￥繧九ｯ縺
m_fSisRoboVwM = TRUE;
break;
}
if ( strcmp(szIpMask, KEIHIN_MOLE) == 0 )	// 譎ｯ蜩√Δ繝ｼ繝ｫ縺ｯ辟｡隕悶〒縺縺
{
return TRUE;
}
int	nSetuzokuNum = atoi(p);	// 1縲
if ( (nSetuzokuNum-1) < 0 )
{
if ( strcmp(szIpMask, REOMOTE_MENT_IP_AT) == 0 )
{
m_sRouter.fMent = eMentAuto;	// auto遒ｺ螳
}
break;
}
break;
} else if (p && strncmp("pp", p, lstrlen("pp")) == 0) {	// pp
if ( strcmp(szIpMask, SIS_IP) == 0 )
{
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
m_sRouter.m_nSISNo = atoi(p);
break;
}
if ( strcmp(szIpMask, ROBO_IP) == 0 )
{
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
m_sRouter.m_nRoboNo = atoi(p);
SetIpC2B(szIpMask, m_sRouter.con[m_sRouter.m_nRoboNo-1].ip);
break;
}
if ( strcmp(szIpMask, KEIHIN_MOLE) == 0 )	// 譎ｯ蜩√Δ繝ｼ繝ｫ縺ｯ辟｡隕悶〒縺縺
{
return TRUE;
}
if ( strcmp(szIpMask, VUP_IP) == 0 )	// VUPIP縺ｯ辟｡隕悶〒縺縺
{
return TRUE;
}
if ( strcmp(szIpMask, "210.227.65.146") == 0 || strcmp(szIpMask, "122.103.88.162") == 0 )	// J-NET IP縺ｯ辟｡隕悶〒縺縺
{
return TRUE;
}
int	nSetuzokuNum = atoi(p);	// 1縲
if ( (nSetuzokuNum-1) < 0 )
{
if ( strcmp(szIpMask, REOMOTE_MENT_IP_AT) == 0 )
{
m_sRouter.fMent = eMentAuto;	// auto遒ｺ螳
break;
}
}
}
} else if (p && strncmp(pp, p, lstrlen(pp)) != 0) {
break;
} else if (!p) {
break;
}
p = strtok(NULL, SEARCH_CHR);	// 謗･邯壼育分蜿ｷ
if ( p == NULL )
break;
if ( p && strncmp("anonymous", p, lstrlen("anonymous")) == 0 )	// 繧｢繝弱ル繝槭せ螳夂ｾｩ
{	// OA縺ｯ繝｡繝ｳ繝繝輔Λ繧ｰ蟶ｸ縺ｫ繧ｪ繝
if ( !(GetRtCategory(m_sRouter.kind) == eRtType1000_OA) && m_sRouter.fMent == eMentNoUse )
m_sRouter.fMent = /*m_sRouter.fMent ? m_sRouter.fMent :*/ eMentAnonymous;	// 繝｡繝ｳ繝繝翫Φ繧ｹ險ｭ螳夲ｼｯｼｮ
break;
}
int	nSetuzokuNum = atoi(p) - 1;	// 1縲
if ( nSetuzokuNum < 0 ) break;	// anonymous縺ｯ菴輔ｂ縺励↑縺
if ( lstrcmp(szIpMask, "61.114.182.54"	) == 0	||
lstrcmp(szIpMask, "61.114.182.53"	) == 0	||
lstrcmp(szIpMask, "61.114.186.0/25") == 0	||
lstrcmp(szIpMask, "61.114.177.148"	) == 0	||
strncmp(_default, szIpMask, lstrlen(_default)) == 0 )
{
if ( m_nAdslProc == nSetuzokuNum + 1 ) 	// ADSL謗･邯壼医→諤昴ｏ繧後ｋ謗･邯壼育分蜿ｷ
{
if ( m_sRouter.fBbUse >= 0 || m_sRouter.fBbUse != eLaboBB_KobetuLan )
{
m_sRouter.fBbUse = -1;// abs(m_sRouter.fBbUse);	// Labo-BB髢ｲ隕ｧ縺ｮ謗･邯壼育分蜿ｷ縺ｫADSL縺碁∈謚槭＆繧後※縺繧?
}
} else {
m_sRouter.m_nVWNo = nSetuzokuNum + 1;
if ( m_nStockVwNo == m_nCurrentPP && m_nStockVwNo != 0 ) { // 隕壹∴縺ｦ縺翫＞縺溘ｮPP逡ｪ蜿ｷ縺ｨ縺ｲ縺ｨ縺励＞
m_nStockVwNo = 0;
strncpy0(m_sRouter.szVwID, m_szStockVwID, sizeof(m_sRouter.szVwID));
strncpy0(m_sRouter.szVwPass, m_szStockVwPass, sizeof(m_sRouter.szVwPass));
m_fSisRoboVwM = TRUE;
}
}
} else if ( strcmp(szIpMask, SIS05_IP) == 0 ) {	// 172.31.191.0/24
m_sRouter.sSIS05.no = atoi(p);	// pp auth蜿･縺ｮ蜑阪↓蠢縺壹￥繧九ｯ縺
m_fSisRoboVwM = TRUE;
break;
}
p = strtok(NULL, SEARCH_CHR);	// metric 2縺後↑縺繧縺､縺ｯ縺縺(VPN or LaboBB=ADSL(蝗ｺ螳)縺ｧLaboINS縺後↑縺縺ｨ縺鬼IS05繝昴ｼ繝L2B1縺ｨ縺)
if ( p == NULL )
break;
SetIpC2B(szIpMask, m_sRouter.con[nSetuzokuNum].ip, m_sRouter.con[nSetuzokuNum].mask);
break;
}
case 6:	// filter
{
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
20010,20015,20020,20025,20030,20035,
```

### WriteTftpFile
```cpp
wsprintf(szIniFile, "%s%s\\%s", pMxBas->GetBaseDir(), MX_DIR_CONF, MX_COMMON_INI);
wsprintf(szConfigFile, "%s\\%s\\%s\\router\\router%02dW.prm", CDkMxBas::GetBaseDir(), MX_DIR_DATA, MX_DIR_CM, no);
wsprintf(szSendFile, "%s%s\\%s", pMxBas->GetBaseDir(), ROUTER_TMP_DIR, RT_SEND_FILE);
str.Format("險ｭ螳壽嶌霎ｼ繧ｨ繝ｩ繝ｼ %s", m_szDebug);
```

### PutFilterPass
```cpp
if (!_Put((CStoS)strPut))
if (!_Put((CStoS)strPut))
```

### PutNat_C
```cpp
if (!_PutTit(tit))
if (!_Put("nat descriptor log on"))
if (!_Put("nat descriptor masquerade ttl hold all"))
if (!_Put("nat descriptor type 1 nat"))
wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.ip[2], m_sRouter.ip[3]);
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor address inner 1 %s", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor static 1 %d %d.%d.%d.%d=172.16.%d.%d %d",
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor static 1 249 %d.%d.249.1=%s 255", m_sRouter.uip[0], m_sRouter.uip[1], GetIpB2C(ip));
if (!_Put(szBuf))
```

### PutNat_R
```cpp
if (!_PutTit(tit))
if (!_Put("nat descriptor log on"))
if (!_Put("nat descriptor masquerade ttl hold all"))
if (!_Put("nat descriptor type 1 nat"))
wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.uip[2], m_sRouter.ip[3] & 15);
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor address inner 1 %s", GetIpB2C(m_sRouter.ip));
if (!_Put(szBuf))
wsprintf(szBuf, "nat descriptor static 1 %d 10.0.%d.%d=172.16.%d.%d %d",
if (!_Put(szBuf))
```

### PutNat_H
```cpp
if (!_PutTit(tit))
if (!_Put("nat descriptor log on"))
if (!_Put("nat descriptor masquerade ttl hold all"))
```

### PutPpHead
```cpp
if (!_PutTit(tit))
if (!_Put("rip use on"))
const char *pCmd01[] = {
"pp disable all",
if (!_Put(pCmd01, DK_Number(pCmd01)))
```

### PutFmt
```cpp
return _Put(strMsg);
```

### PutPpAnonymous
```cpp
if (!_PutTit(tit))
const char *pCmd01[] = {
"no ip route 10.0.192.0/20",
"pp select anonymous",
"no ip pp rip send",
"no ip pp rip receive",
"no ip pp nat descriptor"
if (!_Put(pCmd01, DK_Number(pCmd01)))
if (!PutFmt("no pp auth username %s", m_sRouter.szAuthId))	return FALSE;
if (!PutFmt("pp bind tunnel%d-tunnel%d", m_sPrm.nLuaTunnelMin, m_sPrm.nLuaTunnelMax))	return FALSE;
if (!PutFmt("pp auth request mschap-v2"))	return FALSE;
if (!PutFmt("pp auth username %s %s", m_sRouter.szVPN_User, m_sRouter.szVPN_Pass))	return FALSE;
if (!PutFmt("ppp ipcp ipaddress on"))	return FALSE;
if (!PutFmt("ppp ipcp msext on"))	return FALSE;
if (!PutFmt("ip pp remote address pool 172.16.60.%d-172.16.60.%d", m_sPrm.nLuaTunnelMin, m_sPrm.nLuaTunnelMax))	return FALSE;
if (!PutFmt("ip pp mtu 1258"))	return FALSE;
if (!PutFmt("pp enable anonymous"))	return FALSE;
if (!_PutTit(tit))
const char *pCmd01[] = {
"pp select anonymous",
"pp bind bri1",
"pp auth request pap",
"pp auth username * *",
"ip pp rip send off",
"ip pp rip receive off",
"pp enable anonymous"
if (!_Put(pCmd01, DK_Number(pCmd01)))
if (!_PutTit(tit))
const char *pCmd01[] = {
"pp select anonymous",
if (!_Put(pCmd01, DK_Number(pCmd01)))
const char *pCmd2[] = {
"ip pp rip send off",
"ip pp rip receive off",
"pp bind bri1"
if (!_Put(pCmd2, DK_Number(pCmd2)))
_Put("ip pp nat descriptor 1");
const char *pCmd02[] = {
"pp auth request pap",
"pp auth username * *",
if (!_Put(pCmd02, DK_Number(pCmd02)))
if (!_Put("pp auth request chap"))
if (!_Put(strAuthPass))
if (!_Put("pp enable anonymous"))
```

### PutIpsec
```cpp
if (!_PutTit("### IPSEC ###"))
if (!PutFmt("ipsec transport %d 1%d udp 1701", nTunnelNo, nTunnelNo))
```

### PutOther
```cpp
if (!_PutTit(tit))
const char *pCmd01[] = {
"syslog info on",
"syslog notice on",
"syslog debug off",
"tftp host any",
if (!_Put(pCmd01, DK_Number(pCmd01)))
if (!_Put("ipsec auto refresh on"))
if ( !_Put("dhcp server rfc2131 compliant except remain-silent") )
wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip), GetIpB2C_Second(m_sRouter.sLAN3.sPriSecIp.mask));
if (!_Put(szBuf))
if (!_Put("no dns server pp"))
wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask));
if (!_Put(szBuf))
wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask));
if (!_Put(szBuf))
wsprintf(szBuf, "dns server pp %d", (m_sRouter.m_nAdslNo && m_sRouter.fBbUse) ? m_sRouter.m_nAdslNo : m_sRouter.m_nVWNo);
if (!_Put(szBuf))
if (!_Put("dns private address spoof on"))
if (!_Put("schedule at 1 */* 6:00 * ntpdate 172.31.191.62"))
if (!_Put("dns service recursive"))
if (!_Put("dhcp service server"))
wsprintf(szBuf, "dhcp scope 1 %s-%s/16", m_szFirstAdd, m_szLastAdd);
if (!_Put(szBuf))
const char *pCmd[] = {
"no dhcp service",
"no dhcp scope 1"
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "dhcp scope option 1 dns=172.16.5.1");
if (!_Put(szBuf))
if (!_Put("dhcp service server"))
wsprintf(szBuf, "dhcp scope 2 %s-%s/29", GetIpB2C(m_byLan3Ip_plus2), GetIpB2C_Second(m_byLan3Ip_plus5));
if (!_Put(szBuf))
wsprintf(szBuf, "dhcp scope option 2 dns=%s", GetIpB2C(m_sRouter.ip_2_cloud));
if (!_Put(szBuf))
if (!_Put("pptp service on"))
wsprintf(szBuf, "ip stealth pp %d", m_sRouter.m_nAdslNo);
if (!_Put(szBuf))
const char *pCmd[] = {
"l2tp service on",
"schedule at 2 startup * lua /xai_check.lua",
"schedule at 3 */* 04:00:00 * terminate lua all",
"schedule at 4 */* 04:00:10 * lua /xai_check.lua",
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "# LIFT_PHASE=%d", m_sIniFileInf.nStatus);	// 繧ｳ繝｡繝ｳ繝医ｯ繝ｫ繝ｼ繧ｿ縺ｸ騾∽ｿ｡縺吶ｋ縺ｨ豸医∴繧
if (!_Put(szBuf))
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
if (!_Put(pCmd, DK_Number(pCmd)))
wsprintf(szBuf, "ip wan1 mtu 1454");	// 蝗ｺ螳壼､
if (!_Put(szBuf))
wsprintf(szBuf, "wan1 auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
if (!_Put(szBuf))
wsprintf(szBuf, "wan1 access-point name %s", m_szWanApiName);	// mobiledata.ntt.com
if (!_Put(szBuf))
wsprintf(szBuf, "description 10 \"No=%d Kind=%d Ip=%s MTU=%d Gateway=%s\"", pInfo->no, nKind, GetIpB2C(pInfo->fixIp, pInfo->yobi1), m_sPutPreRouter.nMTU, GetIpB2C_Second(pInfo->yobi2));
if (!_Put(szBuf))
wsprintf(szBuf, "description 12 \"XaiTactP=%d PH=%s\"", m_sPutPreRouter.nLuaPort, m_sPutPreRouter.szPH_CodeA2910);
if (!_Put(szBuf))
wsprintf(szBuf, "description 13 \"UserID=%s\"", m_sPutPreRouter.szAdslID);
if (!_Put(szBuf))
wsprintf(szBuf, "description 14 \"Pass=%s\"", m_sPutPreRouter.szAdslPass);
if (!_Put(szBuf))
const char *pCmd02[] = {
"description 1 \"configversion=5.0\"",// RTX1000莉･螟悶ｯ譖ｸ縺
"save",
if (!_Put(pCmd02, DK_Number(pCmd02)))
```

### _PutFilterName
```cpp
return _Put(strGetLog);
```

### PutRevUpFile
```cpp
wsprintf(szRevUpFilePath, "%s\\%s\\%s", pMxBas->GetBaseDir(), ROUTER_CONFIG_DIR, CDkRouter::GetBinFileName(m_sRouter.kind));
str.Format("%s縺悟ｭ伜惠縺励↑縺縺溘ａRevUp縺ｧ縺阪∪縺帙ｓ", szRevUpFilePath);
wsprintf(szReDir, "%s%s\\%s", pMxBas->GetBaseDir(), ROUTER_TMP_DIR, RT_REDIRECT_FILE);
wsprintf(szCmd, "cmd /c tftp -i %s put %s exec > %s", GetIpB2C(pSendIp), szRevUpFilePath, szReDir);
```

### PutTftp
```cpp
wsprintf(szIp, "%d.%d.%d.%d", pSendIp[0], pSendIp[1], pSendIp[2], pSendIp[3]);
wsprintf(szRevUpFilePath, "%s\\%s\\%s", pMxBas->GetBaseDir(), ROUTER_CONFIG_DIR, CDkRouter::GetBinFileName(m_sRouter.kind));
wsprintf(szReDir, "%s%s\\%s", pMxBas->GetBaseDir(), ROUTER_TMP_DIR, RT_REDIRECT_FILE);
wsprintf(m_szSendLuaFile,	"%s\\%s\\%s\\router\\routerMukou.lua",		CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);	// lua辟｡蜉ｹ繝輔ぃ繧､繝ｫ繝代せ
wsprintf(m_szSendLuaChkFile,"%s\\%s\\%s\\router\\routerChkMukou.lua",	CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);	// luachk辟｡蜉ｹ繝輔ぃ繧､繝ｫ繝代せ
wsprintf(m_szSendPrmFile,	"%s\\%s\\%s\\router\\routerMukou.prm",		CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);	// prm辟｡蜉ｹ繝輔ぃ繧､繝ｫ繝代せ
wsprintf(szRouterLuaFile,	"%s\\%s\\%s\\router\\router.lua",		CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);
wsprintf(szRouterLuaChkFile, "%s\\%s\\%s\\router\\routerChk.lua",	CDkMxBas::GetBaseDir(), MX_DIR_CONF, MX_DIR_CM);
wsprintf(szRouterPrmFile,	 "%s\\%s\\%s\\router\\router%02dW.prm",	CDkMxBas::GetBaseDir(), MX_DIR_DATA, MX_DIR_CM, nRouterNo);
str.Format("笳熟LuaPort=%d IsXai=%d", m_sRouter.nLuaPort, IsXai() );
```

### GetTftp
```cpp
wsprintf(szIp, "%s", ip);
str.Format("CDkRouter::GetTftp pass=%s", szPass);
wsprintf(szIp, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
```

### WriteRegstryData
```cpp
wsprintf(m_szRMAdd, "%d.%d.%d.%d", m_bRMAdd[0], m_bRMAdd[1], m_bRMAdd[2], m_bRMAdd[3]);
```

