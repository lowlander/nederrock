//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_SMALLER_PARSER_NODE_HPP
#define NEDERROCK_SRC_SMALLER_PARSER_NODE_HPP

#include "smaller_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <string>

class Smaller_Parser_Node {
public:
  Smaller_Parser_Node(std::wstring&& keyword);

  static Smaller_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  std::wstring m_keyword;
};

#endif // NEDERROCK_SRC_SMALLER_PARSER_NODE_HPP
