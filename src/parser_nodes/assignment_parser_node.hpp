//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_ASSIGNMENT_PARSER_NODE_HPP
#define NEDERROCK_SRC_ASSIGNMENT_PARSER_NODE_HPP

#include "assignment_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "is_parser_node_pre.hpp"
#include "assignable_parser_node_pre.hpp"
#include "poetic_number_parser_node_pre.hpp"
#include "poetic_string_parser_node_pre.hpp"
#include "compoundable_operator_parser_node_pre.hpp"
#include "target_parser_node_pre.hpp"
#include "literal_parser_node_pre.hpp"
#include "expression_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>
#include <variant>

class Assignment_Parser_Node {
public:
  struct Choice_1_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Literal_Parser_Node_Ptr m_literal;
  };

  struct Choice_1_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Poetic_Number_Parser_Node_Ptr m_poetic_number;
  };

  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Assignable_Parser_Node_Ptr            m_assignable;
    Is_Parser_Node_Ptr                    m_is;
    Separator_Parser_Node_Vector          m_separators;
    std::variant<Choice_1_1, Choice_1_2>  m_choice;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Assignable_Parser_Node_Ptr      m_assignable;
    Separator_Parser_Node_Vector    m_separators;
    std::wstring                    m_keyword;
    Poetic_String_Parser_Node_Ptr   m_poetic_string;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::wstring                  m_keyword;
    Separator_Parser_Node_Vector  m_separators;
    Expression_Parser_Node_Ptr    m_expression;
    Target_Parser_Node_Ptr        m_target;
  };

  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::wstring                            m_keyword_1;
    Separator_Parser_Node_Vector            m_separators_1;
    Assignable_Parser_Node_Ptr              m_assignable;
    Separator_Parser_Node_Vector            m_separators_2;
    std::wstring                            m_keyword_2;
    Compoundable_Operator_Parser_Node_Ptr   m_compoundable_operator;
    Expression_Parser_Node_Ptr              m_expression;
  };

  struct Choice_5 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::wstring                  m_keyword_1;
    Separator_Parser_Node_Vector  m_separators_1;
    Assignable_Parser_Node_Ptr    m_assignable;
    Separator_Parser_Node_Vector  m_separators_2;
    std::wstring                  m_keyword_2;
    Separator_Parser_Node_Vector  m_separators_3;
    Expression_Parser_Node_Ptr    m_expression;
  };
public:
  Assignment_Parser_Node(Choice_1&& choice);
  Assignment_Parser_Node(Choice_2&& choice);
  Assignment_Parser_Node(Choice_3&& choice);
  Assignment_Parser_Node(Choice_4&& choice);
  Assignment_Parser_Node(Choice_5&& choice);

  static Assignment_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Assignment_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Assignment_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Assignment_Parser_Node_Ptr parse_choice_4(Token_Stream& input);
  static Assignment_Parser_Node_Ptr parse_choice_5(Token_Stream& input);

  static Assignment_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3, Choice_4, Choice_5> m_choice;
};

#endif // NEDERROCK_SRC_ASSIGNMENT_PARSER_NODE_HPP
