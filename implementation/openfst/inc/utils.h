#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include <string>
#include <vector>

FILE* fopen_with_errmsg(const char* path,const char* mode);

std::vector<std::wstring> split(std::wstring str, wchar_t del);

double cost_from_frequency(int frequency);
double cost_from_probability(double p);

#endif
