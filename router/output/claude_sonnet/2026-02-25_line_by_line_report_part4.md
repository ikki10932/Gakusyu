# router01W.dat 行別書き込み解析レポート（パート4: 行601〜791 最終）

---

## IPフィルター 続き（行601〜615）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 601 | `ip filter 10610 pass 172.16.60.0/24 172.16.60.0/24 udp 58301 *` | `PutFilter` | 固定文字列 | pCmd配列固定要素 |
| 602 | `ip filter 10615 pass 172.16.60.0/24 172.16.60.0/24 udp * 58301` | `PutFilter` | 固定文字列 | 同上 |
| 603 | `ip filter 10620 pass 172.16.60.0/24 172.16.60.0/24 tcp * 58378` | `PutFilter` | 固定文字列 | 同上 |
| 604 | `ip filter 10625 pass 172.16.60.0/24 172.16.60.0/24 tcp 58378 *` | `PutFilter` | 固定文字列 | 同上 |
| 605 | `ip filter 12600 pass [長IPリスト] 172.16.5.0/24 udp * 500` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（XAIサーバーIPリスト + m_sRouter.ip派生）(Router1200.cpp:3577) |
| 606 | `ip filter 12650 pass 172.16.5.0/24 [長IPリスト] udp * 500` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（逆方向）(Router1200.cpp:3586) |
| 607 | `ip filter 12700 pass [長IPリスト] 172.16.5.0/24 esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（ESP）(Router1200.cpp:3577) |
| 608 | `ip filter 12750 pass 172.16.5.0/24 [長IPリスト] esp * *` | `PutFilter` | `m_sRouter.ip` | pCmd03_1配列固定要素（ESP逆方向）(Router1200.cpp:3586) |

---

## ip filter dynamic エントリ（行609〜689）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 609 | `ip filter dynamic 25 172.16.0.0/16 211.129.14.134,211.129.12.43 domain` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 25 %s 211.129.14.134,211.129.12.43 domain", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 610 | `ip filter dynamic 26 172.16.5.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 611 | `ip filter dynamic 27 172.16.15.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 612 | `ip filter dynamic 28 172.16.60.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | 同上 |
| 613 | `ip filter dynamic 35 172.16.0.0/16 61.114.182.53 filter 35 in 30` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 614 | `ip filter dynamic 36 172.16.5.0/24 61.114.182.53 filter 36 in 31` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 + 固定IP |
| 615 | `ip filter dynamic 37 172.16.15.0/24 61.114.182.53 filter 37 in 32` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 616 | `ip filter dynamic 38 172.16.60.0/24 61.114.182.53 filter 38 in 33` | `PutFilter` | 固定文字列 | 同上 |
| 617 | `ip filter dynamic 45 172.16.0.0/16 61.114.182.54 filter 45 in 40` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 618 | `ip filter dynamic 46 172.16.5.0/24 61.114.182.54 filter 46 in 41` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 619 | `ip filter dynamic 47 172.16.15.0/24 61.114.182.54 filter 47 in 42` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 620 | `ip filter dynamic 48 172.16.60.0/24 61.114.182.54 filter 48 in 43` | `PutFilter` | 固定文字列 | 同上 |
| 621 | `ip filter dynamic 55 172.16.0.0/16 61.114.186.0/25 filter 55 in 50` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 622 | `ip filter dynamic 56 172.16.5.0/24 61.114.186.0/25 filter 56 in 51` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 623 | `ip filter dynamic 57 172.16.15.0/24 61.114.186.0/25 filter 57 in 52` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 624 | `ip filter dynamic 58 172.16.60.0/24 61.114.186.0/25 filter 58 in 53` | `PutFilter` | 固定文字列 | 同上 |
| 625 | `ip filter dynamic 59 172.16.80.0/22 61.114.186.0/25 filter 59 in 54` | `PutFilter` | 固定文字列 | 同上 |
| 626 | `ip filter dynamic 65 172.16.0.0/16 61.114.177.148 filter 65 in 60` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 627 | `ip filter dynamic 66 172.16.5.0/24 61.114.177.148 filter 66 in 61` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 628 | `ip filter dynamic 67 172.16.15.0/24 61.114.177.148 filter 67 in 62` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 629 | `ip filter dynamic 68 172.16.60.0/24 61.114.177.148 filter 68 in 63` | `PutFilter` | 固定文字列 | 同上 |
| 630 | `ip filter dynamic 75 172.16.0.0/16 202.229.106.177 filter 75 in 70` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 631 | `ip filter dynamic 76 172.16.5.0/24 202.229.106.177 filter 76 in 71` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 632 | `ip filter dynamic 77 172.16.15.0/24 202.229.106.177 filter 77 in 72` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 633 | `ip filter dynamic 78 172.16.60.0/24 202.229.106.177 filter 78 in 73` | `PutFilter` | 固定文字列 | 同上 |
| 634 | `ip filter dynamic 79 172.16.80.0/22 202.229.106.177 filter 79 in 74` | `PutFilter` | 固定文字列 | 同上 |
| 635 | `ip filter dynamic 85 172.16.0.0/16 * filter 85 in 80` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 85 %s * filter 85 in 80", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 636 | `ip filter dynamic 86 172.16.0.0/16 * filter 86 in 81` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 86 %s * filter 86 in 81", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 637 | `ip filter dynamic 87 172.16.15.0/24 * filter 87 in 82` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 638 | `ip filter dynamic 88 172.16.0.0/16 * filter 88 in 83` | `PutFilter` | 固定文字列 | 同上 |
| 639 | `ip filter dynamic 89 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 89 in 84` | `PutFilter` | 固定文字列 | 同上 |
| 640 | `ip filter dynamic 97 172.16.5.0/24 * filter 97 in 92` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 641 | `ip filter dynamic 98 172.16.5.0/24 * filter 98 in 93` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 642 | `ip filter dynamic 99 172.16.5.0/24 * filter 99 in 94` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 643 | `ip filter dynamic 2097 172.16.5.0/24 * filter 2097 in 2092` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 644 | `ip filter dynamic 2098 172.16.5.0/24 * filter 2098 in 2093` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 645 | `ip filter dynamic 2099 172.16.5.0/24 * filter 2099 in 2094` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 646 | `ip filter dynamic 3097 172.16.5.0/24 * filter 3097 in 3092` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 647 | `ip filter dynamic 3098 172.16.5.0/24 * filter 3098 in 3093` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 648 | `ip filter dynamic 3099 172.16.5.0/24 * filter 3099 in 3094` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 649 | `ip filter dynamic 2025 172.16.0.0/16 211.129.14.134,211.129.12.43 domain` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 650 | `ip filter dynamic 2026 * 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 651 | `ip filter dynamic 2027 172.16.15.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | 同上 |
| 652 | `ip filter dynamic 2028 172.16.60.0/24 211.129.14.134,211.129.12.43 domain` | `PutFilter` | 固定文字列 | 同上 |
| 653 | `ip filter dynamic 2035 172.16.0.0/16 61.114.182.53 filter 2035 in 2030` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 654 | `ip filter dynamic 2036 172.16.5.0/24 61.114.182.53 filter 2036 in 2031` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 655 | `ip filter dynamic 2037 172.16.15.0/24 61.114.182.53 filter 2037 in 2032` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 656 | `ip filter dynamic 2038 172.16.60.0/24 61.114.182.53 filter 2038 in 2033` | `PutFilter` | 固定文字列 | 同上 |
| 657 | `ip filter dynamic 2045 172.16.0.0/16 61.114.182.54 filter 2045 in 2040` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 658 | `ip filter dynamic 2046 172.16.5.0/24 61.114.182.54 filter 2046 in 2041` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 659 | `ip filter dynamic 2047 172.16.15.0/24 61.114.182.54 filter 2047 in 2042` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 660 | `ip filter dynamic 2048 172.16.60.0/24 61.114.182.54 filter 2048 in 2043` | `PutFilter` | 固定文字列 | 同上 |
| 661 | `ip filter dynamic 2055 172.16.0.0/16 61.114.186.0/25 filter 2055 in 2050` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 662 | `ip filter dynamic 2056 172.16.5.0/24 61.114.186.0/25 filter 2056 in 2051` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 663 | `ip filter dynamic 2057 172.16.15.0/24 61.114.186.0/25 filter 2057 in 2052` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 664 | `ip filter dynamic 2058 172.16.60.0/24 61.114.186.0/25 filter 2058 in 2053` | `PutFilter` | 固定文字列 | 同上 |
| 665 | `ip filter dynamic 2059 172.16.80.0/22 61.114.186.0/25 filter 2059 in 2054` | `PutFilter` | 固定文字列 | 同上 |
| 666 | `ip filter dynamic 2065 172.16.0.0/16 61.114.177.148 filter 2065 in 2060` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 667 | `ip filter dynamic 2066 172.16.5.0/24 61.114.177.148 filter 2066 in 2061` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 668 | `ip filter dynamic 2067 172.16.15.0/24 61.114.177.148 filter 2067 in 2062` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 669 | `ip filter dynamic 2068 172.16.60.0/24 61.114.177.148 filter 2068 in 2063` | `PutFilter` | 固定文字列 | 同上 |
| 670 | `ip filter dynamic 2075 172.16.0.0/16 202.229.106.177 filter 2075 in 2070` | `PutFilter` | `m_sRouter.mask` | GetIpB2C(bMaskedIP, m_sRouter.mask) + 固定IP |
| 671 | `ip filter dynamic 2076 172.16.5.0/24 202.229.106.177 filter 2076 in 2071` | `PutFilter` | `m_sRouter.ip` | m_sRouter.ip派生 |
| 672 | `ip filter dynamic 2077 172.16.15.0/24 202.229.106.177 filter 2077 in 2072` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 673 | `ip filter dynamic 2078 172.16.60.0/24 202.229.106.177 filter 2078 in 2073` | `PutFilter` | 固定文字列 | 同上 |
| 674 | `ip filter dynamic 2079 172.16.80.0/22 202.229.106.177 filter 2079 in 2074` | `PutFilter` | 固定文字列 | 同上 |
| 675 | `ip filter dynamic 2085 172.16.0.0/16 * filter 2085 in 2080` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 2085 %s * filter 2085 in 2080", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 676 | `ip filter dynamic 2086 172.16.0.0/16 * filter 2086 in 2081` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 2086 %s * filter 2086 in 2081", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 677 | `ip filter dynamic 2087 172.16.15.0/24 * filter 2087 in 2082` | `PutFilter` | 固定文字列 | pCmd02配列固定要素 |
| 678 | `ip filter dynamic 2088 172.16.0.0/16 * filter 2088 in 2083` | `PutFilter` | 固定文字列 | 同上 |
| 679 | `ip filter dynamic 2089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 2089 in 2084` | `PutFilter` | 固定文字列 | 同上 |
| 680 | `ip filter dynamic 101406 - 180.55.241.146 udp` | `PutFilter` | 固定文字列 | pCmd配列固定要素 |
| 681 | `ip filter dynamic 101407 - * filter 101407 in 101402` | `PutFilter` | 固定文字列 | 同上 |
| 682 | `ip filter dynamic 101408 - * domain` | `PutFilter` | 固定文字列 | 同上 |
| 683 | `ip filter dynamic 101409 - * filter 101409 in 101404` | `PutFilter` | 固定文字列 | 同上 |
| 684 | `ip filter dynamic 3085 172.16.0.0/16 * filter 3085 in 3080` | `PutFilter` | `m_sRouter.mask` | `wsprintf(szPut, "ip filter dynamic 3085 %s * filter 3085 in 3080", GetIpB2C(bMaskedIP, m_sRouter.mask))` |
| 685 | `ip filter dynamic 3086 172.16.0.0/16 * filter 3086 in 3081` | `PutFilter` | `m_sRouter.mask` | 同上パターン |
| 686 | `ip filter dynamic 3087 172.16.15.0/24 * filter 3087 in 3082` | `PutFilter` | 固定文字列 | pCmd03配列固定要素 |
| 687 | `ip filter dynamic 3088 172.16.0.0/16 * filter 3088 in 3083` | `PutFilter` | 固定文字列 | 同上 |
| 688 | `ip filter dynamic 3089 172.16.121.90-172.16.121.128,172.16.60.1-172.16.60.254 * filter 3089 in 3084` | `PutFilter` | 固定文字列 | 同上 |
| 689 | `ip filter dynamic 10615 172.16.60.0/24 172.16.60.0/24 filter 10615 in 10610 10620 out 10625` | `PutFilter` | 固定文字列 | 固定コマンド |

