//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_ALTERNATE_PARSER_NODE_HPP
#define NEDERROCK_SRC_ALTERNATE_PARSER_NODE_HPP

#include "alternate_parser_node_pre.hpp"

#include "separator_parser_node_pre.hpp"
#include "statement_parser_node_pre.hpp"
#include "eol_parser_node_pre.hpp"
#include "block_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

class Alternate_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr scope) const;
    void collect_variables(Scope_State_Ptr scope) const;

    Separator_Parser_Node_Ptr  m_separator_1;
    std::wstring                m_keyword;
    Separator_Parser_Node_Ptr  m_separator_2;
    Statement_Parser_Node_Ptr  m_statement;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr scope) const;
    void collect_variables(Scope_State_Ptr scope) const;

    Eol_Parser_Node_Vector     m_eols;
    std::wstring                m_keyword;
    Separator_Parser_Node_Ptr  m_separator;
    Statement_Parser_Node_Ptr  m_statement;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr scope) const;
    void collect_variables(Scope_State_Ptr scope) const;

    Eol_Parser_Node_Vector     m_eols;
    std::wstring                m_keyword;
    Eol_Parser_Node_Ptr        m_eol;
    Block_Parser_Node_Ptr      m_block;
  };

  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr scope) const;
    void collect_variables(Scope_State_Ptr scope) const;

    Eol_Parser_Node_Ptr        m_eol;
  };
public:
  Alternate_Parser_Node(Choice_1&& choice);
  Alternate_Parser_Node(Choice_2&& choice);
  Alternate_Parser_Node(Choice_3&& choice);
  Alternate_Parser_Node(Choice_4&& choice);

  static Alternate_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Alternate_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Alternate_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Alternate_Parser_Node_Ptr parse_choice_4(Token_Stream& input);
  static Alternate_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr scope) const;
  void collect_variables(Scope_State_Ptr scope) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3, Choice_4> m_choice;
};

#endif // NEDERROCK_SRC_ALTERNATE_PARSER_NODE_HPP
