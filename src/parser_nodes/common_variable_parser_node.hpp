//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_COMMON_VARIABLE_PARSER_NODE_HPP
#define NEDERROCK_SRC_COMMON_VARIABLE_PARSER_NODE_HPP

#include "common_variable_parser_node_pre.hpp"
#include "common_prefix_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "letter_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>

class Common_Variable_Parser_Node {
public:
  Common_Variable_Parser_Node(Common_Prefix_Parser_Node_Ptr&& common_prefix,
                              Separator_Parser_Node_Vector&& separators,
                              Letter_Parser_Node_Vector&& letters);

  static Common_Variable_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;

  std::wstring to_string();
private:
  Common_Prefix_Parser_Node_Ptr   m_common_prefix;
  Separator_Parser_Node_Vector    m_separators;
  Letter_Parser_Node_Vector       m_letters;
};

#endif // NEDERROCK_SRC_COMMON_VARIABLE_PARSER_NODE_HPP