---

## ip filter set エントリ（行690〜700）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 690 | `ip filter set xai_tunnel_lan2 out 10605 10615 dynamic 10615` | `PutFilter` | 固定文字列 | 固定フィルターセット名+番号 |
| 691 | `ip filter set dk_lan1 in 1400 1405 1410 1430 1431 1432 1433 1434 1435 1436 1437 1440 1441` | `PutFilter` | 固定文字列 | `_Put(szPut, FALSE)` フィルターセット (Router1200.cpp:3252) |
| 692 | `ip filter set dk_lan1 out 1400 1405 1430 1431 1432 1433 1434 1435 1436 1437 1440 1441` | `PutFilter` | 固定文字列 | 同上 |
| 693 | `ip filter set dk_pp_lan2 in 1 2 3 4 ...` | `PutFilter` | 固定文字列（一部`m_sRouter.ip`依存ID含む） | `_Put(szPut, FALSE)` 複数フィルタIDをまとめた文字列 (Router1200.cpp:3261) |
| 694 | `ip filter set dk_pp_lan2 out 1 2 3 4 ...dynamic ...` | `PutFilter` | 固定文字列（一部`m_sRouter.ip`依存ID含む） | 同上（out方向）(Router1200.cpp:3275) |
| 695 | `ip filter set dk_lan3 in 3 4 3010 3420 ...dynamic 3530 3550 3590` | `PutFilter` | 固定文字列 | 同上 (Router1200.cpp:3284) |
| 696 | `ip filter set dk_lan3 out 3 4 104 105 3015 ...` | `PutFilter` | 固定文字列 | 同上 |
| 697 | `ip filter set dk_pp_lab in 9` | `PutFilter` | 固定文字列 | `_Put("ip filter set dk_pp_lab in 9")` (Router1200.cpp:3289) |
| 698 | `ip filter set dk_pp_lab out 1 2 3 4 5 6 104 105 ... dynamic ...` | `PutFilter` | 固定文字列 | `_Put(szPut, FALSE)` (Router1200.cpp:3297) |
| 699 | `ip filter set getlog in 1 2 3 4 5 6 100` | `PutFilter` | 固定文字列 | `_Put(szPut, FALSE)` (Router1200.cpp:3306) |
| 700 | `ip filter set getlog out 1 2 3 4 5 6 100` | `PutFilter` | 固定文字列 | 同上 |

