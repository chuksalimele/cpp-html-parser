
#include "include/element_query.h"


namespace chtml{

    DLL_EXPORT HTMLNode findElementById(HTMLNode parent, std::string element_id){
        HTMLNode found_node;

        for(HTMLNode* node :  parent.children){
            
            //std::cout << node->tag.name << std::endl;

            for(Attribute attr : node->attributes){
                if(attr.name == "id" && attr.value == element_id){
                    //std::cout << node->tag.name  << " found id : " << attr.name << " value = " << attr.value << std::endl;
                    return *node;
                }
            }
            if(node->children.size() > 0){
                found_node = findElementById(*node, element_id);
                if(found_node.parentStartIndex != -1){
                    return found_node;
                }
            }
        }

        return found_node;
    }


    DLL_EXPORT HTMLNode findElementByElementId(HTMLNode node, std::string parent_id, std::string child_id){
        HTMLNode found_node;

        HTMLNode parent_node = findElementById(node, parent_id);
        found_node = findElementById( parent_node, child_id);

        return found_node;
    }

    HTMLNode findFirstElementByClassName0(HTMLNode parent, std::vector<std::string_view> classes){
        HTMLNode found_node;

        for(HTMLNode* node :  parent.children){
            for(Attribute attr : node->attributes){
                if(attr.name == "class" && find(classes, attr.value) != -1){
                    return *node;
                }
            }
            if(node->children.size() > 0){
                found_node = findFirstElementByClassName0(*node, classes);
                if(found_node.parentStartIndex != -1){
                    return found_node;
                }
            }
        }

        return found_node;
    }
    
    DLL_EXPORT HTMLNode findFirstElementByClassName(HTMLNode parent, std::string class_name){
        return findFirstElementByClassName0(parent, splitStr(class_name, ' '));
    }

    std::vector<HTMLNode> findAllElementsByClassName0(HTMLNode parent, std::vector<std::string_view> classes){
        std::vector<HTMLNode> found_nodes;

        for(HTMLNode* node :  parent.children){
            for(Attribute attr : node->attributes){
                if(attr.name == "class" &&  find(classes, attr.value) != -1){
                    found_nodes.push_back(*node);
                    break;
                }
            }
            if(node->children.size() > 0){
                std::vector<HTMLNode> nodes = findAllElementsByClassName0(*node, classes);
                for(HTMLNode n :  nodes){
                    found_nodes.push_back(n);
                }
            }
        }

        return found_nodes;
    }

    DLL_EXPORT std::vector<HTMLNode> findAllElementsByClassName(HTMLNode parent, std::string class_name){        
        return findAllElementsByClassName0(parent, splitStr(std::string_view(class_name), ' '));
    }

    HTMLNode findFirstElementByTag0(HTMLNode parent, std::string lwc_tag_name){
        HTMLNode found_node;        
        for(HTMLNode* node :  parent.children){
            if(node->tag.name == lwc_tag_name){
                return *node;
            }
            if(node->children.size() > 0){
                found_node = findFirstElementByTag0(*node, lwc_tag_name);
                if(found_node.parentStartIndex != -1){
                    return found_node;
                }
            }
        }

        return found_node;
    }

    DLL_EXPORT HTMLNode findFirstElementByTag(HTMLNode parent, std::string tag_name){
        std::string lwc_tag_name = toLowerCaseStr(tag_name);
        return findFirstElementByTag0(parent, lwc_tag_name);
    }

    std::vector<HTMLNode> findAllElementsByTag0(HTMLNode parent, std::string lwc_tag_name){
        std::vector<HTMLNode> found_nodes;

        for(HTMLNode* node :  parent.children){
            if(node->tag.name == lwc_tag_name){
                found_nodes.push_back(*node);
            }
            if(node->children.size() > 0){
                std::vector<HTMLNode> nodes = findAllElementsByTag0(*node, lwc_tag_name);
                for(HTMLNode n :  nodes){
                    found_nodes.push_back(n);
                }
            }
        }

        return found_nodes;
    }

    DLL_EXPORT std::vector<HTMLNode> findAllElementsByTag(HTMLNode parent, std::string tag_name){
            std::string lwc_tag_name = toLowerCaseStr(tag_name);
            return findAllElementsByTag0(parent, lwc_tag_name);
    }
}