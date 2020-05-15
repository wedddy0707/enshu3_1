#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string>
#include <vector>
#include "inc/haikuFst.h"

int main(int argc,char *argv[])
{
  std::string  wsymb_path       = "./word_symbols.txt";
  std::string  fst_name         = "./haiku_prefix.fst";
  std::string  input_file_path  = "./data/sample_input.txt";
  std::wstring prefix;

  for(int i = 0; i < argc; i++) {
    if (*argv[i]=='-') {
      if (i+1==argc) {
        exit(1);
      }
      char *target = argv[i+1];
      switch(*(argv[i]+1)) {
        case 'w':
          wsymb_path = target;
          break;
        case 'i':
          input_file_path = target;
          break;
        case 'o':
          fst_name = target;
          break;
        default:
          exit(1);
      }
    }
  }

  myFst::haikuPrefixFst(
      wsymb_path,
      input_file_path,
      fst_name
      );

  return 0;
}
