#ifndef haiku_fst_h
#define haiku_fst_h

#include <wchar.h>
#include <vector>
#include <string>
#include <map>

namespace myFst {
  void haikuPrefixFst (
      const std::string&,
      const std::string&,
      const std::string&
      );

  void haikuJudgeFst (
      const std::string&,
      const std::string&,
      const std::string&
      );
}
#endif
