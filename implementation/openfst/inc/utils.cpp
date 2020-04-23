#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>
#include <string>
#include <set>

FILE* fopen_with_errmsg(const char* path,const char* mode)
{
  FILE* fp;

  if((fp = fopen(path,mode)) < 0) {
    fprintf(stderr,"Failed to Open File. \"%s\" with mode \"%s\"\n", path, mode);
    exit(EXIT_FAILURE);
  }

  return fp;
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
