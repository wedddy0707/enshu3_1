#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <fst/fstlib.h>
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include "utils.h"

#define LEX_LIMIT  100000
#define STR_LENGTH 1000 // ファイルを行ごとに読むときのバッファサイズ

typedef std::wstring wstr_t;

namespace symbol {
  enum id_of_special_characters {
    ID_OF_EPS, // epsilon
    ID_OF_UNK, // unknown character
    ID_OF_BOS,
    ID_OF_EOS,
    ID_OF_0B0,
    ID_OF_0B1
  };

  int min_id() {
    return (int)std::max ((int)ID_OF_EPS,
                std::max ((int)ID_OF_UNK,
                std::max ((int)ID_OF_BOS,
                std::max ((int)ID_OF_EOS,
                std::max ((int)ID_OF_0B0,
                          (int)ID_OF_0B1))))) + 1;
  }


  void make_symbs_from_BCCJ (
        std::set<wchar_t>&       characters,
        std::set<wstr_t>&        words,
        std::map<wstr_t,double>& cost_of_word,
        std::map<wstr_t,wstr_t>& pron_of_word
  )
  {
    FILE *fp = fopen_with_errmsg("./data/BCCWJ_frequencylist_suw_ver1_0.tsv","r");
    fwide(fp,1);
    
    wchar_t input_string[STR_LENGTH] = L"";
    
    // 最初の行は捨てる
    fgetws(input_string,STR_LENGTH,fp);
    fputws(input_string,       stdout);
  
    while(fgetws(input_string,STR_LENGTH,fp) != NULL) {
      std::vector<std::wstring> line = split(std::wstring(input_string),L"\t");
      std::wstring lForm = line[1];
      std::wstring lemma = line[2];
  
      fputws(line[2].c_str(),stdout);
      fputws(L"\n",stdout);
  
      for(auto x : lemma) {
        characters.insert(x);
      }
 
      words.insert(lemma);

      if((pron_of_word.find(lemma)==pron_of_word.end())) {
        pron_of_word[lemma] = lForm;
      }
  
      if (std::wcstol(line[0].c_str(),NULL,10) >= LEX_LIMIT) {
        break;
      }
    }
    fclose(fp);
  }
  
  void make_ids (
        const std::set<wchar_t>&      characters,
        const std::set<std::wstring>& words,
        std::map<wchar_t,int>&        id_of_character,
        std::map<std::wstring,int>&   id_of_word
  )
  {
    int id_iter = min_id();
    for(const auto& x : characters) {
      id_of_character[x] = id_iter;
      id_iter++;
    }
    id_iter = min_id();
    for(const auto& x : words) {
      id_of_word[x] = id_iter;
      id_iter++;
    }
  }
  
  void make_character_symbols_file (
        const std::map<wchar_t,int>& id_of_character
  )
  {
    FILE *fp = fopen_with_errmsg("./character_symbols.txt","w+");
    fprintf(fp,"<eps> %d\n",ID_OF_EPS); // epsilon
    fprintf(fp,"<unk> %d\n",ID_OF_UNK); // unknown character
    for(const auto& x : id_of_character) {
      fprintf(fp,"%lc %d\n",x.first,x.second);
    }
    fclose(fp);
  }
  void make_word_symbols_file (
        const std::map<wstr_t,int>& id_of_word
  )
  {
    FILE *fp = fopen_with_errmsg("./word_symbols.txt","w+");
    fprintf(fp,"<eps> %d\n",ID_OF_EPS); // epsilon
    fprintf(fp,"<unk> %d\n",ID_OF_UNK); // unknown character
    fprintf(fp,"<S> %d\n",  ID_OF_BOS); // mark of "beginning of sentence"
    fprintf(fp,"</S> %d\n", ID_OF_EOS); // mark of "end of sentence"
    fprintf(fp,"0 %d\n",ID_OF_0B0);
    fprintf(fp,"1 %d\n",ID_OF_0B1);
    for(const auto& x : id_of_word) {
      fprintf(fp,"%ls %d\n",x.first.c_str(),x.second);
    }
    fclose(fp);
  }
  void make_binary_symbols_file ()
  {
    FILE *fp = fopen_with_errmsg("./binary_symbols.txt","w+");
    fprintf(fp,"<eps> %d\n",ID_OF_EPS);
    fprintf(fp,"<unk> %d\n",ID_OF_UNK);
    fprintf(fp,"0 %d\n",ID_OF_0B0);
    fprintf(fp,"1 %d\n",ID_OF_0B1);
    fclose(fp);
  }

  void make_symbol_file (
        const std::map<wchar_t,int>& id_of_character,
        const std::map<wstr_t ,int>& id_of_word
  )
  {
    make_character_symbols_file(id_of_character);
    make_word_symbols_file     (id_of_word);
    make_binary_symbols_file   ();
  }
  
  void make_pron_file (
      const std::map<wstr_t,wstr_t>& pron_of_word
  )
  {
    FILE *fp = fopen_with_errmsg("./word_prons.txt","w+");
    for(const auto& x : pron_of_word) {
      fprintf(fp,"%ls %ls\n",x.first.c_str(),x.second.c_str());
    }
    fclose(fp);
  }

  void make (
      std::string char_symbols_file_name = "character_symbols.txt",
      std::string word_symbols_file_name = "word_symbols.txt",
      std::string  bin_symbols_file_name = "binary_symbols.txt",
      std::string   word_prons_file_name = "word_prons.txt"
  )
  {
    std::set<wchar_t>                   characters      ;
    std::set<std::wstring>              words           ;
    std::map<std::wstring,double>       cost_of_word    ;
    std::map<std::wstring,std::wstring> pron_of_word    ;
    std::map<wchar_t,int>               id_of_character ;
    std::map<std::wstring,int>          id_of_word      ;

    setlocale(LC_CTYPE, "ja_JP.UTF-8");
    make_symbs_from_BCCJ(characters,words,cost_of_word,pron_of_word);
    make_ids(characters,words,id_of_character,id_of_word);
    make_symbol_file(id_of_character,id_of_word);
    make_pron_file(pron_of_word);
  }
  
  void makeMapFromSymbFile(
      std::string            path,
      std::map<wchar_t,int> *mp
  )
  {
    FILE* fp = fopen_with_errmsg(path.c_str(),"r");
    wchar_t s[100];
    int     id;

    while(fscanf(fp,"%ls%d",s,&id)==2) {
      (*mp)[s[0]] = id;
    }
  }
  void makeMapFromSymbFile(
      std::string           path,
      std::map<wstr_t,int> *mp
  )
  {
    FILE* fp = fopen_with_errmsg(path.c_str(),"r");
    wchar_t s[100];
    int     id;

    while(fscanf(fp,"%ls%d",s,&id)==2) {
      (*mp)[wstr_t(s)] = id;
    }
  }
  void makeMapFromSymbFile(
      std::string              path,
      std::map<wstr_t,wstr_t> *mp
  )
  {
    FILE* fp = fopen_with_errmsg(path.c_str(),"r");
    wchar_t s[100];
    wchar_t t[100];

    while(fscanf(fp,"%ls%ls",s,t)==2) {
      (*mp)[wstr_t(s)] = t;
    }
  }
}
