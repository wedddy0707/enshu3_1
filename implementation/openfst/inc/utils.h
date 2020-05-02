#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include <string>
#include <vector>

FILE* fopen_with_errmsg(const char* path,const char* mode);

std::vector<std::wstring> split(std::wstring str, wchar_t del);
std::wstring join(const std::vector<std::wstring>& v, const wchar_t* delim = 0);

double cost_from_frequency(int frequency);
double cost_from_probability(double p);

int num_of_moras_of(std::wstring word);
#endif
