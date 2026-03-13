#include <chrono>
#include <random>
#include "Benchmark.h"
#include "FileHandler.h"
#include "KnowledgeTree.hpp"
#include "EntitySearcher.h"
#include "tree_serializer.h"
#include <memory>



// измеряет время загрузки файлов (подается файл, переписывается в строку, преобразуется в дерево)
 double Benchmark::measureLoadTime(const std::string& filename){
    auto start = std::chrono::steady_clock::now();
    FileHandler file(".");
    auto content = file.readFile(filename);
    if (!content) {
        return -1;
    }
    TreeSerializer serializer;
    std::unique_ptr<KnowledgeTree> tree = serializer.deserialize(*content);
    auto end = std::chrono::steady_clock::now();
    double ms = std::chrono::duration <double, std::milli>(end-start).count();
    return ms;
}


// измеряет время метода objectExists
double Benchmark::measureSearchTime(const Node* root, const std::string& objectName){
     if (!root){
         return -1;
     }
     auto start = std::chrono::steady_clock::now();
     EntitySearcher::objectExists(root, objectName);
     auto end = std::chrono::steady_clock::now();
     double ms = std::chrono::duration<double, std::milli>(end-start).count();
     return ms;
 }





double Benchmark::measureGameSpeed(const Node* root, int iterations){
     if (!root || iterations <= 0){
         return -1;
     }

     // генерация случайных чисел
     std::mt19937  rng(std::random_device{}());
     std::uniform_int_distribution<int> dist (0,1);

     int totalQuestions = 0;
    auto start = std::chrono::steady_clock::now();
     for (int i = 0; i<iterations; ++i){
         const Node* current = root;
         while (current!= nullptr && !current->isAnswer()){
             totalQuestions++;
             bool answer = (dist(rng)==1);
             if (answer) {
                 if (current->hasYesBranch()) {
                     current = current->getYesBranch();
                 } else {
                     break;
                 }
             } else {
                 if (current->hasNoBranch()) {
                     current = current->getNoBranch();
                 } else {
                     break;
                 }
             }
         }
     }

    auto end = std::chrono::steady_clock::now();
    double sec = std::chrono::duration<double>(end - start).count();
    if (sec<=0){
        return -1;
    }
    else{
        return static_cast<double>(totalQuestions) / sec;
    }
 }

