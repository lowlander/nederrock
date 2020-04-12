//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_READLINE_PARSER_NODE_PRE_HPP
#define NEDERROCK_SRC_READLINE_PARSER_NODE_PRE_HPP

#include <memory>
#include <vector>

class Readline_Parser_Node;

using Readline_Parser_Node_Ptr = std::shared_ptr<Readline_Parser_Node>;
using Readline_Parser_Node_Vector = std::vector<Readline_Parser_Node_Ptr>;

#endif // NEDERROCK_SRC_READLINE_PARSER_NODE_PRE_HPP
