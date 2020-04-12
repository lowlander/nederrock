//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "small_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Small_Parser_Node::Small_Parser_Node(std::wstring&& keyword)
  : m_keyword(std::move(keyword))
{
}

Small_Parser_Node_Ptr
Small_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"low", L"small", L"weak" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Small_Parser_Node>(std::move(keyword));
}

void Small_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}
