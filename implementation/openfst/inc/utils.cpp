#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <locale.h>

#include <cmath>
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
  std::vector<std::wstring> result;
  
  int first = 0;
  int last  = str.find_first_of(del);

  if (last == std::wstring::npos) {
    result.push_back(str);
    return result;
  }

  while (first < str.size()) {
    std::wstring subStr(str, first, last - first);

    result.push_back(subStr);

    first = last+1;
    last  = str.find_first_of(del,first);

    if (last == std::wstring::npos) {
      last = str.size();
    }
  }

  return result;
}

std::wstring join(const std::vector<std::wstring>& v, const wchar_t* delim = 0) {
  std::wstring s;
  if (!v.empty()) {
    s += v[0];
    for (decltype(v.size()) i = 1, c = v.size(); i < c; ++i) {
      if (delim) s += delim;
      s += v[i];
    }
  }
  return s;
}

double cost_from_frequency(int frequency)
{
  return (-1.0) * (double)frequency * std::log((double)frequency);  
}
double cost_from_probability(double p)
{
  return 1.0 / p;
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

bool is_lower_kana(wchar_t kana) {
  return
    kana == L'ァ' or kana == L'ぁ' or
    kana == L'ィ' or kana == L'ぃ' or
    kana == L'ゥ' or kana == L'ぅ' or
    kana == L'ェ' or kana == L'ぇ' or
    kana == L'ォ' or kana == L'ぉ' or
    kana == L'ャ' or kana == L'ゃ' or
    kana == L'ュ' or kana == L'ゅ' or
    kana == L'ョ' or kana == L'ょ' or
    kana == L'ッ' or kana == L'っ';
}

int num_of_moras_of(std::wstring word) {
  int n = 0;
  for(auto c : word) {
    if((is_katakana(c) or is_hiragana(c)) and not is_lower_kana(c)) {
      n++;
    }
    else if(c==L'ッ' or c==L'っ') {
      n++;
    }
  }

  return n;
}
