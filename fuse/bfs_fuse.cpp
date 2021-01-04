#include "bfs_fuse.h"
#include "../rpc/rpc.h"

//static Rpc* RPC = new Rpc{"0x00b6940F75B283A8e91D4205055550195b408B48",
//        "0xe0587EB72a30940E36495E62CCD71A6e6BC5c8cE"};
static Rpc *RPC = nullptr;

int bfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *) {
    std::cout << "[DEBUG] READ" << path << std::endl;
    int status = RPC->read_file(path, buf, size, offset);
    return status;
}

int bfs_write(const char *path, const char *data, size_t size, off_t offset, struct fuse_file_info *) {
//    std::cout << offset - 1 << std::endl;
//    if (RPC->write_file(path, reinterpret_cast<const uint8_t *>(data), size)) // TODO: Add offset
//        return size;
    if (offset == 0) {
        if (RPC->write_file(path, std::string(data, size), size)) // TODO: Add offset
            return size;
    } else {
        int x = RPC->write_file(path, std::string(data, size), size,
                                offset);
        if (x)
            return size;
    }

    return 0;
}

int bfs_create_file(const char *path, mode_t mode, dev_t dev) {

    std::cout << "[DEBUG] Create file" << std::endl;
    RPC->create_file(path);
    return 0;
}

int bfs_mkdir(const char *path, mode_t) {
    std::cout << "[DEBUG] MKDIR" << path << std::endl;
    RPC->create_dir(path);
    return 0;
}

int bfs_opendir(const char *path, struct fuse_file_info *) {
    std::cout << "[DEBUG] OPENDIR" << path << std::endl;
    return 0; // TODO: Check directory existence and permissions
}

int bfs_open(const char *path, struct fuse_file_info *fileInfo) {
    std::cout << "[DEBUG] Open File" << path << std::endl;
    return 0;
    int exists = RPC->check_file_exist(path);
    // TODO: if not exists create else return 0
    if (exists == 1) {
        return 0;
    } else {
        return -1;
    } // TODO: Check if file exists, permissions
}

int bfs_readdir(const char *path, void *buff, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
    std::cout << "[DEBUG] [READDIR] " << path << std::endl;
    std::vector<std::string> dirEntries = RPC->list_dir(path);
    dirEntries.emplace_back(".");
    dirEntries.emplace_back("..");
    for (const auto &entry:dirEntries) {
        if (filler(buff, entry.data(), nullptr, 0) != 0)
        {
            std::cout << "Read error" << std::endl;
            return -EXIT_FAILURE;
        }
    }
    return 0;
}

int bfs_getattr(const char *path, struct stat *stbuf) {
    std::cout << "[DEBUG] Get ATTR" << path << std::endl;

    if (strcmp(path, ".") != 0 && strcmp(path, "..") != 0 && strcmp(path, "/") != 0) {
        if (RPC->get_file_size(path) == -1) {
//            RPC->get_stat(path, stbuf);

            return -ENOENT;
        }
        RPC->get_stat(path, stbuf);

        if (stbuf->st_mode == 0)
            stbuf = nullptr;
        return 0;
    }
    memset(stbuf, 0, sizeof(struct stat));
    if (strcmp(path, "/") == 0) {
        stbuf->st_mode = S_IFDIR | 0755;
        stbuf->st_nlink = 2;
        return 0;
    }
    stbuf->st_mode = S_IFREG | 0777;
    stbuf->st_nlink = 1;
    return 0;
}

void *bfs_init(struct fuse_conn_info *conn) {
    return 0;
}

void bfs_destroy(void *) {
    std::cout << "[DEBUG] [DESTROY] " << std::endl;
}

int bfs_unlink(const char *path) {
    std::cout << "[DEBUG] [UNLINK] " << std::endl;
    int status = RPC->remove_file(path);

    return status == 1 ? 0 : -1;
}

int bfs_rmdir(const char *path) {
    std::cout << "[DEBUG] [RMDIR] " << std::endl;
    int status = RPC->remove_dir(path);
    return status == 1 ? 0 : -1;
}

int bfs_truncate (const char* path , off_t t) {
    std::cout << "[DEBUG] Truncated!" << std::endl;
    return 0;
}

