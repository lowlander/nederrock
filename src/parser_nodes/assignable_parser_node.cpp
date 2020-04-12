//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "assignable_parser_node.hpp"
#include "variable_parser_node.hpp"
#include "indexer_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Assignable_Parser_Node::Assignable_Parser_Node(Variable_Parser_Node_Ptr&& variable,
                                               Indexer_Parser_Node_Ptr&& indexer)
  : m_variable(std::move(variable))
  , m_indexer(std::move(indexer))
{
}

Assignable_Parser_Node_Ptr
Assignable_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto variable = Variable_Parser_Node::parse(input);
  if (variable == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto indexer = Indexer_Parser_Node::parse(input);

  return std::make_shared<Assignable_Parser_Node>(std::move(variable), std::move(indexer));
}

void Assignable_Parser_Node::dump(std::wostream& output) const
{
  m_variable->dump(output);
  if (m_indexer) m_indexer->dump(output);
}

void Assignable_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  m_variable->generate_cpp(state);
  const auto name = m_variable->get_name();
  if (!name.empty()){
    state->set_last_variable(name);
  }
  if (m_indexer) m_indexer->generate_cpp(state);
}

void Assignable_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_variable->collect_variables(state);
  if (m_indexer) m_indexer->collect_variables(state);
}
