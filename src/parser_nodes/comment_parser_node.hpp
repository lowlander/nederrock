//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_COMMENT_PARSER_NODE_HPP
#define NEDERROCK_SRC_COMMENT_PARSER_NODE_HPP

#include "comment_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <istream>
#include <string>

class Comment_Parser_Node {
public:
  Comment_Parser_Node(std::wstring&& text);

  static Comment_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  std::wstring m_text;
};

#endif // NEDERROCK_SRC_COMMENT_PARSER_NODE_HPP
