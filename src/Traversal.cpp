//
// Created by juliv on 20.01.2023.
//

#include "Traversal.h"
#include "Generator.h"


const std::vector<std::string> Traversal::HEADER_EXTENSIONS = {".h"};
const std::vector<std::string> Traversal::SOURCE_EXTENSIONS = {".cpp"};



void Traversal::traverse(std::filesystem::path path, bool include_root, std::string root_name, bool enable_printer) {
    std::vector<std::string> package;
    if(include_root){
        package.push_back(root_name);
    }

    HierarchyPrinter printer;
    traverse(path, package, printer);
    if(enable_printer){
        printer.print();
    }
}

bool Traversal::traverse(std::filesystem::path path, std::vector<std::string> &package, HierarchyPrinter &hierarchyPrinter) {
    std::vector<std::string> subpackages;
    std::vector<std::string> sources;
    std::vector<std::string> headers;

    if(!package.empty()){
        hierarchyPrinter.entry(package.back());
    }

    for (const auto & entry : std::filesystem::directory_iterator(path)){
        std::string name = entry.path().filename().string();
        if(entry.is_directory()){
            // add to package
            package.push_back(name);

            //call recursively
            bool subpackage_valid = traverse(entry, package, hierarchyPrinter);

            // remove from package
            package.pop_back();

            if(subpackage_valid){
                subpackages.push_back(name);
            }

        }else if(entry.is_regular_file()){
            std::string extension = entry.path().extension().string();
            if(std::count(HEADER_EXTENSIONS.begin(), HEADER_EXTENSIONS.end(), extension)){ // HEADER
                headers.push_back(name);
            }else if (std::count(SOURCE_EXTENSIONS.begin(), SOURCE_EXTENSIONS.end(), extension)){ // SOURCE
                sources.push_back(name);
            }
        }
    }


    if((subpackages.size() + sources.size() + headers.size()) > 0 && package.size() > 0){
        // Call generator
        Generator::generatePackage(path, package, sources, headers, subpackages);
        Generator::generateCmakeList(path);
        hierarchyPrinter.leave();
        return true;
    }else{
        hierarchyPrinter.leave();
        return false;
    }
}

