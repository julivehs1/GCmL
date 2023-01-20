#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

using namespace boost::program_options;

void on_age(int age)
{
  std::cout << "On age: " << age << '\n';
}

#include "Traversal.h"

int main(int argc, const char *argv[])
{
    try{
        options_description desc{"Options"};
        desc.add_options()
            ("help,h", "Help screen")
            ("path", value<std::filesystem::path>()->required(), "input file")
            ("root,r", "Enables creation of Root Package")
            ("diagram,d", "Prints the package hierarchy")
            ("root_name", value<std::string>(), "Name of Root");

        boost::program_options::positional_options_description pos_desc;
        pos_desc.add("path", -1);

        variables_map vm;
        store(command_line_parser(argc, argv).options(desc).positional(pos_desc).run(), vm);
        notify(vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
        }else{
            std::filesystem::path path = std::filesystem::current_path();
            path /= vm["path"].as<std::filesystem::path>();
            path = std::filesystem::canonical(path);

            std::string root_name = vm.count("root_name") ? vm["root_name"].as<std::string>() : path.filename().string();
            bool enable_root = vm.count("root");
            bool enable_print = vm.count("diagram");
            Traversal::traverse(path, enable_root, root_name, enable_print);
        }

    }catch (const error &ex){
        std::cerr << ex.what() << '\n';
    }
}