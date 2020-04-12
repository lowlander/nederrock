//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_COMMON_PREFIX_PARSER_NODE_HPP
#define NEDERROCK_SRC_COMMON_PREFIX_PARSER_NODE_HPP

#include "common_prefix_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <string>

class Common_Prefix_Parser_Node {
public:
  Common_Prefix_Parser_Node(std::wstring&& keyword);

  static Common_Prefix_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;

  std::wstring to_string();
private:
  std::wstring m_keyword;
};

#endif // NEDERROCK_SRC_COMMON_PREFIX_PARSER_NODE_HPP
