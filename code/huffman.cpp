#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <string>
#include <dirent.h>
#include <chrono>
#include "huffman.h"

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
std::string compressDataHuffman(const std::string& data, const std::unordered_map<char, std::string>& huffmanCodes)
{
    std::string compressedData;

    // Comprime os dados usando a tabela de códigos de Huffman
    for (char c : data) {
        compressedData += huffmanCodes.at(c);
    }

    return compressedData;
}

// Função para descomprimir dados usando a árvore de Huffman
std::string decompressDataHuffman(const std::string& compressedData, HuffmanNode* root)
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

std::string buildCompressHuffman(std::string& data){


    std::unordered_map<char, int> frequencies;
    for (char c : data) {
        frequencies[c]++;
    }
    HuffmanNode* root = buildHuffmanTree(frequencies);
    std::unordered_map<char, std::string> huffmanCodes;
    buildHuffmanCodes(root, "", huffmanCodes);
    std::string compressedData = compressDataHuffman(data, huffmanCodes);
    return compressedData;
     
}

