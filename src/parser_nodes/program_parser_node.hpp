//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_PROGRAM_PARSER_NODE_HPP
#define NEDERROCK_SRC_PROGRAM_PARSER_NODE_HPP

#include "program_parser_node_pre.hpp"
#include "line_parser_node_pre.hpp"

#include "token_stream.hpp"

#include <istream>
#include <vector>

class Program_Parser_Node {
public:
  Program_Parser_Node(Line_Parser_Node_Vector&& lines);

  static Program_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(std::wostream& output) const;
private:
  Line_Parser_Node_Vector m_lines;
};

#endif // NEDERROCK_SRC_PROGRAM_PARSER_NODE_HPP
