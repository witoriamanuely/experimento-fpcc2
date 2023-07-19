#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <bitset>
#include <dirent.h>
#include <chrono>
#include "flc.h"

// Função para comprimir dados usando Fixed-Length Code
std::string compressData(const std::string& data, int codeLength)
{
    std::unordered_map<char, std::string> codeTable;
    int nextCode = 0;

    // Constrói a tabela de códigos de comprimento fixo
    for (char c : data) {
        if (codeTable.find(c) == codeTable.end()) {
            codeTable[c] = std::to_string(nextCode);
            nextCode++;
        }
    }

    // Comprime os dados usando os códigos de comprimento fixo
    std::string compressedData;
    for (char c : data) {
        compressedData += codeTable[c];
    }

    return compressedData;
}

// Função para descomprimir dados usando Fixed-Length Code
std::string decompressData(const std::string& compressedData, int codeLength)
{
    std::unordered_map<std::string, char> codeTable;
    int nextCode = 0;

    // Constrói a tabela de códigos de comprimento fixo
    for (char c = 0; c < 256; ++c) {
        std::string code = std::to_string(nextCode);
        codeTable[code] = c;
        nextCode++;
    }

    // Descomprime os dados usando os códigos de comprimento fixo
    std::string decompressedData;
    std::string currentCode;
    for (char c : compressedData) {
        currentCode += c;
        if (currentCode.length() == codeLength) {
            decompressedData += codeTable[currentCode];
            currentCode.clear();
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

std::string buildCompressFLC(std::string& data){
    int codeLength = 4;
    std::string compressedData = compressData(data, codeLength);
    return compressedData;
}


