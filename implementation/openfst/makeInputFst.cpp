#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <fst/fstlib.h>
#include <iostream>
#include <set>
#include <map>

int main() {
  // localeを日本に設定して、日本語を扱えるようにする
  setlocale(LC_CTYPE, "ja_JP.UTF-8");

  // 入力文字列を取得して、input_stringに納める
  FILE *input_fp;
  wchar_t input_string[600] = L"";

  if((input_fp = fopen("./data/sample_input.txt","r")) < 0) {
    fprintf(stderr,"ファイルを開けませんでした\n");
    return EXIT_FAILURE;
  }
  if(fscanf(input_fp,"%ls",input_string) != 1) {
    fprintf(stderr,"文字列を読み込めませんでした\n");
    return EXIT_FAILURE;
  }

  fclose(input_fp);

  // デバッグ
  for(auto c : input_string) {
    if(c == L'\0') {
      break;
    }
    fprintf(stdout,"%lc\n",c);
  }
  fprintf(stdout,"%ls\n",input_string);

  // アルファベットを作る
  std::set<wchar_t>     symbs;
  for(auto c : input_string) {
    if(c == L'\0') break;
    symbs.insert(c);
  }
  std::map<wchar_t,int> id_of_symb;
  int id_iter = 1;
  for(auto c : symbs) {
    id_of_symb[c] = id_iter;
    id_iter++;
  }
  // ファイルに書き込み
  FILE *isymb_fp;
  FILE *osymb_fp;

  if((isymb_fp = fopen("./data/inputFst/input_isymb.txt","w+")) < 0) {
    fprintf(stderr,"ファイルを開けませんでした\n");
    return EXIT_FAILURE;
  }
  if((osymb_fp = fopen("./data/inputFst/input_osymb.txt","w+")) < 0) {
    fprintf(stderr,"ファイルを開けませんでした\n");
    return EXIT_FAILURE;
  }
  fprintf(isymb_fp,"<eps> 0\n");
  fprintf(osymb_fp,"<eps> 0\n");
  id_iter = 1;
  for(auto c : symbs) {
    fprintf(isymb_fp,"%lc %d\n",c,id_iter);
    fprintf(osymb_fp,"%lc %d\n",c,id_iter);
    id_iter++;
  }
  fclose(isymb_fp);
  fclose(osymb_fp);

  fst::StdVectorFst t;

  int q0 = t.AddState();
  t.SetStart(q0);  

  int prev_q = q0;
  for(auto c : input_string) {
    if(c=='\0') break;
    
    int q = t.AddState();
    t.AddArc(prev_q, fst::StdArc(id_of_symb[c],id_of_symb[c],1.0,q));
    prev_q = q;
  }
  t.SetFinal(prev_q,1.0);

  t.Write("input_binary.fst");

  return 0;
}
