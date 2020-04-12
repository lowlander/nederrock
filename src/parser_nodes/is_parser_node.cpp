//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "is_parser_node.hpp"
#include "separator_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>

void Is_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Is_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  for (const auto& s: m_separators) { s->dump(output); }
  output << m_keyword;
}

Is_Parser_Node::Is_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Is_Parser_Node::Is_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Is_Parser_Node_Ptr
Is_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_keyword = check_and_remove(input, L"'s");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Is_Parser_Node>(std::move(choice));
}

Is_Parser_Node_Ptr
Is_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_separators = parse_zero_or_more<Separator_Parser_Node>(input);

  choice.m_keyword = check_and_remove(input, { L"=", L"is ", L"was ", L"are ", L"were " } );
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Is_Parser_Node>(std::move(choice));
}

Is_Parser_Node_Ptr
Is_Parser_Node::parse(Token_Stream& input)
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

void Is_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice );
}
