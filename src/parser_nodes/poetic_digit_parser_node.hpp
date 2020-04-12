//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_POETIC_DIGIT_PARSER_NODE_HPP
#define NEDERROCK_SRC_POETIC_DIGIT_PARSER_NODE_HPP

#include "poetic_digit_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <string>

class Poetic_Digit_Parser_Node {
public:
  Poetic_Digit_Parser_Node(std::wstring&& text);

  static Poetic_Digit_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  std::wstring to_string() const;
private:
  std::wstring m_text;
};

#endif // NEDERROCK_SRC_POETIC_DIGIT_PARSER_NODE_HPP
