
#include "include/node.h"
#include "include/parser_executor.h"

namespace chtml{
                        
    void nextChildNode (HTMLNode* node, int offset, const std::string_view html, const std::string_view parent_tag_name, int& estimated_children_count){
        int len = html.size();
        
        int tag_name_index = -1; 
        int tag_name_len = 0; 
        int text_begin_index = -1; // to detect if the next child node is text node
        int text_end_index = -1;
        int tag_begin_index = -1; // index of '<' . We are interested in the very first of it
        int tag_end_index = -1; // index of '>'. We are interested in the very last of it
        int tag_lower_span_len = 0;// yes 0 since we are talking about length. there is no negative length
        int tag_upper_span_len = 0; // yes 0 since we are talking about length. there is no negative length
        int open_angle_index = -1;
        int close_angle_index = -1;
        int single_quote_count = 0;
        int double_quote_count = 0;
        int container_element_head_count = 0;
        int container_element_tail_count = 0;

        bool expect_open = true;    
        bool expect_close = false;      
        bool expect_double_quote = false; // the closing end of the double quote character          
        bool expect_single_quote = false; // the closing end of the single quote character
        bool expect_backtick = false; // the closing end of the backtick character
        
        bool forgiving_error_no_end_of_quote = false;
        bool is_opening_tag = false;
        bool found_script_closing_tag = false;

        char char1st = 0;
        char char2nd = 0;
        char char3rd = 0;

        int char_sep_count = 0;
        bool flag_sep_count = true;

        Tag first_tag;
        Tag current_tag; // needed only when during analysis we need to know the focus tag down the line
        bool is_comment = false;
        int first_none_whitespace_index = -1;

        int estimated_direct_child_tag_count = 0;
        int container_open = 0;
        int container_close = 0;
        int depth = 0;
        int direct_child_count = 0; 

        //trying to get first locations of < and > outside quoutes
        for(int i = offset; i < len; i++){

            //check if we first saw <script>
            bool is_script_encountered = isScript(first_tag.name); 

            if(!is_script_encountered 
                && !expect_single_quote 
                && !expect_double_quote 
                && !expect_backtick){

                int commend_end = getCommentEndIndex(html, i);
                if(commend_end != -1){ //found end of comment
                    i = commend_end; //skip to this point
                    continue;
                }
            }        
            

            if(is_script_encountered){
                scriptClosingTagEnd(html, i, open_angle_index, close_angle_index, found_script_closing_tag);
            }    

            bool is_white_space = isWhiteSpace(html[i]);

            if(!is_white_space){
                if(first_none_whitespace_index == -1){
                    // we need this to filter text nodes with only whitespaces
                    first_none_whitespace_index = i; 
                }            
            }

            if(is_white_space && flag_sep_count){ 
                char_sep_count++; // count space b/w strings e.g <aaa   bbb   cc  dd> has 3 spaces b/w chars
                flag_sep_count = false;

            }else if(!is_white_space){
                flag_sep_count = true;
            }

            if(char_sep_count == 0){ // get the length of the tag name
                if(isLetter(html[i])){
                    tag_name_len++;
                }else if(tag_name_len >= 1 && isAlphaNumeric(html[i])){
                    tag_name_len++;
                }
            }

            if(!is_script_encountered && expect_open && i < len - 2  && html[i] == '<'){
                if(isLetter(html[i + 1])){  //expecting e.g <a
                    tag_name_index = i + 1;
                    is_opening_tag = true;
                }else if(html[i + 1] =='/'  // or expecting e.g </a 
                        &&  isLetter(html[i + 2])
                        && first_tag.name != ""){ //we must make sure the closing tage does not come before opening tag otherwise disregard. e.g </element> can not come before <element>
                    tag_name_index = i + 2;
                    is_opening_tag = false; // e.g </div>
                }else{
                    continue;
                }

                expect_open = false;
                expect_close = true;
                char_sep_count = 0;
                tag_name_len = 0;
                char3rd = 0;
                char2nd = 0;
                char1st = 0;

                open_angle_index  = i;
                
                if(tag_begin_index == -1){
                    tag_begin_index = open_angle_index; // set it only once because we want the very begining
                }   
                
                //i = tag_name_index; @Deprecated - removed so we can know the tag name length

                continue;
            }

            if((expect_single_quote && html[i] == '\'') 
                || (expect_double_quote && html[i] == '"')
                || (expect_backtick && html[i] == '`')){
                char3rd = 0;
                char2nd = 0;
                char1st = 0;    
                expect_close = true;
                expect_single_quote = false;
                expect_double_quote = false;
                expect_backtick = false;
                continue;
            }
            
            if(expect_close && char_sep_count > 0 && isPrintable(html[i])){
                char3rd = char2nd;
                char2nd = char1st;
                char1st = html[i];               
            }

            
            if(expect_close 
                && !is_script_encountered 
                && is_opening_tag
                && char1st == '\'' 
                && char2nd  == '='
                && isPrintable(char3rd) )
            {
                expect_single_quote = true;                
                expect_double_quote = false;
                expect_backtick = false;
                expect_close = false;

            } 

            if(expect_close 
                && !is_script_encountered 
                && is_opening_tag
                && char1st == '"' 
                && char2nd  == '='
                && isPrintable(char3rd) )
            {
                expect_single_quote = false;                
                expect_double_quote = true;
                expect_backtick = false;
                expect_close = false;

            }

            if(expect_close 
                && !is_script_encountered 
                && is_opening_tag
                && char1st == '`' 
                && char2nd  == '='
                && isPrintable(char3rd) )
            {
                expect_single_quote = false;                
                expect_double_quote = false;
                expect_backtick = true;
                expect_close = false;
            }


            if((expect_close && html[i] == '>')
                 || found_script_closing_tag){ 

                expect_open = true;
                expect_close = false;
                                
                if(found_script_closing_tag){
                    tag_name_index = open_angle_index + 2;
                    is_opening_tag = false; // e.g </script>
                    tag_name_len = 6; //we know it is 6 - length of </script>
                }   

                close_angle_index  = i;
                int tag_inner_raw_len = close_angle_index - tag_name_index;
                
                extractTag(current_tag, html, parent_tag_name, tag_name_index , tag_name_len, tag_inner_raw_len);                


                if(first_tag.name.size() != 0){
                    if(current_tag.isContainerElement){                        
                        if(html[open_angle_index + 1] != '/'){ 
                            ++depth;  
                        }else{
                            --depth;
                        }
                        if(depth == 0){
                            ++direct_child_count;
                        }                        
                    }else{
                        if(depth == 0){
                            ++direct_child_count;
                        }
                    }
                    
                    estimated_children_count = direct_child_count;                                        
                }
                
                    

                if(first_tag.name.size() == 0){ // this block is only visited once. only script tag does not come here at all
                    first_tag = current_tag; //here top level tag is any other element apart from script since we are taking care of script in the preceding ELSE IF block
                    container_element_head_count = 1; // first head

                    tag_upper_span_len = close_angle_index - open_angle_index + 1;

                }else if(sameTagType(first_tag.name , current_tag.name)){
                        
                    //counting the head and tail of the container element
                    //e.g <div> is head and </div> is tail

                    if(html[open_angle_index + 1] == '/'){ 
                        container_element_tail_count ++; //increment tail count
                    }else{
                        container_element_head_count ++; //increment head count
                    }
                        
                    tag_lower_span_len = close_angle_index - open_angle_index + 1;
                        
                }

            }            

                        
            if(first_tag.name.size() != 0 && !first_tag.isContainerElement){ // e.g <input /> and <br>
                tag_end_index = close_angle_index; // lets mark it
                break;
            }

            if(first_tag.name.size() != 0  && first_tag.isContainerElement){ //e.g <div></div>
                //let's find the corresponding end of the tag
                int diff = container_element_head_count - container_element_tail_count;    
                bool no_direct = first_tag.isContanerElementNoDirectChildOfItsType;
                if((!no_direct && diff == 0)
                    || (no_direct &&  container_element_tail_count == 1)){
                    //under normal circumstance head count of like-enclosing-tags
                    //is equal to the tail count to arrive at the corresponding
                    //end of a tag BUT of element that does not allow direct child
                    //of its type the like tags must not be equal, once we find
                    //a closing tag we are done.
                    tag_end_index = close_angle_index; // lets mark it
                    break;
                }

            }  

        }

        //checking for preceding text node
        if(first_tag.name.size() != 0  && tag_begin_index >= offset + 1){
            text_begin_index = offset;
            text_end_index = tag_begin_index -1; // before <
        }else if(first_tag.name.size() != 0  && tag_end_index == -1 && first_tag.isContainerElement){ 
            //there was no closing tag for container element
            //forgive the syntax error by adding the closing tag e.g </element>

            tag_end_index = len -1;
            tag_lower_span_len = 0; //since there is a missing closing tag
        }
        else if(first_tag.name.size() == 0 ){
            text_begin_index = offset;
            text_end_index = len -1; // the whole
        }


        if(first_none_whitespace_index <= text_end_index && text_end_index != -1 ){//text node
            node->leafNode = true;
            node->parentStartIndex = text_begin_index;               
            node->parentEndIndex = text_end_index;
            node->innerHTML = html.substr(text_begin_index, text_end_index - text_begin_index + 1);
            //we know most comments will only eventually end up in two type of place
            //either at the text node or at the tag rawInnner g.g <inpt <!-- comment --> id ="myid">

            //parserExecutor.enqueue([node] {
                node->leafTextNoComments = removeTextNodeComments(node->innerHTML); 
            //});
            
            
        }else if (first_tag.name.size() != 0 ){//tag node        
            node->tag = first_tag;
            
           // parserExecutor.enqueue([node] {
                node->attributes = createAttributes(node->tag.rawInner);
            //});

            node->parentStartIndex = tag_begin_index;               
            node->parentEndIndex = tag_end_index;
            //calculate the inner html length
            int inner_html_start = tag_begin_index + tag_upper_span_len;
            int inner_html_len = tag_end_index 
                                    - tag_begin_index 
                                    - tag_upper_span_len 
                                    - tag_lower_span_len +1;

            node->innerHTML = html.substr(inner_html_start, inner_html_len);

            node->leafNode = !node->tag.isContainerElement;

        }


    }

}