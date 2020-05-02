#include <stdio.h>
#include <string>
#include "inc/dictFst.h"

int main(int argc,char *argv[])
{
  std::string csymb_path  = "./character_symbols.txt";
  std::string wsymb_path  = "./word_symbols.txt";
  std::string fst_name = "dict.fst";

  for(int i = 0; i < argc; i++) {
    if (*argv[i]=='-') {
      if (i+1==argc) {
        fprintf(stderr,"Bad Option Usage.");
        exit(1);
      }
      char *target = argv[i+1];

      switch(*(argv[i]+1)) {
        case 'c':
          //fprintf(stdout,"Character symbols are taken from %s\n",target);
          csymb_path = target;
          break;
        case 'w':
          //fprintf(stdout,"Word symbols are taken from %s\n",target);
          wsymb_path = target;
          break;
        case 'o':
          //fprintf(stdout,"The fst name will be %s\n",target);
          fst_name = target;
          break;
        default:
          //fprintf(stderr,"Bad Option: %s", argv[i]);
          exit(1);
      }
    }
  }

  myFst::makeDictFst(
      csymb_path,
      wsymb_path,
      fst_name
      );

}
