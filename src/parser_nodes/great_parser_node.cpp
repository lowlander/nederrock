//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "great_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Great_Parser_Node::Great_Parser_Node(std::wstring&& keyword)
  : m_keyword(keyword)
{
}

Great_Parser_Node_Ptr
Great_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"high", L"great", L"big", L"strong" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Great_Parser_Node>(std::move(keyword));
}

void Great_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}
