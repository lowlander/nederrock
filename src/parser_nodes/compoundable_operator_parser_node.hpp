//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_COMPOUNDABLE_OPERATOR_PARSER_NODE_HPP
#define NEDERROCK_SRC_COMPOUNDABLE_OPERATOR_PARSER_NODE_HPP

#include "compoundable_operator_parser_node_pre.hpp"
#include "add_parser_node_pre.hpp"
#include "subtract_parser_node_pre.hpp"
#include "multiply_parser_node_pre.hpp"
#include "divide_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Compoundable_Operator_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Add_Parser_Node_Ptr m_add;
  };
  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Subtract_Parser_Node_Ptr m_subtract;
  };
  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Multiply_Parser_Node_Ptr m_multiply;
  };
  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Divide_Parser_Node_Ptr m_divide;
  };
public:
  Compoundable_Operator_Parser_Node(Choice_1&& choice);
  Compoundable_Operator_Parser_Node(Choice_2&& choice);
  Compoundable_Operator_Parser_Node(Choice_3&& choice);
  Compoundable_Operator_Parser_Node(Choice_4&& choice);

  static Compoundable_Operator_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Compoundable_Operator_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Compoundable_Operator_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Compoundable_Operator_Parser_Node_Ptr parse_choice_4(Token_Stream& input);

  static Compoundable_Operator_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3, Choice_4> m_choice;
};

#endif // NEDERROCK_SRC_COMPOUNDABLE_OPERATOR_PARSER_NODE_HPP
