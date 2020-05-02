#include <stdio.h>
#include <wchar.h>
#include <string>
#include "inc/haikuFst.h"

int main(int argc,char *argv[])
{
  std::string   wsymb_path;
  std::string   wpron_path;
  std::string   fst_name;

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
        case 'p':
          wpron_path = target;
          break;
        case 'o':
          fst_name = target;
          break;
        case 'k':
        default:
          exit(1);
      }
    }
  }

  myFst::haikuJudgeFst(
      wsymb_path,
      wpron_path,
      fst_name
      );

  return 0;
}
