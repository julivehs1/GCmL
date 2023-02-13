//
// Created by juliv on 20.01.2023.
//

#ifndef GCML_TRAVERSAL_H
#define GCML_TRAVERSAL_H


#include <filesystem>
#include <vector>
#include <string>
#include "HierarchyPrinter.h"

class Traversal {
public:
    static void traverse(std::filesystem::path path, bool include_root, std::string root_name, bool enable_printer, bool enable_linker);
private:
    static bool traverse(std::filesystem::path path, std::vector<std::string> &package, HierarchyPrinter &hierarchyPrinter, bool enable_linker, int level);
    static const std::vector<std::string> HEADER_EXTENSIONS;
    static const std::vector<std::string> SOURCE_EXTENSIONS;

};


#endif //GCML_TRAVERSAL_H
