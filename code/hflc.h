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
std::string compressData(const std::string& data, const std::unordered_map<char, std::string>& huffmanCodes);
void buildHuffmanCodes(HuffmanNode* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes);
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& frequencies);
std::string decompressData(const std::string& compressedData, int codeLength);
std::string buildCompressHFLC(std::string& data);
#endif