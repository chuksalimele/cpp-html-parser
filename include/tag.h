#ifndef TAG_H
#define TAG_H

#include <string>
#include <vector>
#include "util.h"
#include "comment.h"

namespace chtml{

    // Define a Atrribute structure
    struct Attribute {
        std::string name;
        std::string value;
    };

    struct Tag{
        std::string_view name;
        std::string_view rawInner; //e.g in <span id="a" class="b"> [span id="a" class="b"] is rawInner
        bool isContainerElement = false; //e.g <input /> is false and <div></div> is true
        bool isContanerElementNoDirectChildOfItsType = false;
    };

    void scriptClosingTagEnd(const std::string_view html, int& cursor, int& opening_angle_index, int& closing_angle_index, bool& found_script_closing_tag);

    void extractTag(Tag& tag, const std::string_view html, const std::string_view parent_tag_name, const int tag_name_index, const int tag_name_len, const int tag_inner_raw_len);

    std::vector<Attribute> createAttributes(const std::string_view inner_raw);

}

#endif // TAG_H
