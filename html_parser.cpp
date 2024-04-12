
#include "include/html_parser.h"
#include "include/node.h"
#include "include/parser_executor.h"

namespace chtml{    

    void parseHTML(HTMLNode* node) {
            
            int len = node->innerHTML.size();
            int estimated_children_count = 0;

            for(int index = 0; index < len; index++){
                
                if(isScript(node->tag.name)){
                    break; //script does not have a child node
                }

                HTMLNode* child_node = new HTMLNode;

                nextChildNode(child_node, index, node->innerHTML, node->tag.name, estimated_children_count);      
                                
                //std::cout << "estimated_children_count = " << estimated_children_count <<std::endl;
                
                child_node->children.reserve(estimated_children_count);

                index = child_node->parentEndIndex;                

                if(index == -1){
                    break;
                }
                
                if(child_node->tag.name.size() != 0 && child_node->innerHTML.size() != 0){                        
                    
                    //parserExecutor.enqueue([child_node]{
                        parseHTML(child_node); 
                    //});                     

                    node->children.emplace_back(child_node);                                                
                }else if(child_node->tag.name.size() != 0 || child_node->innerHTML.size() != 0){ 
                    // For Self Closing element the tag name is not empty but
                    // the innerHTML is empty. 
                    // For Text node the tag name is empty but the content 
                    // my not neccessay be empty. So here we made sure to only 
                    // add the text node if only the content is not empty hence 
                    // the condition for this IF ELSE block

                    node->children.emplace_back(child_node);     

                }
            }    
        
    }


    DLL_EXPORT HTMLNode parseHTML(const std::string& html){
        HTMLNode* node = new HTMLNode;
        node->innerHTML = html;
        
        //parserExecutor.enqueue([node] {
             parseHTML(node); 
        //});
        
        //parserExecutor.start();


        return *node;
    }
    
    // Recursively deallocate memory for child nodes and their children
    void recursivelyDeallocateChildren(HTMLNode* parent) {
        for (HTMLNode* child : parent->children) {
            recursivelyDeallocateChildren(child);
            delete child;
            child = nullptr;  // Set to nullptr after deletion
        }
        parent->children.clear(); // Deallocate the memory occupied by the children vector
    }

    // Deallocate memory for the entire document rooted at 'root'
    void deallocateDoc(HTMLNode* root){

        /* TODO: Add error handling if root is not a valid root node                
        
        if(!root.isRoot){
            std::runtime_error("Invalid node - expected root node");
        }*/

        try{    
            
            recursivelyDeallocateChildren(root);

            //delete root; // Finally delete the root node
            //root = nullptr;  // Set to nullptr after deletion

        }catch(std::exception& e){
            std::cerr << "Exception : " << e.what() << std::endl;
        } 
    }

}
