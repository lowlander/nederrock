//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_LOOPABLE_PARSER_NODE_HPP
#define NEDERROCK_SRC_LOOPABLE_PARSER_NODE_HPP

#include "loopable_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "eol_parser_node_pre.hpp"
#include "block_parser_node_pre.hpp"
#include "statement_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>

class Loopable_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Ptr m_separator;
    Statement_Parser_Node_Ptr m_statement;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Eol_Parser_Node_Ptr   m_eol_1;
    Block_Parser_Node_Ptr m_block;
    Eol_Parser_Node_Ptr   m_eol_2;
  };
public:
  Loopable_Parser_Node(Choice_1&& choice);
  Loopable_Parser_Node(Choice_2&& choice);

  static Loopable_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Loopable_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Loopable_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_LOOPABLE_PARSER_NODE_HPP
