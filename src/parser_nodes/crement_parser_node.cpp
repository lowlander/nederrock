//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "crement_parser_node.hpp"
#include "increment_parser_node.hpp"
#include "decrement_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Crement_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_increment->dump(output);
}

void Crement_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_decrement->dump(output);
}

Crement_Parser_Node::Crement_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Crement_Parser_Node::Crement_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Crement_Parser_Node_Ptr
Crement_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_increment = Increment_Parser_Node::parse(input);
  if (choice.m_increment == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Crement_Parser_Node>(std::move(choice));
}

Crement_Parser_Node_Ptr
Crement_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_decrement = Decrement_Parser_Node::parse(input);
  if (choice.m_decrement == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Crement_Parser_Node>(std::move(choice));
}

Crement_Parser_Node_Ptr
Crement_Parser_Node::parse(Token_Stream& input)
{
  auto offset =input.tellg();

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

void Crement_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice );
}

void Crement_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_increment->generate_cpp(state);
}

void Crement_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_decrement->generate_cpp(state);
}

void Crement_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice );
}

void Crement_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_increment->collect_variables(state);
}

void Crement_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_decrement->collect_variables(state);
}

void Crement_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.collect_variables(state); }, m_choice );
}
