//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_COMPARISON_PARSER_NODE_HPP
#define NEDERROCK_SRC_COMPARISON_PARSER_NODE_HPP

#include "comparison_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "comparator_parser_node_pre.hpp"
#include "arithmetic_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>
#include <variant>

class Comparison_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Arithmetic_Parser_Node_Ptr  m_arithmetic;
    Separator_Parser_Node_Ptr   m_separator_1;
    Comparator_Parser_Node_Ptr  m_comparator;
    Separator_Parser_Node_Ptr   m_separator_2;
    Comparison_Parser_Node_Ptr  m_comparison;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Arithmetic_Parser_Node_Ptr  m_arithmetic;
  };
public:
  Comparison_Parser_Node(Choice_1&& choice);
  Comparison_Parser_Node(Choice_2&& choice);

  static Comparison_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Comparison_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Comparison_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_COMPARISON_PARSER_NODE_HPP
