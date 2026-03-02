# SDkRouterメンバ変数と設定出力コマンド対応表 (省略なし全コード網羅版)

本ドキュメントは、要求に基づき `SDkRouter` の各メンバ変数が `Router1200.cpp` のどの関数内で、**どのようなコマンド出力処理（ソースコード）として** 生成されているかを、**省略せずに完全に網羅して** 出力したものです。

## m_sRouter.kind
```cpp
[PutConfig] wsprintf(szBuf, "#\t%s Rev.%s", szRtType[m_sRouter.kind - 1], m_sRouter.szRev);
[その他の関数での出力処理] if (m_sRouter.kind == eRTA54i && m_sRouter.kind == eRTA55i )
[その他の関数での出力処理] if ( !(GetRtCategory(m_sRouter.kind) == eRtType1000_OA) && m_sRouter.fMent == eMentNoUse )
[その他の関数での出力処理] wsprintf(szRevUpFilePath, "%s\\%s\\%s", pMxBas->GetBaseDir(), ROUTER_CONFIG_DIR, CDkRouter::GetBinFileName(m_sRouter.kind));
```

## m_sRouter.szRev
```cpp
[PutConfig] wsprintf(szBuf, "#\t%s Rev.%s", szRtType[m_sRouter.kind - 1], m_sRouter.szRev);
```

