#include <wchar.h>
#include <string.h>
#include <locale>
#include <string>
#include <map>
#include <vector>
#include "inc/symbol.h"
#include "inc/utils.h"

int main(int argc, char *argv[])
{

  std::string wsymb_path = "./word_symbols.txt";
  std::string  input_ngram_path = "";
  std::string output_ngram_path = "";

  for(int i = 0; i < argc; i++) {
    if (*argv[i]=='-') {
      if (i+1==argc) {
        fprintf(stderr,"Bad Option Usage.");
        exit(1);
      }
      char *target = argv[i+1];

      switch(*(argv[i]+1)) {
        case 'i':
          input_ngram_path = target;
          break;
        case 'o':
          output_ngram_path = target;
          break;
        case 'w':
          wsymb_path = target;
          break;
        default:
          exit(1);
      }
    }
  }

  setlocale(LC_CTYPE, "ja_JP.UTF-8");
  
  std::map<std::wstring,int> id_of_word;
  symbol::makeMapFromSymbFile(wsymb_path,&id_of_word);

  FILE* ifp;
  FILE* ofp;

  if (input_ngram_path == "") {
    ifp = stdin;
  } else {
    ifp = fopen_with_errmsg(input_ngram_path.c_str(),"r");
  }
  if (output_ngram_path == "") {
    ofp = stdout;
  } else {
    ofp = fopen_with_errmsg(output_ngram_path.c_str(),"w+");
  }
  
  wchar_t line[100];
  while(fgetws(line,100,ifp)!=NULL) {
    std::vector<std::wstring> words_and_freq = split(std::wstring(line),L"\t");
    std::vector<std::wstring> words          = split(words_and_freq[0] ,L" ");

    bool unknown = false;
    for(auto w : words) {
      if(unknown = unknown | (id_of_word.find(w)==id_of_word.end())) break;
    }

    if(not unknown) {
      fputws(line,ofp);
    } else {
      put_warning(argv[0],L"Unknown Word Detected in "+join(words,L","));
    }
  }

  if (ifp!=stdin) {
    fclose(ifp);
  }
  if (ofp!=stdout) {
    fclose(ofp);
  }
  return 0;
}
