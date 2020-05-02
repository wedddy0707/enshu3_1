#include <stdio.h>
#include <string>
#include "inc/inputFst.h"

int main(int argc,char *argv[])
{
  std::string input_file_path = "./data/sample_input.txt";
  std::string csymb_path  = "./character_symbols.txt";
  std::string fst_name = "input.fst";

  for(int i = 0; i < argc; i++) {
    if (*argv[i]=='-') {
      if (i+1==argc) {
        fprintf(stderr,"Bad Option Usage.");
        exit(1);
      }
      switch(*(argv[i]+1)) {
        case 'c':
          //fprintf(stdout,"Symbols are taken from %s\n",argv[i+1]);
          csymb_path = argv[i+1];
          break;
        case 'i':
          //fprintf(stdout,"The input file is set to %s\n",argv[i+1]);
          input_file_path = argv[i+1];
          break;
        case 'o':
          //fprintf(stdout,"The fst name will be %s\n",argv[i+1]);
          fst_name = argv[i+1];
          break;
        default:
          //fprintf(stderr,"Bad Option: %s", argv[i]);
          exit(1);
      }
    }
  }

  myFst::makeInputFst(
      csymb_path,
      input_file_path,
      fst_name
      );

}
