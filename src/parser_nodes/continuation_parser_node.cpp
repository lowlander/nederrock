//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "continuation_parser_node.hpp"
#include "eol_parser_node.hpp"
#include "statement_parser_node.hpp"
#include "separator_parser_node.hpp"

#include "parser_node.hpp"

Continuation_Parser_Node::Continuation_Parser_Node(Eol_Parser_Node_Ptr&& eol,
                                                   Separator_Parser_Node_Vector&& separators,
                                                   Statement_Parser_Node_Ptr&& statement)
  : m_eol(std::move(eol))
  , m_separators(std::move(separators))
  , m_statement(std::move(statement))
{
}

Continuation_Parser_Node_Ptr
Continuation_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto eol = Eol_Parser_Node::parse(input);
  if (eol == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto separators = parse_zero_or_more<Separator_Parser_Node>(input);

  auto statement = Statement_Parser_Node::parse(input);
  if (statement == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Continuation_Parser_Node>(std::move(eol),
                                                    std::move(separators),
                                                    std::move(statement));
}

void Continuation_Parser_Node::dump(std::wostream& output) const
{
  m_eol->dump(output);
  for (const auto& s: m_separators) { s->dump(output); }
  m_statement->dump(output);
}

void Continuation_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  m_statement->generate_cpp(state);
}

void Continuation_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  m_statement->collect_variables(state);
}
