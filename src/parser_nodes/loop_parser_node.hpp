//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_LOOP_PARSER_NODE_HPP
#define NEDERROCK_SRC_LOOP_PARSER_NODE_HPP

#include "loop_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "expression_parser_node_pre.hpp"
#include "loopable_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

class Loop_Parser_Node {
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::wstring                m_keyword;
    Separator_Parser_Node_Ptr   m_separator;
    Expression_Parser_Node_Ptr  m_expression;
    Loopable_Parser_Node_Ptr    m_loopable;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::wstring                m_keyword;
    Separator_Parser_Node_Ptr   m_separator;
    Expression_Parser_Node_Ptr  m_expression;
    Loopable_Parser_Node_Ptr    m_loopable;
  };
public:
  Loop_Parser_Node(Choice_1&& choice);
  Loop_Parser_Node(Choice_2&& choice);

  static Loop_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Loop_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Loop_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_LOOP_PARSER_NODE_HPP
