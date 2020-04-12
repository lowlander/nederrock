//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "false_parser_node.hpp"
#include "letter_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

False_Parser_Node::False_Parser_Node(std::wstring&& keyword)
  : m_keyword(keyword)
{
}

False_Parser_Node_Ptr
False_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();
  auto keyword = check_and_remove(input, { L"false", L"lies", L"wrong", L"no" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  auto letter = Letter_Parser_Node::parse(input);
  if (letter) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<False_Parser_Node>(std::move(keyword));
}

void False_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

void False_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::Variable(false)");
}
