//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_SIMPLE_EXPRESSION_PARSER_NODE_HPP
#define NEDERROCK_SRC_SIMPLE_EXPRESSION_PARSER_NODE_HPP

#include "simple_expression_parser_node_pre.hpp"
#include "function_call_parser_node_pre.hpp"
#include "constant_parser_node_pre.hpp"
#include "lookup_parser_node_pre.hpp"
#include "literal_parser_node_pre.hpp"
#include "pronoun_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Simple_Expression_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Function_Call_Parser_Node_Ptr  m_function_call;
  };
  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Constant_Parser_Node_Ptr       m_constant;
  };
  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Lookup_Parser_Node_Ptr         m_lookup;
  };
  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Literal_Parser_Node_Ptr        m_literal;
  };
  struct Choice_5 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Pronoun_Parser_Node_Ptr        m_pronoun;
  };

public:
  Simple_Expression_Parser_Node(Choice_1&& choice);
  Simple_Expression_Parser_Node(Choice_2&& choice);
  Simple_Expression_Parser_Node(Choice_3&& choice);
  Simple_Expression_Parser_Node(Choice_4&& choice);
  Simple_Expression_Parser_Node(Choice_5&& choice);

  static Simple_Expression_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Simple_Expression_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Simple_Expression_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Simple_Expression_Parser_Node_Ptr parse_choice_4(Token_Stream& input);
  static Simple_Expression_Parser_Node_Ptr parse_choice_5(Token_Stream& input);

  static Simple_Expression_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3, Choice_4, Choice_5> m_choice;
};

#endif // NEDERROCK_SRC_SIMPLE_EXPRESSION_PARSER_NODE_HPP
