//
// Copyright (c) 2020 Erwin Rol <erwin@erwinrol.com>
//
// SPDX-License-Identifier: MIT
//

#ifndef NEDERROCK_SRC_SEPARATOR_PARSER_NODE_HPP
#define NEDERROCK_SRC_SEPARATOR_PARSER_NODE_HPP

#include "separator_parser_node_pre.hpp"

#include "whitespace_parser_node_pre.hpp"
#include "comment_parser_node_pre.hpp"
#include "token_stream.hpp"

#include <istream>
#include <variant>

class Separator_Parser_Node {
public:
  struct Choice_1 {
    void dump(std::wostream& output) const;

    Whitespace_Parser_Node_Ptr m_whitespace;
  };

  struct Choice_2 {
    void dump(std::wostream& output) const;

    Comment_Parser_Node_Ptr m_comment;
  };

  struct Part {
    Part(Choice_1&& choice);
    Part(Choice_2&& choice);
    void dump(std::wostream& output) const;

    std::variant<Choice_1, Choice_2> m_choice;
  };

  using Part_Ptr = std::shared_ptr<Part>;
  using Part_Vector = std::vector<Part_Ptr>;
public:
  Separator_Parser_Node(Part_Vector&& parts);

  static Separator_Parser_Node_Ptr parse(Token_Stream& input);
  static Part_Ptr parse_part_choice_1(Token_Stream& input);
  static Part_Ptr parse_part_choice_2(Token_Stream& input);

  void dump(std::wostream& output) const;
private:
  Part_Vector m_parts;
};

#endif // NEDERROCK_SRC_SEPARATOR_PARSER_NODE_HPP
