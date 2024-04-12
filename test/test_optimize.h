
#include <fstream>
#include "../include/html_parser.h"
#include "../include/element_query.h"
#include "../include/util.h"
#include "benchmark.h"
#include <string>
#include <filesystem>
#include <chrono>


//std::string BATTLE_TEST_HTML = std::filesystem::current_path().string() +"\\test\\html\\battle_test.html";
std::string BATTLE_TEST_HTML = "C:/VisualStudioCodeProjects/cpp-simple-html-parser/test/html/battle_test.html";

void testBattleParse(){

    std::cout << BATTLE_TEST_HTML << std::endl;

    std::ifstream inputFile(BATTLE_TEST_HTML);

    std::string html;
    if (inputFile.is_open()) {
        html.assign(std::istreambuf_iterator<char>(inputFile), std::istreambuf_iterator<char>());
        inputFile.close(); // Close the file
        std::cout << "Content of the file:\n" << html << std::endl;
    } else {
        std::cerr << "Unable to open file for reading." << std::endl;
        return;
    }

    benchmark([html]{

        HTMLNode root = parseHTML(html);    
        
        deallocateDoc(&root);

    }, 1);


} 