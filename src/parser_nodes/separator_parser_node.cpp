//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "separator_parser_node.hpp"

#include "whitespace_parser_node.hpp"
#include "comment_parser_node.hpp"

#include "parser_node.hpp"

#include <istream>
#include <memory>
#include <vector>

void Separator_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  m_whitespace->dump(output);
}

void Separator_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  m_comment->dump(output);
}

void Separator_Parser_Node::Part::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice){ choice.dump(output); }, m_choice);
}

Separator_Parser_Node::Separator_Parser_Node(Part_Vector&& parts)
  : m_parts(std::move(parts))
{
}

Separator_Parser_Node::Part::Part(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Separator_Parser_Node::Part::Part(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Separator_Parser_Node::Part_Ptr
Separator_Parser_Node::parse_part_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_whitespace = Whitespace_Parser_Node::parse(input);
  if (choice.m_whitespace  == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Part>(std::move(choice));
}

Separator_Parser_Node::Part_Ptr
Separator_Parser_Node::parse_part_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_comment = Comment_Parser_Node::parse(input);
  if (choice.m_comment  == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Part>(std::move(choice));
}


Separator_Parser_Node_Ptr
Separator_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  Part_Vector parts;

  while (true) {
    auto tmp_offset = input.tellg();

    auto node = parse_part_choice_1(input);
    if (node) {
      parts.push_back(std::move(node));
      continue;
    }

    input.seekg(tmp_offset);

    node = parse_part_choice_2(input);
    if (node) {
      parts.push_back(std::move(node));
      continue;
    }

    input.seekg(tmp_offset);
    break;
  }

  if (parts.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Separator_Parser_Node>(std::move(parts));
}

void Separator_Parser_Node::dump(std::wostream& output) const
{
  for (const auto& p: m_parts) { p->dump(output); }
}
