#include <wchar.h>
#include <string.h>
#include <locale>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "inc/utils.h"

int main(int argc, char *argv[])
{
  setlocale(LC_CTYPE, "ja_JP.UTF-8");

  FILE *fp;

  if (argc < 2) {
    fp = stdin;
  } else {
    fp = fopen_with_errmsg(argv[1],"r");
  }

  std::map<std::wstring,int> lines;
  wchar_t line[100];
  while(fgetws(line,100,fp)!=NULL) {
    std::vector<std::wstring> words_and_freq = split(line,L"\t");
    lines[words_and_freq[0]] = (int)std::wcstol(words_and_freq[1].c_str(),NULL,10);
  }

  for(auto l : lines) {
    fputws(l.first.c_str(),stdout);
    fputws(L"\t",stdout);
    fputws(std::to_wstring(l.second).c_str(),stdout);
    fputws(L"\n",stdout);
  }

  if (argc >= 2) {
    fclose(fp);
  }
  return 0;
}
