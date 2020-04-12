//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_ROUNDING_PARSER_NODE_HPP
#define NEDERROCK_SRC_ROUNDING_PARSER_NODE_HPP

#include "rounding_parser_node_pre.hpp"
#include "floor_parser_node_pre.hpp"
#include "ceil_parser_node_pre.hpp"
#include "math_round_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Rounding_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Floor_Parser_Node_Ptr m_floor;
  };
  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Ceil_Parser_Node_Ptr m_ceil;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Math_Round_Parser_Node_Ptr m_math_round;
  };
public:
  Rounding_Parser_Node(Choice_1&& choice);
  Rounding_Parser_Node(Choice_2&& choice);
  Rounding_Parser_Node(Choice_3&& choice);

  static Rounding_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Rounding_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Rounding_Parser_Node_Ptr parse_choice_3(Token_Stream& input);

  static Rounding_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3> m_choice;
};

#endif // NEDERROCK_SRC_ROUNDING_PARSER_NODE_HPP
