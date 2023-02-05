#include "FS.h"
#include "FFat.h"
#include <sys/_stdint.h>
#include "FileSystem.hpp"

uint8_t FileSystem::begin() {
  //FFat.format();
  if(!FFat.begin(true)){
        __LOG_E.println("FFat Mount Failed");
        return -1;
  }
  __LOG_I.printf("Total space: %10u\n", FFat.totalBytes());
  __LOG_I.printf("Free space: %10u\n", FFat.freeBytes());
  return 0;
}

uint8_t FileSystem::addToIndex(const char *path) {
  File index_file = FFat.open("/AIndex.FSList", FILE_APPEND);
  if(!index_file) {
    __LOG_E.println("can't open a \"AIndex.FSList\"!!!");
    return -1;
  }
  File file = FFat.open(path,FILE_READ);
  if(!file) {
    __LOG_E.printf("can't open a \"%s\". can't add to index him\n",path);
    return -2;
  }
  String str;
  if(file.isDirectory()) {
    str = "D\'";
  }
  else {
    str = "F\'";
  }
  str += path;
  str += "'\n";
  file.close();
  if(index_file.print(str)){
        __LOG_V.println("index file append");
    } else {
        __LOG_E.println("index append failed");
        index_file.close();
        return -3;
    }
  index_file.close();
  return 0;
}
/*
uint8_t FileSystem::removeToIndex(const char *path){
  File file_index = FFat.open("AIndex.FSList",FILE_)
}
*/

uint8_t FileSystem::write(const char *path, const char *message) {
  File file = FFat.open(path, FILE_WRITE);
  if(!file){
    __LOG_E.printf("- Failed to open \"%s\" for writing!\n",path);
    return -1;
  }
  int writed = file.print(message);
  if(writed) {
    __LOG_V.printf("- File \"%s\" writen!\n",path);
  } else {
    __LOG_E.printf("- File \"%s\" failed!\n",path);
    file.close();
    return -2;
  }
  addToIndex(path);  
  file.close();
  return 0;
}

uint8_t FileSystem::mkdir(const char *path) {
  if(!FFat.mkdir(path)) {
    __LOG_E.printf("directory \"%s\" is not created!\n",path);
    return -1;
  }
  addToIndex(path);
  return 0;
}

uint8_t FileSystem::renameFile(const char * path1, const char * path2){
    __LOG_V.printf("Renaming file %s to %s\r\n", path1, path2);
    if (FFat.rename(path1, path2)) {
        __LOG_V.println("- file renamed");
    } else {
        __LOG_E.println("- rename failed");
        return -1;
    }
    return 0;
}

uint8_t FileSystem::deleteFile(const char * path){
    __LOG_V.printf("Deleting file: %s\r\n", path);
    if(FFat.remove(path)){
        __LOG_V.println("- file deleted");
    } else {
        __LOG_E.printf("- delete \"%s\" failed\n",path);
        return -1;
    }
    return 0;
}

uint8_t FileSystem::listDir(const char * dirname, uint8_t levels){
    __LOG_I.printf("Listing directory: %s\r\n", dirname);

    File root = FFat.open(dirname);
    if(!root){
        __LOG_E.printf("- failed to open directory \"%s\"\n",dirname);
        return -1;
    }
    if(!root.isDirectory()){
        __LOG_I.println(" - not a directory");
        return -2;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            __LOG_I.print("  DIR : ");
            __LOG_I.println(file.name());
            if(levels){
                listDir(file.path(), levels -1);
            }
        } else {
            __LOG_I.print("  FILE: ");
            __LOG_I.print(file.name());
            __LOG_I.print("\tSIZE: ");
            __LOG_I.println(file.size());
        }
        file = root.openNextFile();
    }
    return 0;
}

uint8_t FileSystem::deleteDir(const char *dirname, uint8_t levels) {
  File root = FFat.open(dirname);
  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()) {
      if(levels) {
        if(deleteDir(file.path(), levels-1)) {
          return -2;
        }
      }
    }
    if(FFat.remove(file.path())){
      __LOG_V.printf("- file \"%s\" deleted\n",file.path());
    } else {
      __LOG_E.printf("- delete \"%s\" failed\n",file.path());
      return -1;
    }
      file = root.openNextFile();
  }
  root.close();
  return 0;
}

uint8_t FileSystem::appendFile(const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = FFat.open(path, FILE_APPEND);
    if(!file){
        __LOG_E.println("- failed to open file for appending");
        return -1;
    }
    if(file.print(message)){
        __LOG_V.println("- message appended");
    } else {
        __LOG_E.println("- append failed");
        file.close();
        return -2;
    }
    file.close();
    return 0;
}
