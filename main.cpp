#include <iostream>
#include <filesystem>

//NOLINTNEXTLINE
int main(int argc, char * argv[])  {
    std::cout << "Hello World from Docker!!\n";
    std::cout << "The size of " << std::filesystem::absolute(argv[0]) << " is " << std::filesystem::file_size(argv[0]) << "\n";
    return 0;
}
