//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_PROPER_VARIABLE_PARSER_NODE_HPP
#define NEDERROCK_SRC_PROPER_VARIABLE_PARSER_NODE_HPP

#include "proper_variable_parser_node_pre.hpp"
#include "proper_noun_parser_node_pre.hpp"
#include "separator_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <memory>
#include <istream>
#include <vector>

class Proper_Variable_Parser_Node {
public:
  struct Noun {
    void dump(std::wostream& output) const;
    std::wstring to_string() const;

    wchar_t                       m_char;
    Proper_Noun_Parser_Node_Ptr   m_proper_noun;
  };

  using Noun_Vector = std::vector<Noun>;
public:
  Proper_Variable_Parser_Node(Proper_Noun_Parser_Node_Ptr&& proper_noun,
                              Noun_Vector&& nouns);

  static Proper_Variable_Parser_Node_Ptr parse(Token_Stream& input);

  void dump(std::wostream& output) const;

  std::wstring to_string();
private:
  Proper_Noun_Parser_Node_Ptr   m_proper_noun;
  Noun_Vector                   m_nouns;
};

#endif // NEDERROCK_SRC_PROPER_VARIABLE_PARSER_NODE_HPP
