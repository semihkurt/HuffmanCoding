#include <iostream>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <type_traits>

// @author Semih Kurt

using namespace std;

const std::string FILENAME = "/Users/semih/Development/src/HuffmanCoding/example1.txt";
const int BIT_SIZE = 8;

std::unordered_map<std::string, int> frequenciesMap;
std::unordered_map<std::string, std::string> huffmanValuesMap;

enum class InputEnum{
    ///Input sequence -> One by one example: A, B , C ,F ,H, Q ..
    EachLetter,
    ///Input sequence -> Bi-grams letter pairs example: AA, BA , CA, DD
    Bigram
};

struct Obj{
    Obj() = default;
    Obj(std::string ch,int freq) :
        frequency(freq),
        symbol(ch)
    {};

    int frequency;
    std::string symbol;
};

class Node{
public:
    Node() = default;

    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;

    Obj itself;
};

class ComparisonClass{
public:
    int operator()(std::shared_ptr<Node> a,std::shared_ptr<Node> b){
        //If this acts as greater we create min-heap
        //Means if a is greater than b or not
        return a->itself.frequency > b->itself.frequency;
    }
};

template<typename T>
int calculateTotalSizeBeforeCompression(const T& map){
    int totalSize = 0;
    for(const auto& kv : map){
           totalSize += kv.second * BIT_SIZE;
    }
    return totalSize;
}

int calculateTotalSizeAfterCompression(std::unordered_map<std::string, std::string>& huffmanValueMap, std::unordered_map<std::string,int>& frequencyMap){
    int totalSize = 0;
    for (auto v=huffmanValueMap.begin(); v!=huffmanValueMap.end(); v++){
           totalSize += v->second.size() * frequencyMap.at(v->first);
    }
    return totalSize;
}
/**
 * @brief Process Step 3: Reverse search Huffman Tree and determines huffman values inside and stores into \ref huffmanValuesMap
 * @param root Root Node of Huffman Tree
 * @param str It is necessary for incrementing huffman values by doing Recursion
 */
void storeCodes(std::shared_ptr<Node> root, std::string str)
{
    if ( root == nullptr)
        return;
    if(root->itself.symbol.compare("") != 0)
        huffmanValuesMap[root->itself.symbol]=str;
    storeCodes(root->left, str + "0");
    storeCodes(root->right, str + "1");
}

std::shared_ptr<Node> extractMin(std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, ComparisonClass>& queue)
{
    auto t = queue.top();
    queue.pop();
    return t;
}

template<typename T>
void printPriorityQueue(T& pQueue){
    T t = pQueue;
    cout << "Priority Queue: " << endl;
    while (!t.empty()) {
        auto top = t.top();
        cout << '\t' << t.top()->itself.symbol << " " << std::to_string(t.top()->itself.frequency) << endl;
        t.pop();
    }
    cout << '\n';
}

template< typename T>
void printMap(const T& map){
    for(const auto& kv : map){
        std::ostringstream oss;
        oss << kv.first;
        oss << " ";
        oss << kv.second;
        cout << oss.str() << endl;
    }
}

void processKeyForFrequency(std::unordered_map<std::string,int>& pFrequencyMap,const std::string& pStr){
    if(pFrequencyMap.find(pStr) != pFrequencyMap.end()){
        pFrequencyMap.at(pStr) += 1;
    }else{
        pFrequencyMap.insert({pStr,1});
    }
}

void processKeyForFrequency(const std::string& pStr){
    if(frequenciesMap.find(pStr) != frequenciesMap.end()){
        frequenciesMap.at(pStr) += 1;
    }else{
        frequenciesMap.insert({pStr,1});
    }
}

/**
 * @brief Process Step 2: Construct Huffman Tree and determine the codes for each letter
 * @param pFrequencyMap
 * @return
 */
std::shared_ptr<Node> BuildHuffmanTree(const std::unordered_map<std::string,int>& pFrequencyMap){

    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, ComparisonClass> queue;
    for(auto& t: pFrequencyMap)
    {
        std::shared_ptr<Node> node = std::make_shared<Node>();

        Obj tObj(t.first,t.second);
        node->itself = tObj;
        queue.push(node);
    }
    printPriorityQueue(queue);

    int size = pFrequencyMap.size();
    for(int i = 1 ; i < size ; i++)
    {
        std::shared_ptr<Node> node = std::make_shared<Node>();
        node->left = extractMin(queue);
        node->right = extractMin(queue);
        node->itself.frequency = node->left->itself.frequency + node->right->itself.frequency;
        queue.push(node);
    }
    std::shared_ptr<Node> ROOT_NODE = extractMin(queue);
    return ROOT_NODE;
}

/**
 * @brief Process Step 1 : Determine the counts of each letter
 * @param pFile
 * @param hw:
 *          Part 1 -> Read a character one by one from File.
 *          Part 2 -> Read characters by bi-gram letter pairs from File like AA, AB, AC, CA.
 */
void fillFrequency(ifstream& pFile, const InputEnum& inputEnum){
    /* Assumed that File will only contain Upper case English alphabet letters, it won't contain any lower case or any other character like: _ - * # \n and etc.*/
    /* Process Step 1: Determine the counts of each letter */

    auto fn = [](std::stringstream& ss) {
        std::string tStr;
        ss >> tStr;
        processKeyForFrequency(tStr);
        ss.str("");
        ss.clear();
    };
    char tTemp;
    std::stringstream ss;
    while(pFile.get(tTemp))
    {
        std::string tStr;

        ss << tTemp;

        switch (inputEnum) {
        case InputEnum::EachLetter:
            fn(ss);
            break;
        case InputEnum::Bigram:
            if(ss.str().length() == 2){
                fn(ss);
            }
            break;
        }
    }
}

int main()
{
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    /* Process Step 1 : Determine the counts of each letter */
    bool you_wanna_read_from_file = true;
    if(you_wanna_read_from_file){
        std::string tText;
        ifstream tFileRead(FILENAME);
        if (!tFileRead.is_open()) {
                cerr << "Could not open the file - '" << FILENAME << "'" << endl;
                return 0;
        }
        fillFrequency(tFileRead,InputEnum::Bigram); //* Also, you can check each letter by changing the InputEnum here.  ----------------- */
    }else{
        //Reference: Introduction to Algorithms by Thomas H.Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein
        //Uncompressed output: 800 bits
        //Compressed output: 224 bits
        frequenciesMap = {
            {"A",45},
            {"B",13},
            {"C",12},
            {"D",16},
            {"E",9},
            {"F",5}
        };
    }

    cout << "Total size before compression by Huffman Coding:" << endl;
    cout << std::to_string(calculateTotalSizeBeforeCompression(frequenciesMap)) << " bits" << endl;

    cout << "Frequencies: " << endl;
    printMap(frequenciesMap);

    /* Process Step 2: Construct Huffman Tree and determine the codes for each letter */
    std::shared_ptr<Node> ROOT_NODE = BuildHuffmanTree(frequenciesMap);

    /* Process Step 3: Determine the total size after compressing */
    storeCodes(ROOT_NODE,"");

    cout << "Codes: " << endl;
    printMap(huffmanValuesMap);

    cout << "Total size after compression by Huffman Coding:" << endl;
    cout << std::to_string(calculateTotalSizeAfterCompression(huffmanValuesMap,frequenciesMap)) << " bits" << endl;

    return 0;
}
