#include <stdio.h>
#include <locale.h>
#include <math.h>
#include <string>
#include "inc/utils.h"

int main(int argc, char *argv[])
{
  std::string  input_ngram_txt_path = "";
  std::string output_ngram_txt_path = "";

  for(int i = 0; i < argc; i++) {
    if (*argv[i]=='-') {
      if (i+1==argc) {
        fprintf(stderr,"Bad Option Usage.");
        exit(1);
      }
      char *target = argv[i+1];

      switch(*(argv[i]+1)) {
        case 'i':
          input_ngram_txt_path = target;
          break;
        case 'o':
          output_ngram_txt_path = target;
          break;
        default:
          exit(1);
      }
    }
  }
  
  FILE *ifp;
  FILE *ofp;
  setlocale(LC_CTYPE, "ja_JP.UTF-8");

  if (input_ngram_txt_path == "") {
    ifp = stdin;
  } else {
    ifp = fopen_with_errmsg(input_ngram_txt_path.c_str(),"r");
  }
  if (output_ngram_txt_path == "") {
    ofp = stdout;
  } else {
    ofp = fopen_with_errmsg(output_ngram_txt_path.c_str(),"w+");
  }

  wchar_t line[100];
  while(fgetws(line,100,ifp)!=NULL) {
    std::vector<std::wstring> l = split(std::wstring(line),L'\t');
    
    if(l[l.size()-1]==L"BadNumber\n") {
      l[l.size()-1] = L"0.0\n";
      fputws(join(l,L"\t").c_str(),ofp);
      fputws(L"Warning: BadNumber detected.\n",stderr);
    } else if (l[l.size()-1]==L"Infinity\n") {
      // 重さ無限の辺など、存在しないのと一緒なので消す.
      fputws(L"Warning: Infinity detected.\n",stderr);
    } else if (l[l.size()-1]==L"-Infinity\n") {
      fputws(L"Warning: -Infinity detected.\n",stderr);
    } else {
      l[l.size()-1] = std::to_wstring(1.0 / exp((double)std::wcstol(l[l.size()-1].c_str(),NULL,10)))+L"\n";
      fputws(join(l,L"\t").c_str(),ofp);
    }
  }
  if(ifp!=stdin) {
    fclose(ifp);
  }
  if(ofp!=stdout) {
    fclose(ofp);
  }
  return 0;
}
