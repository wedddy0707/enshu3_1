#include <stdio.h>
#include <wchar.h>
#include <set>
#include <map>
#include <string>
#include "inc/symbol.h"
#include "inc/myFst.h"

int main(int argc, char *argv[]) {
  std::set<wchar_t>             characters     ;
  std::set<std::wstring>        words          ;
  std::map<std::wstring,double> cost_of_word   ;
  std::map<wchar_t,int>         id_of_character;
  std::map<std::wstring,int>    id_of_word     ;

  symbol::make (
      &characters,
      &words,
      &cost_of_word,
      &id_of_character,
      &id_of_word
      );
  myFst::make (
      id_of_character,
      id_of_word,
      cost_of_word
      );


  return 0;
}

