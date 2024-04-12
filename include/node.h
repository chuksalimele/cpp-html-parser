#ifndef NODE_H
#define NODE_H


#include <string>
#include <vector>
#include "basic.h"
#include "tag.h"
#include "comment.h"

namespace chtml{

    // Define a simple HTML node structure
    struct HTMLNode {
        chtml::Tag tag;
        bool leafNode = false; // e.g text node and self closing element (also called void elements)
        std::string leafTextNoComments = ""; //ALL COMMENTS ARE STRIPPED OF
        std::vector<HTMLNode*> children;
        std::vector<chtml::Attribute> attributes;
        std::string_view innerHTML;
        int parentStartIndex = -1; // its start index in the parent  innerHTML
        int parentEndIndex = -1; // its end index in the parent  innerHTML
    };
    

    void nextChildNode (HTMLNode* node, int offset, const std::string_view html, const std::string_view parent_tag_name, int& estimated_children_count);

}

#endif //NODE_H