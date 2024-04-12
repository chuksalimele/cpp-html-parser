#ifndef HTML_PARSER_H
#define HTML_PARSER_H

#ifdef BUILD_EXE
    #define DLL_EXPORT
#else
    #ifdef BUILD_DLL
        #define DLL_EXPORT extern  __declspec(dllexport)
    #else
        #define DLL_EXPORT extern  __declspec(dllimport)
    #endif   
#endif   



#include "tag.h"
#include "node.h"


namespace chtml{

    DLL_EXPORT HTMLNode parseHTML(const std::string& html);

     // Deallocate memory for the entire document rooted at 'root'
    DLL_EXPORT void deallocateDoc(HTMLNode* root);
    
}

#endif //HTML_PARSER_H