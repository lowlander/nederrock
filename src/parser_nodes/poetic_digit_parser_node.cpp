//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "poetic_digit_parser_node.hpp"

#include "parser_node.hpp"

Poetic_Digit_Parser_Node::Poetic_Digit_Parser_Node(std::wstring&& text)
  : m_text(std::move(text))
{
}

Poetic_Digit_Parser_Node_Ptr
Poetic_Digit_Parser_Node::parse(Token_Stream& input)
{
  auto offset = input.tellg();

  std::wstring text;

  while(true) {
    auto tmp_offset = input.tellg();

    auto c = static_cast<wchar_t>(input.get());

    if ((c >= L'A' && c <= L'Z') || (c >= L'a' && c <= L'z') || c == L'\\' || c == L'-' || c == L'\'') {
      text += c;
    } else {
      input.seekg(tmp_offset);
      break;
    }
  }

  if (text.empty()) {
    input.seekg(offset);
    return nullptr;
  }

  return std::make_shared<Poetic_Digit_Parser_Node>(std::move(text));
}

void Poetic_Digit_Parser_Node::dump(std::wostream& output) const
{
  output << m_text;
}

std::wstring Poetic_Digit_Parser_Node::to_string() const
{
  auto res = m_text;
  res.erase (std::remove(res.begin(), res.end(), L'\''), res.end());
  const auto n = res.size() % 10;
  return std::to_wstring(n);
}
