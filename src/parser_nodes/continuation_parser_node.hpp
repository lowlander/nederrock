//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_CONTINUATION_PARSER_NODE_HPP
#define NEDERROCK_SRC_CONTINUATION_PARSER_NODE_HPP

#include "continuation_parser_node_pre.hpp"
#include "eol_parser_node_pre.hpp"
#include "statement_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Continuation_Parser_Node {
public:
  Continuation_Parser_Node(Eol_Parser_Node_Ptr&&          eol,
                           Separator_Parser_Node_Vector&& separators,
                           Statement_Parser_Node_Ptr&&    statement);

  static Continuation_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  Eol_Parser_Node_Ptr           m_eol;
  Separator_Parser_Node_Vector  m_separators;
  Statement_Parser_Node_Ptr     m_statement;
};

#endif // NEDERROCK_SRC_CONTINUATION_PARSER_NODE_HPP
