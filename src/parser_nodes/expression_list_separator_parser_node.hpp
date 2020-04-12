//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_EXPRESSION_LIST_SEPARATOR_PARSER_NODE_HPP
#define NEDERROCK_SRC_EXPRESSION_LIST_SEPARATOR_PARSER_NODE_HPP

#include "expression_list_separator_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

class Expression_List_Separator_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;

    Separator_Parser_Node_Ptr m_separator_1;
    std::wstring m_keyword;
    Separator_Parser_Node_Ptr m_separator_2;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;

    Separator_Parser_Node_Ptr m_separator_1;
    std::wstring m_keyword;
    Separator_Parser_Node_Ptr m_separator_2;
  };
public:
  Expression_List_Separator_Parser_Node(Choice_1&& choice);
  Expression_List_Separator_Parser_Node(Choice_2&& choice);

  static Expression_List_Separator_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Expression_List_Separator_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Expression_List_Separator_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_EXPRESSION_LIST_SEPARATOR_PARSER_NODE_HPP
