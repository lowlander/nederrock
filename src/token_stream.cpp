//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#include "token_stream.hpp"

#include <string_view>
#include <fstream>
#include <streambuf>
#include <iostream>

Token_Stream::Token_Stream(const std::string& filename)
{
  std::wifstream input(filename, std::ios::binary);
  input.imbue(std::locale("en_US.UTF-8"));

  std::wstringstream buf;
  buf << input.rdbuf();

  m_buffer = buf.str();
  m_buffer += L"\n\n";
}

Token_Stream::int_type Token_Stream::get()
{
  if (m_offset >= m_buffer.size()) {
    return WEOF;
  }  else {
    auto c = m_buffer[m_offset++];
    return static_cast<int_type>(c);
  }
}

Token_Stream::int_type Token_Stream::peek()
{
  if (m_offset >= m_buffer.size()) {
    return WEOF;
  }  else {
    auto c = m_buffer[m_offset];
    return static_cast<int_type>(c);
  }
}

size_t Token_Stream::tellg()
{
  return m_offset;
}

void Token_Stream::seekg(size_t offset)
{
  m_offset = offset;
}

bool Token_Stream::eof() const
{
  if (m_offset >= m_buffer.size()) {
    return true;
  } else {
    return false;
  }
}
