#include <stdio.h>
#include <string.h>
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
  void makeInputFst (
      std::string csymb_path,
      std::string input_file_path,
      std::string fst_name
  )
  {
    std::map<wchar_t,int> id_of_char;
    wchar_t input_string[100] = L"";
    fst::StdVectorFst t;

    setlocale(LC_CTYPE, "ja_JP.UTF-8");

    symbol::makeMapFromSymbFile(csymb_path,&id_of_char);

    FILE* fp = fopen_with_errmsg(input_file_path.c_str(),"r");

    fgetws(input_string,100,fp);

    std::wstring msg =
      L"making inputFst: the input string is "+
      std::wstring(input_string)+
      L".\n";
    fputws(msg.c_str(),stdout);
  
    int q0 = t.AddState();
    t.SetStart(q0);  
  
    int prev_q = q0;
    for(auto c : std::wstring(input_string)) {
      if(c==L'\n') {
        continue;
      }

      int q = t.AddState();

      int id;
      if (id_of_char.find(c)==id_of_char.end()) { // Unknown Character
        id = symbol::ID_OF_UNK;
      } else {
        id = id_of_char[c];
      }
      t.AddArc(prev_q,fst::StdArc(id,id,0.0,q));
      prev_q = q;
    }
    t.SetFinal(prev_q,0.0);
  
    t.Write(fst_name);
  }
}
