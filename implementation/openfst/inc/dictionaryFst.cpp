// iostream, fstream だとうまく行かなかったので
// Cのライブラリを愚直に使う作戦
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <fst/fstlib.h>
#include <iostream>
#include <set>
#include <map>
#include "utils.h"

#define STR_LENGTH 600 // ファイルを行ごとに読むときのバッファサイズ
#define ID_OF_EPS  0   // <eps> の ID は 0

void make_symbs (
      std::set<wchar_t>      *isymbs,
      std::set<std::wstring> *osymbs
);
void make_ids (
      std::set<wchar_t>          isymbs,
      std::set<std::wstring>     osymbs,
      std::map<wchar_t,int>     *id_of_isymb,
      std::map<std::wstring,int>*id_of_osymb
);
void make_symbol_file (
      std::map<wchar_t,int>      id_of_isymb,
      std::map<std::wstring,int> id_of_osymb
);
void make_fst (
      std::map<wchar_t,int>      id_of_isymb,
      std::map<std::wstring,int> id_of_osymb
);

namespace dicFst {
  void make() {
    std::set<wchar_t>      isymbs;
    std::set<std::wstring> osymbs;
    std::map<wchar_t,int>      id_of_isymb;
    std::map<std::wstring,int> id_of_osymb;

    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    
    make_symbs(&isymbs, &osymbs);

    make_ids(isymbs,osymbs,&id_of_isymb,&id_of_osymb);

    make_symbol_file(id_of_isymb,id_of_osymb);

    make_fst(id_of_isymb,id_of_osymb);
  }
}


void make_symbs (
      std::set<wchar_t>      *isymbs,
      std::set<std::wstring> *osymbs
)
{
  FILE *fp = fopen_with_errmsg("./data/sample_dictionary.txt","r");
  
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
  fclose(fp);
}

void make_ids (
      std::set<wchar_t>          isymbs,
      std::set<std::wstring>     osymbs,
      std::map<wchar_t,int>     *id_of_isymb,
      std::map<std::wstring,int>*id_of_osymb
)
{
  int id_iter = ID_OF_EPS+1;
  for(auto x : isymbs) {
    (*id_of_isymb)[x] = id_iter;
    id_iter++;
  }
  id_iter = ID_OF_EPS+1;
  for(auto x : osymbs) {
    (*id_of_osymb)[x] = id_iter;
    id_iter++;
  }
}

void make_symbol_file (
      std::map<wchar_t,int>      id_of_isymb,
      std::map<std::wstring,int> id_of_osymb
)
{
  // isymbs, osymbs をファイルに書き込み
  FILE *isymb_fp = fopen_with_errmsg("./data/dictionaryFst/dictionary_isymb.txt","w+");
  FILE *osymb_fp = fopen_with_errmsg("./data/dictionaryFst/dictionary_osymb.txt","w+");
  
  // <eps> の ID は ID_OF_EPS
  fprintf(isymb_fp,"<eps> %d\n",ID_OF_EPS);
  fprintf(osymb_fp,"<eps> %d\n",ID_OF_EPS);
  
  for(auto x : id_of_isymb) {
    fprintf(isymb_fp,"%lc %d\n",x.first,x.second);
  }
  for(auto x : id_of_osymb) {
    fprintf(osymb_fp,"%ls %d\n",x.first.c_str(),x.second);
  }
  fclose(isymb_fp);
  fclose(osymb_fp);
}

void make_fst (
      std::map<wchar_t,int>      id_of_isymb,
      std::map<std::wstring,int> id_of_osymb
)
{
  fst::StdVectorFst t;

  int q0 = t.AddState();
  t.SetStart(q0);
  int q1 = t.AddState();
  t.SetFinal(q1);

  for(auto iter : id_of_osymb) {
    // iter.first  : 語彙
    // iter.second : その語彙の ID
    
    int prev_q = q0;

    for(auto wchar : (iter.first)) {
      int next_q = t.AddState();
      int      i = id_of_isymb[wchar];
      int      o = ID_OF_EPS;
      double   w = 0.0;

      t.AddArc(prev_q, fst::StdArc(i,o,w,next_q));

      prev_q = next_q;
    }
    int    i = ID_OF_EPS;
    int    o = iter.second;
    double w = 0.0;

    t.AddArc(prev_q, fst::StdArc(i,o,w,q1));
  }
  fst::Closure(&t,(fst::ClosureType)0);
  t.Write("dic_binary.fst");
}
