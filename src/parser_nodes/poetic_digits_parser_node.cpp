//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "poetic_digits_parser_node.hpp"
#include "poetic_digit_separator_parser_node.hpp"
#include "poetic_digit_parser_node.hpp"

#include "parser_node.hpp"

void Poetic_Digits_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  for (const auto& p: m_poetic_digit_separator_1) { p->dump(output); }
  m_poetic_digit->dump(output);
  for (const auto& p: m_poetic_digit_separator_2) { p->dump(output); }
  m_poetic_digits->dump(output);
}

void Poetic_Digits_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_poetic_digit->dump(output);
}

Poetic_Digits_Parser_Node::Poetic_Digits_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Poetic_Digits_Parser_Node::Poetic_Digits_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Poetic_Digits_Parser_Node_Ptr
Poetic_Digits_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_poetic_digit_separator_1 = parse_zero_or_more<Poetic_Digit_Separator_Parser_Node>(input);

  choice.m_poetic_digit = Poetic_Digit_Parser_Node::parse(input);
  if (choice.m_poetic_digit == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_poetic_digit_separator_2 = parse_zero_or_more<Poetic_Digit_Separator_Parser_Node>(input);
  if (choice.m_poetic_digit_separator_2.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_poetic_digits = Poetic_Digits_Parser_Node::parse(input);
  if (choice.m_poetic_digits == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Poetic_Digits_Parser_Node>(std::move(choice));
}

Poetic_Digits_Parser_Node_Ptr
Poetic_Digits_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_poetic_digit = Poetic_Digit_Parser_Node::parse(input);
  if (choice.m_poetic_digit == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Poetic_Digits_Parser_Node>(std::move(choice));
}

Poetic_Digits_Parser_Node_Ptr
Poetic_Digits_Parser_Node::parse(Token_Stream& input)
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

void Poetic_Digits_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice );
}

std::wstring Poetic_Digits_Parser_Node::Choice_1::to_string() const
{
  std::wstring res;

  res += m_poetic_digit->to_string();
  res += m_poetic_digits->to_string();

  return res;
}

std::wstring Poetic_Digits_Parser_Node::Choice_2::to_string() const
{
  return m_poetic_digit->to_string();
}

std::wstring Poetic_Digits_Parser_Node::to_string() const
{
  return std::visit([](auto&& choice) { return choice.to_string(); }, m_choice );
}
