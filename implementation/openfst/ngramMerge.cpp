#include <wchar.h>
#include <string.h>
#include <locale>
#include <string>
#include <map>
#include <vector>
#include "inc/utils.h"

int main(int argc, char *argv[])
{
  FILE     *fp[argc];
  wchar_t line[argc][300];
  bool     fin[argc];
  bool      go[argc];
  
  setlocale(LC_CTYPE, "ja_JP.UTF-8");
  
  if (argc < 2) {
    fputws(L"ERROR: No Input File Specified",stderr);
    return 1;
  }

  for(int i = 1; i < argc; i++) {
    fp [i] = fopen_with_errmsg(argv[i],"r");
    fin[i] = false;
    go [i] = true;
  }

  std::map<std::wstring,int> m;

  while(true) {
    bool fin_all = true;
    for(int i = 1; i < argc; i++) {
      if ((not fin[i]) and go[i]) {
        fin[i] = (fgetws(line[i],100,fp[i])==NULL);
        go [i] = false;
      }
      fin_all = fin_all and fin[i];
    }
    
    if (fin_all) {
      break;
    }
    
    for(int i = 1; i < argc; i++) {
      if (not fin[i]) {
        // 各行は word_1<space>...<space>word_n<tab>frequency と書かれている.
        // <tab> で split する.
        std::vector<std::wstring> l = split(line[i],L"\t");
        // map に入れることで words に関してソートされるはず.
        m[l[0]] = i;
      }
    }

    int i = m.begin()->second;
    fputws(line[i],stdout);
    go[i] = true;
    m.erase(m.begin());
  }

  for(int i=1; i<argc; i++) {
    fclose(fp[i]);
  }

  return 0;
}
