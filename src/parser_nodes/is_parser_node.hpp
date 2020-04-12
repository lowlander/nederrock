//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_IS_PARSER_NODE_HPP
#define NEDERROCK_SRC_IS_PARSER_NODE_HPP

#include "is_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <variant>

class Is_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;

    std::wstring m_keyword;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;

    Separator_Parser_Node_Vector  m_separators;
    std::wstring                  m_keyword;
  };
public:
  Is_Parser_Node(Choice_1&& choice);
  Is_Parser_Node(Choice_2&& choice);

  static Is_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Is_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Is_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_IS_PARSER_NODE_HPP
