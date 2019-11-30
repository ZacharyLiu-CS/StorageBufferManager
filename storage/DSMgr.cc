//
// Created by gloomy on 2019/11/30.
//
#include "DSMgr.h"

DataStorageManager::DSMgr::DSMgr(){
    this->read_io_count = 0;
    this->write_io_count = 0;
    this->numPages = 0;
    this->OpenFile("./datafile.dbf");
}

int DataStorageManager::DSMgr::OpenFile(std::string filename){
    this->currFile = fopen(filename.c_str(),"rw");
    if(this->currFile == NULL)
        return -1;
    else
        return 0;

}

int DataStorageManager::DSMgr::CloseFile(){
    if(fclose(this->currFile))
        return -1;
    else return 0;

}

int DataStorageManager::DSMgr::ReadPage(int page_id, bFrame & read_frame){
    long long offset = pages_offset[page_id];
    fseek(currFile, offset, SEEK_SET);
    if(fread(&read_frame, sizeof (read_frame), 1, currFile) != 1) {
        std::cerr << "DSMgr: Read page failed" << std::endl;
        return -1;
    }
    read_io_count ++ ;
    return 0;
}

int DataStorageManager::DSMgr::WritePage(int page_id, bFrame &write_frame){
    long long offset = pages_offset[page_id];
    this->Seek( offset, 0);
    if(fwrite(&write_frame, sizeof(write_frame), 1, currFile) != 1) {
        std::cerr << "DSMgr: Write page failed" << std::endl;
        return -1;
    }
    write_io_count++;
    return 0;
}

int DataStorageManager::DSMgr::Seek(int offset, int pos){
    if(fseek(this->currFile,offset+pos, SEEK_SET))
        return -1;
    else
        return 0;

}

FILE *DataStorageManager::DSMgr::GetFile(){
    return this->currFile;
}

void DataStorageManager::DSMgr::IncNumPages(){
    this->numPages ++;
}

int DataStorageManager::DSMgr::GetNumPages(){
    return numPages;
}

void DataStorageManager::DSMgr::SetUse(int index, int use_bits){
    this->pages_use_bit[index] = use_bits;
}
int DataStorageManager::DSMgr::GetUse(int index) {
    return this->pages_use_bit[index];
}