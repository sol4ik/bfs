#include <iostream>
#include <sys/stat.h>
#include "rpc.h"

int main() {
    uint64_t x = 221;
    auto i = encode(x);
    std::cout << to_string(i) <<std::endl;
//    Rpc rpc{"0x00b6940F75B283A8e91D4205055550195b408B48",
//            "0xc89F63545104C432Ad32105c18F297edB0b1fF10"};
//
////    rpc.create_file("/text1.txt");
//    u_int8_t x[100];
////    rpc.create_file("")
//    rpc.rename_file("/file", "/file2");
//    rpc.create_file("/file3");
  //    std::cout << rpc.list_dir("/");
//    rpc.read_file("/text1.txt", x, 100, 0);
//    struct stat s;
//    std::cout << rpc.get_stat("/text1.txt", &s) << std::endl;
//    std::cout << s.st_mode << std::endl;
//    uint8_t data[1024] = {0xAB, 0xBA};
//    std::cout << "Begin testing!" << std::endl;
//    if (!rpc.create_file("/est")) {
//        std::cout << "Failed to create dir" << std::endl;
//    }

//    // heow
//    if (!rpc.create_file("/test_file.txt")) {
//        std::cerr << "Couldn't create file" << std::endl;
//    }
//    std::cout << "Filed created!" << std::endl;
//    if (!rpc.write_file("/test_file.txt", data, 2)) {
//        std::cerr << "Couldn't write to a file" << std::endl;
//    }
//    std::cout << rpc.get_file_size("/test_file.txt") << std::endl;
//    size_t num = rpc.read_file("/test_file.txt", data, 1024, 0);
//    std::cout << "0x";
//    for (int i = 0; i < num; i++) {
//        std::cerr << std::hex << (unsigned int) data[i];
//    }
//    std::cout << std::endl;
//    if (!rpc.create_dir("/etc")) {
//        std::cerr << "Couldn't create directory" << std::endl;
//    }

//    if (!rpc.create_dir("/etc/file2")) {
//        std::cerr << "Couldn't create directory" << std::endl;
//    }
//    std::cout << "Directory /etc/ has next files: " << std::endl;
//    for (auto &i: rpc.list_dir("/etc/")) {
//        std::cout << i << std::endl;
//    }
//    auto *st = new(struct stat);
//    rpc.get_stat("/etc", st);
//    std::cout << "Mode for directory: " << st->st_mode << std::endl;
//    rpc.get_stat("/test_file.txt", st);
//    std::cout << "Mode for file: " << st->st_mode << std::endl;
//    std::cout << "Size of file: " << st->st_size << std::endl;
//    delete (st);
//    if(!rpc.remove_file("/test_file.txt")){
//        std::cerr << "Couldn't delete test.txt" << std::endl;
//    }
    return 0;
}
