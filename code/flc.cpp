#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <bitset>
#include <dirent.h>
#include <chrono>
#include "flc.h"

// Função para comprimir dados usando Fixed-Length Code
std::string compressDataFLC(const std::string& data, int codeLength)
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
std::string decompressDataFLC(const std::string& compressedData, int codeLength)
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


std::string buildCompressFLC(std::string& data){
    int codeLength = 4;
    std::string compressedData = compressDataFLC(data, codeLength);
    return compressedData;
}


