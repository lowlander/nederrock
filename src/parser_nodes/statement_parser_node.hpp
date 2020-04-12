//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_STATEMENT_PARSER_NODE_HPP
#define NEDERROCK_SRC_STATEMENT_PARSER_NODE_HPP

#include "statement_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "break_parser_node_pre.hpp"
#include "continue_parser_node_pre.hpp"
#include "function_parser_node_pre.hpp"
#include "function_return_parser_node_pre.hpp"
#include "function_call_parser_node_pre.hpp"
#include "loop_parser_node_pre.hpp"
#include "conditional_parser_node_pre.hpp"
#include "operation_parser_node_pre.hpp"
#include "expression_parser_node_pre.hpp"

#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <vector>
#include <variant>
#include <set>

class Statement_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector  m_separators;
    Break_Parser_Node_Ptr         m_break;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector  m_separators;
    Continue_Parser_Node_Ptr      m_continue;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector  m_separators;
    Function_Parser_Node_Ptr      m_function;
  };

  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector    m_separators;
    Function_Call_Parser_Node_Ptr   m_function_call;
  };
  struct Choice_5 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector    m_separators;
    Function_Return_Parser_Node_Ptr m_function_return;
  };
  struct Choice_6 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector  m_separators;
    Loop_Parser_Node_Ptr          m_loop;
  };
  struct Choice_7 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector  m_separators;
    Conditional_Parser_Node_Ptr   m_conditional;
  };
  struct Choice_8 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector  m_separators;
    Operation_Parser_Node_Ptr     m_operation;
  };
  struct Choice_9 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector  m_separators;
    Expression_Parser_Node_Ptr    m_expression;
  };
public:
  Statement_Parser_Node(Choice_1&& choice);
  Statement_Parser_Node(Choice_2&& choice);
  Statement_Parser_Node(Choice_3&& choice);
  Statement_Parser_Node(Choice_4&& choice);
  Statement_Parser_Node(Choice_5&& choice);
  Statement_Parser_Node(Choice_6&& choice);
  Statement_Parser_Node(Choice_7&& choice);
  Statement_Parser_Node(Choice_8&& choice);
  Statement_Parser_Node(Choice_9&& choice);

  static Statement_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_4(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_5(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_6(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_7(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_8(Token_Stream& input);
  static Statement_Parser_Node_Ptr parse_choice_9(Token_Stream& input);

  static Statement_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1,Choice_2,Choice_3,Choice_4,Choice_5,Choice_6,Choice_7,Choice_8,Choice_9> m_choice;
};

#endif // NEDERROCK_SRC_STATEMENT_PARSER_NODE_HPP
