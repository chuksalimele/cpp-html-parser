#ifndef ELEMENT_QUERY_H
#define ELEMENT_QUERY_H


#ifdef BUILD_EXE
    #define DLL_EXPORT
#else
    #ifdef BUILD_DLL
        #define DLL_EXPORT extern  __declspec(dllexport)
    #else
        #define DLL_EXPORT extern  __declspec(dllimport)
    #endif   
#endif   


#include <iostream>
#include <string>
#include <cctype>
#include "node.h"
#include "html_parser.h"
#include "tag.h"
#include "util.h"


namespace chtml{

    DLL_EXPORT HTMLNode findElementById(HTMLNode parent, std::string element_id);

    DLL_EXPORT HTMLNode findElementByElementId(HTMLNode parent, std::string parent_id, std::string child_id);

    DLL_EXPORT HTMLNode findFirstElementByClassName(HTMLNode parent, std::string class_name);

    DLL_EXPORT std::vector<HTMLNode> findAllElementsByClassName(HTMLNode parent, std::string class_name);

    DLL_EXPORT HTMLNode findFirstElementByTag(HTMLNode parent, std::string tag_name);

    DLL_EXPORT std::vector<HTMLNode> findAllElementsByTag(HTMLNode parent, std::string tag_name);

}
#endif //ELEMENT_QUERY_H