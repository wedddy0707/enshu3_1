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

#define STR_LENGTH 600
#define ID_OF_EPS    0

void make_symbs(std::set<wchar_t> *symbs, std::wstring *input_string);
void make_ids  (std::set<wchar_t>  symbs, std::map<wchar_t,int> *id_of_symb);
void make_symbol_file(std::map<wchar_t,int>  id_of_symb);
void make_fst        (std::map<wchar_t,int>  id_of_symb, std::wstring input_string);

namespace inpFst {
  void make() {
    std::wstring          input_string;
    std::set<wchar_t>     symbs;
    std::map<wchar_t,int> id_of_symb;
    
    setlocale(LC_CTYPE, "ja_JP.UTF-8");

    make_symbs(&symbs, &input_string);
    make_ids  ( symbs, &id_of_symb);

    for(auto x : symbs) {
      std::wcout << x << std::endl;
    }
    make_symbol_file(id_of_symb);
    make_fst        (id_of_symb, input_string);
  }
}

void make_symbs (
      std::set<wchar_t> *symbs,
      std::wstring      *input_string
) 
{
  FILE *input_fp = fopen_with_errmsg("./data/sample_input.txt","r");

  wchar_t input_wc_str[STR_LENGTH];
  
  fscanf(input_fp,"%ls",input_wc_str);
  fclose(input_fp);
  
  
  for(int i = 0; i < STR_LENGTH && input_wc_str[i] != L'\0'; i++) {
    symbs->insert(input_wc_str[i]);
  }
  std::wstring temp_string(input_wc_str);
  *input_string = temp_string;
}

void make_ids (
      std::set<wchar_t>      symbs,
      std::map<wchar_t,int> *id_of_symb
)
{
  int id_iter = ID_OF_EPS+1;
  for(auto x : symbs) {
    (*id_of_symb)[x] = id_iter;
    id_iter++;
  }
}

void make_symbol_file (
      std::map<wchar_t,int> id_of_symb
)
{
  // isymbs, osymbs をファイルに書き込み
  FILE *isymb_fp = fopen_with_errmsg("./data/inputFst/input_isymb.txt","w+");
  FILE *osymb_fp = fopen_with_errmsg("./data/inputFst/input_osymb.txt","w+");
  
  fprintf(isymb_fp,"<eps> 0\n");
  fprintf(osymb_fp,"<eps> 0\n");
  
  for(auto x : id_of_symb) {
    fprintf(isymb_fp,"%lc %d\n",x.first,x.second);
    fprintf(osymb_fp,"%lc %d\n",x.first,x.second);
  }
  fclose(isymb_fp);
  fclose(osymb_fp);
}

void make_fst (
      std::map<wchar_t,int> id_of_symb,
      std::wstring input_string
)
{
  fst::StdVectorFst t;

  std::wcout << L"make_fst: " << input_string << std::endl;
  int q0 = t.AddState();
  t.SetStart(q0);  

  int prev_q = q0;
  for(auto c : input_string) {
    int q = t.AddState();
    t.AddArc(prev_q, fst::StdArc(id_of_symb[c],id_of_symb[c],1.0,q));
    prev_q = q;
  }
  t.SetFinal(prev_q,1.0);

  t.Write("inp_binary.fst");
}
