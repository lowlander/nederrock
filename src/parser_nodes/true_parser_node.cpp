//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "true_parser_node.hpp"
#include "letter_parser_node.hpp"

#include "parser_node.hpp"

True_Parser_Node::True_Parser_Node(std::wstring&& keyword)
  : m_keyword(keyword)
{
}

True_Parser_Node_Ptr
True_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"true", L"ok", L"right", L"yes" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto letter = Letter_Parser_Node::parse(input);
  if (letter) {
    // no letter allowed to follow
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<True_Parser_Node>(std::move(keyword));
}

void True_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

void True_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::Variable(true)");
}
