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

namespace myFst {
  void dictionaryFst (
        std::map<wchar_t,int>      id_of_character,
        std::map<std::wstring,int> id_of_word,
        std::map<std::wstring,double> cost_of_word
  )
  {
    fst::StdVectorFst t;
  
    int q0 = t.AddState();
    t.SetStart(q0);
    int q1 = t.AddState();
    t.SetFinal(q1);

    // Unknown Character が来たときの振る舞い
    int q2 = t.AddState();
    t.AddArc(q0,fst::StdArc(symbol::ID_OF_UNK,symbol::ID_OF_EPS,0.0,q2));
    t.AddArc(q2,fst::StdArc(symbol::ID_OF_EPS,symbol::ID_OF_UNK,0.0,q1));

    for(auto iter : id_of_word) {
      // iter.first  : 語彙
      // iter.second : その語彙の ID
      
      int prev_q = q0;
  
      for(auto wchar : (iter.first)) {
        int next_q = t.AddState();
        int      i = id_of_character[wchar];
        int      o = symbol::ID_OF_EPS;
        double   w = 0.0;
  
        t.AddArc(prev_q, fst::StdArc(i,o,w,next_q));
  
        prev_q = next_q;
      }
      int    i = symbol::ID_OF_EPS;
      int    o = iter.second;
      double w = cost_of_word[iter.first];
  
      t.AddArc(prev_q, fst::StdArc(i,o,w,q1));
    }
    fst::Closure(&t,(fst::ClosureType)0);
    t.Write("dic_binary.fst");
  }
  
  void inputFst (
        std::map<wchar_t,int> id_of_character
  )
  {
    wchar_t input_string[100] = L"";
    fst::StdVectorFst t;

    FILE* fp = fopen_with_errmsg("./data/sample_input.txt","r");

    fgetws(input_string,100,fp);
    fputws(L"input_string : ",stdout);
    fputws(input_string,stdout);
  
    int q0 = t.AddState();
    t.SetStart(q0);  
  
    int prev_q = q0;
    for(auto c : std::wstring(input_string)) {
      if(c==L'\n') {
        continue;
      }

      int q = t.AddState();
      fst::StdArc a;

      if (id_of_character.find(c)==id_of_character.end()) { // Unknown Character
        a = fst::StdArc(symbol::ID_OF_UNK,symbol::ID_OF_UNK,0.0,q);
      } else {
        a = fst::StdArc(id_of_character[c],id_of_character[c],0.0,q);
      }

      t.AddArc(prev_q,a);
      prev_q = q;
    }
    t.SetFinal(prev_q,0.0);
  
    t.Write("inp_binary.fst");
  }

  void make (
        std::map<wchar_t,int>         id_of_character,
        std::map<std::wstring,int>    id_of_word,
        std::map<std::wstring,double> cost_of_word
  )
  {
    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    inputFst(id_of_character);
    dictionaryFst (id_of_character, id_of_word, cost_of_word);
  }
}
