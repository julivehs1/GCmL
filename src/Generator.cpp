//
// Created by juliv on 19.01.2023.
//

#include "Generator.h"
#include <filesystem>
#include <fstream>
#include <string>

const std::filesystem::path Generator::PACKAGE_FILE = "package.cmake";
const std::filesystem::path Generator::CMAKE_LISTS_FILE = "CMakeLists.txt";
const std::filesystem::path Generator::LINKER_FILE = "linker.cmake";
const std::string Generator::LINK_PACKAGE_FUNC_NAME = "link_package";
const std::string Generator::LINK_PACKAGE_FUNC =
        "function(" + LINK_PACKAGE_FUNC_NAME + " TARGET LIB)\n"
        "    #Library\n"
        "    get_target_property(LIB_INTERFACE ${LIB} ALIASED_TARGET)\n"
        "\n"
        "    #Target\n"
        "    get_target_property(TARGET_INTERFACE ${TARGET} ALIASED_TARGET)\n"
        "    get_target_property(TARGET_OBJECT_LIST ${TARGET_INTERFACE} INTERFACE_LINK_LIBRARIES)\n"
        "    list(GET TARGET_OBJECT_LIST 0 TARGET_OBJECT)\n"
        "    #Link\n"
        "    target_link_libraries(${TARGET_OBJECT} PRIVATE ${LIB_INTERFACE})\n"
        "endfunction()";


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

    // Object
    if(sources.empty()) {
        package_file << "file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/null.cpp \"\")" << "\n";
        package_file << "add_library(" << package_name << "-OBJECT OBJECT  ${CMAKE_CURRENT_BINARY_DIR}/null.cpp ${HEADER})" << "\n";
       }else{
        package_file << "add_library(" << package_name << "-OBJECT OBJECT  ${SOURCE} ${HEADER})" << "\n";
    }
    package_file << "set_target_properties(" << package_name << "-OBJECT PROPERTIES PUBLIC_HEADER ${HEADER})"  << "\n";

    package_file << "\n";

    //Interface
    package_file << "add_library(" << package_name << "-INTERFACE INTERFACE)" << "\n";
    package_file << "target_link_libraries(" << package_name << "-INTERFACE INTERFACE " << package_name << "-OBJECT $<TARGET_OBJECTS:" << package_name << "-OBJECT>)" << "\n";

    // Create Include dir
    std::string include_dir = generatePackageName(package, "/");
    package_file << "file(COPY ${HEADER} DESTINATION DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/include/" << include_dir << ")" << "\n";
    package_file << "target_include_directories(" << package_name << "-INTERFACE INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/include)" << "\n";

    package_file << "\n";

    //Link Subpackages
    for(const std::string &subpackage : subpackages){
        package_file << "target_link_libraries(" << package_name << "-OBJECT PUBLIC " << package_name << "-" << subpackage << "-INTERFACE" << ")" << "\n";
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

#include <iostream>

void Generator::generateCmakeList(const std::filesystem::path &path, int level) {
    std::filesystem::path cmakelists_path = path;
    cmakelists_path /= CMAKE_LISTS_FILE;

    std::string include = "include(" + PACKAGE_FILE.string() + ")";
    std::string linker = "";
    if(level > 0){
        linker += "include(";
        for(int i = 0; i < level; i++){
            linker += "../";
        }
        linker += LINKER_FILE.string() + ")";
    }


    if(std::filesystem::exists(cmakelists_path)){ // CHECK and insert
        bool inject_linker = !isPresent(cmakelists_path, linker);
        bool inject_include = !isPresent(cmakelists_path, include);

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

        readFile.close();

        std::ofstream newFile(cmakelists_path, std::ofstream::trunc);
        if(inject_include){
            newFile << include << std::endl;
        }
        if(inject_linker){
            newFile << linker << std::endl;
        }

        for(const auto& line : lines){
            newFile << line << std::endl;
        }
        newFile.flush();
        newFile.close();

    }else{ // Create new
        std::ofstream cmakelists_file(cmakelists_path);
        cmakelists_file << include << "\n";
        cmakelists_file << linker << "\n";
        cmakelists_file.flush();
        cmakelists_file.close();
    }
}

void Generator::generateLinker(const std::filesystem::path &path) {
    std::filesystem::path linker_path = path;
    linker_path /= LINKER_FILE;

    std::ofstream linker_file(linker_path);

    linker_file << LINK_PACKAGE_FUNC;

    linker_file.close();
}

bool Generator::isPresent(const std::filesystem::path &path, std::string element) {
    std::string fileLine;
    std::vector<std::string> lines;
    std::ifstream readFile(path);
    if (readFile.is_open())
    {
        while (std::getline(readFile, fileLine))
        {
            lines.push_back(fileLine);
        }
    }

    readFile.close();

    return  std::count_if(lines.begin(), lines.end(), [&element](std::string line){return line.find(element) != std::string::npos;}) > 0;
}

