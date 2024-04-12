
#include "include/comment.h"

namespace chtml{
    
    int getCommentEndIndex(const std::string_view html , int i){

        int len = html.size();
        if( i + 3 <  len){ // check for sequence starting with <!--
                if(html[i] == '<' &&
                    html[i+1] == '!' &&
                    html[i+2] == '-' &&
                    html[i+3] == '-'){
                        //at this point comments starts. now let get where it ends
                        for(int c = i + 4; c < len; c++){
                            if(c + 2 <  len){
                                if(html[c] == '-' &&
                                    html[c+1] == '-' &&
                                    html[c+2] == '>'){
                                        return c + 2; //YES skip to this point where the comment ends. The next i increment will be outside the comment block                                    
                                }
                            }
                        }

                    }
            }

        return -1;
    }

    std::string removeTextNodeComments(const std::string_view html){

        size_t len = html.size();

        std::vector<char> text; //without comments

        //const char* chars = html.c_str();
        for(int i= 0; i < len; i++){
            size_t commend_end = getCommentEndIndex(html, i);
            if(commend_end != -1){ //found end of comment
                i = commend_end; //skip to this point
                continue;
            }

            text.push_back(html[i]);

        }

        //finally append NUL_TERMINATOR to signified end of a string
        text.push_back(NUL_TERMINATOR);

        return std::string(text.data());
    }

}