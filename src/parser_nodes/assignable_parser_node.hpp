//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_ASSIGNABLE_PARSER_NODE_HPP
#define NEDERROCK_SRC_ASSIGNABLE_PARSER_NODE_HPP

#include "assignable_parser_node_pre.hpp"
#include "variable_parser_node_pre.hpp"
#include "indexer_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Assignable_Parser_Node {
public:
  Assignable_Parser_Node(Variable_Parser_Node_Ptr&& variable,
                         Indexer_Parser_Node_Ptr&& indexer);

  static Assignable_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  Variable_Parser_Node_Ptr  m_variable;
  Indexer_Parser_Node_Ptr   m_indexer;
};

#endif // NEDERROCK_SRC_ASSIGNABLE_PARSER_NODE_HPP
