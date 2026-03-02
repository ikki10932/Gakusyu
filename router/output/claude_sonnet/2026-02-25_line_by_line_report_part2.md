# router01W.dat 行別書き込み解析レポート（パート2: 行201〜400）

---

## TUNNEL 5 続き（行201〜212） ※メンテナンス用VPN

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 201 | `ipsec ike encryption 5 aes-cbc` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike encryption %d aes-cbc", m_sRouter.m_nAdslNo)` |
| 202 | `ipsec ike group 5 modp1024` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", m_sRouter.m_nAdslNo)` |
| 203 | `ipsec ike hash 5 sha` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", m_sRouter.m_nAdslNo)` |
| 204 | `ipsec ike local address 5 172.16.5.1` | `PutTunnel` | `m_sRouter.m_nAdslNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", m_sRouter.m_nAdslNo, GetIpB2C(m_sRouter.ip))` |
| 205 | `ipsec ike pfs 5 on` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", m_sRouter.m_nAdslNo)` |
| 206 | `ipsec ike pre-shared-key 5 text dkccsupport` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text *", m_sRouter.m_nAdslNo)` ※実出力値は"dkccsupport"（固定パスフレーズ） |
| 207 | `ipsec ike remote address 5 any` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike remote address %d any", m_sRouter.m_nAdslNo)` |
| 208 | `ipsec ike remote name 5 ccsupport` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ipsec ike remote name %d *", m_sRouter.m_nAdslNo)` ※実出力値は"ccsupport" |
| 209 | `ipsec ike keepalive use 5 on heartbeat 10 2` | `PutTunnel` | `m_sRouter.m_nAdslNo` or `m_sRouter.m_nCarMentNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 10 2", ...)` |
| 210 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 211 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 212 | `tunnel enable 5` | `PutTunnel` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "tunnel enable %d", m_sRouter.m_nAdslNo)` |

---

## TUNNEL 4（行213〜232） ※バックアップトンネル

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 213 | （空行） | − | − | − |
| 214 | `### TUNNEL 4 ###` | `PutTunnel` | `nSetuzokuNo`(=4) | `wsprintf(szBuf, "### TUNNEL %d ###", nSetuzokuNo)` |
| 215 | （空行） | − | − | − |
| 216 | `tunnel select 4` | `PutTunnel` | `nSetuzokuNo`(=4) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 217 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 218 | `ipsec tunnel 104` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 219 | `ipsec sa policy 104 4 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 220 | `ipsec ike duration ipsec-sa 4 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` |
| 221 | `ipsec ike encryption 4 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` |
| 222 | `ipsec ike group 4 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` |
| 223 | `ipsec ike hash 4 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` |
| 224 | `ipsec ike keepalive use 4 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` |
| 225 | `ipsec ike local address 4 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` |
| 226 | `ipsec ike pfs 4 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` |
| 227 | `ipsec ike pre-shared-key 4 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` |
| 228 | `ipsec ike remote address 4 61.114.186.110` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 229 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 230 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 231 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 232 | `tunnel enable 4` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |

---

## TUNNEL 16〜20（行233〜302） ※L2TP/IPsec VPN（5本同パターン）

> **注**: TUNNEL 16〜20 は同一パターンの繰り返し。番号のみ異なる（nSetuzokuNo）。  
> 使用メンバ変数は `nSetuzokuNo`（ループ変数）、`m_sRouter.szVPN_Key`（PSK値）。

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 233 | `tunnel select 16` | `PutTunnel` | `nSetuzokuNo`(=16) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 234 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 235 | `ipsec tunnel 116` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 236 | `ipsec sa policy 116 16 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp aes-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 237 | `ipsec ike keepalive use 16 off` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d off", nSetuzokuNo)` |
| 238 | `ipsec ike nat-traversal 16 on` | `PutTunnel` | `nSetuzokuNo` | 固定文字列にnSetuzokuNo埋め込み |
| 239 | `ipsec ike pre-shared-key 16 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | `PutFmt("ipsec ike pre-shared-key %d text %s", nTunnelNo, m_sRouter.szVPN_Key)` |
| 240 | `ipsec ike remote address 16 any` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike remote address %d any", nSetuzokuNo)` |
| 241 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 242 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 243 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 244 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 245 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 246 | `tunnel enable 16` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |
| 247 | `tunnel select 17` | `PutTunnel` | `nSetuzokuNo`(=17) | 同上パターン |
| 248 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 249 | `ipsec tunnel 117` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 250 | `ipsec sa policy 117 17 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 251 | `ipsec ike keepalive use 17 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 252 | `ipsec ike nat-traversal 17 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 253 | `ipsec ike pre-shared-key 17 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン |
| 254 | `ipsec ike remote address 17 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 255 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 256 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 257 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 258 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 259 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 260 | `tunnel enable 17` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 261 | `tunnel select 18` | `PutTunnel` | `nSetuzokuNo`(=18) | 同上パターン |
| 262 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 263 | `ipsec tunnel 118` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 264 | `ipsec sa policy 118 18 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 265 | `ipsec ike keepalive use 18 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 266 | `ipsec ike nat-traversal 18 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 267 | `ipsec ike pre-shared-key 18 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン |
| 268 | `ipsec ike remote address 18 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 269 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 270 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 271 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 272 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 273 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 274 | `tunnel enable 18` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 275 | `tunnel select 19` | `PutTunnel` | `nSetuzokuNo`(=19) | 同上パターン |
| 276 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 277 | `ipsec tunnel 119` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 278 | `ipsec sa policy 119 19 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 279 | `ipsec ike keepalive use 19 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 280 | `ipsec ike nat-traversal 19 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 281 | `ipsec ike pre-shared-key 19 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン |
| 282 | `ipsec ike remote address 19 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 283 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 284 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 285 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 286 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 287 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 288 | `tunnel enable 19` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 289 | `tunnel select 20` | `PutTunnel` | `nSetuzokuNo`(=20) | 同上パターン |
| 290 | `tunnel encapsulation l2tp` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 291 | `ipsec tunnel 120` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 292 | `ipsec sa policy 120 20 esp aes-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 293 | `ipsec ike keepalive use 20 off` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 294 | `ipsec ike nat-traversal 20 on` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 295 | `ipsec ike pre-shared-key 20 text e6BQyqm7oTS66KtZPm$ZPP$hwcU7t8rK` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.szVPN_Key` | 同上パターン |
| 296 | `ipsec ike remote address 20 any` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |
| 297 | `l2tp tunnel auth off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 298 | `l2tp tunnel disconnect time off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 299 | `l2tp keepalive use on` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 300 | `ip tunnel tcp mss limit auto` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 301 | `ip tunnel secure filter name xai_tunnel_lan2` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 302 | `tunnel enable 20` | `PutTunnel` | `nSetuzokuNo` | 同上パターン |

---

## IPSec transport設定（行303〜310）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 303 | （空行） | − | − | − |
| 304 | `### IPSEC ###` | `PutTunnel` | 固定文字列 | `_PutTit(tit)` セクションタイトル |
| 305 | （空行） | − | − | − |
| 306 | `ipsec transport 16 116 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=16) | 固定文字列にnSetuzokuNo埋め込み（pCmd03または固定配列） |
| 307 | `ipsec transport 17 117 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=17) | 同上パターン |
| 308 | `ipsec transport 18 118 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=18) | 同上パターン |
| 309 | `ipsec transport 19 119 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=19) | 同上パターン |
| 310 | `ipsec transport 20 120 udp 1701` | `PutTunnel` | `nSetuzokuNo`(=20) | 同上パターン |

