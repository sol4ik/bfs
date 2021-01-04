#ifndef RPC_RPC_H
#define RPC_RPC_H

#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "encode.h"
#include "decode.h"
#include "curl_wrapper.h"

enum eth_method {
    call,
    sendTx,
    getTxReceipt
};

class Rpc {
public:
    Rpc(const bytes &userAddr, const bytes &mmAddress) : fromAddr{userAddr},
                                                         memoryManagerAddress{mmAddress},
                                                         curl{} {}

    Rpc(const std::string &userAddr, const std::string &mmAddress) : fromAddr{from_hex(userAddr)},
                                                                     memoryManagerAddress{from_hex(mmAddress)},
                                                                     curl{} {}

    /* Work with files */
    int check_file_exist(const std::string &path);

    int create_file(const std::string &path);

    int write_file(const std::string &path, const std::string &data, size_t length);

    int write_file(const std::string &path, const std::string &data, size_t length, off_t offset);

    int rename_file(const std::string& old_path, const std::string& new_path);

    int read_file(const std::string &path, char *buf,
                  size_t buf_size, off_t offset) ;

    ssize_t get_file_size(const std::string &path);

    int remove_file(const std::string &path);

    /* Work with directories */

    int create_dir(const std::string &path);

    std::vector<std::string> list_dir(const std::string &path);

    int remove_dir(const std::string &path);

    int get_stat(const std::string &path, struct stat *st);

//private:
    bytes fromAddr;
    bytes memoryManagerAddress;
    Curl curl;

    template<typename... Args>
    std::string form_json(eth_method method, const std::string &func_sig, Args... args);

    static std::string process_json(eth_method method, const std::string &json);

    bool get_tx_status(const std::string &hash);
};

#include "rpc.cpp"

#endif //RPC_RPC_H
