#ifndef __AST_UTILS_H
#define __AST_UTILS_H

enum Ptype {PInt, PReal, PString, PBoolean, PVoid};

class StringNode{
  public:
    StringNode(const uint32_t line, const uint32_t col, std::string p_name) {
        this->line = line;
        this->col = col;
        this->name = p_name;
    };
    StringNode(const uint32_t line, const uint32_t col, const char* p_name) {
        this->line = line;
        this->col = col;
        this->name = p_name;
    };
    ~StringNode() = default;
    
    uint32_t line;
    uint32_t col;
    std::string name;
};
#endif