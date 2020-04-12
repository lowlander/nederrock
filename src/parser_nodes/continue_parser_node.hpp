//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_CONTINUE_PARSER_NODE_HPP
#define NEDERROCK_SRC_CONTINUE_PARSER_NODE_HPP

#include "continue_parser_node_pre.hpp"
#include "ignore_rest_of_line_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

class Continue_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;

    std::wstring m_keyword;
    Ignore_Rest_Of_Line_Parser_Node_Ptr m_ignore_rest_of_line;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;

    std::wstring m_keyword;
  };

public:
  Continue_Parser_Node(Choice_1&& choice);
  Continue_Parser_Node(Choice_2&& choice);

  static Continue_Parser_Node_Ptr parse_choice_1(Token_Stream& input);
  static Continue_Parser_Node_Ptr parse_choice_2(Token_Stream& input);

  static Continue_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::variant<Choice_1, Choice_2> m_choice;
};

#endif // NEDERROCK_SRC_CONTINUE_PARSER_NODE_HPP