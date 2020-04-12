//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_SIMPLE_VARIABLE_PARSER_NODE_HPP
#define NEDERROCK_SRC_SIMPLE_VARIABLE_PARSER_NODE_HPP

#include "simple_variable_parser_node_pre.hpp"
#include "letter_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>

class Simple_Variable_Parser_Node {
public:
  Simple_Variable_Parser_Node(Letter_Parser_Node_Vector&& letters);

  static Simple_Variable_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  std::wstring to_string();
private:
  Letter_Parser_Node_Vector m_letters;
};

#endif // NEDERROCK_SRC_SIMPLE_VARIABLE_PARSER_NODE_HPP
