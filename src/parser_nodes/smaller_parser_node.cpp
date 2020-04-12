//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "smaller_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Smaller_Parser_Node::Smaller_Parser_Node(std::wstring&& keyword)
  : m_keyword(keyword)
{
}

Smaller_Parser_Node_Ptr
Smaller_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"lower", L"less", L"smaller", L"weaker" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Smaller_Parser_Node>(std::move(keyword));
}

void Smaller_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}
