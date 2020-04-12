//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_COMMENT_PARSER_NODE_PRE_HPP
#define NEDERROCK_SRC_COMMENT_PARSER_NODE_PRE_HPP

#include <memory>
#include <vector>

class Comment_Parser_Node;

using Comment_Parser_Node_Ptr = std::shared_ptr<Comment_Parser_Node>;
using Comment_Parser_Node_Vector = std::vector<Comment_Parser_Node_Ptr>;

#endif // NEDERROCK_SRC_COMMENT_PARSER_NODE_PRE_HPP
