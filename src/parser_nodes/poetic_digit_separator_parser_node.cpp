//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "poetic_digit_separator_parser_node.hpp"
#include "separator_parser_node.hpp"

#include "parser_node.hpp"

void Poetic_Digit_Separator_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_separator->dump(output);
}

void Poetic_Digit_Separator_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_char;
}

Poetic_Digit_Separator_Parser_Node::Poetic_Digit_Separator_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Poetic_Digit_Separator_Parser_Node::Poetic_Digit_Separator_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Poetic_Digit_Separator_Parser_Node_Ptr
Poetic_Digit_Separator_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Poetic_Digit_Separator_Parser_Node>(std::move(choice));
}

Poetic_Digit_Separator_Parser_Node_Ptr
Poetic_Digit_Separator_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_char = static_cast<wchar_t>(input.get());
  switch(choice.m_char) {
    // 0-9\',;:?!+_/
    case L'0':
    case L'1':
    case L'2':
    case L'3':
    case L'4':
    case L'5':
    case L'6':
    case L'7':
    case L'8':
    case L'9':
    case L'\'':
    case L',':
    case L';':
    case L':':
    case L'?':
    case L'!':
    case L'+':
    case L'_':
    case L'/':
      break;

    default:
      input.seekg(offset);
      return nullptr;
  }

  return std::make_shared<Poetic_Digit_Separator_Parser_Node>(std::move(choice));
}

Poetic_Digit_Separator_Parser_Node_Ptr
Poetic_Digit_Separator_Parser_Node::parse(Token_Stream& input)
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

void Poetic_Digit_Separator_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output);}, m_choice);
}
