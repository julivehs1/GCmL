//
// Created by juliv on 20.01.2023.
//

#ifndef GCML_HIERARCHYPRINTER_H
#define GCML_HIERARCHYPRINTER_H

#include <string>
#include <vector>

typedef struct {
    int level;
    std::string name;
} Entry;

class HierarchyPrinter {
public:
    void entry(std::string name);
    void leave();
    void print();
private:
    static std::string createIndentation(int n);
    static std::string createSpacing(std::vector<std::string> spacing, char corner);

    bool is_last_of_level (int index);

    int m_indentation = 0;
    std::vector<Entry> m_entries;

};


#endif //GCML_HIERARCHYPRINTER_H
