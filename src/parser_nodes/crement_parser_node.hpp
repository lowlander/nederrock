//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_CREMENT_PARSER_NODE_HPP
#define NEDERROCK_SRC_CREMENT_PARSER_NODE_HPP

#include "crement_parser_node_pre.hpp"
#include "increment_parser_node_pre.hpp"
#include "decrement_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Crement_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Increment_Parser_Node_Ptr m_increment;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Decrement_Parser_Node_Ptr m_decrement;
  };
public:
  Crement_Parser_Node(Choice_1&& choice);
  Crement_Parser_Node(Choice_2&& choice);

  static Crement_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Crement_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Crement_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_CREMENT_PARSER_NODE_HPP
