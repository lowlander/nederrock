//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_FUNCTION_PARSER_NODE_HPP
#define NEDERROCK_SRC_FUNCTION_PARSER_NODE_HPP

#include "function_parser_node_pre.hpp"
#include "variable_parser_node_pre.hpp"
#include "variable_list_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "eol_parser_node_pre.hpp"
#include "block_parser_node_pre.hpp"
#include "token_stream.hpp"
#include "scope_state.hpp"

#include <memory>
#include <istream>

class Function_Parser_Node {
public:
  Function_Parser_Node(Variable_Parser_Node_Ptr&& variable,
                       Separator_Parser_Node_Ptr&& separator_1,
                       std::wstring&& keyword,
                       Separator_Parser_Node_Ptr&& separator_2,
                       Variable_List_Parser_Node_Ptr&& variable_list,
                       Eol_Parser_Node_Ptr&& eol_1,
                       Block_Parser_Node_Ptr&& block,
                       Eol_Parser_Node_Ptr&& eol_2);

  static Function_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;
  void generate_cpp(Scope_State_Ptr state) const;
  void collect_variables(Scope_State_Ptr state) const;
private:
  Variable_Parser_Node_Ptr      m_variable;
  Separator_Parser_Node_Ptr     m_separator_1;
  std::wstring                  m_keyword;
  Separator_Parser_Node_Ptr     m_separator_2;
  Variable_List_Parser_Node_Ptr m_variable_list;
  Eol_Parser_Node_Ptr           m_eol_1;
  Block_Parser_Node_Ptr         m_block;
  Eol_Parser_Node_Ptr           m_eol_2;
};

#endif // NEDERROCK_SRC_FUNCTION_PARSER_NODE_HPP
