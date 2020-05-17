#ifndef CONTEXT_FST_H
#define CONTEXT_FST_H

#include <string>

namespace myFst {
  void contextAbsorbFst (
      const std::string&,
      const std::string&,
      const std::string&
  );
  void contextGenerateFst (
      const std::string&,
      const std::string&,
      const std::string&
  );
}

#endif
