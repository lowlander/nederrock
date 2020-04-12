//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "function_return_parser_node.hpp"
#include "return_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "expression_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Function_Return_Parser_Node::Function_Return_Parser_Node(Return_Parser_Node_Ptr&& return_,
                            Separator_Parser_Node_Ptr&& separator,
                            Expression_Parser_Node_Ptr&&  expression)
  : m_return(std::move(return_))
  , m_separator(std::move(separator))
  , m_expression(std::move(expression))
{
}

Function_Return_Parser_Node_Ptr
Function_Return_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto return_ = Return_Parser_Node::parse(input);
  if (return_ == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto separator = Separator_Parser_Node::parse(input);
  if (separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto expression = Expression_Parser_Node::parse(input);
  if (expression == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Function_Return_Parser_Node>(std::move(return_),
                                                       std::move(separator),
                                                       std::move(expression));
}

void Function_Return_Parser_Node::dump(std::wostream& output) const
{
  m_return->dump(output);
  m_separator->dump(output);
  m_expression->dump(output);
}

void Function_Return_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"return rockstar::Variable(");
  m_expression->generate_cpp(state);
  state->add_code(L");\n");
}


void Function_Return_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_expression->collect_variables(state);
}
