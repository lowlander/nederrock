//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_PROPER_NOUN_PARSER_NODE_HPP
#define NEDERROCK_SRC_PROPER_NOUN_PARSER_NODE_HPP

#include "proper_noun_parser_node_pre.hpp"
#include "uppercase_letter_parser_node_pre.hpp"
#include "letter_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>

class Proper_Noun_Parser_Node {
public:
  Proper_Noun_Parser_Node(Uppercase_Letter_Parser_Node_Ptr&& uppercase_letter,
                          Letter_Parser_Node_Vector&& letters);

  static Proper_Noun_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;

  std::wstring to_string();
private:
  Uppercase_Letter_Parser_Node_Ptr m_uppercase_letter;
  Letter_Parser_Node_Vector        m_letters;
};

#endif // NEDERROCK_SRC_PROPER_NOUN_PARSER_NODE_HPP
