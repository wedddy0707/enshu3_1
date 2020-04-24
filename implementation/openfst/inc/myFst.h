#ifndef MY_FST_H
#define MY_FST_H

#include <wchar.h>
#include <set>
#include <map>
#include <string>

namespace myFst {
  void dictionaryFst (
        std::map<wchar_t,int>      id_of_character,
        std::map<std::wstring,int> id_of_word,
        std::map<std::wstring,double> cost_of_word
  );
  void inputFst (
        std::map<wchar_t,int> id_of_character
  );
  void make (
        std::map<wchar_t,int>         id_of_character,
        std::map<std::wstring,int>    id_of_word,
        std::map<std::wstring,double> cost_of_word
  );
}

#endif
