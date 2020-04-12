//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_LITERAL_PARSER_NODE_HPP
#define NEDERROCK_SRC_LITERAL_PARSER_NODE_HPP

#include "literal_parser_node_pre.hpp"
#include "constant_parser_node_pre.hpp"
#include "number_parser_node_pre.hpp"
#include "string_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Literal_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Constant_Parser_Node_Ptr m_constant;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Number_Parser_Node_Ptr m_number;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    String_Parser_Node_Ptr m_string;
  };

public:
  Literal_Parser_Node(Choice_1&& choice);
  Literal_Parser_Node(Choice_2&& choice);
  Literal_Parser_Node(Choice_3&& choice);

  static Literal_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Literal_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Literal_Parser_Node_Ptr parse_choice_3(Token_Stream& input);

  static Literal_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3> m_choice;
};

#endif // NEDERROCK_SRC_LITERAL_PARSER_NODE_HPP
