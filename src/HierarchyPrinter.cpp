//
// Created by juliv on 20.01.2023.
//

#include "HierarchyPrinter.h"


void HierarchyPrinter::entry(std::string name) {
    m_entries.push_back({m_indentation, name});
    m_indentation++;
}

void HierarchyPrinter::leave() {
    m_indentation--;
}

#include <iostream>
void HierarchyPrinter::print() {
    std::cout << "Package Hierarchy:" << std::endl;
    m_entries.push_back({0, ""});

    std::vector<std::string> spacing;
    for(int i = 0; i < m_entries.size() - 1; i++){
        std::string name = m_entries[i].name;

        std::cout << createSpacing(spacing, m_entries[i].level == 0 ? char(0) : (is_last_of_level(i) ? char(192) : char(195)));

        std::string indented_string;
        if(m_entries[i].level > 0){
            indented_string += char(196);
            indented_string += ' ';
        }
        indented_string += name;
        indented_string += ' ';
        std::cout << indented_string;

        if(m_entries[i + 1].level > m_entries[i].level){ // Enter
            std::cout << char(196) << char(191);
            spacing.push_back(createIndentation(indented_string.size() + 1));
        }else if(m_entries[i + 1].level < m_entries[i].level){ // Leave
            spacing.pop_back();
        }
        std::cout << std::endl;
    }
}

std::string HierarchyPrinter::createSpacing(std::vector<std::string> spacing, char corner) {
    std::string s;
    int n = spacing.size();
    for(int i = 0; i < n; i++){
        s += spacing[i];
        if(i < n - 1){
            s += char(179);
        }
    }
    s += corner;
    return s;
}

std::string HierarchyPrinter::createIndentation(int n) {
    std::string s;
    for(int i = 0; i < n; i++){
        s += ' ';
    }
    return s;
}

bool HierarchyPrinter::is_last_of_level(int index) {
    int level = m_entries[index].level;
    for(int i = index + 1; i < m_entries.size(); ++i){
        if(m_entries[i].level < level){
            return true;
        }else if(m_entries[i].level == level){
            return false;
        }
    }
    return true;
}
