//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "continue_parser_node.hpp"
#include "ignore_rest_of_line_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <variant>
#include <string>

void Continue_Parser_Node::Choice_1::dump(std::wostream& output) const
{
  output << m_keyword;
  m_ignore_rest_of_line->dump(output);
}

void Continue_Parser_Node::Choice_2::dump(std::wostream& output) const
{
  output << m_keyword;
}

Continue_Parser_Node::Continue_Parser_Node(Choice_1&& choice)
  : m_choice(std::move(choice))
{
}

Continue_Parser_Node::Continue_Parser_Node(Choice_2&& choice)
  : m_choice(std::move(choice))
{
}

Continue_Parser_Node_Ptr
Continue_Parser_Node::parse_choice_1(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_1 choice;

  choice.m_keyword = check_and_remove(input, L"continue");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  choice.m_ignore_rest_of_line = Ignore_Rest_Of_Line_Parser_Node::parse(input);
  if (choice.m_ignore_rest_of_line == nullptr) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Continue_Parser_Node>(std::move(choice));
}

Continue_Parser_Node_Ptr
Continue_Parser_Node::parse_choice_2(Token_Stream& input)
{
  auto offset = input.tellg();

  Choice_2 choice;

  choice.m_keyword = check_and_remove(input, L"take it to the top");
  if (choice.m_keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Continue_Parser_Node>(std::move(choice));
}

Continue_Parser_Node_Ptr
Continue_Parser_Node::parse(Token_Stream& input)
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

void Continue_Parser_Node::dump(std::wostream& output) const
{
  std::visit([&output](auto&& choice) { choice.dump(output); }, m_choice);
}

void Continue_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"continue");
}
