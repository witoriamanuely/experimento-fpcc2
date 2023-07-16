#include <fstream>
#include <iostream>
#include <vector>
#include <dirent.h>



void resizeFile(const std::string& inputFilePath, const std::string& outputFilePath, std::size_t newSizeBytes) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Failed to open input file." << std::endl;
        return;
    }
    
    // Obter o tamanho atual do arquivo
    inputFile.seekg(0, std::ios::end);
    std::size_t currentSizeBytes = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);
    
    // Ler o conteúdo do arquivo para um vetor de bytes
    std::vector<char> fileContent(currentSizeBytes);
    inputFile.read(fileContent.data(), currentSizeBytes);
    
    // Redimensionar o vetor para o novo tamanho desejado
    fileContent.resize(newSizeBytes);
    
    // Gravar o conteúdo do vetor no novo arquivo
    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Failed to create output file." << std::endl;
        return;
    }
    outputFile.write(fileContent.data(), newSizeBytes);
    
    std::cout << "File resized successfully." << std::endl;
}

int main(int argc, char* argv[]) {


    std::string inputFolderPath = "largecalgarycorpus/";
    std::string outputFolderPath = "resize/";
    std::string fileName = "";

    if (argc > 1){
        fileName = argv[1];
        inputFolderPath = inputFolderPath + fileName;
    }

    std::size_t newSizeBytes = 65536; 

    std::string inputFilePath = inputFolderPath; 
    std::string outputFilePath = outputFolderPath + "65536_" + fileName + "_resize";
   
    resizeFile(inputFilePath, outputFilePath, newSizeBytes);

         

    return 0;
}
