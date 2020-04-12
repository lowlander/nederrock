//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "rounding_parser_node.hpp"
#include "floor_parser_node.hpp"
#include "ceil_parser_node.hpp"
#include "math_round_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Rounding_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_floor->dump(output);
}

void Rounding_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_ceil->dump(output);
}

void Rounding_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_math_round->dump(output);
}

Rounding_Parser_Node::Rounding_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Rounding_Parser_Node::Rounding_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Rounding_Parser_Node::Rounding_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Rounding_Parser_Node_Ptr
Rounding_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_floor = Floor_Parser_Node::parse(input);
  if (choice.m_floor == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Rounding_Parser_Node>(std::move(choice));
}

Rounding_Parser_Node_Ptr
Rounding_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_ceil = Ceil_Parser_Node::parse(input);
  if (choice.m_ceil == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Rounding_Parser_Node>(std::move(choice));
}

Rounding_Parser_Node_Ptr
Rounding_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_3 choice;

  choice.m_math_round = Math_Round_Parser_Node::parse(input);
  if (choice.m_math_round == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Rounding_Parser_Node>(std::move(choice));
}

Rounding_Parser_Node_Ptr
Rounding_Parser_Node::parse(Token_Stream& input)
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

void Rounding_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice );
}


void Rounding_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_floor->generate_cpp(state);
}

void Rounding_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_ceil->generate_cpp(state);
}

void Rounding_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_math_round->generate_cpp(state);
}

void Rounding_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice );
}


void Rounding_Parser_Node::Choice_1::collect_variables(Scope_State_Ptr state) const
{
  m_floor->collect_variables(state);
}

void Rounding_Parser_Node::Choice_2::collect_variables(Scope_State_Ptr state) const
{
  m_ceil->collect_variables(state);
}

void Rounding_Parser_Node::Choice_3::collect_variables(Scope_State_Ptr state) const
{
  m_math_round->collect_variables(state);
}

void Rounding_Parser_Node::collect_variables(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.collect_variables(state); }, m_choice );
}
