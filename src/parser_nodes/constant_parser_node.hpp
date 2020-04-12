//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_CONSTANT_PARSER_NODE_HPP
#define NEDERROCK_SRC_CONSTANT_PARSER_NODE_HPP

#include "constant_parser_node_pre.hpp"
#include "null_parser_node_pre.hpp"
#include "true_parser_node_pre.hpp"
#include "false_parser_node_pre.hpp"
#include "mysterious_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Constant_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Null_Parser_Node_Ptr m_null;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    True_Parser_Node_Ptr m_true;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    False_Parser_Node_Ptr m_false;
  };

  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Mysterious_Parser_Node_Ptr m_mysterious;
  };

public:
  Constant_Parser_Node(Choice_1&& choice);
  Constant_Parser_Node(Choice_2&& choice);
  Constant_Parser_Node(Choice_3&& choice);
  Constant_Parser_Node(Choice_4&& choice);

  static Constant_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Constant_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Constant_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Constant_Parser_Node_Ptr parse_choice_4(Token_Stream& input);
  static Constant_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3, Choice_4> m_choice;
};

#endif // NEDERROCK_SRC_CONSTANT_PARSER_NODE_HPP
