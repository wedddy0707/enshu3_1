#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#include <string>
#include <vector>

FILE* fopen_with_errmsg(const char* path,const char* mode);
int put_warning(const std::string&,const std::wstring&);

std::vector<std::wstring> split(const std::wstring&,const wchar_t*);
std::wstring join(const std::vector<std::wstring>&, const wchar_t*);
std::wstring find_and_erase(const std::wstring&, const wchar_t*);

double cost_from_frequency(int frequency);
double cost_from_probability(double p);

int num_of_moras_of(std::wstring word);
#endif
