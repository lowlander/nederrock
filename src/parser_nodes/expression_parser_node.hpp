//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_EXPRESSION_PARSER_NODE_HPP
#define NEDERROCK_SRC_EXPRESSION_PARSER_NODE_HPP

#include "expression_parser_node_pre.hpp"
#include "boolean_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Expression_Parser_Node {
public:
  Expression_Parser_Node(Boolean_Parser_Node_Ptr&& boolean);

  static Expression_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  Boolean_Parser_Node_Ptr m_boolean;
};

#endif // NEDERROCK_SRC_EXPRESSION_PARSER_NODE_HPP