## m_sRouter.ip
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan1 address %s", GetIpB2C(m_sRouter.ip, m_sRouter.mask));
[PutLanIp] wsprintf(szBuf, "ip lan1 secondary address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s/29", GetIpB2C(m_sRouter.ip_2_cloud));
[PutPp] wsprintf(szBuf, "ip pp remote address 192.168.%d.%d", m_sRouter.ip[2], m_sRouter.ncon[n].type + 47);
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
[PutTunnel] wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
[PutTunnel] wsprintf(szBuf,"ip tunnel address %s", GetIpB2C(m_sRouter.ip_2, NULL));
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nCarMentNo, GetIpB2C(m_sRouter.ip));
[PutFilter] wsprintf(szPut,"ip filter 3442 pass %s %s udp * domain", GetIpB2C(m_sRouter.sLAN3.ipVirus), GetIpB2C_Second(m_sRouter.ip));
[PutFilter] wsprintf(szPut,"ip filter 3443 pass %s %s udp domain *", GetIpB2C(m_sRouter.ip), GetIpB2C_Second(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 10200 pass %s %s tcp * 4500", IP_JNET_MAIN_IP, GetIpB2C(m_sRouter.ip));
[PutFilter] wsprintf(szPut, "ip filter 10201 pass %s %s tcp * 4500", IP_JNET_SUB_IP, GetIpB2C(m_sRouter.ip));
[PutFilter] wsprintf(szPut, "ip filter 10205 pass %s %s tcp * 4500", GetIpB2C(m_sRouter.ip), IP_JNET_MAIN_IP);
[PutFilter] wsprintf(szPut, "ip filter 10206 pass %s %s tcp * 4500", GetIpB2C(m_sRouter.ip), IP_JNET_SUB_IP);
[PutFilter] if ( !PutFmt("ip filter 10500 pass * %s tcp * %d", GetIpB2C(m_sRouter.ip), (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nLuaPort : m_sRouter.nLuaPort) )	return FALSE;
[PutFilter] if ( !PutFmt("ip filter 10505 pass %s * tcp %d", GetIpB2C(m_sRouter.ip), (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nLuaPort : m_sRouter.nLuaPort) )	return FALSE;
[PutFilter] if ( !PutFmt("ip filter 10510 pass %s %s udp * 500", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
[PutFilter] if ( !PutFmt("ip filter 10520 pass %s %s esp * *", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
[PutFilter] if ( !PutFmt("ip filter 10525 pass %s %s esp * *", GetIpB2C(m_sRouter.ip), GetLuaIP()) )	return FALSE;
[PutFilter] if ( !PutFmt("ip filter 10550 pass %s %s udp * 1701", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
[PutFilter] if ( !PutFmt("ip filter 10560 pass %s %s udp * 4500", GetLuaIP(), GetIpB2C(m_sRouter.ip)) )	return FALSE;
[PutFilter] if ( !PutFmt("ip filter 10575 pass %s %s udp * *", GetIpB2C(m_sRouter.ip), GetLuaIP()) )	return FALSE;
[PutIpRoute] wsprintf(szBuf, "ip route %d.%d.%d.%d gateway pp anonymous metric 2", m_sRouter.ip[0], m_sRouter.ip[1], m_sRouter.ip[2], nIp);
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 2000 3 %s tcp 1723", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 2000 4 %s gre", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 2000 1 %s udp 500", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 2000 2 %s esp", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 2000 6 %s udp 4500", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 2000 7 %s tcp %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort);
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 3000 1 %s udp 500", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 3000 2 %s esp", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 3000 3 %s udp domain", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 3000 4 %s tcp www", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 3000 5 %s tcp http", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor static 2012 2 %s=%s 1", GetIpB2C(bVPNadd), GetIpB2C_Second(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 3013 1 %s udp 500", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 3013 2 %s tcp 4500", GetIpB2C(m_sRouter.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor masquerade static 2000 5 %s udp 4500", GetIpB2C(m_sRouter.ip));
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);			// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip, m_sRouter.mask);
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);
[その他の関数での出力処理] wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.ip[2], m_sRouter.ip[3]);
[その他の関数での出力処理] wsprintf(szBuf, "nat descriptor address inner 1 %s", GetIpB2C(m_sRouter.ip));
[その他の関数での出力処理] wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.uip[2], m_sRouter.ip[3] & 15);
[その他の関数での出力処理] wsprintf(szBuf, "dhcp scope option 2 dns=%s", GetIpB2C(m_sRouter.ip_2_cloud));
```

## m_sRouter.mask
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan1 address %s", GetIpB2C(m_sRouter.ip, m_sRouter.mask));
[PutLanIp] wsprintf(szBuf, "ip lan1 secondary address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutFilter] wsprintf(szPut, "ip filter 10 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 15 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 80 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 81 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 85 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 86 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 90 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 95 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 91 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 96 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 1400 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 1405 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2090 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2095 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2091 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 2096 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3090 pass * %s tcp pop3 *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3095 pass %s * tcp * pop3", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3091 pass * %s tcp smtp *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter 3096 pass %s * tcp * smtp", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut,"ip filter 3800 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo, GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut,"ip filter 3805 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].mask));
[PutFilter] wsprintf( szPut, "ip filter 10000 reject %s * * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
[PutFilter] wsprintf( szPut, "ip filter 10005 reject * %s * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
[PutFilter] wsprintf(szPut, "ip filter dynamic 85 %s * filter 85 in 80", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 86 %s * filter 86 in 81", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 95 %s * filter 95 in 90", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 96 %s * filter 96 in 91", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 2085 %s * filter 2085 in 2080", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 2086 %s * filter 2086 in 2081", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 2095 %s * filter 2095 in 2090", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 2096 %s * filter 2096 in 2091", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 3085 %s * filter 3085 in 3080", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 3086 %s * filter 3086 in 3081", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 3095 %s * filter 3095 in 3090", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "ip filter dynamic 3096 %s * filter 3096 in 3091", GetIpB2C(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut, "url filter 10 pass * %s", GetIpB2C(bMaskedIP, m_sRouter.mask));
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);			// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip, m_sRouter.mask);
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);
```

## m_sRouter.uip
```cpp
[PutTunnel] wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask);
[その他の関数での出力処理] wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.ip[2], m_sRouter.ip[3]);
[その他の関数での出力処理] wsprintf(szBuf, "nat descriptor static 1 249 %d.%d.249.1=%s 255", m_sRouter.uip[0], m_sRouter.uip[1], GetIpB2C(ip));
[その他の関数での出力処理] wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.uip[2], m_sRouter.ip[3] & 15);
```

## m_sRouter.port[d].kaisen
```cpp
[PutPp] wsprintf(szBuf, "pp always-on %s", (ADSL_KIND(m_sRouter.port[1].kaisen)== eKaisenFixADSL) ? "on" : "off");
[その他の関数での出力処理] m_sRouter.port[2].kaisen = eKaisenIP_TEL;				// L3繝昴ｼ繝医↓IP髮ｻ隧ｱ繧偵そ繝繝
[その他の関数での出力処理] m_sRouter.port[2].kaisen = eKaisenLAN;					// L3繝昴ｼ繝医↓蛟句挨LAN繧偵そ繝繝
[その他の関数での出力処理] m_sRouter.port[1].kaisen = eKaisenFixADSL;
```

## m_sRouter.port[d].Tel
```cpp
[PutIsdn] wsprintf(szBuf, "isdn local address bri1 %s", m_sRouter.port[0].Tel);
```

## m_sRouter.con[w].no
```cpp
[PutPp] wsprintf(szBuf, "### PP %d ###", m_sRouter.con[n].no);
[PutPp] wsprintf(szBuf, "pp select %d", m_sRouter.con[n].no);
[PutPp] wsprintf(szBuf, "pp bind tunnel%d", m_sRouter.con[n].no);
[PutPp] wsprintf(szBuf, "pp disable %d", m_sRouter.con[n].no);
[PutPp] wsprintf(szBuf, "pp enable %d", m_sRouter.con[n].no);
[PutTunnel] wsprintf(szBuf, "tunnel enable %d", m_sRouter.con[n].no);
[PutTunnel] wsprintf(szBuf, "tunnel backup pp %d", m_sRouter.con[n].no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d metric 2", ROBO_IP, m_sRouter.con[n].no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d",GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
```

## m_sRouter.con[w].ip
```cpp
[PutTunnel] wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask));
[PutFilter] wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
[PutFilterOA_LAN] wsprintf(szIp, "%s", GetIpB2C(m_sRouter.con[n].ip));
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.con[n].ip), m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d",GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
[その他の関数での出力処理] wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask));
```

## m_sRouter.con[w].mask
```cpp
[PutTunnel] wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask));
[PutFilter] wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
[PutFilterOA_LAN] wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.con[n].mask));
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d",GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask), m_sRouter.con[n].no);
[その他の関数での出力処理] wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask));
```

## m_sRouter.con[w].Tel
```cpp
[PutPp] wsprintf(szBuf, "isdn remote address call %s", m_sRouter.con[n].Tel);
[PutPp] wsprintf(szBuf, "isdn remote address arrive %s", m_sRouter.con[n].Tel);
```

## m_sRouter.szVwID
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szVwID, m_sRouter.szVwPass);
[その他の関数での出力処理] strncpy0(m_sRouter.szVwID, m_szStockVwID, sizeof(m_sRouter.szVwID));
```

## m_sRouter.szVwPass
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szVwID, m_sRouter.szVwPass);
[その他の関数での出力処理] strncpy0(m_sRouter.szVwPass, m_szStockVwPass, sizeof(m_sRouter.szVwPass));
```

## m_sRouter.shOverTimer
```cpp
[PutPp] wsprintf(szBuf, "isdn forced disconnect time %d", m_sRouter.shOverTimer * 60);	// 謗･邯夊ｶ驕弱ち繧､繝
```

## m_sRouter.fMent
```cpp
[PutIpRoute] wsprintf(szBuf, "ip route 10.0.208.0/20 gateway tunnel %d", abs(m_sRouter.fMent));
[その他の関数での出力処理] m_sRouter.fMent = eMentFixADSL;	// 繝｡繝ｳ繝繝翫Φ繧ｹ險ｭ螳夲ｼｯｼｮ
[その他の関数での出力処理] m_sRouter.fMent = 0 - atoi(p);	// ADSL蝗ｺ螳壹′驕ｸ謚槭＆繧後※縺縺溘ｏ縺代〒縺ｯ縺ｪ縺
[その他の関数での出力処理] m_sRouter.fMent = eMentAuto;	// auto遒ｺ螳
[その他の関数での出力処理] if ( !(GetRtCategory(m_sRouter.kind) == eRtType1000_OA) && m_sRouter.fMent == eMentNoUse )
[その他の関数での出力処理] m_sRouter.fMent = /*m_sRouter.fMent ? m_sRouter.fMent :*/ eMentAnonymous;	// 繝｡繝ｳ繝繝翫Φ繧ｹ險ｭ螳夲ｼｯｼｮ
```

## m_sRouter.m_nSISNo
```cpp
[その他の関数での出力処理] m_sRouter.m_nSISNo = atoi(p);
```

## m_sRouter.m_nVWNo
```cpp
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d weight 999999999 hide gateway pp %d weight 1",  pCmd[nBit], m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo);
[その他の関数での出力処理] m_sRouter.m_nVWNo					= m_nCurrentPP+1;
[その他の関数での出力処理] m_sRouter.m_nVWNo = nSetuzokuNum + 1;
[その他の関数での出力処理] wsprintf(szBuf, "dns server pp %d", (m_sRouter.m_nAdslNo && m_sRouter.fBbUse) ? m_sRouter.m_nAdslNo : m_sRouter.m_nVWNo);
```

## m_sRouter.m_nRoboNo
```cpp
[その他の関数での出力処理] m_sRouter.m_nRoboNo = atoi(p);
[その他の関数での出力処理] SetIpC2B(szIpMask, m_sRouter.con[m_sRouter.m_nRoboNo-1].ip);
```

## m_sRouter.szAdslID
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass);
```

## m_sRouter.szAdslPass
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szAdslID, m_sRouter.szAdslPass);
```

## m_sRouter.m_nAdslNo
```cpp
[PutTunnel] wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nAdslNo);
[PutTunnel] wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nAdslNo);
[PutTunnel] wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nAdslNo + 100);
[PutTunnel] wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc md5-hmac", m_sRouter.m_nAdslNo + 100, m_sRouter.m_nAdslNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike pre-shared-key %d text *", m_sRouter.m_nAdslNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike remote address %d any", m_sRouter.m_nAdslNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike remote name %d *", m_sRouter.m_nAdslNo);
[PutTunnel] wsprintf(szBuf, "tunnel enable %d",m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route default gateway pp %d", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", pCmd[nBit], m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", KEIHIN_MOLE, m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d weight 999999999 hide gateway pp %d weight 1",  pCmd[nBit], m_sRouter.m_nAdslNo, m_sRouter.m_nVWNo);
[PutIpRoute] wsprintf(szBuf,"ip route %s gateway pp %d metric 2", pCmd[nBit], m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route default gateway pp %d metric 2", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d metric 2", KEIHIN_MOLE, m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route 210.227.65.146 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route 122.103.88.162 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.con[n].ip), m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.sSIS05.accessIp), m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route 26.26.26.26 gateway pp %d metric 2", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route 10.0.208.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route 10.0.224.0/20 gateway tunnel %d", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", m_szRMAdd, m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route 10.0.208.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route 10.0.224.0/24 gateway tunnel %d", m_sRouter.m_nAdslNo);
[その他の関数での出力処理] wsprintf(szBuf, "dns server pp %d", (m_sRouter.m_nAdslNo && m_sRouter.fBbUse) ? m_sRouter.m_nAdslNo : m_sRouter.m_nVWNo);
[その他の関数での出力処理] wsprintf(szBuf, "ip stealth pp %d", m_sRouter.m_nAdslNo);
```

## m_sRouter.fBbUse
```cpp
[その他の関数での出力処理] if ( m_sRouter.fBbUse < 0 && m_sRouter.fBbUse != eLaboBB_KobetuLan )
[その他の関数での出力処理] m_sRouter.fBbUse = eLaboBB_Adsl;
[その他の関数での出力処理] m_sRouter.fBbUse = eLaboBB_FixAdsl;
[その他の関数での出力処理] m_sRouter.fBbUse = eLaboBB_KobetuLan;	// BB謗･邯壹ｯ蛟句挨LAN縺ｧ豎ｺ螳
[その他の関数での出力処理] if ( m_sRouter.fBbUse >= 0 || m_sRouter.fBbUse != eLaboBB_KobetuLan )
[その他の関数での出力処理] m_sRouter.fBbUse = -1;// abs(m_sRouter.fBbUse);	// Labo-BB髢ｲ隕ｧ縺ｮ謗･邯壼育分蜿ｷ縺ｫADSL縺碁∈謚槭＆繧後※縺繧?
[その他の関数での出力処理] wsprintf(szBuf, "dns server pp %d", (m_sRouter.m_nAdslNo && m_sRouter.fBbUse) ? m_sRouter.m_nAdslNo : m_sRouter.m_nVWNo);
```

## m_sRouter.ncon[w].no
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, (m_sRouter.ncon[n].no == m_sRouter.m_nJnetNo) ? IP_JNET_MAIN_IP : IP_JNET_SUB_IP);
[PutTunnel] wsprintf(szBuf, "tunnel backup pp %d switch-router=on", m_sRouter.ncon[n].no);
```

## m_sRouter.ncon[w].type
```cpp
[PutPp] wsprintf(szBuf, "ip pp remote address 192.168.%d.%d", m_sRouter.ip[2], m_sRouter.ncon[n].type + 47);
[PutPp] wsprintf(szBuf, "ip pp remote address 172.16.60.%d", m_sRouter.ncon[n].type + 47);
[PutTunnel] wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, (m_sRouter.ncon[n].type == eDKVPN || m_sRouter.ncon[n].type == eKobetuVPN) ? "off" : "on");
```

## m_sRouter.ncon[w].fixIp
```cpp
[PutTunnel] wsprintf(szBuf, "tunnel endpoint address %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
[PutTunnel] wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
[PutFilter] wsprintf(szLan2Fix,	"%s", GetIpB2C_Second(m_sRouter.ncon[n].fixIp));// LAN縺ｮ蝗ｺ螳唔P繧｢繝峨Ξ繧ｹ
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.ncon[n].fixIp), m_sRouter.m_nAdslNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway %s", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));
[PutNatOther] wsprintf(szBuf, "nat descriptor address outer 2000 %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutNatOther] wsprintf(szBuf, "nat descriptor address outer 3013 %s", GetIpB2C(m_sRouter.ncon[n].fixIp));
```

## m_sRouter.ncon[w].ip
```cpp
[PutTunnel] wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].ip));
```

## m_sRouter.sSIS05.no
```cpp
[PutIpRoute] str.Format("ip route 10.142.65.0/24 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
[PutIpRoute] str.Format("ip route 10.142.165.0/24 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
[PutIpRoute] str.Format("ip route 10.145.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str + sCr;
[PutIpRoute] str.Format("ip route 10.141.0.0/16 gateway tunnel %d", m_sRouter.sSIS05.no);	strPut += str;
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d", SIS05_IP, m_sRouter.sSIS05.no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d", VUP_IP, m_sRouter.sSIS05.no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d metric 2", SIS05_IP, m_sRouter.sSIS05.no);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d metric 2", VUP_IP, m_sRouter.sSIS05.no);
[その他の関数での出力処理] m_sRouter.sSIS05.no = atoi(p);	// pp auth蜿･縺ｮ蜑阪↓蠢縺壹￥繧九ｯ縺
```

## m_sRouter.sSIS05.szHoleCode
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", (lstrlen(m_sRouter.sSIS05.szHoleCode) == 0) ? "0.0.0.0" : m_sRouter.sSIS05.szHoleCode, GetIpB2C(m_sRouter.sSIS05.passIp));
```

## m_sRouter.sSIS05.passIp
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", (lstrlen(m_sRouter.sSIS05.szHoleCode) == 0) ? "0.0.0.0" : m_sRouter.sSIS05.szHoleCode, GetIpB2C(m_sRouter.sSIS05.passIp));
```

## m_sRouter.sSIS05.accessIp
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.sSIS05.accessIp));
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway pp %d", GetIpB2C(m_sRouter.sSIS05.accessIp), m_sRouter.m_nAdslNo);
```

## m_sRouter.sLAN3.ip
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 2444 pass %s * tcp * www,https", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 2445 pass * %s tcp www,https *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3422 pass-log %s 172.16.0.0/16 icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3427 pass-log 172.16.0.0/16 %s icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3442 pass %s %s udp * domain", GetIpB2C(m_sRouter.sLAN3.ipVirus), GetIpB2C_Second(m_sRouter.ip));
[PutFilter] wsprintf(szPut,"ip filter 3443 pass %s %s udp domain *", GetIpB2C(m_sRouter.ip), GetIpB2C_Second(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3444 pass %s * tcp * www,https", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3445 pass * %s tcp www,https *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3446 pass-log %s 10.0.192.0/18 icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3447 pass-log 10.0.192.0/18 %s icmp * *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3448 pass-log %s 10.0.192.0/18 tcp 3389 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut,"ip filter 3449 pass-log 10.0.192.0/18 %s tcp * 3389", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3501 pass-log %s 172.16.0.0/16 tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3506 pass-log 172.16.0.0/16 %s tcp,udp netbios_ns-netbios_ssn * ", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3521 pass-log %s 172.16.0.0/16 tcp,udp * 445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3526 pass-log 172.16.0.0/16 %s tcp,udp 445 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3561 pass-log %s 172.16.0.0/16 tcp,udp netbios_ns-netbios_ssn *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3566 pass-log 172.16.0.0/16 %s tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3581 pass-log %s 172.16.0.0/16 tcp,udp 445 *", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter 3586 pass-log 172.16.0.0/16 %s tcp,udp * 445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter dynamic 2444 %s * filter 2444 in 2445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "ip filter dynamic 3444 %s * filter 3444 out 3445", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 21 pass download.windowsupdate.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 22 pass update.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 23 pass windowsupdate.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 24 pass download.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 25 pass ntservicepack.microsoft.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 26 pass wustat.windows.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 27 pass trendmicro.co.jp %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutFilter] wsprintf(szPut, "url filter 28 pass trendmicro.com %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutIpRoute] wsprintf(szBuf,"ip route %s gateway %s", pCmd[nBit], GetIpB2C(m_sRouter.sLAN3.ipGateWay));
[PutIpRoute] wsprintf(szBuf, "ip route default gateway %s", GetIpB2C(m_sRouter.sLAN3.ipGateWay));
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway %s", KEIHIN_MOLE, GetIpB2C(m_sRouter.sLAN3.ipGateWay));
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway %s", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));
[PutNatOther] wsprintf(szBuf, "nat descriptor address outer 8000 %s", GetIpB2C(m_sRouter.sLAN3.ip));
[PutNatOther] wsprintf(szBuf, "nat descriptor address inner 1000 %s", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutNatOther] wsprintf(szBuf, "nat descriptor static 1000 252 172.16.252.1=%s 1", GetIpB2C(m_sRouter.sLAN3.ipVirus));
[PutNatOther] wsprintf(szBuf, "nat descriptor address outer 3000 %s", GetIpB2C(m_sRouter.sLAN3.ip));
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask);	// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.sLAN3.ipGateWay);
```

## m_sRouter.sLAN3.mask
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 2450 pass %s 61.114.186.0/25 tcp * https", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 2451 pass 61.114.186.0/25 %s tcp https *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3450 pass %s 61.114.186.0/25 tcp * https", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3451 pass 61.114.186.0/25 %s tcp https *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3500 pass-log %s 172.16.60.0/24 tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3505 pass-log 172.16.60.0/24 %s tcp,udp netbios_ns-netbios_ssn *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3510 pass-log %s 172.16.60.0/24 tcp,udp * 135", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3515 pass-log 172.16.60.0/24 %s tcp,udp 135 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3520 pass-log %s 172.16.60.0/24 tcp,udp * 445", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3525 pass-log 172.16.60.0/24 %s tcp,udp 445 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3530 pass %s 172.16.60.0/24 tcp * 21", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3535 pass 172.16.60.0/24 %s tcp 21 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3540 pass %s 172.16.60.0/24 tcp * ftpdata", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3545 pass 172.16.60.0/24 %s tcp ftpdata *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3550 pass %s 172.16.60.0/24 tcp 58010-58019 2638", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3555 pass 172.16.60.0/24 %s tcp 2638 58010-58019", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3560 pass-log %s 172.16.60.0/24 tcp,udp netbios_ns-netbios_ssn *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3565 pass-log 172.16.60.0/24 %s tcp,udp * netbios_ns-netbios_ssn", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3570 pass-log %s 172.16.60.0/24 tcp,udp 135 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3575 pass-log 172.16.60.0/24 %s tcp,udp * 135", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3580 pass-log %s 172.16.60.0/24 tcp,udp 445 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3585 pass-log 172.16.60.0/24 %s tcp,udp * 445", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3590 pass %s 172.16.60.0/24 tcp * 58006", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 3595 pass 172.16.60.0/24 %s tcp 58006 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 10650 pass %s * tcp * 58300", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 10655 pass * %s tcp 58300 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 10660 pass %s * udp 58301 *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 10665 pass * %s udp * 58301", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter 10670 pass %s * tcp * %d", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), m_sRouter.nRdpRecvPort);
[PutFilter] wsprintf(szPut,"ip filter 10675 pass * %s tcp %d *", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), m_sRouter.nRdpRecvPort);
[PutFilter] wsprintf(szPut,"ip filter dynamic 2450 %s 61.114.186.0/25 filter 2450 in 2451", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter dynamic 3450 %s 61.114.186.0/25 filter 3450 out 3451", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter dynamic 3530 %s 172.16.60.0/24 ftp", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter dynamic 3550 %s 172.16.60.0/24 filter 3550 in 3555", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter dynamic 3590 %s 172.16.60.0/24 filter 3590 in 3595", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilter] wsprintf(szPut,"ip filter dynamic 10665 * %s filter 10665 in 10660 10670 out 10675", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask));
[PutFilterOA_LAN] wsprintf(szPut,"ip filter dynamic 8530 %s 192.168.%d.2-192.168.%d.254 ftp", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask), nRouterIP3, nRouterIP3);
[PutFilterOA_LAN] wsprintf(szPut,"ip filter dynamic 8550 %s 192.168.%d.2-192.168.%d.254 filter 8550 in 8555", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask),nRouterIP3, nRouterIP3);
[PutFilterOA_LAN] wsprintf(szPut,"ip filter dynamic 8590 %s 192.168.%d.2-192.168.%d.254 filter 8590 in 8595", GetIpB2C(bL3MaskedIP, m_sRouter.sLAN3.mask),nRouterIP3, nRouterIP3);
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.sLAN3.ip, m_sRouter.sLAN3.mask);	// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
```

## m_sRouter.sLAN3.ipGateWay
```cpp
[PutIpRoute] wsprintf(szBuf,"ip route %s gateway %s", pCmd[nBit], GetIpB2C(m_sRouter.sLAN3.ipGateWay));
[PutIpRoute] wsprintf(szBuf, "ip route default gateway %s", GetIpB2C(m_sRouter.sLAN3.ipGateWay));
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway %s", KEIHIN_MOLE, GetIpB2C(m_sRouter.sLAN3.ipGateWay));
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway %s", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C_Second(m_sRouter.sLAN3.ipGateWay));
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.sLAN3.ipGateWay);
```

## m_sRouter.sLAN3.sPriSecIp
```cpp
[PutFilter] wsprintf(szIp, "%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip));
[PutFilter] wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.mask));
[PutFilterOA_LAN] wsprintf(szIp, "%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip));
[PutFilterOA_LAN] wsprintf(szIp2,"%s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.mask));
[その他の関数での出力処理] wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.sLAN3.sPriSecIp.ip), GetIpB2C_Second(m_sRouter.sLAN3.sPriSecIp.mask));
```

## m_sRouter.sLAN3.sSinraiCon
```cpp
[PutFilter] wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3801].mask), GetIpB2C_Second(bMaskedIP, m_sRouter.mask));
[PutFilter] wsprintf(szPut,"ip filter %d pass %s %s * * *", nFinNo,	GetIpB2C(bMaskedIP, m_sRouter.mask), GetIpB2C_Second(m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].ip, m_sRouter.sLAN3.sSinraiCon[nFinNo-3806].mask));
```

## m_sRouter.szRoboID
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szRoboID, m_sRouter.szRoboPass);
```

## m_sRouter.szRoboPass
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.szRoboID, m_sRouter.szRoboPass);
```

## m_sRouter.m_nDeltaBkNo
```cpp
[PutTunnel] wsprintf(szBuf, "tunnel backup tunnel %d switch-interface=on", m_sRouter.m_nDeltaBkNo);
```

## m_sRouter.nMTU
```cpp
[PutPp] wsprintf(szBuf, "ip pp mtu %d", (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nMTU : m_sRouter.nMTU);
[その他の関数での出力処理] m_sRouter.nMTU = atoi(p);
```

## m_sRouter.ip_2
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan1 secondary address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s/29", GetIpB2C(m_sRouter.ip_2_cloud));
[PutTunnel] wsprintf(szBuf,"ip tunnel address %s", GetIpB2C(m_sRouter.ip_2, NULL));
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);			// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);
[その他の関数での出力処理] wsprintf(szBuf, "dhcp scope option 2 dns=%s", GetIpB2C(m_sRouter.ip_2_cloud));
```

## m_sRouter.mask_2
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan1 secondary address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s", GetIpB2C(m_sRouter.ip_2, m_sRouter.mask_2));
[PutFilter] wsprintf( szPut, "ip filter 10000 reject %s * * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
[PutFilter] wsprintf( szPut, "ip filter 10005 reject * %s * * *", GetIpB2C(bMySegmentIP, m_sRouter.mask_2) );
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);			// LAN3蝗ｺ螳夲ｼｩｼｰ繧｢繝峨Ξ繧ｹ縺ｮ蜿門ｾ
[その他の関数での出力処理] SetIpC2B(p, m_sRouter.ip_2, m_sRouter.mask_2);
```

## m_sRouter.m_ptcSvIp
```cpp
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.m_ptcSvIp), m_sRouter.m_nIpTelNo);
[PutIpRoute] wsprintf(szBuf, "ip keepalive %d icmp-echo 60 3 %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.m_ptcSvIp));
```

## m_sRouter.ptc_ip
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.ptc_ip, NULL));
[PutFilter] wsprintf(szMainRt,	"%s", GetIpB2C(m_sRouter.ptc_ip));				// 繝｡繧､繝ｳ繝ｫ繝ｼ繧ｿ
```

## m_sRouter.center_ip
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.center_ip, NULL));
[PutFilter] wsprintf(szBackUpRt, "%s", GetIpB2C_Second(m_sRouter.center_ip));	// 繝舌ャ繧ｯ繧｢繝繝励Ν繝ｼ繧ｿ
```

