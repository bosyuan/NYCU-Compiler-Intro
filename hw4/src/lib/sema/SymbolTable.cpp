#include "sema/SemanticAnalyzer.hpp"

void dumpDemarcation(const char chr) {
  for (size_t i = 0; i < 110; ++i) {
    printf("%c", chr);
  }
  puts("");
}

bool SymbolTable::lookup(const char* name) {
    for (int i = 0; i < static_cast<int>(entries.size()); i++) {
        if (strcmp(entries[i].name, name) == 0) {
            return true;
        }
    }
    return false;
}

void SymbolTable::addSymbol(const char* name, const char* kind, int level, PType* type, const char* attribute) {
    SymbolEntry new_entry;
    new_entry.name = name;
    new_entry.kind = kind;
    new_entry.level = level;
    new_entry.type = type;
    new_entry.attribute = attribute;
    entries.push_back(new_entry);
}

SymbolEntry* SymbolTable::lookup_local(const char* name) {
    for (int i = 0; i < static_cast<int>(entries.size()); i++) {
        if (strcmp(entries[i].name, name) == 0) {
            return &entries[i];
        }
    }
    return nullptr;
}

void SymbolManager::pushScope(SymbolTable *new_scope) {
    tables.push(new_scope);
}

void SymbolManager::popScope() {
    tables.pop();
}

void SymbolManager::printCurrentScope() {
    if (!print_table) {
        return;
    }
    SymbolTable *current_table = tables.top();
    dumpDemarcation('=');
    printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type",
                                        "Attribute");
    dumpDemarcation('-');
    for (int i = 0; i < static_cast<int>(current_table->entries.size()); i++) {
        printf("%-33s", current_table->entries[i].name);
        printf("%-11s", current_table->entries[i].kind);
        std::string scoope = (current_table->entries[i].level == 0) ? "(global)" : "(local)";
        printf("%d%-10s", current_table->entries[i].level, scoope.c_str());
        std::string type_str = current_table->entries[i].type->getPTypeCString();
        printf("%-17s", type_str.c_str());
        printf("%-11s", current_table->entries[i].attribute);
        puts("");
    }
    dumpDemarcation('-');
}

void SymbolManager::pushSymbol(const char* name, const char* kind, int level, PType* type, const char* attribute) {
    SymbolTable *current_table = tables.top();
    current_table->addSymbol(name, kind, level, type, attribute);
}

void SymbolManager::set_print_table(bool print) {
    print_table = print;
}

SymbolEntry* SymbolManager::lookup_global(const char* name) {
    std::stack<SymbolTable *> temp;
    SymbolEntry* result = nullptr;
    while(!tables.empty()) {
        SymbolTable *current_table = tables.top();
        result = current_table->lookup_local(name);
        if (result != nullptr) {
            break;
        }
        temp.push(current_table);
        tables.pop();
    }
    while(!temp.empty()) {
        tables.push(temp.top());
        temp.pop();
    }
    return result;
}