---

## NATディスクリプター設定（行701〜763）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 701 | （空行） | − | − | − |
| 702 | `# NAT Descriptor configuration` | `PutNatOther` | 固定文字列 | セクションタイトル |
| 703 | （空行） | − | − | − |
| 704 | `nat descriptor log on` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 705 | `nat descriptor masquerade ttl hold all` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 706 | `nat descriptor type 1 nat` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 707 | `nat descriptor address outer 1 10.6.5.1` | `PutNatOther` | `m_sRouter.uip`, `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor address outer 1 %d.%d.%d.%d", m_sRouter.uip[0], m_sRouter.uip[1], m_sRouter.ip[2], m_sRouter.ip[3])` |
| 708 | `nat descriptor address inner 1 172.16.5.1` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor address inner 1 %s", GetIpB2C(m_sRouter.ip))` |
| 709 | `nat descriptor static 1 1 10.6.1.1=172.16.1.1 1` | `PutNatOther` | 固定文字列（`m_sRouter.uip`派生） | 固定または uip基準の静的NATエントリ |
| 710 | `nat descriptor static 1 2 10.6.2.1=172.16.2.1 2` | `PutNatOther` | 固定文字列 | 同上 |
| 711 | `nat descriptor static 1 4 10.6.4.1=172.16.4.1 8` | `PutNatOther` | 固定文字列 | 同上 |
| 712 | `nat descriptor static 1 5 10.6.5.1=172.16.5.1 10` | `PutNatOther` | 固定文字列 | 同上 |
| 713 | `nat descriptor static 1 10 10.6.10.1=172.16.10.1 2` | `PutNatOther` | 固定文字列 | 同上 |
| 714 | `nat descriptor static 1 11 10.6.11.1=172.16.11.1 128` | `PutNatOther` | 固定文字列 | 同上 |
| 715 | `nat descriptor static 1 12 10.6.12.1=172.16.12.1 128` | `PutNatOther` | 固定文字列 | 同上 |
| 716 | `nat descriptor static 1 13 10.6.13.1=172.16.13.1 8` | `PutNatOther` | 固定文字列 | 同上 |
| 717 | `nat descriptor static 1 14 10.6.14.1=172.16.14.1 12` | `PutNatOther` | 固定文字列 | 同上 |
| 718 | `nat descriptor static 1 15 10.6.15.1=172.16.15.1 8` | `PutNatOther` | 固定文字列 | 同上 |
| 719 | `nat descriptor static 1 20 10.6.20.1=172.16.20.1 2` | `PutNatOther` | 固定文字列 | 同上 |
| 720 | `nat descriptor static 1 21 10.6.21.1=172.16.21.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 721 | `nat descriptor static 1 30 10.6.30.1=172.16.30.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 722 | `nat descriptor static 1 31 10.6.31.1=172.16.31.1 8` | `PutNatOther` | 固定文字列 | 同上 |
| 723 | `nat descriptor static 1 40 10.6.40.1=172.16.40.1 32` | `PutNatOther` | 固定文字列 | 同上 |
| 724 | `nat descriptor static 1 50 10.6.50.1=172.16.50.1 3` | `PutNatOther` | 固定文字列 | 同上 |
| 725 | `nat descriptor static 1 52 10.6.52.1=172.16.52.1 8` | `PutNatOther` | 固定文字列 | 同上 |
| 726 | `nat descriptor static 1 60 10.6.60.1=172.16.60.1 127` | `PutNatOther` | 固定文字列 | 同上 |
| 727 | `nat descriptor static 1 121 10.6.121.20=172.16.121.20 40` | `PutNatOther` | 固定文字列 | 同上 |
| 728 | `nat descriptor static 1 80 10.6.80.1=172.16.80.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 729 | `nat descriptor static 1 81 10.6.81.1=172.16.81.1 16` | `PutNatOther` | 固定文字列 | 同上 |
| 730 | `nat descriptor static 1 82 10.6.82.1=172.16.82.1 16` | `PutNatOther` | 固定文字列 | 同上 |
| 731 | `nat descriptor static 1 83 10.6.83.1=172.16.83.1 16` | `PutNatOther` | 固定文字列 | 同上 |
| 732 | `nat descriptor static 1 110 10.6.110.1=172.16.110.1 2` | `PutNatOther` | 固定文字列 | 同上 |
| 733 | `nat descriptor static 1 100 10.6.100.1=172.16.100.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 734 | `nat descriptor static 1 122 10.6.122.1=172.16.122.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 735 | `nat descriptor type 2000 masquerade` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 736 | `nat descriptor address outer 2000 153.142.244.124` | `PutNatOther` | `m_sRouter.ncon[n].fixIp` | `wsprintf(szBuf, "nat descriptor address outer 2000 %s", GetIpB2C(m_sRouter.ncon[n].fixIp))` |
| 737 | `nat descriptor masquerade static 2000 1 172.16.5.1 udp 500` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 1 %s udp 500", GetIpB2C(m_sRouter.ip))` |
| 738 | `nat descriptor masquerade static 2000 2 172.16.5.1 esp` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 2 %s esp", GetIpB2C(m_sRouter.ip))` |
| 739 | `nat descriptor masquerade static 2000 6 172.16.5.1 udp 4500` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 6 %s udp 4500", GetIpB2C(m_sRouter.ip))` |
| 740 | `nat descriptor masquerade static 2000 7 172.16.5.1 tcp 53196` | `PutNatOther` | `m_sRouter.ip`, `m_sRouter.nLuaPort` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 7 %s tcp %d", GetIpB2C(m_sRouter.ip), m_sRouter.nLuaPort)` |
| 741 | `nat descriptor type 2012 masquerade` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 742 | `nat descriptor address outer 2012 172.23.176.129` | `PutNatOther` | 固定文字列 | 固定コマンド（固定VPNアドレス） |
| 743 | `nat descriptor address inner 2012 auto` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 744 | `nat descriptor static 2012 2 172.23.176.130=172.16.5.1 1` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor static 2012 2 %s=%s 1", GetIpB2C(bVPNadd), GetIpB2C_Second(m_sRouter.ip))` |
| 745 | `nat descriptor static 2012 3 172.23.176.131=172.16.60.1 1` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 746 | `nat descriptor static 2012 4 172.23.176.132=172.16.1.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 747 | `nat descriptor static 2012 5 172.23.176.133=172.16.21.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 748 | `nat descriptor static 2012 6 172.23.176.134=172.16.80.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 749 | `nat descriptor static 2012 7 172.23.176.135=172.16.15.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 750 | `nat descriptor static 2012 8 172.23.176.136=172.16.52.8 1` | `PutNatOther` | 固定文字列 | 同上 |
| 751 | `nat descriptor static 2012 9 172.23.176.137=172.16.50.2 1` | `PutNatOther` | 固定文字列 | 同上 |
| 752 | `nat descriptor static 2012 10 172.23.176.138=172.16.20.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 753 | `nat descriptor static 2012 11 172.23.176.139=172.16.20.2 1` | `PutNatOther` | 固定文字列 | 同上 |
| 754 | `nat descriptor static 2012 12 172.23.176.140=172.16.110.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 755 | `nat descriptor static 2012 13 172.23.176.141=172.16.110.2 1` | `PutNatOther` | 固定文字列 | 同上 |
| 756 | `nat descriptor static 2012 14 172.23.176.142=172.16.122.1 1` | `PutNatOther` | 固定文字列 | 同上 |
| 757 | `nat descriptor static 2012 15 172.23.176.143=172.16.15.100 1` | `PutNatOther` | 固定文字列 | 同上 |
| 758 | `nat descriptor type 12 masquerade` | `PutNatOther` | 固定文字列 | 固定コマンド |
| 759 | `nat descriptor masquerade static 2000 3 172.16.5.1 tcp 1723` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 3 %s tcp 1723", GetIpB2C(m_sRouter.ip))` |
| 760 | `nat descriptor masquerade static 2000 4 172.16.5.1 gre` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 4 %s gre", GetIpB2C(m_sRouter.ip))` |
| 761 | `nat descriptor masquerade static 2000 5 172.16.5.1 udp 4500` | `PutNatOther` | `m_sRouter.ip` | `wsprintf(szBuf, "nat descriptor masquerade static 2000 5 %s udp 4500", GetIpB2C(m_sRouter.ip))` |
| 762 | `nat descriptor sip 2000 off` | `PutNatOther` | 固定文字列 | 固定コマンド |

