//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "mysterious_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Mysterious_Parser_Node::Mysterious_Parser_Node(std::wstring&& keyword)
  : m_keyword(keyword)
{
}

Mysterious_Parser_Node_Ptr
Mysterious_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();
  auto keyword = check_and_remove(input, L"mysterious" );
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Mysterious_Parser_Node>(std::move(keyword));
}

void Mysterious_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Mysterious_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::Variable()");
}
