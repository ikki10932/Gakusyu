# router01W.dat 行別書き込み解析レポート（パート3: 行401〜600）

---

## IPフィルター 個別エントリ続き（行401〜530）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 401 | `ip filter 58 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 402 | `ip filter 59 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 403 | `ip filter 60 pass 61.114.177.148 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 404 | `ip filter 61 pass 61.114.177.148 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 405 | `ip filter 62 pass 61.114.177.148 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 406 | `ip filter 63 pass 61.114.177.148 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 |
| 407 | `ip filter 65 pass 172.16.0.0/16 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 408 | `ip filter 66 pass 172.16.5.0/24 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 409 | `ip filter 67 pass 172.16.15.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 410 | `ip filter 68 pass 172.16.60.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | 同上 |
| 411 | `ip filter 70 pass 202.229.106.177 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 412 | `ip filter 71 pass 202.229.106.177 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 413 | `ip filter 72 pass 202.229.106.177 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 414 | `ip filter 73 pass 202.229.106.177 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 415 | `ip filter 74 pass 202.229.106.177 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 416 | `ip filter 75 pass 172.16.0.0/16 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 417 | `ip filter 76 pass 172.16.5.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 418 | `ip filter 77 pass 172.16.15.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 419 | `ip filter 78 pass 172.16.60.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 420 | `ip filter 79 pass 172.16.80.0/22 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 421 | `ip filter 80 pass 202.226.76.11 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 80 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3370) |
| 422 | `ip filter 81 pass 202.226.76.11 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 81 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3372) |
| 423 | `ip filter 82 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 424 | `ip filter 83 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | `PutFilter` | 固定文字列 | `_Put("ip filter 83 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *")` (Router1200.cpp:3477) |
| 425 | `ip filter 84 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | `PutFilter` | 固定文字列 | `_Put("ip filter 84 pass * 172.16.121.90-...")` (Router1200.cpp:3480) |
| 426 | `ip filter 85 pass 172.16.0.0/16 202.226.76.11 tcp * https` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 85 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3484) |
| 427 | `ip filter 86 pass 172.16.0.0/16 202.226.76.11 tcp * www` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 86 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3486) |
| 428 | `ip filter 87 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 (Router1200.cpp:3488) |
| 429 | `ip filter 88 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | `PutFilter` | 固定文字列 | `_Put("ip filter 88 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https")` (Router1200.cpp:3490) |
| 430 | `ip filter 89 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | `PutFilter` | 固定文字列 | `_Put("ip filter 89 pass ...")` (Router1200.cpp:3493) |
| 431 | `ip filter 92 pass * 172.16.5.0/24 tcp https,5222,5223 *` | `PutFilter` | `m_sRouter.ip` | `wsprintf(szPut, "ip filter 92 pass * %s tcp https,5222,5223 *",...)` m_sRouter.ip派生 |
| 432 | `ip filter 93 pass * 172.16.5.0/24 udp domain,ntp *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 433 | `ip filter 94 pass * 172.16.5.0/24 icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 434 | `ip filter 97 pass 172.16.5.0/24 * tcp * https,5222,5223` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 435 | `ip filter 98 pass 172.16.5.0/24 * udp * domain,ntp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 436 | `ip filter 99 pass 172.16.5.0/24 * icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 437 | `ip filter 100 pass-log * * * * *` | `PutFilter` | 固定文字列 | pCmd01配列固定要素 |
| 438 | `ip filter 101 reject * * established * netbios_ns-netbios_ssn` | `PutFilter` | 固定文字列 | pCmd01配列固定要素 |
| 439 | `ip filter 102 reject * * established * 135` | `PutFilter` | 固定文字列 | 同上 |
| 440 | `ip filter 103 reject * * established * 445` | `PutFilter` | 固定文字列 | 同上 |
| 441 | `ip filter 104 reject * * established * https` | `PutFilter` | 固定文字列 | 同上 |
| 442 | `ip filter 105 reject * * established * www` | `PutFilter` | 固定文字列 | 同上 |
| 443 | `ip filter 106 reject * * established * pop3` | `PutFilter` | 固定文字列 | 同上 |
| 444 | `ip filter 107 reject * * established * smtp` | `PutFilter` | 固定文字列 | 同上 |
| 445 | `ip filter 1400 pass 172.16.0.0/16 * * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 1400 pass %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 446 | `ip filter 1401 pass 172.21.0.0/16 * * * *` | `PutFilter` | 固定文字列 | `_Put("ip filter 1401 pass 172.21.0.0/16 * * * *")` (Router1200.cpp:3548) |
| 447 | `ip filter 1405 pass * 172.16.0.0/16 * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 1405 pass * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 448 | `ip filter 1406 pass * 172.21.0.0/16 * * *` | `PutFilter` | 固定文字列 | `_Put("ip filter 1406 pass * 172.21.0.0/16 * * *")` (Router1200.cpp:3558) |
| 449 | `ip filter 1410 pass * * udp dhcpc dhcps` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 (Router1200.cpp:3553) |
| 450 | `ip filter 1430 pass 10.0.0.0/8 10.0.208.0/20 * * *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 451 | `ip filter 1431 pass 10.0.208.0/20 10.0.0.0/8 * * *` | `PutFilter` | 固定文字列 | 同上 |
| 452 | `ip filter 1432 pass 10.0.0.0/8 172.30.1.110 tcp * 21` | `PutFilter` | 固定文字列 | 同上 |
| 453 | `ip filter 1433 pass 172.30.1.110 10.0.0.0/8 tcp 21 *` | `PutFilter` | 固定文字列 | 同上 |
| 454 | `ip filter 1434 pass 10.0.0.0/8 172.30.1.110 tcp * ftpdata` | `PutFilter` | 固定文字列 | 同上 |
| 455 | `ip filter 1435 pass 172.30.1.110 10.0.0.0/8 tcp ftpdata *` | `PutFilter` | 固定文字列 | 同上 |
| 456 | `ip filter 1436 pass 10.0.0.0/8 172.31.191.0/24 * * *` | `PutFilter` | 固定文字列 | 同上 |
| 457 | `ip filter 1437 pass 172.31.191.0/24 10.0.0.0/8 * * *` | `PutFilter` | 固定文字列 | 同上 |
| 458 | `ip filter 1438 pass 10.0.0.0/8 10.0.0.0/8 * * *` | `PutFilter` | 固定文字列 | 同上 |
| 459 | `ip filter 1440 pass 172.18.0.0/24 10.0.192.0/18 * * *` | `PutFilter` | 固定文字列 | 同上 |
| 460 | `ip filter 1441 pass 10.0.192.0/18 172.18.0.0/24 * * *` | `PutFilter` | 固定文字列 | 同上 |
| 461 | `ip filter 2010 reject 172.16.0.0/16 * * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2010 reject %s * * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 462 | `ip filter 2015 reject * 172.16.0.0/16 * * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2015 reject * %s * * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 463 | `ip filter 2025 pass 172.16.0.0/16 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 464 | `ip filter 2026 pass * 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 465 | `ip filter 2027 pass 172.16.15.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | 同上 |
| 466 | `ip filter 2028 pass 172.16.60.0/24 211.129.14.134,211.129.12.43 udp * domain` | `PutFilter` | 固定文字列 | 同上 |
| 467 | `ip filter 2030 pass 61.114.182.53 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 468 | `ip filter 2031 pass 61.114.182.53 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 469 | `ip filter 2032 pass 61.114.182.53 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 470 | `ip filter 2033 pass 61.114.182.53 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 |
| 471 | `ip filter 2035 pass 172.16.0.0/16 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 472 | `ip filter 2036 pass 172.16.5.0/24 61.114.182.53 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 473 | `ip filter 2037 pass 172.16.15.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 474 | `ip filter 2038 pass 172.16.60.0/24 61.114.182.53 tcp * https` | `PutFilter` | 固定文字列 | 同上 |
| 475 | `ip filter 2040 pass 61.114.182.54 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 476 | `ip filter 2041 pass 61.114.182.54 172.16.5.0/24 tcp www *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 477 | `ip filter 2042 pass 61.114.182.54 172.16.15.0/24 tcp www *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 478 | `ip filter 2043 pass 61.114.182.54 172.16.60.0/24 tcp www *` | `PutFilter` | 固定文字列 | 同上 |
| 479 | `ip filter 2045 pass 172.16.0.0/16 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 480 | `ip filter 2046 pass 172.16.5.0/24 61.114.182.54 tcp * www` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 481 | `ip filter 2047 pass 172.16.15.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 482 | `ip filter 2048 pass 172.16.60.0/24 61.114.182.54 tcp * www` | `PutFilter` | 固定文字列 | 同上 |
| 483 | `ip filter 2050 pass 61.114.186.0/25 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 484 | `ip filter 2051 pass 61.114.186.0/25 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 485 | `ip filter 2052 pass 61.114.186.0/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 486 | `ip filter 2053 pass 61.114.186.0/25 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 487 | `ip filter 2054 pass 61.114.186.0/25 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 488 | `ip filter 2055 pass 172.16.0.0/16 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 489 | `ip filter 2056 pass 172.16.5.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 490 | `ip filter 2057 pass 172.16.15.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 491 | `ip filter 2058 pass 172.16.60.0/24 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 492 | `ip filter 2059 pass 172.16.80.0/22 61.114.186.0/25 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 493 | `ip filter 2060 pass 61.114.177.148 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 494 | `ip filter 2061 pass 61.114.177.148 172.16.5.0/24 tcp https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 495 | `ip filter 2062 pass 61.114.177.148 172.16.15.0/24 tcp https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 496 | `ip filter 2063 pass 61.114.177.148 172.16.60.0/24 tcp https *` | `PutFilter` | 固定文字列 | 同上 |
| 497 | `ip filter 2065 pass 172.16.0.0/16 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 498 | `ip filter 2066 pass 172.16.5.0/24 61.114.177.148 tcp * https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 499 | `ip filter 2067 pass 172.16.15.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 500 | `ip filter 2068 pass 172.16.60.0/24 61.114.177.148 tcp * https` | `PutFilter` | 固定文字列 | 同上 |
| 501 | `ip filter 2070 pass 202.229.106.177 172.16.0.0/16 tcp www,https *` | `PutFilter` | `m_sRouter.mask` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 502 | `ip filter 2071 pass 202.229.106.177 172.16.5.0/24 tcp www,https *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 503 | `ip filter 2072 pass 202.229.106.177 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 504 | `ip filter 2073 pass 202.229.106.177 172.16.60.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 505 | `ip filter 2074 pass 202.229.106.177 172.16.80.0/22 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 506 | `ip filter 2075 pass 172.16.0.0/16 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 507 | `ip filter 2076 pass 172.16.5.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 508 | `ip filter 2077 pass 172.16.15.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 509 | `ip filter 2078 pass 172.16.60.0/24 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 510 | `ip filter 2079 pass 172.16.80.0/22 202.229.106.177 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 511 | `ip filter 2080 pass 202.226.76.11 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 512 | `ip filter 2081 pass 202.226.76.11 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 513 | `ip filter 2082 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 514 | `ip filter 2083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 515 | `ip filter 2084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | `PutFilter` | 固定文字列 | 固定文字列 |
| 516 | `ip filter 2085 pass 172.16.0.0/16 202.226.76.11 tcp * https` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 517 | `ip filter 2086 pass 172.16.0.0/16 202.226.76.11 tcp * www` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 518 | `ip filter 2087 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 519 | `ip filter 2088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 520 | `ip filter 2089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 521 | `ip filter 2092 pass * 172.16.5.0/24 tcp https,5222,5223 *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3499) |
| 522 | `ip filter 2093 pass * 172.16.5.0/24 udp domain,ntp *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3501) |
| 523 | `ip filter 2094 pass * 172.16.5.0/24 icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3506) |
| 524 | `ip filter 2097 pass 172.16.5.0/24 * tcp * https,5222,5223` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3508) |
| 525 | `ip filter 2098 pass 172.16.5.0/24 * udp * domain,ntp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 526 | `ip filter 2099 pass 172.16.5.0/24 * icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 527 | `ip filter 2200 pass * 172.16.5.1 tcp * 1723` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2200 pass * %s tcp * 1723", GetIpB2C(m_sRouter.ip))` |
| 528 | `ip filter 2250 pass 172.16.5.1 * tcp 1723 *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2250 pass %s * tcp 1723 *", GetIpB2C(m_sRouter.ip))` |
| 529 | `ip filter 2300 pass * 172.16.5.1 gre * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2300 pass * %s gre * *", GetIpB2C(m_sRouter.ip))` |
| 530 | `ip filter 2350 pass 172.16.5.1 * gre * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 2350 pass %s * gre * *", GetIpB2C(m_sRouter.ip))` |