// Examples
//
//
//int bfs_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fileInfo) {
//    std::cout << "[DEBUG] [READ] " << std::endl;
//    return 0;
//}
//
//int bfs_write(const char *path, const char *data, size_t size, off_t offset, struct fuse_file_info * fileInfo) {
//    std::cout << "[DEBUG] [WRITE] " << std::endl;
//    std::cout << data << std::endl;
//    return size;
//}
//
//int bfs_create_file(const char *path, mode_t mode, dev_t dev) {
//
//    std::cout << "[DEBUG] Create file" << std::endl;
//    return 0;
//}
//
//int bfs_mkdir(const char *path, mode_t) {
//    std::cout << "[DEBUG] MKDIR" << path << std::endl;
//    return 0;
//}
//
//int bfs_opendir(const char *path, struct fuse_file_info *fileInfo) {
//    std::cout << "[DEBUG] OPENDIR" << path << std::endl;
//    return 0; // TODO: Check directory existence and permissions
//}
//
//int bfs_open(const char *path, struct fuse_file_info *fileInfo) {
//    std::cout << "[DEBUG] Open File" << path << std::endl;
//    std::cout << fileInfo->flags << std::endl;
//    // TODO: if not exists create else return 0
//    return 0; // TODO: Check if file exists, permissions
//}
//
//int bfs_readdir(const char *path, void *buff, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fileInfo) {
//    std::cout << "[DEBUG] [READDIR] " << path << std::endl;
//    return 0;
//}
//
//int bfs_getattr(const char *path, struct stat *stbuf) {
//    std::cout << "[DEBUG] Get ATTR" << path << std::endl;
//
//    if (strcmp(path, "/x") == 0) {
////        stbuf->st_mode =  S_IFDIR | 0444;
////        stbuf->st_nlink = 2;
////        std::cout << stbuf->st_mode << std::endl;
//        return -ENOENT;;
//    }
//    if (strcmp(path, "/") == 0) {
//        stbuf->st_mode = S_IFDIR | 0755;
//        stbuf->st_nlink = 2;
//        return 0;
//    }
//    stbuf->st_mode = S_IFREG | 0777;
//    std::cout << stbuf->st_mode << std::endl;
//    stbuf->st_nlink = 1;
//
//    return 0;
//}
//
//void *bfs_init(struct fuse_conn_info *conn) {
//    return 0;
//}
//
//void bfs_destroy(void *) {
//    std::cout << "[DEBUG] [DESTROY] " << std::endl;
//}
//
//int bfs_unlink(const char *path) {
//    std::cout << "[DEBUG] [UNLINK] " << std::endl;
//    return 0;
//}
//
//int bfs_rmdir(const char *path) {
//    std::cout << "[DEBUG] [RMDIR] " << std::endl;
//    return 0;
//}

int bfs_utimens(const char* path , const timespec* t) {
    std::cout << "[DEBUG] utimes" << std::endl;
    return 0;
}
//int bfs_tuncate (const char* path ,off_t t) {
//    std::cout << "[DEBUG] Truncated!" << std::endl;
//
//    return 0;
//}

int bfs_release(const char* path, struct fuse_file_info* file_info) {
    std::cout << "[DEBUG] Release!" << path << std::endl;

    return 0;
}

int bfs_rename(const char* path_old, const char* path_new) {
    std::cout << "[DEBUG] Rename " << path_old << " " << path_new << std::endl;
    std::cout << RPC->rename_file(path_old, path_new) << std::endl;
    return 0;
}
struct fuse_operations bfs_operations = {
        .getattr = bfs_getattr,
        .mknod = bfs_create_file,
        .mkdir = bfs_mkdir,
        .unlink = bfs_unlink,
        .rmdir = bfs_rmdir,
        .rename = bfs_rename,
        .truncate = bfs_truncate,

        .open = bfs_open, // We don`t need to open a file in BFS

        .read = bfs_read,
        .write = bfs_write,
//        .release = bfs_release,
        .opendir = bfs_opendir,
        .readdir = bfs_readdir,

        .init = bfs_init,

        .destroy = bfs_destroy,

        .utimens     = bfs_utimens,

};



int main(int argc, char *argv[]) {
    if ((getuid() == 0) || (geteuid() == 0)) {
        std::cout << "[WARNING]:\t Running as root!" << std::endl;
    }
//    if (argc < 4) {
//        std::cerr << "Usage: BFS [mountpoint] [Used Address] [Memory Manager Address]" << std::endl;
//    }

//    RPC = new Rpc{argv[3], argv[4]};
    RPC = new Rpc{"0x001F6044daa380e5C9dBbB0807AcE1EA9Cf3B10B",
        "0xF49a8d792D9A3B2644f947A3f23e64888688B3c3"};
    return fuse_main(argc, argv, &bfs_operations, nullptr);
}
