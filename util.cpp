
#include "include/util.h"

namespace chtml{
        
    bool isLetter(char c){
        return (c >= 'a' && c <= 'z') 
                || (c >= 'A' && c <='Z');
    }

    bool isAlphaNumeric(char c){
        return isLetter(c)
                || (c >= '0' && c <= '9');
    }


    bool isWhiteSpace(char c){
        return c == ' ' 
                || c == '\n'  
                || c == '\r'   
                || c == '\t'  
                || c == '\f' 
                || c == '\v'; 
    }

    bool isPrintable(char c) {
        return !isWhiteSpace(c) && (c >= 32 && c <= 126);
    }

    /* 
    Keep in mind there are more.
    */
    bool isValidAttrChar(char c){
        return isAlphaNumeric(c)  
                || c == ':'   
                || c == '-' 
                || c == '_'   
                || c == '@'   
                || c == '.'; 
    }


    std::string toLowerCaseStr(std::string str) {
        std::string result;
        result.reserve(str.size()); // reserve space for efficiency
        
        for (char c : str) {
            result += std::tolower(c);
        }
        
        return result;
    }


    std::vector<std::string_view> splitStr(const std:: string_view sv, char sep){
                
        std::vector<std::string_view> classes;
        std::string str(sv);
        std::istringstream stream(str);
        std::string cls;
        
        // Splitting the string by space charater and storing tokens in a vector
        while (std::getline(stream, cls, sep)) {
            classes.push_back(cls);
        }

        return classes;
    }

    bool find (const std::vector<std::string_view> vec, const std::string_view item){
        for(std::string_view sv : vec){
            if(sv == item){
                return true;
            }
        }

        return false;
    }

    bool isOutsideQuote(const LANG_CONTEXT lang, const std::string_view str, int index,  int& single_quote_count, int& double_quote_count, int& backtick_count){
            
            if(str[index] == '"'){
                double_quote_count ++;
                if(lang == JAVASCRIPT && index > 0 && str[index-1] == '\\'){ //checking for escaped double quote character
                    double_quote_count --; //cancel the count increment
                }
            }

            if(str[index] == '\''){
                single_quote_count ++;
                if(lang == JAVASCRIPT && index > 0 && str[index-1] == '\\'){ //checking for escaped single quote character
                    single_quote_count --; //cancel the count increment
                }
            }
            

            if(str[index] == '`'){
                backtick_count ++;
                if(lang == JAVASCRIPT && index > 0 && str[index-1] == '\\'){ //checkinf for escaped backtick character
                    backtick_count --; //cancel the count increment
                }
            }
            
            return single_quote_count % 2 == 0  &&  double_quote_count % 2 == 0  &&  backtick_count % 2 == 0;
    }
    bool isScript(const std::string_view name){
        if(name.size() != 6){
            return false;
        }
        return   (name[0] == 's' || name[0] == 'S')
               &&(name[1] == 'c' || name[1] == 'C')
               &&(name[2] == 'r' || name[2] == 'R')
               &&(name[3] == 'i' || name[3] == 'I')
               &&(name[4] == 'p' || name[4] == 'P')
               &&(name[5] == 't' || name[5] == 'T');               
    }
    
    bool isSameCase(char a, char b) {
        return (a ^ b) & 32;
    }

    bool isBothSameCase(const char a, const char b){
        return (a >= 'A' && a <= 'Z' && b >= 'A' && b <= 'Z')
                ||(a >= 'a' && a <= 'z' && b >= 'a' && b <= 'z');
    }

    bool isLower(const char c){
        return c == 'a' && c <= 'z';
    }
    bool isUpper(const char c){
        return c == 'A' && c <= 'Z';
    }

    bool sameTagType(const std::string_view tag1, const std::string_view tag2){

        if(tag1.size() != tag2.size()){
            return false;
        }
        
        int len = tag1.size();

        for(int i=0; i<len; i++){            
            if(isBothSameCase(tag1[i] , tag2[i])
                 && tag1[i] != tag2[i]){
                return false;
            }else if (isUpper(tag1[i]) 
                        && isLower(tag2[i]) 
                        && tag1[i] != tag2[i] + ('A' -'a')){ 
                return false;
            }else if (isLower(tag1[i]) 
                        && isUpper(tag2[i]) 
                        && tag1[i] != tag2[i] + ('a' - 'A')){
                return false;
            }
        }

        return true;
    }

}