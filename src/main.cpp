

#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include "log.h"
#include "stages.h"


std::string filename(const std::string &filename) {
    const std::filesystem::path file_path(filename);

    if (!file_path.has_extension() || file_path.extension() != ".c") {
        throw std::invalid_argument("Error: The source file must have a .c extension.");
    }

    if (!exists(file_path) || !is_regular_file(file_path)) {
        throw std::invalid_argument("Error: The file does not exist or is not a regular file.");
    }

    return file_path.stem().string();
}


namespace po = boost::program_options;

int main(int argc, char *argv[]) {
    try {
        Log &logger = Log::instance();
        logger.setLogLevel(LogLevel::Debug);

        // Define the available options
        po::options_description desc("Allowed options");
        desc.add_options()("help,h", "produce help message")("lex", "perform lexing")("parse", "perform parsing")(
                "codegen", "perform code generation")("S", "generate assembly")(
                "output-file,o", po::value<std::string>(), "specify output file");

        // Positional argument for the input file
        po::positional_options_description p;
        p.add("input-file", 1); // Add input-file as a positional argument

        // Add input file option
        po::options_description all_options;
        all_options.add(desc).add_options()("input-file", po::value<std::string>(), "input file");

        // Map to store the values of the options
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(all_options).positional(p).run(), vm);
        po::notify(vm);

        // Handle help message
        if (vm.contains("help")) {
            std::cout << desc << "\n";
            return 1;
        }

        // Enforce that only one of --lex, --parse, or --codegen may be used
        int mode_count = (vm.count("lex") ? 1 : 0) + (vm.count("parse") ? 1 : 0) + (vm.count("codegen") ? 1 : 0);

        if (mode_count > 1) {
            std::cerr << "Error: Only one of --lex, --parse, or --codegen may be used at a time.\n";
            return 1;
        }

        std::string basename;

        // Handle the input file
        if (vm.count("input-file")) {
            std::string input_file = vm["input-file"].as<std::string>();
            std::cout << "Input file: " << input_file << "\n";
            basename = filename(input_file);
        } else {
            std::cerr << "Error: Input file is required.\n";
            return 1;
        }

        // Handle the optional output file
        std::string output_file;
        if (vm.count("output-file")) {
            output_file = vm["output-file"].as<std::string>();
        } else {
            // Set default output file if not provided
            output_file = "a.out"; // Default name
        }
        std::cout << "Output file: " << output_file << "\n";

        Stages::preprocess(basename);

        // Handle lex, parse, or codegen options
        if (vm.count("lex") || vm.count("parse") || vm.count("codegen")) {
            std::cout << "Lexing...\n";
            // Insert your lexing logic here
        }
        if (vm.count("parse")||vm.count("codegen")) {
            std::cout << "Parsing...\n";
            // Insert your parsing logic here
        }
        if (vm.count("codegen")) {
            std::cout << "Generating code...\n";
            // Insert your code generation logic here
        }


        if (vm.count("S")) {
            std::cout << "Generating assembly...\n";
            // Insert your assembly generation logic here
            return 0;
        }


        // Stages::compile(basename);
        Stages::assemble(basename);
        Stages::link(basename);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
