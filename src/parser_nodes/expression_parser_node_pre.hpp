//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_EXPRESSION_PARSER_NODE_PRE_HPP
#define NEDERROCK_SRC_EXPRESSION_PARSER_NODE_PRE_HPP

#include <memory>
#include <vector>

class Expression_Parser_Node;

using Expression_Parser_Node_Ptr = std::shared_ptr<Expression_Parser_Node>;
using Expression_Parser_Node_Vector = std::vector<Expression_Parser_Node_Ptr>;

#endif // NEDERROCK_SRC_EXPRESSION_PARSER_NODE_PRE_HPP
