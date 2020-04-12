//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "return_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>

Return_Parser_Node::Return_Parser_Node(std::wstring&& keyword)
  : m_keyword(std::move(keyword))
{
}

Return_Parser_Node_Ptr Return_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"return" , L"give back" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Return_Parser_Node>(std::move(keyword));
}

void Return_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}
