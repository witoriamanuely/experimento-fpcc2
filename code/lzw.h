#include <iostream>
#include <unordered_map>
#include <string>
#include <dirent.h>
#include <fstream>
#include <chrono>
#ifndef LZW_H
#define LZW_H

std::unordered_map<std::string, int> initializeDictionary();
std::string compressData(const std::string& data);
std::string decompressData(const std::string& compressedData);
std::string buildCompressLZW(std::string& data);
#endif