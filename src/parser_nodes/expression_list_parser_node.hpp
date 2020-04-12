//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_EXPRESSION_LIST_PARSER_NODE_HPP
#define NEDERROCK_SRC_EXPRESSION_LIST_PARSER_NODE_HPP

#include "expression_list_parser_node_pre.hpp"
#include "expression_list_separator_parser_node_pre.hpp"
#include "simple_expression_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Expression_List_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Simple_Expression_Parser_Node_Ptr           m_simple_expression;
    Expression_List_Separator_Parser_Node_Ptr   m_expression_list_separator;
    Expression_List_Parser_Node_Ptr             m_expression_list;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Simple_Expression_Parser_Node_Ptr           m_simple_expression;
  };
public:
  Expression_List_Parser_Node(Choice_1&& choice);
  Expression_List_Parser_Node(Choice_2&& choice);

  static Expression_List_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Expression_List_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Expression_List_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_EXPRESSION_LIST_PARSER_NODE_HPP
