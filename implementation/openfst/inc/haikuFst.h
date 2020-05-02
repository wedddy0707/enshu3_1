#ifndef haiku_fst_h
#define haiku_fst_h

#include <wchar.h>
#include <vector>
#include <string>
#include <map>

namespace myFst {
  void haikuPrefixFst (
      const std::string&                wsymb_path,
      const std::vector<std::wstring>&  words,
      const std::string&                fst_name
      );

  void haikuJudgeFst (
      const std::string& wsymb_path,
      const std::string& wpron_path,
      const std::string&   fst_name
      );
}
#endif
