#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <dirent.h>
#include <chrono>

// Estrutura de um nó da árvore de Huffman
struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int frequency)
        : data(data), frequency(frequency), left(nullptr), right(nullptr) {}
};

// Comparador personalizado para a fila de prioridade (min-heap)
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b)
    {
        return a->frequency > b->frequency;
    }
};

// Função para criar a árvore de Huffman
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& frequencies)
{
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> pq;

    // Cria um nó para cada caractere e adiciona na fila de prioridade
    for (const auto& pair : frequencies) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Constrói a árvore de Huffman
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();

        HuffmanNode* newNode = new HuffmanNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    // Retorna o nó raiz da árvore de Huffman
    return pq.top();
}

// Função auxiliar para construir a tabela de códigos de Huffman
void buildHuffmanCodes(HuffmanNode* root, std::string code, std::unordered_map<char, std::string>& huffmanCodes)
{
    if (root == nullptr) {
        return;
    }

    // Se o nó é uma folha, armazena o código de Huffman
    if (!root->left && !root->right) {
        huffmanCodes[root->data] = code;
    }

    // Percorre a árvore recursivamente
    buildHuffmanCodes(root->left, code + "0", huffmanCodes);
    buildHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Função para comprimir dados usando a tabela de códigos de Huffman
std::string compressData(const std::string& data, const std::unordered_map<char, std::string>& huffmanCodes)
{
    std::string compressedData;

    // Comprime os dados usando a tabela de códigos de Huffman
    for (char c : data) {
        compressedData += huffmanCodes.at(c);
    }

    return compressedData;
}

// Função para descomprimir dados usando a árvore de Huffman
std::string decompressData(const std::string& compressedData, HuffmanNode* root)
{
    std::string decompressedData;
    HuffmanNode* currentNode = root;

    // Percorre a string comprimida e reconstrói os dados originais
    for (char bit : compressedData) {
        if (bit == '0') {
            currentNode = currentNode->left;
        } else {
            currentNode = currentNode->right;
        }

        // Se é um nó folha, adiciona o caractere correspondente
        if (!currentNode->left && !currentNode->right) {
            decompressedData += currentNode->data;
            currentNode = root;
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
    // Verifica se a pasta existe
    DIR* dir = opendir(inputFolderPath.c_str());
    if (dir == nullptr) {
        std::cerr << "A pasta de entrada não existe ou não é um diretório válido." << std::endl;
        return 1;
    }
    closedir(dir);

    // Frequência dos caracteres
    std::unordered_map<char, int> frequencies;

    // Itera pelos arquivos na pasta de entrada
    dir = opendir(inputFolderPath.c_str());
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_type == DT_REG) {
            // Ler o conteúdo do arquivo
            std::ifstream inFile(inputFolderPath + entry->d_name);
            if (!inFile) {
                std::cerr << "Erro ao abrir o arquivo de entrada: " << entry->d_name << std::endl;
                continue;
            }

            std::string data((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

            // Atualiza a frequência dos caracteres
            for (char c : data) {
                frequencies[c]++;
            }

            inFile.close();
        }
    }
    closedir(dir);

    // Construir a árvore de Huffman
    HuffmanNode* root = buildHuffmanTree(frequencies);

    // Construir a tabela de códigos de Huffman
    std::unordered_map<char, std::string> huffmanCodes;
    buildHuffmanCodes(root, "", huffmanCodes);
    
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
            std::string compressedData = compressData(data, huffmanCodes);
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
            std::cout << "Tempo de execução da Compressão: " << seconds << " segundos" << std::endl;
            std::string decompressedData = decompressData(compressedData, root);
            
            
            /*std::cout << "Arquivo de entrada: " << inputFolderPath + entry->d_name << std::endl;
            std::cout << "Tamanho original: " << data.size() << " bytes" << std::endl;
            std::cout << "Tamanho comprimido: " << compressedData.size() / 8 << " bytes" << std::endl;
            std::cout << "Tamanho descomprimido: " << decompressedData.size() / 8 << " bytes" << std::endl;
            std::cout << "Taxa de compressão: " << ((compressedData.size() / 8.0) / data.size()) * 100 << "%" << std::endl;*/
        }
    }
    closedir(dir);
    for (const auto& pair : myDictEntropy) {
        std::cout << "file: " << pair.first << ", entropy: " << pair.second << std::endl;
        
        }
    

    return 0;
}
