#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#include <string>
#include <set>
#include <vector>

FILE* fopen_with_errmsg(const char* path,const char* mode)
{
  FILE* fp;

  if((fp = fopen(path,mode)) == NULL) {
    fprintf(stderr,"Failed to Open File. \"%s\" with mode \"%s\"\n", path, mode);
    exit(EXIT_FAILURE);
  }

  return fp;
}

std::vector<std::wstring> split(std::wstring str, wchar_t del)
{
  int first = 0;
  int last  = str.find_first_of(del);

  std::vector<std::wstring> result;

  while (first < str.size()) {
    std::wstring subStr(str, first, last - first);

    result.push_back(subStr);

    first = last+1;
    last  = str.find_first_of(del,first);

    if (last == std::wstring::npos) {
      last = str.size();
    }
  }
}

wchar_t katakana_of(wchar_t hira) {
  return hira + (L'ア'-L'あ');
}

wchar_t hiragana_of(wchar_t kata) {
  return kata + (L'あ'-L'ア');
}

bool is_katakana(wchar_t kana) {
  return L'ア' <= kana && kana <= L'ン';
}

bool is_hiragana(wchar_t kana) {
  return L'あ' <= kana && kana <= L'ん';
}
