#include <iostream>
#include <unordered_map>
#include <string>
#include <dirent.h>
#include <fstream>
#include <chrono>

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
            std::cout << "Arquivo de entrada: " << inputFolderPath + entry->d_name << std::endl;
            std::cout << "Tamanho original: " << data.size() << " bytes" << std::endl;
            // Comprimir os dados
            std::string compressedData = compressData(data);
            auto end = std::chrono::high_resolution_clock::now();
            float value = (static_cast<float>((compressedData.size())/ data.size())) * 100;
            float entropy = calculateEntropy(compressedData);

            // Calcular a duração em segundos
            std::chrono::duration<double> duration = end - start;
            myDictRatios[inputFolderPath + entry->d_name] = value;
            myDictSizes[inputFolderPath + entry->d_name] = compressedData.size();
            myDictEntropy[inputFolderPath + entry->d_name] = entropy;
            double seconds = duration.count();
            // Imprimir o tempo de execução
            //std::cout << "Tempo de execução da Compressão: " << seconds << " segundos" << std::endl;
            //std::string decompressedData = decompressData(compressedData);
            
            // Calcular a entropia dos dados originais
            
            
            //std::cout << "Tamanho comprimido: " << compressedData.size() << " bytes" << std::endl;
            //std::cout << "Tamanho descomprimido: " << decompressedData.size() / 8 << " bytes" << std::endl;
            //std::cout << "Taxa de compressão: " << value << "%" << std::endl;
        }
    }
    closedir(dir);
     for (const auto& pair : myDictEntropy) {
        std::cout << "file: " << pair.first << ", entropy: " << pair.second << std::endl;
     }
    
    return 0;
}
