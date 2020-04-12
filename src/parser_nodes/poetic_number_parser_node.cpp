//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "poetic_number_parser_node.hpp"
#include "poetic_digit_separator_parser_node.hpp"
#include "poetic_digits_parser_node.hpp"
#include "poetic_decimal_parser_node.hpp"

#include "parser_node.hpp"

Poetic_Number_Parser_Node::Poetic_Number_Parser_Node(
      Poetic_Digit_Separator_Parser_Node_Vector&& poetic_digit_separators_1,
      Poetic_Digits_Parser_Node_Ptr&&             poetic_digits,
      Poetic_Digit_Separator_Parser_Node_Vector&& poetic_digit_separators_2,
      Poetic_Decimal_Parser_Node_Ptr&&            poetic_decimal,
      Poetic_Digit_Separator_Parser_Node_Vector&& poetic_digit_separators_3)
  : m_poetic_digit_separators_1(std::move(poetic_digit_separators_1))
  , m_poetic_digits(std::move(poetic_digits))
  , m_poetic_digit_separators_2(std::move(poetic_digit_separators_2))
  , m_poetic_decimal(std::move(poetic_decimal))
  , m_poetic_digit_separators_3(std::move(poetic_digit_separators_3))
{
}

Poetic_Number_Parser_Node_Ptr
Poetic_Number_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto separators_1 = parse_zero_or_more<Poetic_Digit_Separator_Parser_Node>(input);

  auto poetic_digits = Poetic_Digits_Parser_Node::parse(input);
  if (poetic_digits == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  auto separators_2 = parse_zero_or_more<Poetic_Digit_Separator_Parser_Node>(input);

  auto poetic_decimal = Poetic_Decimal_Parser_Node::parse(input);

  auto separators_3 = parse_zero_or_more<Poetic_Digit_Separator_Parser_Node>(input);

  return std::make_shared<Poetic_Number_Parser_Node>(std::move(separators_1),
                                                     std::move(poetic_digits),
                                                     std::move(separators_2),
                                                     std::move(poetic_decimal),
                                                     std::move(separators_3));
}

void Poetic_Number_Parser_Node::dump(std::wostream& output) const
{
  for (const auto& s: m_poetic_digit_separators_1) { s->dump(output); }
  m_poetic_digits->dump(output);
  for (const auto& s: m_poetic_digit_separators_2) { s->dump(output); }
  if (m_poetic_decimal) { m_poetic_decimal->dump(output); }
  for (const auto& s: m_poetic_digit_separators_3) { s->dump(output); }
}

std::wstring Poetic_Number_Parser_Node::to_string() const
{
  std::wstring res;

  res += m_poetic_digits->to_string();

  if (m_poetic_decimal) res += m_poetic_decimal->to_string();

  return res;
}

void Poetic_Number_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  const auto value = to_string();

  state->add_code(L"rockstar::Variable(L\"");
  state->add_code(value);
  state->add_code(L"\",");
  state->add_code(value);
  state->add_code(L")");
}
