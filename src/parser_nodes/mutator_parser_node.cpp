//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "mutator_parser_node.hpp"
#include "split_parser_node.hpp"
#include "cast_parser_node.hpp"
#include "join_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Mutator_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_split->dump(output);
}

void Mutator_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_cast->dump(output);
}

void Mutator_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_join->dump(output);
}

Mutator_Parser_Node::Mutator_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Mutator_Parser_Node::Mutator_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Mutator_Parser_Node::Mutator_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Mutator_Parser_Node_Ptr
Mutator_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_split = Split_Parser_Node::parse(input);
  if (choice.m_split == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Mutator_Parser_Node>(std::move(choice));
}

Mutator_Parser_Node_Ptr
Mutator_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_cast = Cast_Parser_Node::parse(input);
  if (choice.m_cast == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Mutator_Parser_Node>(std::move(choice));
}

Mutator_Parser_Node_Ptr
Mutator_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_3 choice;

  choice.m_join = Join_Parser_Node::parse(input);
  if (choice.m_join == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Mutator_Parser_Node>(std::move(choice));
}

Mutator_Parser_Node_Ptr
Mutator_Parser_Node::parse(Token_Stream& input)
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

  node = parse_choice_3(input);
  if (node) {
    return node;
  }

  input.seekg(offset);
  return nullptr;
}

void Mutator_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice );
}

void Mutator_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_split->generate_cpp(state);
}

void Mutator_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_cast->generate_cpp(state);
}

void Mutator_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_join->generate_cpp(state);
}

void Mutator_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice );
}
