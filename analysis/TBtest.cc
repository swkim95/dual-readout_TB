#include <iostream>
#include "../include/TBread.h"
#include <string>

int TBtest() {
  std::cout << "hi" << std::endl;

  TBread reader;
  reader.setPedestalPath("sample_ped.csv");

  auto s = reader.pedestalpath();
  std::cout << s << std::endl;
  return 0;
}
