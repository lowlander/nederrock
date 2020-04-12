//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_VARIABLE_LIST_PARSER_NODE_HPP
#define NEDERROCK_SRC_VARIABLE_LIST_PARSER_NODE_HPP

#include "variable_list_parser_node_pre.hpp"
#include "variable_list_separator_parser_node_pre.hpp"
#include "variable_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

class Variable_List_Parser_Node {
public:

  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;
    size_t get_variable_count() const;

    Variable_Parser_Node_Ptr                 m_variable;
    Variable_List_Separator_Parser_Node_Ptr  m_variable_list_separator;
    Variable_List_Parser_Node_Ptr            m_variable_list;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;
    size_t get_variable_count() const;

    Variable_Parser_Node_Ptr                 m_variable;
  };
public:
  Variable_List_Parser_Node(Choice_1&& choice);
  Variable_List_Parser_Node(Choice_2&& choice);

  static Variable_List_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Variable_List_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Variable_List_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
  size_t get_variable_count() const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_VARIABLE_LIST_PARSER_NODE_HPP
