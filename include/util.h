#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include "basic.h"

namespace chtml{

    bool isLetter(char c);

    bool isAlphaNumeric(char c);

    bool isWhiteSpace(char c);

    bool isPrintable(char c);

    /* 
    Keep in mind there are more.
    */
    bool isValidAttrChar(char c);

    std::string toLowerCaseStr(std::string str);

    std::vector<std::string_view> splitStr(const std::string_view str, char sep);

    bool find (const std::vector<std::string_view> vec, const std::string_view item);

    bool isOutsideQuote(const LANG_CONTEXT lang, const std::string_view str, int index,  int& single_quote_count, int& double_quote_count, int& backtick_count);

    bool sameTagType(const std::string_view tag1, const std::string_view tag2);

    bool isScript(const std::string_view name);

}

#endif //UTIL_H