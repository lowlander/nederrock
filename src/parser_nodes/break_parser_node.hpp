//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_BREAK_PARSER_NODE_HPP
#define NEDERROCK_SRC_BREAK_PARSER_NODE_HPP

#include "break_parser_node_pre.hpp"
#include "ignore_rest_of_line_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>

class Break_Parser_Node {
public:
  Break_Parser_Node(std::wstring&& keyword,
                    Ignore_Rest_Of_Line_Parser_Node_Ptr&& rest_of_line);

  static Break_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::wstring                          m_keyword;
  Ignore_Rest_Of_Line_Parser_Node_Ptr   m_rest_of_line;
};

#endif // NEDERROCK_SRC_BREAK_PARSER_NODE_HPP
