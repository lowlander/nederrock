//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_MODIFIER_PARSER_NODE_HPP
#define NEDERROCK_SRC_MODIFIER_PARSER_NODE_HPP

#include "modifier_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "expression_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Modifier_Parser_Node {
public:
  Modifier_Parser_Node(Separator_Parser_Node_Ptr&& separator_1,
                       std::wstring&& keyword,
                       Separator_Parser_Node_Ptr&& separator_2,
                       Expression_Parser_Node_Ptr&& expression);

  static Modifier_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  Separator_Parser_Node_Ptr   m_separator_1;
  std::wstring                m_keyword;
  Separator_Parser_Node_Ptr   m_separator_2;
  Expression_Parser_Node_Ptr  m_expression;
};

#endif // NEDERROCK_SRC_MODIFIER_PARSER_NODE_HPP
