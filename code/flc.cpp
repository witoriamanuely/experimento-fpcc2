#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <bitset>
#include <dirent.h>
#include <chrono>

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


int main(int argc, char* argv[])
{   
    
    // Pasta com os arquivos de entrada
    std::string inputFolderPath = "calgarycorpus/";
    if (argc > 1){
        inputFolderPath = argv[1];
    }


    int codeLength = 4;
    // Verifica se a pasta existe
    DIR* dir = opendir(inputFolderPath.c_str());
    struct dirent* entry;
    if (dir == nullptr) {
        std::cerr << "A pasta de entrada não existe ou não é um diretório válido." << std::endl;
        return 1;
    }
    closedir(dir);
    std::unordered_map<std::string, float> myDictRatios;
    std::unordered_map<std::string, float> myDictSizes;
    std::unordered_map<std::string, float> myDictEntropy;
    // Itera novamente pelos arquivos na pasta de entrada para realizar a compressão e descompressão
    dir = opendir(inputFolderPath.c_str());
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            // Ler o conteúdo do arquivo
            std::ifstream inFile(inputFolderPath + entry->d_name);
            if (!inFile) {
                std::cerr << "Erro ao abrir o arquivo de entrada: " << entry->d_name << std::endl;
                continue;
            }

            std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

            auto start = std::chrono::high_resolution_clock::now();
            // Comprimir os dados
            std::string compressedData = compressData(data, codeLength);
            auto end = std::chrono::high_resolution_clock::now();

            // Calcular a duração em segundos
            float entropy = calculateEntropy(compressedData);

            // Calcular a duração em segundos
            std::chrono::duration<double> duration = end - start;
            myDictRatios[inputFolderPath + entry->d_name] = ((compressedData.size() / 8.0) / data.size()) * 100;
            myDictSizes[inputFolderPath + entry->d_name] = compressedData.size();
            myDictEntropy[inputFolderPath + entry->d_name] = entropy;
            double seconds = duration.count();
            
            // Imprimir o tempo de execução
            //std::cout << "Tempo de execução da Compressão: " << seconds << " segundos" << std::endl;
            //std::string decompressedData = decompressData(compressedData, codeLength);
            
            //std::cout << "Arquivo de entrada: " << inputFolderPath + entry->d_name << std::endl;
            //std::cout << "Tamanho original: " << data.size() << " bytes" << std::endl;
            //std::cout << "Tamanho comprimido: " << compressedData.size() / 8 << " bytes" << std::endl;
            //std::cout << "Tamanho descomprimido: " << decompressedData.size() / 8 << " bytes" << std::endl;
            //std::cout << "Taxa de compressão: " <<((compressedData.size() / 8.0) / data.size()) * 100 << "%" << std::endl;
        }
    }
    closedir(dir);
   for (const auto& pair : myDictEntropy) {
        std::cout << "file: " << pair.first << ", entropy: " << pair.second << std::endl;
     }
    
    return 0;
}
