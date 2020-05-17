#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <fst/fstlib.h>
#include <locale.h>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include "utils.h"
#include "symbol.h"

typedef std::wstring wstr_t;

namespace myFst {
  void contextAbsorbFst (
      const std::string& wsymb_path,
      const std::string& input_file_path,
      const std::string& fst_name = "haiku_absorb.fst"
  )
  {
    fst::VectorFst<fst::StdArc> t;
    std::map<wstr_t,int> id_of_word;

    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    
    symbol::makeMapFromSymbFile(wsymb_path,&id_of_word);
    
    FILE* fp = fopen_with_errmsg(input_file_path.c_str(),"r");

    wchar_t str[100];
    
    fgetws(str,100,fp);

    std::vector<wstr_t> prefix_words = split(find_and_erase(str,L"\n"),L" ");

    int prev_q = t.AddState(); t.SetStart(prev_q);
    
    for(const auto& w : prefix_words) {
      int id;
      if (id_of_word.find(w)==id_of_word.end()) { // Unknown Character
        //id = symbol::ID_OF_UNK;
        continue;
      } else {
        id = id_of_word.at(w);
      }

      int    next_q = t.AddState();
      t.AddArc(prev_q,fst::StdArc(id,symbol::ID_OF_EPS,0.0,next_q));
      prev_q = next_q;
    }
    t.SetFinal(prev_q);

    for(const auto& w : id_of_word) {
      const int& id = w.second;
      t.AddArc(prev_q,fst::StdArc(id,id,0.0,prev_q));
    }

    t.Write(fst_name);
  }
  void contextGenerateFst (
      const std::string& wsymb_path,
      const std::string& input_file_path,
      const std::string& fst_name = "context_generate.fst"
  )
  {
    fst::StdVectorFst    t;
    std::map<wstr_t,int> id_of_word;

    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    
    symbol::makeMapFromSymbFile(wsymb_path,&id_of_word);
    
    FILE* fp = fopen_with_errmsg(input_file_path.c_str(),"r");

    wchar_t str[100];
    
    fgetws(str,100,fp);

    std::vector<wstr_t> prefix_words = split(find_and_erase(str,L"\n"),L" ");

    int prev_q = t.AddState(); t.SetStart(prev_q);
    
    for(const auto& w : prefix_words) {
      int id;
      if (id_of_word.find(w)==id_of_word.end()) { // Unknown Character
        //id = symbol::ID_OF_UNK;
        continue;
      } else {
        id = id_of_word.at(w);
      }

      int    next_q = t.AddState();
      t.AddArc(prev_q,fst::StdArc(symbol::ID_OF_EPS,id,0.0,next_q));
      prev_q = next_q;
    }
    t.SetFinal(prev_q);

    for(const auto& w : id_of_word) {
      const int& id = w.second;
      t.AddArc(prev_q,fst::StdArc(id,id,0.0,prev_q));
    }

    t.Write(fst_name);
  }
}
