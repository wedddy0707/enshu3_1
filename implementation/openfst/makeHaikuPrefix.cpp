#include <stdio.h>
#include <wchar.h>
#include <string>
#include <vector>
#include "inc/haikuFst.h"

int main(int argc,char *argv[])
{
  std::string wsymb_path;
  std::string fst_name;

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
      std::vector<std::wstring>({L"君",L"が",L"持つ"}),
      fst_name
      );

  return 0;
}
