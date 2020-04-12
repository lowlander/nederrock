//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "boolean_parser_node.hpp"
#include "nor_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Boolean_Parser_Node::Boolean_Parser_Node(Nor_Parser_Node_Ptr&& nor)
  : m_nor(std::move(nor))
{
}

Boolean_Parser_Node_Ptr
Boolean_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto nor = Nor_Parser_Node::parse(input);
  if (nor == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Boolean_Parser_Node>(std::move(nor));
}

void Boolean_Parser_Node::dump(std::wostream& output) const
{
  m_nor->dump(output);
}

void Boolean_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  m_nor->generate_cpp(state);
}

void Boolean_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_nor->collect_variables(state);
}
