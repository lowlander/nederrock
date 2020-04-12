//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_MUTATION_PARSER_NODE_HPP
#define NEDERROCK_SRC_MUTATION_PARSER_NODE_HPP

#include "mutation_parser_node_pre.hpp"
#include "mutator_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "expression_parser_node_pre.hpp"
#include "target_parser_node_pre.hpp"
#include "modifier_parser_node_pre.hpp"
#include "assignable_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Mutation_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Mutator_Parser_Node_Ptr    m_mutator;
    Separator_Parser_Node_Ptr  m_separator;
    Expression_Parser_Node_Ptr m_expression;
    Target_Parser_Node_Ptr     m_target;
    Modifier_Parser_Node_Ptr   m_modifier;

  };
  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Mutator_Parser_Node_Ptr    m_mutator;
    Separator_Parser_Node_Ptr  m_separator;
    Assignable_Parser_Node_Ptr m_assignable;
    Modifier_Parser_Node_Ptr   m_modifier;
  };
public:
  Mutation_Parser_Node(Choice_1&& choice);
  Mutation_Parser_Node(Choice_2&& choice);

  static Mutation_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Mutation_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Mutation_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_MUTATION_PARSER_NODE_HPP
