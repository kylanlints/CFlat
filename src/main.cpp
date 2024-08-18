#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include "tokenization.hpp"
#include "arenaAllocator.hpp"
#include "parser.hpp"
#include "generate.hpp"

int main(int argc, char* argv[])
{
    auto start_time = std::chrono::high_resolution_clock::now();

    if (argc != 2 || strlen(argv[1]) < 4 || std::string(argv[1]).compare(strlen(argv[1]) - 3, 3, ".cf"))
    {
        std::cout << "Incorrect usage please use:" << std::endl;
        std::cout << "./cf <input.cf>" << std::endl;

        return EXIT_FAILURE;
    }
    
    std::ifstream ifs(argv[1]);
    std::string contents((std::istreambuf_iterator<char>(ifs)),
                         (std::istreambuf_iterator<char>()));
    ifs.close();


    //std::cout << "Contents are:\n" << contents << std::endl << std::endl;

    Tokenizer tokenizer(std::move(contents));
    std::vector<Token> tokens = tokenizer.Tokenize();

    Parser parser(std::move(tokens));
    NodeRoot root = parser.ParseProgram();

    if (!parser.successful)
    {
        std::cout << "\033[1;31mWas not successful\033[0;37m" << std::endl;

        auto end_time = std::chrono::high_resolution_clock::now();
        
        std::cout << "Took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << " ns\n";

        return EXIT_FAILURE;
    }

    Generator generator(root);
    std::string fname = std::string(argv[1]).erase(strlen(argv[1]) - 3, 3);
    std::fstream file(fname + ".asm", std::ios::out);
    file << generator.gen_assem();
    file.close();

    std::string cmd = "nasm -felf64 " + fname + ".asm && ld " + fname + ".o -o " + fname;
    system(cmd.c_str());

    auto end_time = std::chrono::high_resolution_clock::now();

    std::cout << "Took " << std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() << " ns\n";

    return 0;
}
