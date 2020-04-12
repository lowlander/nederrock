//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_WHITESPACE_PARSER_NODE_HPP
#define NEDERROCK_SRC_WHITESPACE_PARSER_NODE_HPP

#include "whitespace_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <string>

class Whitespace_Parser_Node {
public:
  Whitespace_Parser_Node(wchar_t c);

  static Whitespace_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  wchar_t m_char;
};

#endif // NEDERROCK_SRC_WHITESPACE_PARSER_NODE_HPP
