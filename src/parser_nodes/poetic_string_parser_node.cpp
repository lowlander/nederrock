//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "poetic_string_parser_node.hpp"

#include "parser_node.hpp"

#include <memory>
#include <istream>
#include <string>

Poetic_String_Parser_Node::Poetic_String_Parser_Node(std::wstring&& text)
  : m_text(std::move(text))
{
}

Poetic_String_Parser_Node_Ptr
Poetic_String_Parser_Node::parse(Token_Stream& input)
{
  std::wstring text;

  while(true) {
    auto tmp_offset = input.tellg();
    auto c = input.get();

    if (c == L'\n') {
      input.seekg(tmp_offset);
      break;
    }

    text += static_cast<wchar_t>(c);
  }

  return std::make_shared<Poetic_String_Parser_Node>(std::move(text));
}

void Poetic_String_Parser_Node::dump(std::wostream& output) const
{
  output << m_text;
}

void Poetic_String_Parser_Node::generate_cpp(Scope_State_Ptr state) const
{
  state->add_code(L"rockstar::Variable(L\"");
  state->add_code(m_text);
  state->add_code(L"\")");
}