---

## IPフィルター VPN/IPsec用（行531〜554）

> **注**: 行531,537,543,549 は非常に長いIPアドレスリスト（XAI/DKVPNサーバー群）を含む。  
> これらは `pCmd02_1`/`pCmd02_2` 配列の固定文字列 (Router1200.cpp:3519,3532) に `m_sRouter.ip` を合成している。

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 531 | `ip filter 2600 pass [長IPリスト] 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | `_Put(pCmd02_1, ...)` pCmd02_1配列要素 + m_sRouter.ip (Router1200.cpp:3519) |
| 532 | `ip filter 2601 pass 61.114.186.111 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | `wsprintf(szPut, "ip filter 2601 pass %s %s udp * 500", GetIpB2C(m_sRouter.ncon[n].fixIp), GetIpB2C(m_sRouter.ip))` |
| 533 | `ip filter 2603 pass 52.197.151.142 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 3用） |
| 534 | `ip filter 2604 pass 61.114.186.110 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 4用） |
| 535 | `ip filter 2606 pass 153.142.244.126 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 6用） |
| 536 | `ip filter 2607 pass 153.142.244.125 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン（tunnel 7用） |
| 537 | `ip filter 2650 pass 172.16.5.1 [長IPリスト] udp * 500` | `PutFilter` | `m_sRouter.ip` | `_Put(pCmd02_2, ...)` pCmd02_2配列要素 + m_sRouter.ip (Router1200.cpp:3532) |
| 538 | `ip filter 2651 pass 172.16.5.1 61.114.186.111 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 逆方向（PuFilterでm_sRouter.ip + ncon[n].fixIp） |
| 539 | `ip filter 2653 pass 172.16.5.1 52.197.151.142 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン |
| 540 | `ip filter 2654 pass 172.16.5.1 61.114.186.110 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン |
| 541 | `ip filter 2656 pass 172.16.5.1 153.142.244.126 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン |
| 542 | `ip filter 2657 pass 172.16.5.1 153.142.244.125 udp * 500` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上パターン |
| 543 | `ip filter 2700 pass [長IPリスト] 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd02_1配列 + m_sRouter.ip (ESP版) (Router1200.cpp:3519) |
| 544 | `ip filter 2701 pass 61.114.186.111 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同様パターン（ESP) |
| 545 | `ip filter 2703 pass 52.197.151.142 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 546 | `ip filter 2704 pass 61.114.186.110 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 547 | `ip filter 2706 pass 153.142.244.126 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 548 | `ip filter 2707 pass 153.142.244.125 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 549 | `ip filter 2750 pass 172.16.5.1 [長IPリスト] esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd02_2配列 + m_sRouter.ip (ESP逆方向) (Router1200.cpp:3532) |
| 550 | `ip filter 2751 pass 172.16.5.1 61.114.186.111 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上（逆方向ESP） |
| 551 | `ip filter 2753 pass 172.16.5.1 52.197.151.142 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 552 | `ip filter 2754 pass 172.16.5.1 61.114.186.110 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 553 | `ip filter 2756 pass 172.16.5.1 153.142.244.126 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 554 | `ip filter 2757 pass 172.16.5.1 153.142.244.125 esp * *` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.ncon[n].fixIp` | 同上 |