## m_sRouter.sLte.ip
```cpp
[PutFilter] wsprintf(szLan2Fix,	"%s", GetIpB2C_Second(m_sRouter.sLte.ip));		// LAN縺ｮ蝗ｺ螳唔P繧｢繝峨Ξ繧ｹ
```

## m_sRouter.sLte.szID
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
[その他の関数での出力処理] wsprintf(szBuf, "wan1 auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
```

## m_sRouter.sLte.szPass
```cpp
[PutPp] wsprintf(szBuf, "pp auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
[その他の関数での出力処理] wsprintf(szBuf, "wan1 auth myname %s %s", m_sRouter.sLte.szID, m_sRouter.sLte.szPass);
```

## m_sRouter.sLte.nMTU
```cpp
[PutPp] wsprintf(szBuf, "ip pp mtu %d", (m_sRouter.port[ePortIdxUSB1].kaisen == eKaisenLTE) ? m_sRouter.sLte.nMTU : m_sRouter.nMTU);
```

## m_sRouter.fSetIpTelFlg
```cpp
[その他の関数での出力処理] if ( (m_sRouter.m_nIpTelNo > 0 || m_sRouter.m_nIpTelNoBk > 0) && m_sRouter.fSetIpTelFlg != eIpTelLAN1 )	// IP髮ｻ隧ｱ
[その他の関数での出力処理] m_sRouter.fSetIpTelFlg = eIpTelLAN3;					// IP髮ｻ隧ｱ縺ｯｼｬｼ｡ｼｮｼ薙→縺吶ｋ
[その他の関数での出力処理] m_sRouter.fSetIpTelFlg = eIpTelLAN1;	// ｼｩｼｰ髮ｻ隧ｱ縺鯉ｼｬｼ｡ｼｮｼ代〒險ｭ螳壹＆繧後※縺繧九％縺ｨ縺檎｢ｺ螳
```

## m_sRouter.m_nIpTelNo
```cpp
[PutTunnel] wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nIpTelNo+100);
[PutTunnel] wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc sha-hmac", m_sRouter.m_nIpTelNo+100, m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike encryption %d aes-cbc", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike group %d modp1024", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike hash %d sha", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive log %d off", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 60 10", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf,"ipsec ike pfs %d on", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text CallCentre", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.ptc_ip, NULL));
[PutTunnel] wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nIpTelNo);
[PutTunnel] wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nIpTelNoBk+100);
[PutTunnel] wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc sha-hmac", m_sRouter.m_nIpTelNoBk+100, m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike encryption %d aes-cbc", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike group %d modp1024", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike hash %d sha", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive log %d off", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 60 10", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf,"ipsec ike pfs %d on", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text CallCentre", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.center_ip, NULL));
[PutTunnel] wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nIpTelNoBk);
[PutIpRoute] wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d  keepalive %d gateway tunnel %d weight 0", m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNoBk);
[PutIpRoute] wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d", m_sRouter.m_nIpTelNo);
[PutIpRoute] wsprintf(szBuf, "ip route %s gateway tunnel %d", GetIpB2C(m_sRouter.m_ptcSvIp), m_sRouter.m_nIpTelNo);
[PutIpRoute] wsprintf(szBuf, "ip keepalive %d icmp-echo 60 3 %s", m_sRouter.m_nIpTelNo, GetIpB2C(m_sRouter.m_ptcSvIp));
[PutIpRoute] wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d", m_sRouter.m_nIpTelNoBk);
[その他の関数での出力処理] if ( (m_sRouter.m_nIpTelNo > 0 || m_sRouter.m_nIpTelNoBk > 0) && m_sRouter.fSetIpTelFlg != eIpTelLAN1 )	// IP髮ｻ隧ｱ
[その他の関数での出力処理] if ( m_sRouter.m_nIpTelNo != atoi(p) )	// m_nIpTelNo縺ｯpre髢｢謨ｰ縺ｧ蜈･繧
[その他の関数での出力処理] m_sRouter.m_nIpTelNoBk = atoi(p);
[その他の関数での出力処理] DebDisp("IP髮ｻ隧ｱBK縺ｮtunnel逡ｪ蜿ｷ=%d", m_sRouter.m_nIpTelNoBk);
```

## m_sRouter.m_nIpTelNoBk
```cpp
[PutTunnel] wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nIpTelNoBk+100);
[PutTunnel] wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc sha-hmac", m_sRouter.m_nIpTelNoBk+100, m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike encryption %d aes-cbc", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike group %d modp1024", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike hash %d sha", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive log %d off", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 60 10", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf,"ipsec ike pfs %d on", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text CallCentre", m_sRouter.m_nIpTelNoBk);
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", m_sRouter.m_nIpTelNoBk, GetIpB2C(m_sRouter.center_ip, NULL));
[PutTunnel] wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nIpTelNoBk);
[PutIpRoute] wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d  keepalive %d gateway tunnel %d weight 0", m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNo, m_sRouter.m_nIpTelNoBk);
[PutIpRoute] wsprintf(szBuf, "ip route 172.21.0.0/16 gateway tunnel %d", m_sRouter.m_nIpTelNoBk);
[その他の関数での出力処理] if ( (m_sRouter.m_nIpTelNo > 0 || m_sRouter.m_nIpTelNoBk > 0) && m_sRouter.fSetIpTelFlg != eIpTelLAN1 )	// IP髮ｻ隧ｱ
[その他の関数での出力処理] m_sRouter.m_nIpTelNoBk = atoi(p);
[その他の関数での出力処理] DebDisp("IP髮ｻ隧ｱBK縺ｮtunnel逡ｪ蜿ｷ=%d", m_sRouter.m_nIpTelNoBk);
```

## m_sRouter.m_nCarMentNo
```cpp
[PutTunnel] wsprintf(szBuf, "### TUNNEL %d ###", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf, "tunnel select %d", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf, "ipsec tunnel %d", m_sRouter.m_nCarMentNo+100);
[PutTunnel] wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc sha-hmac", m_sRouter.m_nCarMentNo+100, m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike encryption %d aes-cbc", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike group %d modp1024", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike hash %d sha", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 10 2", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", m_sRouter.m_nCarMentNo, GetIpB2C(m_sRouter.ip));
[PutTunnel] wsprintf(szBuf,"ipsec ike pfs %d on", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text *", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d any", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike remote name %d *", m_sRouter.m_nCarMentNo);
[PutTunnel] wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nCarMentNo);
[PutIpRoute] wsprintf(szBuf, "ip route 10.0.240.0/24 gateway tunnel %d", m_sRouter.m_nCarMentNo);
[その他の関数での出力処理] m_sRouter.m_nCarMentNo = atoi(p);
[その他の関数での出力処理] DebDisp("霆贋ｸ｡邂｡逅繧ｷ繧ｹ繝繝繝｡繝ｳ繝繝翫Φ繧ｹ縺ｮtunnel逡ｪ蜿ｷ=%d", m_sRouter.m_nCarMentNo);
```

## m_sRouter.m_nMSS
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan1 tcp mss limit %d", m_sRouter.m_nMSS);
[その他の関数での出力処理] m_sRouter.m_nMSS = atoi(p);
```

