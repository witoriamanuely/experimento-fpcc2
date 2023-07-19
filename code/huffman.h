
#include <iostream>
#include <unordered_map>
#include <string>
#include <dirent.h>
#include <fstream>
#include <chrono>

#ifndef HUFFMAN_H
#define HUFFMAN_H

struct HuffmanNode;
struct Compare;
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& frequencies);
void buildHuffmanCodes(HuffmanNode* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes);
std::string decompressData(const std::string& compressedData, HuffmanNode* root);
std::string compressData(const std::string& data, const std::unordered_map<char, std::string>& huffmanCodes);
std::string buildCompressHuffman(std::string& data);

#endif