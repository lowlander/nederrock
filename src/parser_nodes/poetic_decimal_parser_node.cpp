//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "poetic_decimal_parser_node.hpp"
#include "poetic_decimal_digit_separator_parser_node.hpp"
#include "poetic_decimal_digits_parser_node.hpp"

#include "parser_node.hpp"

void Poetic_Decimal_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_char;
  for (const auto& p: m_poetic_decimal_digit_separators_1) { p->dump(output); }
  m_poetic_decimal_digits->dump(output);
  for (const auto& p: m_poetic_decimal_digit_separators_2) { p->dump(output); }
}

void Poetic_Decimal_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_char;
  for (const auto& p: m_poetic_decimal_digit_separators) { p->dump(output); }
}

Poetic_Decimal_Parser_Node::Poetic_Decimal_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Poetic_Decimal_Parser_Node::Poetic_Decimal_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Poetic_Decimal_Parser_Node_Ptr
Poetic_Decimal_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_char = static_cast<wchar_t>(input.get());
  if (choice.m_char != L'.') {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_poetic_decimal_digit_separators_1 =
    parse_zero_or_more<Poetic_Decimal_Digit_Separator_Parser_Node>(input);

  choice.m_poetic_decimal_digits = Poetic_Decimal_Digits_Parser_Node::parse(input);
  if (choice.m_poetic_decimal_digits == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_poetic_decimal_digit_separators_2 =
    parse_zero_or_more<Poetic_Decimal_Digit_Separator_Parser_Node>(input);

  return std::make_shared<Poetic_Decimal_Parser_Node>(std::move(choice));
}

Poetic_Decimal_Parser_Node_Ptr
Poetic_Decimal_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_char = static_cast<wchar_t>(input.get());
  if (choice.m_char != L'.') {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_poetic_decimal_digit_separators =
    parse_zero_or_more<Poetic_Decimal_Digit_Separator_Parser_Node>(input);

  return std::make_shared<Poetic_Decimal_Parser_Node>(std::move(choice));
}

Poetic_Decimal_Parser_Node_Ptr
Poetic_Decimal_Parser_Node::parse(Token_Stream& input)
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

void Poetic_Decimal_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

std::wstring Poetic_Decimal_Parser_Node::Choice_1::to_string() const
{
  std::wstring res;

  res += L".";
  res += m_poetic_decimal_digits->to_string();

  return res;
}

std::wstring Poetic_Decimal_Parser_Node::Choice_2::to_string() const
{
  return L".";
}

std::wstring Poetic_Decimal_Parser_Node::to_string() const
{
  return std::visit([](auto&& choice) { return choice.to_string(); }, m_choice);
}
