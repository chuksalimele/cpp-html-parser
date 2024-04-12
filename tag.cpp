

#include "include/tag.h"

namespace chtml{

    void scriptClosingTagEnd(const std::string_view html, int& cursor, int& opening_angle_index, int& closing_angle_index, bool& found_script_closing_tag){
        
        found_script_closing_tag = false;

        int len = html.size();
        //const char* html_chars = html.c_str();
        int single_quote_count = 0;
        int double_quote_count = 0;
        int backtick_count = 0;
        
        int close_tag_len = 8; //length-1 of </script>

        for(int i= cursor; i < len - close_tag_len; i++){
                    
            bool is_outside_quote = isOutsideQuote(JAVASCRIPT, html,  i,   single_quote_count, double_quote_count, backtick_count);

            if(!is_outside_quote){
                continue;
            }

            if(html[i] == '<'
                && html[i + 1] == '/'
                && html[i + 2] == 's'
                && html[i + 3] == 'c'
                && html[i + 4] == 'r'
                && html[i + 5] == 'i'
                && html[i + 6] == 'p'
                && html[i + 7] == 't'){

                    //lets check if the next character is alphanumeric 
                    //- it shouldn't be otherwise it is no longer script closing tag
                    //e.g <scriptE>, the 'E' charater has simply nullify the script closing tag
                    if(isAlphaNumeric(html[i + 8])){ 
                        continue; //we don't have a valid script closing tag
                    }

                    int start = i;
                    for(i = i + 8; i < len; i++){

                        if(html[i] == '>'){
                            opening_angle_index = start;
                            closing_angle_index = i;
                            cursor = i;
                            found_script_closing_tag = true;
                            return;
                        }
                    }    

                }

        }                    

    }

    void extractTag(Tag& tag, const std::string_view html, const std::string_view parent_tag_name, int tag_name_index, int tag_name_len, int tag_inner_raw_len){
        
        tag.name = html.substr(tag_name_index, tag_name_len);
        tag.rawInner = html.substr(tag_name_index , tag_inner_raw_len);
        
        //convert to lowercase tag name
        //tag.name =  toLowerCaseStr(tag.name); //@Deprecated - forbidden since string_view characters are readonly - doing this will cause corruption

        tag.isContainerElement =  !find(SELF_CLOSING_ELEMENTS, tag.name );
        tag.isContanerElementNoDirectChildOfItsType = find(ELEMNETS_WITH_NO_IMMEDIATE_CHILD_OF_ITS_TYPE, tag.name );
                
    }

    std::vector<Attribute> createAttributes(std::string_view inner_raw){

        int len = inner_raw.size();
        int single_quote_count = 0;
        int double_quote_count = 0;
        int backtick_count = 0;
        Attribute attr;
        std::vector<Attribute> attrVec;

        std::vector<char> clean_raw; //without comments
        //const char* chars = inner_raw.c_str();

        for(int i= 0; i < len; i++){
                    
            bool is_outside_quote = isOutsideQuote(HTML, inner_raw,  i,   single_quote_count, double_quote_count, backtick_count);

            if(is_outside_quote){
                int commend_end = getCommentEndIndex(inner_raw, i);
                if(commend_end != -1){ //found end of comment
                    i = commend_end; //skip to this point
                    continue;
                }
            }

            clean_raw.push_back(inner_raw[i]);//collect only data without comments
        }

        //finally append NUL_TERMINATOR to signified end of a string    
        clean_raw.push_back(NUL_TERMINATOR); 

        const char* clean_str =  clean_raw.data();

        len = clean_raw.size();
        single_quote_count = 0;
        double_quote_count = 0;
        backtick_count = 0;

        std::vector<char> name , value;
        bool flagWhite = false, is_name = false;

        for(int i= 0; i < len; i++){
                    
            if(!flagWhite){
                if(isWhiteSpace(clean_str[i])){
                    flagWhite = true;
                }                
                continue;
            }

            //we have moved pass tag name 

            //get name    
            for(; i<len; i++){
                
                if(isValidAttrChar(clean_str[i])){
                    name.push_back(clean_str[i]);
                }else{
                    break;
                }            
            }

            //get value
            bool is_get_value = false;
            int v_count = 0;        
            if(name.size() > 0) // if there is no name there is no value
                for(; i<len; i++){
                    if(clean_str[i] == '='){
                        is_get_value = true;
                        continue;
                    }
                    
                    if(!is_get_value && isWhiteSpace(clean_str[i])){
                        continue; //hold on until we arrive at '='
                    }

                    if(!is_get_value && !isWhiteSpace(clean_str[i])){
                        if(i < len -1 && isValidAttrChar(clean_str[i])){
                            //move one step back since it is
                            //the start of a new attribute
                            i--;                             
                        }
                        break; 
                    }


                    bool is_outside_quote = isOutsideQuote(HTML, clean_str,  i,   single_quote_count, double_quote_count, backtick_count);            

                    if(!is_outside_quote){
                        v_count++;
                        if(v_count > 1){//skip the first since we know it is quote charater representing a string e.g "some string value"
                            value.push_back(clean_str[i]);
                        }                                
                    }else if(v_count == 0){// we have not started collecting data so keep move
                        //we expect only whitespace in btw
                        if(!isWhiteSpace(clean_str[i])){ 
                            if(i < len -1 && isValidAttrChar(clean_str[i])){
                                //move one step back since it is
                                //the start of a new attribute
                                i--;                             
                            }
                            break;//unexpected character to leave
                        }
                        continue;
                    } else{//end of quote                                 
                        break;
                    }

                }

            
            //finally append NUL_TERMINATOR to signified end of a string
            if(name.size() > 0){// there is at least a name
                name.push_back(NUL_TERMINATOR); 
                value.push_back(NUL_TERMINATOR);    

                attr.name = std::string(name.data());    
                attr.value = std::string(value.data()); 
                attrVec.push_back(attr);

                //clear name and value
                name.clear();
                value.clear();            
            }                 

        }

        return attrVec;
    }

}