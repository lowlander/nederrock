//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "poetic_decimal_digit_separator_parser_node.hpp"
#include "separator_parser_node.hpp"
#include "poetic_digit_separator_parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Poetic_Decimal_Digit_Separator_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_separator->dump(output);
}

void Poetic_Decimal_Digit_Separator_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_poetic_digit_separator->dump(output);
}

void Poetic_Decimal_Digit_Separator_Parser_Node::Choice_3::dump(std::wostream& output) const
{
  output << m_char;
}

Poetic_Decimal_Digit_Separator_Parser_Node::Poetic_Decimal_Digit_Separator_Parser_Node(Choice_1&& choice)
 : m_choice(std::move(choice))
{
}

Poetic_Decimal_Digit_Separator_Parser_Node::Poetic_Decimal_Digit_Separator_Parser_Node(Choice_2&& choice)
 : m_choice(std::move(choice))
{
}

Poetic_Decimal_Digit_Separator_Parser_Node::Poetic_Decimal_Digit_Separator_Parser_Node(Choice_3&& choice)
 : m_choice(std::move(choice))
{
}

Poetic_Decimal_Digit_Separator_Parser_Node_Ptr
Poetic_Decimal_Digit_Separator_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Poetic_Decimal_Digit_Separator_Parser_Node>(std::move(choice));
}

Poetic_Decimal_Digit_Separator_Parser_Node_Ptr
Poetic_Decimal_Digit_Separator_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_poetic_digit_separator = Poetic_Digit_Separator_Parser_Node::parse(input);
  if (choice.m_poetic_digit_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Poetic_Decimal_Digit_Separator_Parser_Node>(std::move(choice));
}

Poetic_Decimal_Digit_Separator_Parser_Node_Ptr
Poetic_Decimal_Digit_Separator_Parser_Node::parse_choice_3(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_3 choice;

  choice.m_char = static_cast<wchar_t>(input.get());
  if (choice.m_char != L'.') {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Poetic_Decimal_Digit_Separator_Parser_Node>(std::move(choice));
}

Poetic_Decimal_Digit_Separator_Parser_Node_Ptr
Poetic_Decimal_Digit_Separator_Parser_Node::parse(Token_Stream& input)
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

  node = parse_choice_3(input);
  if (node) {
    return node;
  }

  input.seekg(offset);
  return nullptr;
}

void Poetic_Decimal_Digit_Separator_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice);
}
