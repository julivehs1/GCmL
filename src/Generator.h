//
// Created by juliv on 19.01.2023.
//

#ifndef GCML_GENERATOR_H
#define GCML_GENERATOR_H

#include <filesystem>
#include <vector>

class Generator {
public:
    static void generatePackage(const std::filesystem::path &path, const std::vector<std::string> &package, const std::vector<std::string> &sources, const std::vector<std::string> &headers, const std::vector<std::string> &subpackages);
    static void generateCmakeList(const std::filesystem::path &path);
private:
    static std::string generatePackageName(const std::vector<std::string> &package, const std::string &seperator);

    static const std::filesystem::path PACKAGE_FILE;
    static const std::filesystem::path CMAKE_LISTS_FILE;
};


#endif //GCML_GENERATOR_H
