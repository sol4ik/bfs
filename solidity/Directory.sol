pragma solidity >=0.4.25 <=0.4.26;
pragma experimental ABIEncoderV2;

import './File.sol';
import './Stat.sol';
// import 'https://github.com/ethereum/dapp-bin/blob/master/library/stringUtils.sol';

contract DirectoryFactory{
    function dir_create(string _dir_name, string _curr_dir) public returns(Directory){
        Directory new_dir = new Directory(_dir_name, _curr_dir, this);
        return new_dir;
    }

}

contract Directory {
    FileStat.stat entry_stat;
    DirectoryFactory public factory;
    string dir_name;
    string previous_dir;
    mapping(string => Directory) internal directories;
    string[] dir_list;
    mapping(string => File) internal files;
    string[] file_list;

    constructor(string _name, string _current_dir, DirectoryFactory _factory) public {
        dir_name = _name;
        previous_dir = _current_dir;
        factory = _factory;
        uint time = now;
        entry_stat.atime = time;
        entry_stat.mtime = time;
        entry_stat.ctime = time;
        // entry_stat.mode = (40000 | 777);
        entry_stat.mode = 16893;
    }

    /// @dev Does a byte-by-byte lexicographical comparison of two strings.
    /// @return a negative number if `_a` is smaller, zero if they are equal
    /// and a positive numbe if `_b` is smaller.
    function compare(string _a, string _b) returns (int) {
        bytes memory a = bytes(_a);
        bytes memory b = bytes(_b);
        uint minLength = a.length;
        if (b.length < minLength) minLength = b.length;
        //@todo unroll the loop into increments of 32 and do full 32 byte comparisons
        for (uint i = 0; i < minLength; i ++)
            if (a[i] < b[i])
                return -1;
            else if (a[i] > b[i])
                return 1;
        if (a.length < b.length)
            return -1;
        else if (a.length > b.length)
            return 1;
        else
            return 0;
    }
    /// @dev Compares two strings and returns true iff they are equal.
    function equal(string _a, string _b) public returns (bool) {
        return compare(_a, _b) == 0;
    }
    /// @dev Finds the index of the first occurrence of _needle in _haystack
    function indexOf(string _haystack, string _needle) returns (int)
    {
        bytes memory h = bytes(_haystack);
        bytes memory n = bytes(_needle);
        if(h.length < 1 || n.length < 1 || (n.length > h.length))
            return -1;
        else if(h.length > (2**128 -1)) // since we have to be able to return -1 (if the char isn't found or input error), this function must return an "int" type with a max length of (2^128 - 1)
            return -1;
        else
        {
            uint subindex = 0;
            for (uint i = 0; i < h.length; i ++)
            {
                if (h[i] == n[0]) // found the first char of b
                {
                    subindex = 1;
                    while(subindex < n.length && (i + subindex) < h.length && h[i + subindex] == n[subindex]) // search until the chars don't match or until we reach the end of a or b
                    {
                        subindex++;
                    }
                    if(subindex == n.length)
                        return int(i);
                }
            }
            return -1;
        }
    }
    function find(string value) returns(int) {
        uint i = 0;
        while (i < dir_list.length) {
            if(equal(dir_list[i], value) == true) {
                return int(i);
            }
            i++;
        }
        i = 0;
        while (i < file_list.length) {
            if(equal(file_list[i], value) == true) {
                return int(i + dir_list.length);
            }
            i++;
        }
        return -1;

    }

    function is_file(string filename) returns(bool) {
        int i = find(filename);
        if(i == -1)
            return false;
        if(uint(i) < dir_list.length)
            return false;
        else
            return true;
    }

    function removeByIndex(uint i) {
        if(uint(i) < dir_list.length) {
            while (i<dir_list.length-1) {
                dir_list[i] = dir_list[i+1];
                i++;
            }
            dir_list.length--;
        } else {
            while(uint(i) - dir_list.length < file_list.length-1) {
                file_list[i - dir_list.length] = file_list[i+1 - dir_list.length];
                i++;
            }
            file_list.length--;
        }
    }

    function create_file(string _name) public {
        file_list.push(_name);
        files[_name] = new File();
    }

    function removeByValue(string value) public {
        int i = find(value);
        if(i != -1) {
            if(uint(i) < dir_list.length)
                removeByIndex(uint(i));
            else
                removeByIndex(uint(i) - dir_list.length);
        }
    }

    function file_write(string _file_name, byte[] _data) public {
        files[_file_name].write(_data);
    }

    function file_write(string _file_name, byte[] _data, uint off_t) public {
        files[_file_name].write(_data, off_t);
    }

    function file_read(string _file_name) view public returns(byte[] memory){
        return files[_file_name].read();
    }

    function file_read(string _file_name, uint off_t) view public returns(bytes32){
        return files[_file_name].bytesToBytes32(off_t);
    }


    function list_dir() public returns(string[]){
        string[] memory files = new string[](dir_list.length + file_list.length);
        for(uint i = 0; i < file_list.length; i++)
            files[i] = file_list[i];
        for(uint j = 0; j < dir_list.length; j++)
            files[j + file_list.length] = dir_list[j];
        return files;
    }

    function get_file_size(string _file_name) public returns(uint256){
        return files[_file_name].get_file_size();
    }

    function create_directory(string _name) public{
        dir_list.push(_name);
        directories[_name] = factory.dir_create(_name, dir_name);
    }

    function get_dir_by_name(string _name) view public returns(Directory) {
        return directories[_name];
    }

    function get_file_by_name(string _name) view public returns(File) {
        return files[_name];
    }

    function delete_file(string _name) public {
        delete files[_name];
        removeByValue(_name);
    }

    function delete_dir(string _name) public {
        delete directories[_name];
        removeByValue(_name);
    }

    function get_stat_dir() public returns(FileStat.stat){
        return entry_stat;
    }

    function get_stat_file(string _name) public returns(FileStat.stat){
        return files[_name].get_stat();
    }

    function append_file(File file, string new_name) public {
        files[new_name] = file;
        files[new_name].update_mtime();
        file_list.push(new_name);
    }
}
