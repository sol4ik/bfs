pragma solidity >=0.4.25 <0.4.26;

import './Directory.sol';
import './Stat.sol';
pragma experimental ABIEncoderV2;

contract File {
    bytes _data;
    FileStat.stat public entry_stat;

    function read() view public returns(bytes memory) {
        entry_stat.atime = now;
        return _data;
    }

    constructor() public {
        uint time = now;
        entry_stat.atime = time;
        entry_stat.mtime = time;
        entry_stat.ctime = time;
        // entry_stat.mode = (100000 | 770);
        entry_stat.mode = 33204;
    }

    function update_mtime() public
    {
        entry_stat.mtime = now;
    }

    function bytesToBytes32(uint offset) public returns (bytes32) {
        bytes32 out;

        for (uint i = 0; i < 32; i++) {
            out |= bytes32(_data[offset + i] & 0xFF) >> (i * 8);
        }
        return out;
    }

    function write(bytes memory data) public {
        uint time = now;
        entry_stat.atime = time;
        entry_stat.mtime = time;
        _data = data;
        entry_stat.size = data.length;
    }

    function write(bytes memory data, uint off_t) public {
        bytes memory new_data = new bytes(data.length + off_t);
        for(uint i = 0; i < off_t; i++){
            new_data[i] = _data[i];
        }
        i = off_t;
        for(uint j = 0; j < data.length; j++){
            new_data[i++] = data[j];
        }
        _data = new_data;
        // delete new_data;
        uint time = now;
        entry_stat.atime = time;
        entry_stat.mtime = time;
        entry_stat.size = off_t + data.length;
    }

    function get_file_size() view public returns(uint256){
        return entry_stat.size;
    }

    function get_stat() public returns(FileStat.stat){
        return entry_stat;
    }

}
