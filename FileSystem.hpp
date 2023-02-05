#include <sys/_stdint.h>
#ifndef _FILESYSTEM_HPP_
# define _FILESYSTEM_HPP_

#include <FS.h>
#include <FFat.h>
#define __LOG Serial
#define __LOG_I Serial
#define __LOG_D Serial
#define __LOG_E Serial
#define __LOG_W Serial
#define __LOG_V Serial
class FileSystem {
  public:
    uint8_t begin();
    uint8_t write(const char *path, const char *message);
    uint8_t read(const char *path);
    uint8_t listDir(const char * dirname, uint8_t levels);
    uint8_t renameFile(const char * path1, const char * path2);
    uint8_t deleteFile(const char * path);
    uint8_t deleteDir(const char *dirname, uint8_t levels);
    uint8_t appendFile(const char * path, const char * message);
    uint8_t addToIndex(const char *path);
    uint8_t mkdir(const char *path);
  private:
};

#endif