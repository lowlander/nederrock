//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "consequent_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "statement_parser_node.hpp"
#include "eol_parser_node.hpp"
#include "block_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

void Consequent_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_separator->dump(output);
  m_statement->dump(output);
}

void Consequent_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_eol->dump(output);
  m_block->dump(output);
}

Consequent_Parser_Node::Consequent_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Consequent_Parser_Node::Consequent_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Consequent_Parser_Node_Ptr
Consequent_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_statement = Statement_Parser_Node::parse(input);
  if (choice.m_statement == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Consequent_Parser_Node>(std::move(choice));
}

Consequent_Parser_Node_Ptr
Consequent_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_eol = Eol_Parser_Node::parse(input);
  if (choice.m_eol == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_block = Block_Parser_Node::parse(input);
  if (choice.m_block == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Consequent_Parser_Node>(std::move(choice));
}

Consequent_Parser_Node_Ptr
Consequent_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto node = parse_choice_1(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  node = parse_choice_2(input);
  if (node) {
    return node;
  }

  input.seekg(offset);

  return nullptr;
}

void Consequent_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Consequent_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_statement->generate_cpp(state);
}

void Consequent_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_block->generate_cpp(state);
}

void Consequent_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}

void Consequent_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_statement->collect_variables(state);
}

void Consequent_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_block->collect_variables(state);
}

void Consequent_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.collect_variables(state); }, m_choice);
}
