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

bool is_kireji(const std::wstring& word) {
  if (word==L"や" or
      word==L"かな" or
      word==L"けり" or
      word==L"なり" or
      word==L"ぞ" or
      word==L"かも") {
    return true;
  } else {
    return false;
  }
}

bool is_joshi(const std::wstring& word) {
  return
    word==L"の" or
    word==L"が" or
    word==L"と" or
    word==L"に" or
    word==L"は" or
    word==L"で" or
    false;
}

namespace myFst {
  fst::VectorFst<fst::StdArc> moraFst_with_random_delimit (
      const std::map<std::wstring,int>&          id_of_word,
      const std::map<std::wstring,std::wstring>& pr_of_word
  )
  {
    fst::VectorFst<fst::StdArc> t;
  
    int q0 = t.AddState();
    t.SetStart(q0);
    int q1 = t.AddState();
    t.SetFinal(q1,0.0);

    for(const auto& iter : id_of_word) {
      const std::wstring& word = iter.first;
      const          int&   id = iter.second;
      
      if(id < symbol::min_id()){continue;}

      const int moras = num_of_moras_of(pr_of_word.at(word));
      int      prev_q = t.AddState();
      
      t.AddArc(q0,fst::StdArc(id,symbol::ID_OF_EPS,0.0,prev_q));

      if (not is_joshi(word)) {
        t.AddArc(q0,fst::StdArc(id,symbol::ID_OF_0B0,0.0,prev_q));
      }

      for(int cnt=0; cnt < moras; cnt++) {
        int next_q = t.AddState();
        t.AddArc(prev_q,fst::StdArc(symbol::ID_OF_EPS,symbol::ID_OF_0B1,0.0,next_q));
        prev_q = next_q;
      }

      t.AddArc(prev_q,fst::StdArc(symbol::ID_OF_EPS,symbol::ID_OF_EPS,0.0,q1));
      
      // デバッグ用の出力.
      std::wstring msg =
        L"making haikuJudge: the pron. of "+
        iter.first+
        L" is "+pr_of_word.at(iter.first)+
        L"\n";
      fputws(msg.c_str(),stdout);
    }
    fst::Closure(&t,(fst::ClosureType)0);

    return t;
  }

  fst::VectorFst<fst::StdArc> fst_01111101111111011111()
  {
    fst::VectorFst<fst::StdArc> t; // 01111101111111011111 を受理するFst

    int prev_q = t.AddState();
    t.SetStart(prev_q);

    for(int i = 0; i < 1+5+1+7+1+5; i++) {
      int  next_q  = t.AddState();
      int isymb_id = (i==0 | i==1+5 | i==1+5+1+7) ? symbol::ID_OF_0B0 : symbol::ID_OF_0B1;
      
      t.AddArc(prev_q,fst::StdArc(isymb_id,symbol::ID_OF_EPS,0.0,next_q));
      
      prev_q = next_q;
    }
    t.SetFinal(prev_q,0.0);

    return t;
  }
  
  fst::VectorFst<fst::StdArc> fst_011111011111110111110111111101111111()
  {
    fst::VectorFst<fst::StdArc> t; // 011111011111110111110111111101111111 を受理するFst

    int prev_q = t.AddState();
    t.SetStart(prev_q);

    for(int i = 0; i < 1+5+1+7+1+5+1+7+1+7; i++) {
      int  next_q  = t.AddState();
      int isymb_id =
        (i==0 | i==1+5 | i==1+5+1+7 | i==1+5+1+7+1+5 | i==1+5+1+7+1+5+1+7)
        ? symbol::ID_OF_0B0
        : symbol::ID_OF_0B1;
      
      t.AddArc(prev_q,fst::StdArc(isymb_id,symbol::ID_OF_EPS,0.0,next_q));
      
      prev_q = next_q;
    }
    t.SetFinal(prev_q,0.0);

    return t;
  }

  void haikuPrefixFst (
      const std::string& wsymb_path,
      const std::string& input_file_path,
      const std::string& fst_name = "haiku_prefix.fst"
  )
  {
    fst::VectorFst<fst::StdArc> t;
    std::map<std::wstring,int> id_of_word;

    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    
    symbol::makeMapFromSymbFile(wsymb_path,&id_of_word);
    
    FILE* fp = fopen_with_errmsg(input_file_path.c_str(),"r");

    wchar_t str[100];
    
    fgetws(str,100,fp);

    std::vector<std::wstring> prefix_words = split(find_and_erase(str,L"\n"),L" ");

    int prev_q = t.AddState(); t.SetStart(prev_q);
    
    for(const auto& w : prefix_words) {
      int id;
      if (w==L"\n") {
        continue;
      } else if (id_of_word.find(w)==id_of_word.end()) { // Unknown Character
        continue;
      } else {
        id = id_of_word.at(w);
      }

      int    next_q = t.AddState();
      t.AddArc(prev_q,fst::StdArc(id,id,0.0,next_q));
      prev_q = next_q;
    }
    t.SetFinal(prev_q);

    for(const auto& w : id_of_word) {
      const int& id = w.second;
      t.AddArc(prev_q,fst::StdArc(id,id,0.0,prev_q));
    }

    t.Write(fst_name);
  }

  void haikuJudgeFst (
      const std::string&  wsymb_path = "./word_symbols.txt",
      const std::string&  wpron_path = "./word_prons.txt",
      const std::string&    fst_name = "haikuJudge.fst"
  )
  {
    std::map<std::wstring,int> id_of_word;
    std::map<std::wstring,std::wstring> pr_of_word;
    fst::VectorFst<fst::StdArc> t;
    
    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    
    symbol::makeMapFromSymbFile(wsymb_path,&id_of_word);
    symbol::makeMapFromSymbFile(wpron_path,&pr_of_word);
    
    fst::Compose(
        moraFst_with_random_delimit(id_of_word,pr_of_word),
        fst_01111101111111011111(),
        &t
        );
    t.Write(fst_name);
  }
}
