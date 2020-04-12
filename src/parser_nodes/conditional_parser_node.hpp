//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_CONDITIONAL_PARSER_NODE_HPP
#define NEDERROCK_SRC_CONDITIONAL_PARSER_NODE_HPP

#include "conditional_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "expression_parser_node_pre.hpp"
#include "consequent_parser_node_pre.hpp"
#include "alternate_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"
#include <memory>
#include <istream>
#include <string>

class Conditional_Parser_Node {
public:
  Conditional_Parser_Node(std::wstring&& keyword,
                          Separator_Parser_Node_Ptr&& separator,
                          Expression_Parser_Node_Ptr&& expression,
                          Consequent_Parser_Node_Ptr&& consequent,
                          Alternate_Parser_Node_Ptr&& alternate);

  static Conditional_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::wstring                m_keyword;
  Separator_Parser_Node_Ptr   m_separator;
  Expression_Parser_Node_Ptr  m_expression;
  Consequent_Parser_Node_Ptr  m_consequent;
  Alternate_Parser_Node_Ptr   m_alternate;
};

#endif // NEDERROCK_SRC_CONDITIONAL_PARSER_NODE_HPP
