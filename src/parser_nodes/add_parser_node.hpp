//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_ADD_PARSER_NODE_HPP
#define NEDERROCK_SRC_ADD_PARSER_NODE_HPP

#include "add_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <istream>
#include <string>

class Add_Parser_Node {
public:
  Add_Parser_Node(Separator_Parser_Node_Vector&& separators_1,
                  std::wstring&& keyword,
                  Separator_Parser_Node_Vector&& separators_2);

  static Add_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr scope) const;
private:
  Separator_Parser_Node_Vector  m_separators_1;
  std::wstring                  m_keyword;
  Separator_Parser_Node_Vector  m_separators_2;
};

#endif // NEDERROCK_SRC_ADD_PARSER_NODE_HPP
