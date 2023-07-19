#include <iostream>
#include <unordered_map>
#include <string>
#include <dirent.h>
#include <fstream>
#include <chrono>
#include "lzw.h"

// Função para inicializar o dicionário com os caracteres ASCII
std::unordered_map<std::string, int> initializeDictionary()
{
    std::unordered_map<std::string, int> dictionary;
    for (int i = 0; i < 256; ++i) {
        std::string ch(1, char(i));
        dictionary[ch] = i;
    }
    return dictionary;
}

// Função para comprimir dados usando o algoritmo LZW
std::string compressData(const std::string& data)
{
    std::unordered_map<std::string, int> dictionary = initializeDictionary();
    std::string currentSymbol;
    std::string compressedData;
    int nextCode = 256;

    for (char c : data) {
        std::string currentSymbolPlusC = currentSymbol + c;
        if (dictionary.count(currentSymbolPlusC)) {
            currentSymbol = currentSymbolPlusC;
        } else {
            compressedData += std::to_string(dictionary[currentSymbol]) + " ";
            dictionary[currentSymbolPlusC] = nextCode;
            ++nextCode;
            currentSymbol = std::string(1, c);
        }
    }

    if (!currentSymbol.empty()) {
        compressedData += std::to_string(dictionary[currentSymbol]) + " ";
    }

    return compressedData;
}

// Função para descomprimir dados usando o algoritmo LZW
std::string decompressData(const std::string& compressedData)
{
    std::unordered_map<int, std::string> dictionary;
    for (int i = 0; i < 256; ++i) {
        std::string ch(1, char(i));
        dictionary[i] = ch;
    }
    int nextCode = 256;

    std::string decompressedData;
    std::string currentCode;
    for (char c : compressedData) {
        if (c == ' ') {
            int code = stoi(currentCode);
            if (dictionary.count(code)) {
                decompressedData += dictionary[code];
                if (!currentCode.empty()) {
                    dictionary[nextCode] = dictionary[code] + dictionary[stoi(currentCode)];
                    ++nextCode;
                }
            }
            currentCode.clear();
        } else {
            currentCode += c;
        }
    }

    return decompressedData;
}

float calculateEntropy(const std::string& data) {
    std::unordered_map<char, int> charCount;

    // Conta a ocorrência de cada caractere
    for (char c : data) {
        charCount[c]++;
    }

    // Calcula a entropia
    float entropy = 0.0;
    int dataSize = data.size();

    for (const auto& pair : charCount) {
        float probability = static_cast<float>(pair.second) / dataSize;
        entropy -= probability * std::log2(probability);
    }

    return entropy;
}


std::string buildCompressLZW(std::string& data){
    std::string compressedData = compressData(data);
    return compressedData;
}

