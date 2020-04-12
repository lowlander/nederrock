//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "eq_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Eq_Parser_Node::Eq_Parser_Node(Choice_1&& choice)
  : m_choice(choice)
{
}

Eq_Parser_Node::Eq_Parser_Node(Choice_2&& choice)
  : m_choice(choice)
{
}

Eq_Parser_Node_Ptr
Eq_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_keyword = check_and_remove(input, { L"aint", L"ain't" } );
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Eq_Parser_Node>(std::move(choice));
}

Eq_Parser_Node_Ptr
Eq_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_keyword = check_and_remove(input, L"is" );
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Eq_Parser_Node>(std::move(choice));
}

Eq_Parser_Node_Ptr
Eq_Parser_Node::parse(Token_Stream& input)
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

void Eq_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Eq_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Eq_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Eq_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"!=");
}

void Eq_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"==");
}

void Eq_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice) { choice.generate_cpp(state); }, m_choice);
}