---

## その他設定セクション（行763〜791）

| 行 | .datの内容 | 書込関数 | SDkRouterメンバ | 根拠フォーマット |
|---|---|---|---|---|
| 763 | （空行） | − | − | − |
| 764 | `# Other configuration` | `PutConfig` | 固定文字列 | セクションタイトル (Router1200.cpp:2940付近) |
| 765 | （空行） | − | − | − |
| 766 | `syslog info on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 (Router1200.cpp:2986) |
| 767 | `syslog notice on` | `PutConfig` | 固定文字列 | 同上 |
| 768 | `syslog debug off` | `PutConfig` | 固定文字列 | 同上 |
| 769 | `tftp host any` | `PutConfig` | 固定文字列 | 同上 |
| 770 | `ipsec auto refresh on` | `PutConfig` | 固定文字列 | 同上 |
| 771 | `dhcp server rfc2131 compliant except remain-silent` | `PutConfig` | 固定文字列 | 同上 |
| 772 | `dns server 211.129.14.134 211.129.12.43` | `PutConfig` | `m_sRouter.con[n].ip`, `m_sRouter.con[n].mask` | `wsprintf(szBuf, "dns server %s %s", GetIpB2C(m_sRouter.con[n].ip), GetIpB2C_Second(m_sRouter.con[n].mask))` または固定 |
| 773 | `dns server pp 5` | `PutConfig` | `m_sRouter.m_nAdslNo`, `m_sRouter.fBbUse`, `m_sRouter.m_nVWNo` | `wsprintf(szBuf, "dns server pp %d", (m_sRouter.m_nAdslNo && m_sRouter.fBbUse) ? m_sRouter.m_nAdslNo : m_sRouter.m_nVWNo)` |
| 774 | `dns private address spoof on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 |
| 775 | `schedule at 1 */* 6:00 * ntpdate 172.31.191.62` | `PutConfig` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3316) |
| 776 | `dns service recursive` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 |
| 777 | `no dhcp service` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 |
| 778 | `no dhcp scope 1` | `PutConfig` | 固定文字列 | 同上 |
| 779 | `dhcp scope option 1 dns=172.16.5.1` | `PutConfig` | `m_sRouter.ip` | `wsprintf(szBuf, "dhcp scope option 1 dns=%s", GetIpB2C(m_sRouter.ip))` |
| 780 | `pptp service on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 |
| 781 | `ip stealth pp 5` | `PutConfig` | `m_sRouter.m_nAdslNo` | `wsprintf(szBuf, "ip stealth pp %d", m_sRouter.m_nAdslNo)` |
| 782 | `l2tp service on` | `PutConfig` | 固定文字列 | pCmd02配列固定要素 |
| 783 | `schedule at 2 startup * lua /xai_check.lua` | `PutConfig` | 固定文字列 | pCmd03配列固定要素 (Router1200.cpp:3316) |
| 784 | `schedule at 3 */* 04:00:00 * terminate lua all` | `PutConfig` | 固定文字列 | 同上 |
| 785 | `schedule at 4 */* 04:00:10 * lua /xai_check.lua` | `PutConfig` | 固定文字列 | 同上 |
| 786 | `# LIFT_PHASE=1` | − | − | コメント行（書き込み不明） |
| 787 | `description 1 "configversion=5.0"` | `PutConfig` | 固定文字列 | 固定コマンド |
| 788 | `save` | `PutConfig` | 固定文字列 | 固定コマンド |
| 789 | `description 1 "configversion=5.1"` | `PutConfig` | 固定文字列 | 固定コマンド |
| 790 | `save` | `PutConfig` | 固定文字列 | 固定コマンド |
| 791 | （空行） | − | − | ファイル末尾 |

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
