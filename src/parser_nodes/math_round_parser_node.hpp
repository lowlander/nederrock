//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_MATH_ROUND_PARSER_NODE_HPP
#define NEDERROCK_SRC_MATH_ROUND_PARSER_NODE_HPP

#include "math_round_parser_node_pre.hpp"

#include "separator_parser_node_pre.hpp"
#include "variable_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

class Math_Round_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::wstring                m_keyword_1;
    Separator_Parser_Node_Ptr   m_separator_1;
    Variable_Parser_Node_Ptr    m_variable;
    Separator_Parser_Node_Ptr   m_separator_2;
    std::wstring                m_keyword_2;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::wstring                m_keyword_1;
    Separator_Parser_Node_Ptr   m_separator_1;
    std::wstring                m_keyword_2;
    Separator_Parser_Node_Ptr   m_separator_2;
    Variable_Parser_Node_Ptr    m_variable;
  };
public:
  Math_Round_Parser_Node(Choice_1&& choice);
  Math_Round_Parser_Node(Choice_2&& choice);

  static Math_Round_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Math_Round_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Math_Round_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_MATH_ROUND_PARSER_NODE_HPP
