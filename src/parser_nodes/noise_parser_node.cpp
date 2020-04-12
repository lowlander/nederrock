//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "noise_parser_node.hpp"
#include "separator_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

void Noise_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_separator->dump(output);
}

void Noise_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_char;
}

Noise_Parser_Node::Noise_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Noise_Parser_Node::Noise_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Noise_Parser_Node_Ptr
Noise_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_separator = Separator_Parser_Node::parse(input);
  if (choice.m_separator == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Noise_Parser_Node>(std::move(choice));
}

Noise_Parser_Node_Ptr
Noise_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_char = static_cast<wchar_t>(input.get());
  switch(choice.m_char) {
    case L';':
    case L',':
    case L'?':
    case L'!':
    case L'&':
    case L'.':
    break;

  default:
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Noise_Parser_Node>(std::move(choice));
}

Noise_Parser_Node_Ptr
Noise_Parser_Node::parse(Token_Stream& input)
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

void Noise_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}
