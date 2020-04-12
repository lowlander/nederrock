//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_BOOLEAN_PARSER_NODE_HPP
#define NEDERROCK_SRC_BOOLEAN_PARSER_NODE_HPP

#include "boolean_parser_node_pre.hpp"
#include "nor_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Boolean_Parser_Node {
public:
  Boolean_Parser_Node(Nor_Parser_Node_Ptr&& nor);

  static Boolean_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  Nor_Parser_Node_Ptr m_nor;
};

#endif // NEDERROCK_SRC_BOOLEAN_PARSER_NODE_HPP
