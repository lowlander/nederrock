//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_SUBTRACT_PARSER_NODE_HPP
#define NEDERROCK_SRC_SUBTRACT_PARSER_NODE_HPP

#include "subtract_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Subtract_Parser_Node {
public:
  Subtract_Parser_Node(Separator_Parser_Node_Vector&& separators_1,
                  std::wstring&& keyword,
                  Separator_Parser_Node_Vector&& separators_2);

  static Subtract_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
private:
  Separator_Parser_Node_Vector  m_separators_1;
  std::wstring                    m_keyword;
  Separator_Parser_Node_Vector  m_separators_2;
};

#endif // NEDERROCK_SRC_SUBTRACT_PARSER_NODE_HPP
