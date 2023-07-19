#include <iostream>
#include <unordered_map>
#include <string>
#include <dirent.h>
#include <fstream>
#include <chrono>

#ifndef FLC_H
#define FLC_H
std::string compressData(const std::string& data, int codeLength);
std::string buildCompressFLC(std::string& data);
#endif