//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_COMPARATOR_PARSER_NODE_HPP
#define NEDERROCK_SRC_COMPARATOR_PARSER_NODE_HPP

#include "comparator_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "greater_parser_node_pre.hpp"
#include "great_parser_node_pre.hpp"
#include "small_parser_node_pre.hpp"
#include "smaller_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>
#include <variant>

class Comparator_Parser_Node {
private:
  struct Choice_1 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    std::wstring                m_keyword_1;
    Separator_Parser_Node_Ptr   m_separator_1;
    Greater_Parser_Node_Ptr     m_greater;
    Separator_Parser_Node_Ptr   m_separator_2;
    std::wstring                m_keyword_2;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    std::wstring                m_keyword_1;
    Separator_Parser_Node_Ptr   m_separator_1;
    Smaller_Parser_Node_Ptr     m_smaller;
    Separator_Parser_Node_Ptr   m_separator_2;
    std::wstring                m_keyword_2;
  };

  struct Choice_3 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    std::wstring                m_keyword_1;
    Separator_Parser_Node_Ptr   m_separator_1;
    std::wstring                m_keyword_2;
    Separator_Parser_Node_Ptr   m_separator_2;
    Great_Parser_Node_Ptr       m_great;
    Separator_Parser_Node_Ptr   m_separator_3;
    std::wstring                m_keyword_3;
  };

  struct Choice_4 {
    void dump(std::wostream& output) const;
    void generate_cpp(Scope_State_Ptr state) const;

    std::wstring                m_keyword_1;
    Separator_Parser_Node_Ptr   m_separator_1;
    std::wstring                m_keyword_2;
    Separator_Parser_Node_Ptr   m_separator_2;
    Small_Parser_Node_Ptr       m_small;
    Separator_Parser_Node_Ptr   m_separator_3;
    std::wstring                m_keyword_3;
  };
public:
  Comparator_Parser_Node(Choice_1&& choice);
  Comparator_Parser_Node(Choice_2&& choice);
  Comparator_Parser_Node(Choice_3&& choice);
  Comparator_Parser_Node(Choice_4&& choice);

  static Comparator_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Comparator_Parser_Node_Ptr parse_choice_2(Token_Stream& input);
  static Comparator_Parser_Node_Ptr parse_choice_3(Token_Stream& input);
  static Comparator_Parser_Node_Ptr parse_choice_4(Token_Stream& input);

  static Comparator_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2, Choice_3, Choice_4> m_choice;
};

#endif // NEDERROCK_SRC_COMPARATOR_PARSER_NODE_HPP
