//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_VARIABLE_PARSER_NODE_HPP
#define NEDERROCK_SRC_VARIABLE_PARSER_NODE_HPP

#include "variable_parser_node_pre.hpp"
#include "common_variable_parser_node_pre.hpp"
#include "proper_variable_parser_node_pre.hpp"
#include "pronoun_parser_node_pre.hpp"
#include "simple_variable_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Variable_Parser_Node {
private:

  struct Choice_1 {
    void dump(std::wostream& output) const;
    std::wstring to_string() const;
    std::wstring get_name() const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Common_Variable_Parser_Node_Ptr m_common_variable;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    std::wstring to_string() const;
    std::wstring get_name() const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Proper_Variable_Parser_Node_Ptr m_proper_variable;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    std::wstring to_string() const;
    std::wstring get_name() const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Pronoun_Parser_Node_Ptr m_pronoun;
  };

  struct Choice_4 {
    void dump(std::wostream& output) const;
    std::wstring to_string() const;
    std::wstring get_name() const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Simple_Variable_Parser_Node_Ptr m_simple_variable;
  };
public:
  Variable_Parser_Node(Choice_1&& choice);
  Variable_Parser_Node(Choice_2&& choice);
  Variable_Parser_Node(Choice_3&& choice);
  Variable_Parser_Node(Choice_4&& choice);

  static Variable_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Variable_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Variable_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Variable_Parser_Node_Ptr parse_choice_4(Token_Stream& input);

  static Variable_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;

  std::wstring to_string() const;
  std::wstring get_name() const;
private:
  std::variant<Choice_1, Choice_2, Choice_3, Choice_4> m_choice;
};

#endif // NEDERROCK_SRC_VARIABLE_PARSER_NODE_HPP
