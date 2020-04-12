//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_PRODUCT_PARSER_NODE_HPP
#define NEDERROCK_SRC_PRODUCT_PARSER_NODE_HPP

#include "product_parser_node_pre.hpp"
#include "expression_list_parser_node_pre.hpp"
#include "divide_parser_node_pre.hpp"
#include "multiply_parser_node_pre.hpp"
#include "simple_expression_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>
#include <vector>
#include <variant>

class Product_Parser_Node {
private:
  struct Rest {
    using Ptr = std::shared_ptr<Rest>;
    using Vector = std::vector<Ptr>;

    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    std::variant<Multiply_Parser_Node_Ptr, Divide_Parser_Node_Ptr>  m_mul_div;
    Expression_List_Parser_Node_Ptr                                  m_expression_list;
  };

  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Simple_Expression_Parser_Node_Ptr  m_simple_expression;
    Rest::Vector                        m_rest;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Expression_List_Parser_Node_Ptr m_expression_list;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Simple_Expression_Parser_Node_Ptr m_simple_expression;
  };
public:
  using Ptr = std::shared_ptr<Product_Parser_Node>;
public:
  Product_Parser_Node(Choice_1&& choice);
  Product_Parser_Node(Choice_2&& choice);
  Product_Parser_Node(Choice_3&& choice);

  static Rest::Ptr parse_rest(Token_Stream& input);
  static Product_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Product_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Product_Parser_Node_Ptr parse_choice_3(Token_Stream& input);

  static Product_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3> m_choice;
};

#endif // NEDERROCK_SRC_PRODUCT_PARSER_NODE_HPP
