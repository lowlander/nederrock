//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_ARITHMETIC_PARSER_NODE_HPP
#define NEDERROCK_SRC_ARITHMETIC_PARSER_NODE_HPP

#include "arithmetic_parser_node_pre.hpp"

#include "product_parser_node_pre.hpp"
#include "add_parser_node_pre.hpp"
#include "subtract_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>
#include <vector>
#include <variant>

class Arithmetic_Parser_Node {
public:
  struct Rest {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::variant<Add_Parser_Node_Ptr, Subtract_Parser_Node_Ptr> m_add_sub;
    Product_Parser_Node_Ptr                                     m_product;
  };

  using Rest_Ptr =  std::shared_ptr<Rest>;
  using Rest_Vector = std::vector<Rest_Ptr>;

  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Product_Parser_Node_Ptr m_product;
    Rest_Vector             m_rest;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Product_Parser_Node_Ptr m_product;
  };
public:
  Arithmetic_Parser_Node(Choice_1&& choice);
  Arithmetic_Parser_Node(Choice_2&& choice);

  static Rest_Ptr parse_rest(Token_Stream& input);
  static Arithmetic_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Arithmetic_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Arithmetic_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_ARITHMETIC_PARSER_NODE_HPP
