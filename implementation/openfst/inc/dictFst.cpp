#include <stdio.h>
#include <wchar.h>
#include <fst/fstlib.h>
#include <set>
#include <map>
#include <string>
#include <locale.h>
#include "utils.h"
#include "symbol.h"

typedef std::wstring wstr_t;

namespace myFst {
  void makeDictFst (
      std::string csymb_path,
      std::string wsymb_path,
      std::string fst_name
  )
  {
    fst::StdVectorFst t;
    std::map<wchar_t ,int> id_of_char;
    std::map<wstr_t  ,int> id_of_word;
    
    setlocale(LC_CTYPE, "ja_JP.UTF-8");

    symbol::makeMapFromSymbFile(csymb_path,&id_of_char);
    symbol::makeMapFromSymbFile(wsymb_path,&id_of_word);
  
    int q0 = t.AddState();t.SetStart(q0);
    int q1 = t.AddState();t.SetFinal(q1);

    for(auto iter : id_of_word) {
      wstr_t word = iter.first;
      int      id = iter.second;
      int  prev_q = q0;
  
      for(auto wchar : (iter.first)) {
        int next_q = t.AddState();
        int      i = id_of_char[wchar];
        int      o = symbol::ID_OF_EPS;
  
        t.AddArc(prev_q, fst::StdArc(i,o,0.0,next_q));
  
        prev_q = next_q;
      }
      int i = symbol::ID_OF_EPS;
      int o = iter.second;
  
      t.AddArc(prev_q, fst::StdArc(i,o,0.0,q1));

      // デバッグ用
      std::wstring msg =
        L"making dictFst: now we get a new lexcon \""+word+L"\".\n";
      fputws(msg.c_str(),stdout);
    }
    fst::Closure(&t,(fst::ClosureType)0);

    t.Write(fst_name);
  }

}
