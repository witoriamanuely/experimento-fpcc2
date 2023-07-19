#include <iostream>
#include <unordered_map>
#include <string>
#include <dirent.h>
#include <fstream>
#include <chrono>
#ifndef HFLC_H
#define HFLC_H

struct HuffmanNode;
struct Compare;
std::string compressDataHFLC(const std::string& data, const std::unordered_map<char, std::string>& huffmanCodes);
void buildHuffmanCodesHFLC(HuffmanNode* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes);
HuffmanNode* buildHuffmanTreeHFLC(const std::unordered_map<char, int>& frequencies);
std::string decompressDataHFLC(const std::string& compressedData, int codeLength);
std::string buildCompressHFLC(std::string& data);
#endif