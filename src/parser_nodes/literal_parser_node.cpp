//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "literal_parser_node.hpp"
#include "constant_parser_node.hpp"
#include "number_parser_node.hpp"
#include "string_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Literal_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_constant->dump(output);
}

void Literal_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_number->dump(output);
}

void Literal_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  m_string->dump(output);
}

Literal_Parser_Node::Literal_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Literal_Parser_Node::Literal_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Literal_Parser_Node::Literal_Parser_Node(Choice_3&& choice)
  : m_choice(std::move(choice))
{
}

Literal_Parser_Node_Ptr
Literal_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_1 choice;

  choice.m_constant = Constant_Parser_Node::parse(input);
  if (choice.m_constant == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Literal_Parser_Node>(std::move(choice));
}

Literal_Parser_Node_Ptr
Literal_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_2 choice;

  choice.m_number = Number_Parser_Node::parse(input);
  if (choice.m_number == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Literal_Parser_Node>(std::move(choice));
}

Literal_Parser_Node_Ptr
Literal_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset =input.tellg();

  Choice_3 choice;

  choice.m_string = String_Parser_Node::parse(input);
  if (choice.m_string == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Literal_Parser_Node>(std::move(choice));
}

Literal_Parser_Node_Ptr
Literal_Parser_Node::parse(Token_Stream& input)
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

void Literal_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice );
}

void Literal_Parser_Node::Choice_1::generate_cpp(Scope_State_Ptr state) const
{
  m_constant->generate_cpp(state);
}

void Literal_Parser_Node::Choice_2::generate_cpp(Scope_State_Ptr state) const
{
  m_number->generate_cpp(state);
}

void Literal_Parser_Node::Choice_3::generate_cpp(Scope_State_Ptr state) const
{
  m_string->generate_cpp(state);
}

void Literal_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  std::visit([&state](auto&& choice){ choice.generate_cpp(state); }, m_choice );
}
