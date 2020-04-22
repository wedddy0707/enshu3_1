#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <fst/fstlib.h>
#include <iostream>
#include <set>
#include <map>

#define STR_LENGTH 600 // ファイルを行ごとに読むときのバッファサイズ
#define ID_OF_EPS  0   // <eps> の ID は 0

FILE* fopen_with_errmsg(const char* path,const char* mode)
{
  FILE* fp;

  if((fp = fopen(path,mode)) < 0) {
    fprintf(stderr,"Failed to Open File. \"%s\" with mode \"%s\"\n", path, mode);
    exit(EXIT_FAILURE);
  }

  return fp;
}

int make_symbs(FILE* fp, std::set<wchar_t>* isymbs, std::set<std::wstring>* osymbs)
{
  wchar_t input_string[STR_LENGTH] = L"";
  
  // １行ずつ読んでいき、isymbs,osymbs を作る
  while(fscanf(fp,"%ls",input_string) == 1) {
    fprintf(stdout,"input lexcon: %ls\n",input_string);

    // isymbs は文字の集合なので、input_string をさらに文字ごとに見ていく
    for(int i=0; i < STR_LENGTH && input_string[i] != '\0'; i++) {
      isymbs->insert(input_string[i]);
    }

    // osymbs は語彙の集合なので、input_string をそのまま追加していく
    osymbs->insert(input_string);
  }

  return 0;
}


int main() {
  // localeを日本に設定して、日本語を扱えるようにする
  setlocale(LC_CTYPE, "ja_JP.UTF-8");

  // ./data/sample_dictionary.txt をオープン
  // iostream, fstream だとうまく行かなかったので
  // Cのライブラリを愚直に使う作戦
  FILE *input_fp = fopen_with_errmsg("./data/sample_dictionary.txt","r");
  
  // isymbs, osymbs を用意する
  //wchar_t input_string[STR_LENGTH] = L"";
  std::set<wchar_t>      isymbs;
  std::set<std::wstring> osymbs;

  make_symbs(input_fp, &isymbs, &osymbs);

  // ./data/sample_dictionary.txt の役目終わり. ファイルを閉じる
  fclose(input_fp);
  
  // デバッグ
  for(auto e : osymbs) {
    fprintf(stdout,"osymb: %ls\n", e.c_str());
  }

  // isymbs, osymbs をファイルに書き込み
  FILE *isymb_fp = fopen_with_errmsg("./data/dictionaryFst/dictionary_isymb.txt","w+");
  FILE *osymb_fp = fopen_with_errmsg("./data/dictionaryFst/dictionary_osymb.txt","w+");

  std::map<wchar_t,int>      id_of_isymb; // symb を受け取って ID を返す写像
  std::map<std::wstring,int> id_of_osymb; // これらは後半で Fst を作るのに必要
  
  // <eps> の ID は ID_OF_EPS
  fprintf(isymb_fp,"<eps> %d\n",ID_OF_EPS);
  fprintf(osymb_fp,"<eps> %d\n",ID_OF_EPS);
  
  // その場で、記号の ID を作りながらファイルへ書き込む作戦
  int iid_iter = ID_OF_EPS+1;
  int oid_iter = ID_OF_EPS+1;
  
  for(auto c : isymbs) {
    fprintf(isymb_fp,"%lc %d\n",c,iid_iter);
    id_of_isymb[c] = iid_iter;
    iid_iter++;
  }
  for(auto s : osymbs) {
    fprintf(osymb_fp,"%ls %d\n",s.c_str(),oid_iter);
    id_of_osymb[s] = oid_iter;
    oid_iter++;
  }
  fclose(isymb_fp);
  fclose(osymb_fp);

  fst::StdVectorFst t;

  int q0 = t.AddState();
  t.SetStart(q0);
  int q1 = t.AddState();
  t.SetFinal(q1);

  for(auto s : osymbs) {
    int prev_q = q0;
    for(auto wc : s) {
      int q = t.AddState();
      t.AddArc(prev_q, fst::StdArc(id_of_isymb[wc],ID_OF_EPS,1.0,q));
      prev_q = q;
    }
    t.AddArc(prev_q, fst::StdArc(ID_OF_EPS,id_of_osymb[s],1.0,q1));
  }

  fst::Closure(&t,(fst::ClosureType)0);
  t.Write("dic_binary.fst");
  return 0;
}
