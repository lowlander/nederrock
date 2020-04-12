//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_FALSE_PARSER_NODE_HPP
#define NEDERROCK_SRC_FALSE_PARSER_NODE_HPP

#include "false_parser_node_pre.hpp"
#include "letter_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>
#include <string>

class False_Parser_Node {
public:
  False_Parser_Node(std::wstring&& keyword);

  static False_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  std::wstring m_keyword;
};

#endif // NEDERROCK_SRC_FALSE_PARSER_NODE_HPP
