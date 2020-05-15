#ifndef SYMBOL_H
#define SYMBOL_H

#include <wchar.h>
#include <set>
#include <map>
#include <string>

namespace symbol {
  enum id_of_special_characters {
    ID_OF_EPS, // <eps> の ID は 0
    ID_OF_UNK,  // unknown character の ID は 1
    ID_OF_0B0,
    ID_OF_0B1
  };

  int min_id();

  void make_symbs_from_BCCJ (
        std::set<wchar_t>&             characters,
        std::set<std::wstring>&        words,
        std::map<std::wstring,double>& cost_of_word,
        std::map<std::wstring,std::wstring>& mora_of_word
  );
  void make_ids (
        const std::set<wchar_t>&      characters,
        const std::set<std::wstring>& words,
        std::map<wchar_t,int>&        id_of_character,
        std::map<std::wstring,int>&   id_of_word
  );
  void make_symbol_file (
        const std::map<wchar_t,int>&      id_of_character,
        const std::map<std::wstring,int>& id_of_word
  );
  void makeMapFromSymbFile(std::string,std::map<wchar_t,int> *);
  void makeMapFromSymbFile(std::string,std::map<std::wstring,int> *);
  void makeMapFromSymbFile(std::string,std::map<std::wstring,std::wstring> *);
  void make (
      std::string char_symbols_file_name = "character_symbols.txt",
      std::string word_symbols_file_name = "word_symbols.txt",
      std::string  bin_symbols_file_name = "binary_symbols.txt",
      std::string   word_prons_file_name = "word_prons.txt"
  );
}

#endif
