//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_MUTATOR_PARSER_NODE_HPP
#define NEDERROCK_SRC_MUTATOR_PARSER_NODE_HPP

#include "mutator_parser_node_pre.hpp"
#include "split_parser_node_pre.hpp"
#include "cast_parser_node_pre.hpp"
#include "join_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Mutator_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Split_Parser_Node_Ptr m_split;
  };
  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Cast_Parser_Node_Ptr m_cast;
  };
  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    Join_Parser_Node_Ptr m_join;
  };
public:
  Mutator_Parser_Node(Choice_1&& choice);
  Mutator_Parser_Node(Choice_2&& choice);
  Mutator_Parser_Node(Choice_3&& choice);

  static Mutator_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Mutator_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Mutator_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Mutator_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3> m_choice;
};

#endif // NEDERROCK_SRC_MUTATOR_PARSER_NODE_HPP
