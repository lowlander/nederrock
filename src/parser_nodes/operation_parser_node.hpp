//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_OPERATION_PARSER_NODE_HPP
#define NEDERROCK_SRC_OPERATION_PARSER_NODE_HPP

#include "operation_parser_node_pre.hpp"
#include "readline_parser_node_pre.hpp"
#include "output_parser_node_pre.hpp"
#include "crement_parser_node_pre.hpp"
#include "mutation_parser_node_pre.hpp"
#include "assignment_parser_node_pre.hpp"
#include "rounding_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Operation_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Readline_Parser_Node_Ptr m_readline;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Output_Parser_Node_Ptr m_output;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Crement_Parser_Node_Ptr m_crement;
  };

  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Mutation_Parser_Node_Ptr m_mutation;
  };

  struct Choice_5 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Assignment_Parser_Node_Ptr m_assignment;
  };

  struct Choice_6 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Rounding_Parser_Node_Ptr m_rounding;
  };
public:
  Operation_Parser_Node(Choice_1&& choice);
  Operation_Parser_Node(Choice_2&& choice);
  Operation_Parser_Node(Choice_3&& choice);
  Operation_Parser_Node(Choice_4&& choice);
  Operation_Parser_Node(Choice_5&& choice);
  Operation_Parser_Node(Choice_6&& choice);

  static Operation_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Operation_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Operation_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Operation_Parser_Node_Ptr parse_choice_4(Token_Stream& input);
  static Operation_Parser_Node_Ptr parse_choice_5(Token_Stream& input);
  static Operation_Parser_Node_Ptr parse_choice_6(Token_Stream& input);

  static Operation_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1,Choice_2,Choice_3,Choice_4,Choice_5,Choice_6> m_choice;
};

#endif // NEDERROCK_SRC_OPERATION_PARSER_NODE_HPP
