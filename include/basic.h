#ifndef BASIC_H
#define BASIC_H

#include <vector>
#include <string>

namespace chtml{


    const char NUL_TERMINATOR = 0;

    const std::vector <std::string_view>  SELF_CLOSING_ELEMENTS   =
                                            {"area","base","br","col","embed",
                                            "hr","img","input","link","meta",
                                            "param","source","track","wbr"};


    //SOME ERRORS IN HERE - PLEASE RESEARCH LATER TO GET THE CORRECT LIST
    const std::vector <std::string_view>  ELEMNETS_WITH_NO_IMMEDIATE_CHILD_OF_ITS_TYPE = 
                                            {                                                
        "a", "abbr", "acronym", "address", "b", "bdi", "bdo", "big", "blockquote", "button", 
        "caption", "cite", "code", "del", "dfn", "dialog", "em", "figcaption", "h1", "h2", 
        "h3", "h4", "h5", "h6", "i", "iframe", "ins", "kbd", "label", "legend", "mark", 
        "option", "output", "p", "pre", "q", "ruby", "s", "samp", "select", "small", 
        "strong", "sub", "sup", "textarea", "time", "tt", "u", "var"};  


    enum LANG_CONTEXT{
        HTML,
        JAVASCRIPT
    };

}

#endif // BASIC_H