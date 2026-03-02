# 解析前提コンテキスト

## 1. SDkRouter の定義場所

- SDkRouter の定義は Router.h に存在する。
- 構造体定義の内容は work/sdkrouter_definition.md にも整理済み。
- 解析時は Router.h を正とする。

---

## 2. クラス継承関係

ルータ関連クラスの継承構造は以下の通り。

CDkRouter
  ↓
CDkRouter1000
  ↓
CDkRouter1200
  ↓
CDkRouter830
  ↓
CDkRouter1220

- CDkRouter が基底クラス。
- CDkRouter1220 が最下位の派生クラス。

---

## 3. 解析時の重要ポイント

- SDkRouter のメンバーがどの関数で使用されるかを明確にする。
- 仮想関数がある場合はオーバーライド関係を追跡する。
- 基底クラスで定義された処理が派生クラスで再定義されているかを確認する。

---

## 4. 注意事項

- クラス継承と構造体定義は分けて解析する。
- Router.h を最優先で参照する。
- 推測は禁止。不明は不明と記載する。