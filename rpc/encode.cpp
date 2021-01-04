#include "encode.h"

#include <cryptopp/keccak.h>
#include <boost/locale.hpp>
#include <iomanip>

std::string char2hex(byte c) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(c);
    return ss.str();
}

std::string get_method_id(const std::string &msg) {
	CryptoPP::Keccak_256 hash;
	hash.Update((const byte *) msg.data(), msg.size());
	std::string tmp, code;
	tmp.resize(hash.DigestSize());
	hash.Final((byte *) &tmp[0]);
	tmp = tmp.substr(0,4);
	for (byte c : tmp) {
		code += char2hex(c);
	}
	return code;
}

std::string encode(uint64_t num, size_t pad) {
	std::stringstream ss;
    ss << std::setfill('0') << std::setw(2 * pad) << std::hex << num;
    return ss.str();
}

std::string encode(const std::string &str, size_t pad) {
    auto utf_str = boost::locale::conv::to_utf<char>(str, "UTF-8");
    size_t sz = utf_str.length();
    auto code = encode(sz);
    for (auto c : utf_str)
        code += char2hex(c);
    while (code.size() % (2 * pad) != 0)
        code += '0';
    return code;
}

std::string encode(const bytes &bytes) {
    return encode(to_string(bytes, false));
}

std::string to_string(const bytes &data, bool flag) {
    std::string res;
    char hex_byte[3];
    for (const auto &byte: data) {
        sprintf(hex_byte, "%02x", byte);
        res.push_back(hex_byte[0]);
        res.push_back(hex_byte[1]);
    }
    res = flag ? "0x" + res : res;
    return res;
}