---

## IPフィルター設定ヘッダー（行311〜315）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 311 | （空行） | − | − | − |
| 312 | `# IP filter configuration` | `PutFilter` | 固定文字列 | `_PutTit(tit)` セクションタイトル (Router1200.cpp:3338) |
| 313 | （空行） | − | − | − |
| 314 | `ip filter source-route on` | `PutFilter` | 固定文字列 | pCmd02配列要素 (Router1200.cpp:3365) |
| 315 | `ip filter directed-broadcast on` | `PutFilter` | 固定文字列 | 同上 pCmd02配列要素 |

---

## TUNNEL 2（行316〜333） ※本線VPNトンネル

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 316 | `tunnel select 2` | `PutTunnel` | `nSetuzokuNo`(=2) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 317 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 318 | `ipsec tunnel 102` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 319 | `ipsec sa policy 102 2 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 320 | `ipsec ike duration ipsec-sa 2 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` |
| 321 | `ipsec ike encryption 2 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` |
| 322 | `ipsec ike group 2 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` |
| 323 | `ipsec ike hash 2 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` |
| 324 | `ipsec ike keepalive use 2 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` |
| 325 | `ipsec ike local address 2 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` |
| 326 | `ipsec ike pfs 2 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` |
| 327 | `ipsec ike pre-shared-key 2 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` |
| 328 | `ipsec ike remote address 2 192.0.2.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 329 | `tunnel backup tunnel 8 switch-interface=on` | `PutTunnel` | `m_sRouter.m_nDeltaBkNo`(=8) | `wsprintf(szBuf, "tunnel backup tunnel %d switch-interface=on", m_sRouter.m_nDeltaBkNo)` |
| 330 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 331 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 332 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 333 | `tunnel enable 2` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |

---

## TUNNEL 8（行334〜350） ※バックアップ本線VPN

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 334 | `tunnel select 8` | `PutTunnel` | `nSetuzokuNo`(=8) | `wsprintf(szBuf, "tunnel select %d", nSetuzokuNo)` |
| 335 | `tunnel encapsulation ipsec` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 336 | `ipsec tunnel 108` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec tunnel %d", nSetuzokuNo+100)` |
| 337 | `ipsec sa policy 108 8 esp 3des-cbc sha-hmac` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec sa policy %d %d esp 3des-cbc sha-hmac", nSetuzokuNo+100, nSetuzokuNo)` |
| 338 | `ipsec ike duration ipsec-sa 8 28800` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike duration ipsec-sa %d 28800", nSetuzokuNo)` |
| 339 | `ipsec ike encryption 8 3des-cbc` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike encryption %d 3des-cbc", nSetuzokuNo)` |
| 340 | `ipsec ike group 8 modp1024` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike group %d modp1024", nSetuzokuNo)` |
| 341 | `ipsec ike hash 8 sha` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike hash %d sha", nSetuzokuNo)` |
| 342 | `ipsec ike keepalive use 8 on heartbeat 60 10` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike keepalive use %d on heartbeat 60 10", nSetuzokuNo)` |
| 343 | `ipsec ike local address 8 172.16.5.1` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ip` | `wsprintf(szBuf, "ipsec ike local address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ip))` |
| 344 | `ipsec ike pfs 8 on` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "ipsec ike pfs %d on", nSetuzokuNo)` |
| 345 | `ipsec ike pre-shared-key 8 text 153.142.244.124` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[i].fixIp` | `wsprintf(szBuf, "ipsec ike pre-shared-key %d text %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[i].fixIp))` |
| 346 | `ipsec ike remote address 8 192.0.2.254` | `PutTunnel` | `nSetuzokuNo`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "ipsec ike remote address %d %s", nSetuzokuNo, GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 347 | `ip tunnel nat descriptor 2012` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 348 | `ip tunnel rip send off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 349 | `ip tunnel rip receive off` | `PutTunnel` | 固定文字列 | 固定コマンド |
| 350 | `tunnel enable 8` | `PutTunnel` | `nSetuzokuNo` | `wsprintf(szBuf, "tunnel enable %d", nSetuzokuNo)` |

---

## IPフィルター個別エントリ（行351〜400）

> **注**: `ip filter` 系エントリの多くは `PutFilter` 関数内でフォーマット文字列を生成。  
> IPアドレス部に `m_sRouter.ip`（=172.16.5.1）や `m_sRouter.mask`（=172.16.0.0/16相当）が使われる場合と、 pCmd配列の固定文字列の場合がある。

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 351 | `ip filter 2602 pass 192.0.2.1 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | `wsprintf(szPut, "ip filter 2602 pass %s %s udp * 500", ...)` ncon[n].fixIp + m_sRouter.ip |
| 352 | `ip filter 2608 pass 192.0.2.254 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | 同上（tunnel 8用） |
| 353 | `ip filter 2652 pass 172.16.5.1 192.0.2.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | 同上（逆方向） |
| 354 | `ip filter 2658 pass 172.16.5.1 192.0.2.254 udp * 500` | `PutFilter` | `m_sRouter.ip` | 同上 |
| 355 | `ip filter 2702 pass 192.0.2.1 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上（ESP） |
| 356 | `ip filter 2708 pass 192.0.2.254 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上 |
| 357 | `ip filter 2752 pass 172.16.5.1 192.0.2.1 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上（逆方向） |
| 358 | `ip filter 2758 pass 172.16.5.1 192.0.2.254 esp * *` | `PutFilter` | `m_sRouter.ip` | 同上 |
| 359 | （空行） | − | − | − |
| 360 | `ip filter 1 reject * * udp,tcp netbios_ns-netbios_ssn *` | `PutFilter` | 固定文字列 | pCmd01配列要素 (Router1200.cpp:3365) |
| 361 | `ip filter 2 reject * * udp,tcp * netbios_ns-netbios_ssn` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 |
| 362 | `ip filter 3 reject * * udp,tcp 135 *` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 |
| 363 | `ip filter 4 reject * * udp,tcp * 135` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 |
| 364 | `ip filter 5 reject * * udp,tcp 445 *` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 |
| 365 | `ip filter 6 reject * * udp,tcp * 445` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 |
| 366 | `ip filter 9 reject * * * * *` | `PutFilter` | 固定文字列 | 同上 pCmd01配列要素 |
| 367 | `ip filter 10 reject 172.16.0.0/16 * * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 10 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 368 | `ip filter 15 reject * 172.16.0.0/16 * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 15 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 369 | `ip filter 20 pass 211.129.14.134,211.129.12.43 172.16.0.0/16 udp domain *` | `PutFilter` | `m_sRouter.mask` | pCmd02配列: 固定IP + `GetIpB2C(bMaskedIP, m_sRouter.mask)` |
| 370 | `ip filter 21 pass 211.129.14.134,211.129.12.43 172.16.5.0/24 udp domain *` | `PutFilter` | `m_sRouter.ip` | 固定IPと m_sRouter.ip から生成 |
| 371 | `ip filter 22 pass 211.129.14.134,211.129.12.43 172.16.15.0/24 udp domain *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 372 | `ip filter 23 pass 211.129.14.134,211.129.12.43 172.16.60.0/24 udp domain *` | `PutFilter` | 固定文字列 | 同上 |
| 373 | `ip filter 25 pass 172.16.0.0/16 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 374 | `ip filter 26 pass 172.16.5.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip 派生 + 固定IP |
| 375 | `ip filter 27 pass 172.16.15.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 376 | `ip filter 28 pass 172.16.60.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | 同上 |
| 377 | `ip filter 30 pass 61.114.182.53 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 378 | `ip filter 31 pass 61.114.182.53 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | 固定IP + m_sRouter.ip派生 |
| 379 | `ip filter 32 pass 61.114.182.53 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 380 | `ip filter 33 pass 61.114.182.53 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 |
| 381 | `ip filter 35 pass 172.16.0.0/16 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 382 | `ip filter 36 pass 172.16.5.0/24 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 383 | `ip filter 37 pass 172.16.15.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 384 | `ip filter 38 pass 172.16.60.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | 同上 |
| 385 | `ip filter 40 pass 61.114.182.54 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 386 | `ip filter 41 pass 61.114.182.54 172.16.5.0/24 tcp www *` | `PutFilter` | `m_sRouter.ip` | 同上 m_sRouter.ip派生 |
| 387 | `ip filter 42 pass 61.114.182.54 172.16.15.0/24 tcp www *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 388 | `ip filter 43 pass 61.114.182.54 172.16.60.0/24 tcp www *` | `PutFilter` | 固定文字列 | 同上 |
| 389 | `ip filter 45 pass 172.16.0.0/16 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 390 | `ip filter 46 pass 172.16.5.0/24 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 391 | `ip filter 47 pass 172.16.15.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 392 | `ip filter 48 pass 172.16.60.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | 同上 |
| 393 | `ip filter 50 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 394 | `ip filter 51 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | 同上 m_sRouter.ip派生 |
| 395 | `ip filter 52 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 396 | `ip filter 53 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 397 | `ip filter 54 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 398 | `ip filter 55 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 399 | `ip filter 56 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 400 | `ip filter 57 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
