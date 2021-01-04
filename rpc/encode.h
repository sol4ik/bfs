#ifndef RPC_ENCODE_H
#define RPC_ENCODE_H

#include <array>
#include <any>
#include <sstream>
#include <string>

#include <cryptopp/cryptlib.h>

#ifdef CRYPTOPP_NO_GLOBAL_BYTE
typedef unsigned char byte;
#endif

typedef std::vector<byte> bytes;

bytes from_hex(const std::string &str);

std::string get_method_id(const std::string &msg);

std::string encode(uint64_t num, size_t pad = 32);

std::string encode(const std::string &str, size_t pad = 32);

std::string encode(const bytes &bytes);

template<typename... Args>
std::string main_encode(const std::string &func, Args... args) {
	std::string code = get_method_id(func), word, data;
    std::stringstream ss{func};

    std::getline(ss, word, '(');
    std::vector<std::any> params{args...};
	size_t params_n = params.size(), offset = 32 * params.size();

    std::vector<std::string> words;
    while (std::getline(ss, word, ',')) {
        if (word[word.size() - 1] == ')')
            word = word.substr(0, word.size() - 1);
        words.push_back(word);
    }

    for (size_t i = 0; i < params_n; ++i) {
		std::string param_code;
		if (words[i] == "string" || words[i] == "bytes") {
			param_code += encode(std::any_cast<std::string>(params[i]));
			data += param_code;
			code += encode(offset);
			offset += param_code.size() / 2;
		}
		else if (words[i] == "uint256" || words[i] == "uint") {
			code += encode(std::any_cast<std::uint64_t>(params[i]));
		}

//        else if (words[i] == "bytes") {
//            param_code += encode(std::any_cast<std::string>(params[i]));
//            data += param_code;
//            code += encode(offset);
//            offset += param_code.size() / 2;
//        }
    }
	return "0x" + code + data;
}

std::string to_string(const bytes &data, bool flag = true);

#endif //RPC_ENCODE_H
