//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_FUNCTION_RETURN_PARSER_NODE_HPP
#define NEDERROCK_SRC_FUNCTION_RETURN_PARSER_NODE_HPP

#include "function_return_parser_node_pre.hpp"
#include "return_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "expression_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Function_Return_Parser_Node  {
public:
  Function_Return_Parser_Node(Return_Parser_Node_Ptr&& return_,
                              Separator_Parser_Node_Ptr&& separator,
                              Expression_Parser_Node_Ptr&&  expression);

  static Function_Return_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  Return_Parser_Node_Ptr      m_return;
  Separator_Parser_Node_Ptr   m_separator;
  Expression_Parser_Node_Ptr  m_expression;
};

#endif // NEDERROCK_SRC_FUNCTION_RETURN_PARSER_NODE_HPP
