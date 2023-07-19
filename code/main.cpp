#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <bitset>
#include <dirent.h>
#include <chrono>
#include "lzw.h"
#include "flc.h"
#include "hflc.h"
#include "huffman.h"
#include "hflc.h"
#include "flc.h"


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

// Função para imprimir as informações da compressão
void printCompressionInfo(const std::string& data, const std::string& compressedData, const std::string& file,double seconds, const std::string& tecnica)
{   
    std::cout << "Técnica: " << tecnica << std::endl;
    std::cout << "Arquivo: " << file << std::endl;
    std::cout << "Tamanho original: " << data.size() << " bytes" << std::endl;
    std::cout << "Tamanho comprimido: " << compressedData.size() / 8 << " bytes" << std::endl;
    std::cout << "Taxa de compressão: " << (compressedData.size() / 8.0) / data.size() * 100 << "%" << std::endl;
    std::cout << "Tempo de execução da Compressão: " << seconds << " segundos" << std::endl;
}


void printCompressionInfoLZW(const std::string& data, const std::string& compressedData, const std::string& file,double seconds, const std::string& tecnica)
{   
    double compressedValue = static_cast<double>(compressedData.size()) / data.size();
    std::cout << "Técnica: " << tecnica << std::endl;
    std::cout << "Arquivo: " << file << std::endl;
    std::cout << "Tamanho original: " << data.size() << " bytes" << std::endl;
    std::cout << "Tamanho comprimido: " << compressedData.size() << " bytes" << std::endl;
    std::cout << "Taxa de compressão: " << compressedValue * 100 << "%" << std::endl;
    std::cout << "Tempo de execução da Compressão: " << seconds << " segundos" << std::endl;
}


int main(int argc, char* argv[])
{   
    

    std::string inputFolderPath = "";
    if (argc > 1){
        inputFolderPath = argv[1];
    }else{
        return 1;
    }


    int codeLength = 4;

    DIR* dir = opendir(inputFolderPath.c_str());
    struct dirent* entry;
    if (dir == nullptr) {
        std::cerr << "A pasta de entrada não existe ou não é um diretório válido." << std::endl;
        return 1;
    }
    closedir(dir);
    
    dir = opendir(inputFolderPath.c_str());
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
        
            std::ifstream inFile(inputFolderPath + entry->d_name);
            if (!inFile) {
                std::cerr << "Erro ao abrir o arquivo de entrada: " << entry->d_name << std::endl;
                continue;
            }

            std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

            auto startLZW = std::chrono::high_resolution_clock::now();
            std::string compressedDataLZW =  buildCompressLZW(data);
            auto endLZW = std::chrono::high_resolution_clock::now();
            auto durationLZW = std::chrono::duration_cast<std::chrono::duration<double> >(endLZW - startLZW);

            auto startFLC = std::chrono::high_resolution_clock::now();
            std::string compressedDataFLC =  buildCompressFLC(data);
            auto endFLC = std::chrono::high_resolution_clock::now();
            auto durationFLC = std::chrono::duration_cast<std::chrono::duration<double> >(endFLC - startFLC);


            auto startHFLC = std::chrono::high_resolution_clock::now();
            std::string compressedDataHFLC=  buildCompressHFLC(data);
            auto endHFLC = std::chrono::high_resolution_clock::now();
            auto durationHFLC = std::chrono::duration_cast<std::chrono::duration<double> >(endHFLC - startHFLC);

            auto startHuffman = std::chrono::high_resolution_clock::now();
            std::string compressedDataHuffman =  buildCompressHuffman(data);
            auto endHuffman = std::chrono::high_resolution_clock::now();
            auto durationHuffman = std::chrono::duration_cast<std::chrono::duration<double> >(endHuffman - startHuffman);


            printCompressionInfoLZW(data, compressedDataLZW, inputFolderPath + entry->d_name, durationLZW.count(), "LZW");
            printCompressionInfo(data, compressedDataFLC, inputFolderPath + entry->d_name, durationFLC.count(), "FLC");
            printCompressionInfo(data, compressedDataHFLC, inputFolderPath + entry->d_name, durationHFLC.count(), "HFLC");
            printCompressionInfo(data, compressedDataHuffman, inputFolderPath + entry->d_name, durationHuffman.count(), "Huffman");

        }
    }
    closedir(dir);
  
    return 0;
}