## m_sRouter.m_nJnetNo
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, (m_sRouter.ncon[n].no == m_sRouter.m_nJnetNo) ? IP_JNET_MAIN_IP : IP_JNET_SUB_IP);
[PutTunnel] wsprintf(szBuf,"tunnel backup tunnel %d", m_sRouter.m_nJnetNo2);
[PutIpRoute] wsprintf(szBuf, "ip route 192.168.210.3 gateway tunnel %d", m_sRouter.m_nJnetNo);
[PutIpRoute] wsprintf(szBuf, "ip route 192.168.210.4 gateway tunnel %d", m_sRouter.m_nJnetNo);
```

## m_sRouter.m_nJnetNo2
```cpp
[PutTunnel] wsprintf(szBuf,"tunnel backup tunnel %d", m_sRouter.m_nJnetNo2);
```

## m_sRouter.m_szJnetHoujin
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text %sDN%s", nSetuzokuNo, m_sRouter.m_szJnetHoujin, m_sRouter.m_szJnetHall);
```

## m_sRouter.m_szJnetHall
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text %sDN%s", nSetuzokuNo, m_sRouter.m_szJnetHoujin, m_sRouter.m_szJnetHall);
```

## m_sRouter.m_szJnetVpnName
```cpp
[PutTunnel] wsprintf(szBuf,"ipsec ike local name %d %s key-id", nSetuzokuNo, m_sRouter.m_szJnetVpnName);
```

## m_sRouter.szVPN_User
```cpp
[その他の関数での出力処理] if (!PutFmt("pp auth username %s %s", m_sRouter.szVPN_User, m_sRouter.szVPN_Pass))	return FALSE;
```

## m_sRouter.szVPN_Pass
```cpp
[その他の関数での出力処理] if (!PutFmt("pp auth username %s %s", m_sRouter.szVPN_User, m_sRouter.szVPN_Pass))	return FALSE;
```

## m_sRouter.szVPN_Key
```cpp
[PutTunnel] if (!PutFmt("ipsec ike pre-shared-key %d text %s", nTunnelNo, m_sRouter.szVPN_Key))	return FALSE;
```

## m_sRouter.ip_2_cloud
```cpp
[PutLanIp] wsprintf(szBuf, "ip lan3 address %s/29", GetIpB2C(m_sRouter.ip_2_cloud));
[その他の関数での出力処理] wsprintf(szBuf, "dhcp scope option 2 dns=%s", GetIpB2C(m_sRouter.ip_2_cloud));
```

## m_strADMIN_PASS
```cpp
[PutConfig] str.Format("login password %s", m_strADMIN_PASS);
[PutConfig] str.Format("administrator password %s", m_strADMIN_PASS);
```

## strAngo_Login32
```cpp
[PutConfig] if ( !PutFmt("login password encrypted %s", strAngo_Login32) )
```

## strAngo_Admin32
```cpp
[PutConfig] if ( !PutFmt("administrator password encrypted %s", strAngo_Admin32) )
```

## m_strNewLoginPass
```cpp
[PutConfig] if ( !PutFmt("login password %s", m_strNewLoginPass) )
```

## m_strNewAdminPass
```cpp
[PutConfig] if ( !PutFmt("administrator password %s", m_strNewAdminPass) )
```

## m_cRtPassEx.m_nPH_Code
```cpp
[PutConfig] if ( !PutFmt("snmp syslocation \"Hall %d.%d\"", m_cRtPassEx.m_nPH_Code, nHint) )
```

## nSetuzokuNo
```cpp
[PutTunnel] wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "tunnel select %d", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo + 100);
[PutTunnel] wsprintf(szBuf,"ipsec sa policy %d %d esp aes-cbc md5-hmac", nSetuzokuNo+100, nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike duration isakmp-sa %d 28800", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive log %d off", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
[PutTunnel] wsprintf(szBuf,"ipsec ike local name %d %s key-id", nSetuzokuNo, m_sRouter.m_szJnetVpnName);
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text %sDN%s", nSetuzokuNo, m_sRouter.m_szJnetHoujin, m_sRouter.m_szJnetHall);
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, (m_sRouter.ncon[n].no == m_sRouter.m_nJnetNo) ? IP_JNET_MAIN_IP : IP_JNET_SUB_IP);
[PutTunnel] wsprintf(szBuf,"ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike encryption %d 3des-cbc", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike group %d modp1024", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike hash %d sha", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike pfs %d on", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf,"ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
[PutTunnel] wsprintf(szBuf,"ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.sSIS05.accessIp));
[PutTunnel] wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 10800", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike duration isakmp-sa %d 3600", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike group %d modp768 modp1024", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike keepalive use %d %s", nSetuzokuNo, (m_sRouter.ncon[n].type == eDKVPN || m_sRouter.ncon[n].type == eKobetuVPN) ? "off" : "on");
[PutTunnel] wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip));
[PutTunnel] wsprintf(szBuf, "ipsec ike local id %d %s/%d", nSetuzokuNo, GetIpB2C(m_sRouter.uip), m_nUipMask);
[PutTunnel] wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike pre-shared-key %d text wh1+eb0@rd", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp));
[PutTunnel] wsprintf(szBuf, "ipsec ike remote id %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.con[n].ip, m_sRouter.con[n].mask));
[PutTunnel] wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike keepalive use %d off", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike pre-shared-key %d text *", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, m_szRMAdd);	// GetIpB2C(m_bRMAdd));
[PutTunnel] wsprintf(szBuf, "ipsec ike remote id %d 10.0.224.0/20", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec tunnel %d",nSetuzokuNo + 100);
[PutTunnel] wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac",nSetuzokuNo + 100, nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, m_strPRE_SHARED_KEY);
[PutTunnel] wsprintf(szBuf, "ipsec ike remote address %d any", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike remote name %d *", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 10 2", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac",nSetuzokuNo + 100, nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo);
[PutTunnel] wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo);	// (m_sRouter.ncon[i].port == ePortL2) ? "10 6" : "60 10");
[PutTunnel] wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp));
[PutTunnel] wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].ip));
```

