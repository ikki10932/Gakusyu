import re
import sys

def extract_put_commands(filepath):
    """
    Router1200.cpp から、各関数内でコマンド出力（_Put, PutFmt, wsprintf等）を行っている
    コード行を、そのままの形（引数や変数が含まれた状態）で関数ごとに抽出する。
    """
    with open(filepath, 'r', encoding='shift_jis', errors='ignore') as f:
        lines = f.readlines()
        
    current_func = ""
    in_cmd_array = False
    
    # 抽出結果: { func_name: [code_line, ...] }
    extracted = {} 
    
    # 関数定義の正規表現 (例: BOOL CDkRouter1200::PutConfig(char *tit))
    func_re = re.compile(r'^\s*(?:BOOL|int|void)\s+CDkRouter(?:1200)?::([a-zA-Z0-9_]+)\(')
    
    # コマンド配列の検出 (const char *pCmd[] = { ... })
    cmd_array_start_re = re.compile(r'const\s+char\s+\*[a-zA-Z0-9_]+\s*\[\]\s*=\s*\{')
    
    for i, line in enumerate(lines):
        # 1. 関数定義の検出
        m = func_re.search(line)
        if m:
            current_func = m.group(1)
            if current_func not in extracted:
                extracted[current_func] = []
            continue
            
        if not current_func:
            continue
            
        original_line = line.strip()
        if not original_line or original_line.startswith('//'):
            continue
            
        # 2. 配列形式のコマンド定義 (pCmd)
        if in_cmd_array:
            if '};' in original_line:
                in_cmd_array = False
            else:
                # 配列の中身をそのまま抽出
                extracted[current_func].append(original_line)
            continue
            
        if cmd_array_start_re.search(original_line):
            in_cmd_array = True
            extracted[current_func].append(original_line)
            continue
            
        # 3. _Put, PutFmt, wsprintf でコマンド生成・出力している行を抽出
        if ('_Put(' in original_line or 
            'PutFmt(' in original_line or 
            'wsprintf(' in original_line or
            'str.Format(' in original_line or
            '_PutTit(' in original_line):
            
            # ログ出力やエラー処理などを除外
            if 'DebDisp(' in original_line or 'Error(' in original_line:
                if '_Put' not in original_line and 'wsprintf' not in original_line:
                    continue
                    
            extracted[current_func].append(original_line)

    # 結果をMarkdownファイルに出力
    with open('extracted_raw_commands.md', 'w', encoding='utf-8') as f:
        f.write("# 関数別出力コマンド一覧 (ソースコードそのまま)\n\n")
        f.write("`CDkRouter1200` クラスの各設定処理関数において、ルータ設定用コマンド文字列を組み立て・出力している全コード行（`_Put`, `wsprintf`, `pCmd`配列等）を省略せずに掲載しています。\n\n")
        
        # 主要な出力関数に絞るか、すべて出力するか
        target_funcs = [
            'PutConfig', 'PutLanIp', 'PutIsdn', 'PutPp', 'PutTunnel', 
            'PutFilter', 'PutFilterOA_LAN', 'PutIpRoute', 'PutNatOther',
            'TunnelAdd', 'RouterCmdEdit_TunnelAdd', 'RouterCmdEdit_Change'
        ]
        
        for func in target_funcs:
            if func in extracted and extracted[func]:
                f.write(f"## {func}\n")
                f.write("```cpp\n")
                for cmd_line in extracted[func]:
                    f.write(f"{cmd_line}\n")
                f.write("```\n\n")
                
        # その他の関数
        f.write("## その他の関数での出力処理\n")
        for func, cmds in extracted.items():
            if func not in target_funcs and cmds:
                f.write(f"### {func}\n")
                f.write("```cpp\n")
                for cmd_line in cmds:
                    f.write(f"{cmd_line}\n")
                f.write("```\n\n")

if __name__ == '__main__':
    extract_put_commands(r'c:\_Develop\Doc\Skills\router\input\Router1200.cpp')
