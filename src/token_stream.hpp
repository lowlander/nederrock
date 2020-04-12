//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_TOKEN_STREAM_HPP
#define NEDERROCK_SRC_TOKEN_STREAM_HPP

#include <string_view>
#include <string>
#include <sstream>

class Token_Stream {
public:
  using int_type =std::wstringstream::int_type;
public:
  Token_Stream(const std::string& filename);

  int_type get();
  int_type peek();

  size_t tellg();
  void seekg(size_t offset);

  bool eof() const;
private:
  size_t m_offset{};
  std::wstring m_buffer;
};

#endif // NEDERROCK_SRC_TOKEN_STREAM_HPP
