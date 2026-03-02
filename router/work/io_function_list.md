関数名: WriteTftpFile
  種別: 書込
  API: _Put, wsprintf, PutFmt 等を各部分出力関数（PutConfig, PutLanIp等）経由で呼び出し
  ファイル: Router1200.cpp

関数名: _Put, PutFmt, _PutTit, wsprintf等
  種別: 書込
  API: 内部バッファやファイルストリームへのテキスト書き込み
  ファイル: Router.cpp / Router1200.cpp等

関数名: ReadTftpFile
  種別: 読込
  API: fgets
  ファイル: Router1200.cpp (行単位でコマンド文字列を読み込み)

関数名: Analize
  種別: 読込
  API: strtok, strncmp 等
  ファイル: Router1200.cpp (読み込んだ文字列を ProcPP, ProcIP, ProcNAT 等へ振り分け)

関数名: PreReadFile
  種別: 読込
  API: 事前情報取得
  ファイル: Router.cpp
