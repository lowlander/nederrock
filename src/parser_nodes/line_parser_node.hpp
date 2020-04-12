//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_LINE_PARSER_NODE_HPP
#define NEDERROCK_SRC_LINE_PARSER_NODE_HPP

#include "line_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "statement_parser_node_pre.hpp"
#include "eol_parser_node_pre.hpp"
#include "eof_parser_node_pre.hpp"

#include "token_stream.hpp"
#include "scope_state.hpp"

#include <istream>
#include <vector>
#include <variant>
#include <memory>
#include <set>

class Line_Parser_Node {
public:
  struct Choice_1_1 {
    void dump(std::wostream& output) const;

    Eof_Parser_Node_Ptr m_eof;
  };

  struct Choice_1_2 {
    void dump(std::wostream& output) const;

    Eol_Parser_Node_Vector m_eols;
  };

  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Separator_Parser_Node_Vector          m_separators;
    Statement_Parser_Node_Ptr             m_statement;
    std::variant<Choice_1_1, Choice_1_2>  m_choice;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;
    void collect_variables(Scope_State_Ptr state) const;

    Eol_Parser_Node_Ptr m_eol;
  };
public:
  Line_Parser_Node(Choice_1&& choice);
  Line_Parser_Node(Choice_2&& choice);

  static Line_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Line_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Line_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_LINE_PARSER_NODE_HPP
