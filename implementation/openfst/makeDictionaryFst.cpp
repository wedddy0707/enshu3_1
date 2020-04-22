#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <fst/fstlib.h>
#include <iostream>
#include <set>
#include <map>

#define STR_LENGTH 600
#define ID_OF_EPS  0

int main() {
  // localeを日本に設定して、日本語を扱えるようにする
  setlocale(LC_CTYPE, "ja_JP.UTF-8");

  // sample_dictionary.txt をオープン
  FILE *input_fp;
  
  if((input_fp = fopen("./data/sample_dictionary.txt","r")) < 0) {
    fprintf(stderr,"ファイルを開けませんでした\n");
    return EXIT_FAILURE;
  }
  
  // １行ずつ読んでいき、isymbs,osymbs を作る
  wchar_t input_string[STR_LENGTH] = L"";
  std::set<wchar_t>      isymbs;
  std::set<std::wstring> osymbs;
  
  while(fscanf(input_fp,"%ls",input_string) == 1) {
    fprintf(stdout,"input lexcon: %ls\n",input_string);
    for(int i=0; i < STR_LENGTH && input_string[i] != '\0'; i++) {
      isymbs.insert(input_string[i]);
    }
    osymbs.insert(input_string);
  }
  fclose(input_fp);
  
  // デバッグ
  for(auto e : osymbs) {
    fprintf(stdout,"osymb: %ls\n", e.c_str());
  }

  // ファイルに書き込み
  FILE *isymb_fp;
  FILE *osymb_fp;
  
  std::map<wchar_t,int>      id_of_isymb;
  std::map<std::wstring,int> id_of_osymb;

  if((isymb_fp = fopen("./data/dictionaryFst/dictionary_isymb.txt","w+")) < 0) {
    fprintf(stderr,"ファイルを開けませんでした\n");
    return EXIT_FAILURE;
  }
  if((osymb_fp = fopen("./data/dictionaryFst/dictionary_osymb.txt","w+")) < 0) {
    fprintf(stderr,"ファイルを開けませんでした\n");
    return EXIT_FAILURE;
  }
  
  fprintf(isymb_fp,"<eps> %d\n",ID_OF_EPS);
  fprintf(osymb_fp,"<eps> %d\n",ID_OF_EPS);
  
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
