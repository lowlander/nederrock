//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_UPPERCASE_LETTER_PARSER_NODE_HPP
#define NEDERROCK_SRC_UPPERCASE_LETTER_PARSER_NODE_HPP

#include "uppercase_letter_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <string>
#include <locale>

class Uppercase_Letter_Parser_Node {
public:
  Uppercase_Letter_Parser_Node(wchar_t c);

  static Uppercase_Letter_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  wchar_t to_wchar_t() const;
private:
  wchar_t m_char;
};

#endif // NEDERROCK_SRC_UPPERCASE_LETTER_PARSER_NODE_HPP
