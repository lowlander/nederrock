//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_NUMBER_PARSER_NODE_HPP
#define NEDERROCK_SRC_NUMBER_PARSER_NODE_HPP

#include "number_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>

class Number_Parser_Node {
public:
  Number_Parser_Node(std::wstring&& str);

  static Number_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::wstring m_string;
};

#endif // NEDERROCK_SRC_NUMBER_PARSER_NODE_HPP
