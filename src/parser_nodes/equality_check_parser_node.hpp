//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_EQUALITY_CHECK_PARSER_NODE_HPP
#define NEDERROCK_SRC_EQUALITY_CHECK_PARSER_NODE_HPP

#include "equality_check_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "eq_parser_node_pre.hpp"
#include "not_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Equality_Check_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Not_Parser_Node_Ptr            m_not;
    Separator_Parser_Node_Ptr      m_separator_1;
    Eq_Parser_Node_Ptr             m_eq;
    Separator_Parser_Node_Ptr      m_separator_2;
    Equality_Check_Parser_Node_Ptr m_equality_check;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Not_Parser_Node_Ptr            m_not;
  };
public:
  Equality_Check_Parser_Node(Choice_1&& choice);
  Equality_Check_Parser_Node(Choice_2&& choice);

  static Equality_Check_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Equality_Check_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Equality_Check_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_EQUALITY_CHECK_PARSER_NODE_HPP
