#include <stdio.h>
#include <string>
#include "inc/symbol.h"

int main(int argc,char *argv[])
{
  std::string csymb_name = "character_symbols.txt";
  std::string wsymb_name = "word_symbols.txt";
  std::string bsymb_name = "binary_symbols.txt";

  for(int i = 0; i < argc; i++) {
    if (*argv[i]=='-') {
      if (i+1==argc) {
        fprintf(stderr,"Bad Option Usage.");
        exit(1);
      }
      switch(*(argv[i]+1)) {
        case 'c':
          fprintf(stdout,"The file name of character symbols will be %s\n",argv[i+1]);
          csymb_name = argv[i+1];
          break;
        case 'w':
          fprintf(stdout,"The file name of word symbols will be %s\n",argv[i+1]);
          wsymb_name = argv[i+1];
          break;
        case 'b':
          fprintf(stdout,"The file name of binary symbols will be %s\n",argv[i+1]);
          bsymb_name = argv[i+1];
          break;
        default:
          fprintf(stderr,"Bad Option: %s", argv[i]);
          exit(1);
      }
    }
  }

  symbol::make (
      csymb_name,
      wsymb_name,
      bsymb_name,
      "word_prons.txt"
      );
}
