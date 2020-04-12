//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "expression_parser_node.hpp"
#include "boolean_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Expression_Parser_Node::Expression_Parser_Node(Boolean_Parser_Node_Ptr&& boolean)
  : m_boolean(std::move(boolean))
{
}

Expression_Parser_Node_Ptr
Expression_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto b = Boolean_Parser_Node::parse(input);
  if (b == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Expression_Parser_Node>(std::move(b));
}

void Expression_Parser_Node::dump(std::wostream& output) const
{
  m_boolean->dump(output);
}

void Expression_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  m_boolean->generate_cpp(state);
}

void Expression_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_boolean->collect_variables(state);
}