---

## IPフィルター ICMP/XAI/LAN系（行555〜600）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 555 | `ip filter 2900 pass * 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2900 pass * %s icmp * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` (Router1200.cpp:3543) |
| 556 | `ip filter 2901 pass 61.114.186.111 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 固定IP + GetIpB2C(bMaskedIP, m_sRouter.mask) |
| 557 | `ip filter 2903 pass 52.197.151.142 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 558 | `ip filter 2904 pass 61.114.186.110 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 559 | `ip filter 2906 pass 153.142.244.126 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 560 | `ip filter 2907 pass 153.142.244.125 172.16.0.0/16 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 561 | `ip filter 2950 pass 172.16.0.0/16 * icmp * *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 2950 pass %s * icmp * *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 562 | `ip filter 2951 pass 172.16.0.0/16 61.114.186.111 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 563 | `ip filter 2953 pass 172.16.0.0/16 52.197.151.142 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 564 | `ip filter 2954 pass 172.16.0.0/16 61.114.186.110 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 565 | `ip filter 2956 pass 172.16.0.0/16 153.142.244.126 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 566 | `ip filter 2957 pass 172.16.0.0/16 153.142.244.125 icmp * *` | `PutFilter` | `m_sRouter.mask`, `m_sRouter.ncon[n].fixIp` | 同上 |
| 567 | `ip filter 3080 pass 202.226.76.11 172.16.0.0/16 tcp https *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3080 pass 202.226.76.11 %s tcp https *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 568 | `ip filter 3081 pass 202.226.76.11 172.16.0.0/16 tcp www *` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3081 pass 202.226.76.11 %s tcp www *", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 569 | `ip filter 3082 pass 113.52.158.128/25 172.16.15.0/24 tcp www,https *` | `PutFilter` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3577) |
| 570 | `ip filter 3083 pass 182.248.135.70,106.186.75.198 172.16.0.0/16 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 571 | `ip filter 3084 pass * 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 572 | `ip filter 3085 pass 172.16.0.0/16 202.226.76.11 tcp * https` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3085 pass %s 202.226.76.11 tcp * https", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 573 | `ip filter 3086 pass 172.16.0.0/16 202.226.76.11 tcp * www` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter 3086 pass %s 202.226.76.11 tcp * www", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 574 | `ip filter 3087 pass 172.16.15.0/24 113.52.158.128/25 tcp * www,https` | `PutFilter` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3586) |
| 575 | `ip filter 3088 pass 172.16.0.0/16 182.248.135.70,106.186.75.198 tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 576 | `ip filter 3089 pass 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 577 | `ip filter 3092 pass * 172.16.5.0/24 tcp https,5222,5223 *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3594) |
| 578 | `ip filter 3093 pass * 172.16.5.0/24 udp domain,ntp *` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3596) |
| 579 | `ip filter 3094 pass * 172.16.5.0/24 icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 (Router1200.cpp:3601) |
| 580 | `ip filter 3097 pass 172.16.5.0/24 * tcp * https,5222,5223` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 581 | `ip filter 3098 pass 172.16.5.0/24 * udp * domain,ntp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 582 | `ip filter 3099 pass 172.16.5.0/24 * icmp` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 583 | `ip filter 101402 pass * - udp ntp *` | `PutFilter` | 固定文字列 | pCmd配列固定要素 |
| 584 | `ip filter 101404 pass * - tcp www,https *` | `PutFilter` | 固定文字列 | 同上 |
| 585 | `ip filter 101406 pass - 180.55.241.146 udp * *` | `PutFilter` | 固定文字列 | 同上 |
| 586 | `ip filter 101407 pass - * udp * ntp` | `PutFilter` | 固定文字列 | 同上 |
| 587 | `ip filter 101408 pass - * udp * domain` | `PutFilter` | 固定文字列 | 同上 |
| 588 | `ip filter 101409 pass - * tcp * www,https` | `PutFilter` | 固定文字列 | 同上 |
| 589 | `ip filter 10800 pass 18.179.137.187,3.115.131.124 172.16.15.0/24 tcp www,https,8080 *` | `PutFilter` | 固定文字列 | pCmd配列固定要素 (Router1200.cpp:3577付近) |
| 590 | `ip filter 10801 pass 172.16.15.0/24 18.179.137.187,3.115.131.124 tcp * www,https,8080` | `PutFilter` | 固定文字列 | 同上 |
| 591 | `ip filter 10500 pass * 172.16.5.1 tcp * 53196` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.nLuaPort` | `PutFmt("ip filter 10500 pass * %s tcp * %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort)` |
| 592 | `ip filter 10505 pass 172.16.5.1 * tcp 53196` | `PutFilter` | `m_sRouter.ip`, `m_sRouter.nLuaPort` | `PutFmt("ip filter 10505 pass %s * tcp %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort)` |
| 593 | `ip filter 10510 pass 172.16.5.1 172.16.5.1 udp * 500` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10510 pass %s %s udp * 500", GetLuaIP(), GetIpB2C(m_sRouter.ip))` |
| 594 | `ip filter 10520 pass 172.16.5.1 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10520 pass %s %s esp * *", GetLuaIP(), GetIpB2C(m_sRouter.ip))` |
| 595 | `ip filter 10525 pass 172.16.5.1 172.16.5.1 esp * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10525 pass %s %s esp * *", GetIpB2C(m_sRouter.ip), GetLuaIP())` |
| 596 | `ip filter 10550 pass 172.16.5.1 172.16.5.1 udp * 1701` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10550 pass %s %s udp * 1701", GetLuaIP(), GetIpB2C(m_sRouter.ip))` |
| 597 | `ip filter 10560 pass 172.16.5.1 172.16.5.1 udp * 4500` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10560 pass %s %s udp * 4500", GetLuaIP(), GetIpB2C(m_sRouter.ip))` |
| 598 | `ip filter 10575 pass 172.16.5.1 172.16.5.1 udp * *` | `PutFilter` | `m_sRouter.ip` | `PutFmt("ip filter 10575 pass %s %s udp * *", GetIpB2C(m_sRouter.ip), GetLuaIP())` |
| 599 | `ip filter 10600 pass 172.16.60.0/24 172.16.60.0/24 tcp * 58300` | `PutFilter` | 固定文字列 | pCmd配列固定要素 |
| 600 | `ip filter 10605 pass 172.16.60.0/24 172.16.60.0/24 tcp 58300 *` | `PutFilter` | 固定文字列 | 同上 |
