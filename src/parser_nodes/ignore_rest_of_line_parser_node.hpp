//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_IGNORE_REST_OF_LINE_PARSER_NODE_HPP
#define NEDERROCK_SRC_IGNORE_REST_OF_LINE_PARSER_NODE_HPP

#include "ignore_rest_of_line_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <string>

class Ignore_Rest_Of_Line_Parser_Node {
public:
  Ignore_Rest_Of_Line_Parser_Node(Separator_Parser_Node_Ptr&& separator,
                                  std::wstring&& rest_of_line);

  static Ignore_Rest_Of_Line_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  Separator_Parser_Node_Ptr   m_separator;
  std::wstring                m_rest_of_line;
};

#endif // NEDERROCK_SRC_IGNORE_REST_OF_LINE_PARSER_NODE_HPP
