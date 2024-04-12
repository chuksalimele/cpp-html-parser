#ifndef COMMENT_H
#define COMMENT_H

#include "util.h"
#include "basic.h"


namespace chtml{
    
    int getCommentEndIndex(const std::string_view html , int i);

    std::string removeTextNodeComments(const std::string_view html);

}

#endif // COMMENT_H
