//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "cast_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Cast_Parser_Node::Cast_Parser_Node(std::wstring&& keyword)
  : m_keyword(std::move(keyword))
{
}

Cast_Parser_Node_Ptr
Cast_Parser_Node::Cast_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  auto keyword = check_and_remove(input, { L"cast", L"burn" });
  if (keyword.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Cast_Parser_Node>(std::move(keyword));
}

void Cast_Parser_Node::dump(std::wostream& output) const
{
  output << m_keyword;
}

void Cast_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"cast");
}
