//
// Created by juliv on 19.01.2023.
//

#include "Generator.h"
#include <filesystem>
#include <fstream>
#include <string>

const std::filesystem::path Generator::PACKAGE_FILE = "package.cmake";
const std::filesystem::path Generator::CMAKE_LISTS_FILE = "CMakeLists.txt";


void Generator::generatePackage(const std::filesystem::path &path, const std::vector<std::string> &package,
                                const std::vector<std::string> &sources, const std::vector<std::string> &headers,
                                const std::vector<std::string> &subpackages) {
    std::filesystem::path package_filepath = path;
    package_filepath /= PACKAGE_FILE;
    std::ofstream package_file(package_filepath);

    // Load Sub Directories
    for(const std::string &subpackage : subpackages){
        package_file << "add_subdirectory(" << subpackage << ")" << "\n";
    }
    if(!subpackages.empty()) {
        package_file << "\n";
    }

    // Source
    package_file << "set(SOURCE" << "\n";
    for(const std::string &source : sources){
        package_file << "\t\t" << source << "\n";
    }
    package_file << "\t" << ")" << "\n\n";

    // Source
    package_file << "set(HEADER" << "\n";
    for(const std::string &header : headers){
        package_file << "\t\t" << header << "\n";
    }
    package_file << "\t" << ")" << "\n\n";


    // Library
    std::string package_name = generatePackageName(package, "-");
    std::string nice_package_name = generatePackageName(package, "::");

    if(!sources.empty()) {
        package_file << "add_library(" << package_name << "-OBJECT OBJECT  ${SOURCE} ${HEADER})" << "\n";
        package_file << "add_library(" << package_name << "-INTERFACE INTERFACE)" << "\n";
        package_file << "target_link_libraries(" << package_name << "-INTERFACE INTERFACE " << package_name << "-OBJECT $<TARGET_OBJECTS:" << package_name << "-OBJECT>)" << "\n";
    }else{
        package_file << "add_library(" << package_name << "-INTERFACE INTERFACE ${HEADER})" << "\n";
    }

    //Link Subpackages
    for(const std::string &subpackage : subpackages){
        package_file << "target_link_libraries(" << package_name << "-INTERFACE INTERFACE " << nice_package_name << "::" << subpackage << ")" << "\n";
    }

    //Alias
    package_file << "add_library(" << nice_package_name << " ALIAS " << package_name << "-INTERFACE)";
    package_file.close();
}


std::string Generator::generatePackageName(const std::vector<std::string> &package, const std::string &seperator) {
    std::string name = package.front();
    for(auto it = package.begin() + 1; it != package.end(); ++it){
        name += seperator;
        name += *it;
    }
    return name;
}

void Generator::generateCmakeList(const std::filesystem::path &path) {
    std::filesystem::path cmakelists_path = path;
    cmakelists_path /= CMAKE_LISTS_FILE;

    std::string include = "include(" + PACKAGE_FILE.string() + ")";

    if(std::filesystem::exists(cmakelists_path)){ // CHECK and insert
        std::string fileLine;
        std::vector<std::string> lines;
        std::ifstream readFile(cmakelists_path);
        if (readFile.is_open())
        {
            while (std::getline(readFile, fileLine))
            {
                lines.push_back(fileLine);
            }
        }

        if(std::count_if(lines.begin(), lines.end(), [&include](std::string line){return line == include || line == (include + "\n");}) == 0){ // inject if missing
            std::ofstream newFile(cmakelists_path, std::ofstream::trunc);
            newFile << include << "\n";
            for (auto & line : lines) {
                newFile << line << std::endl;
            }

        }

    }else{ // Create new
        std::ofstream cmakelists_file(cmakelists_path);
        cmakelists_file << include;
        cmakelists_file.close();
    }
}


