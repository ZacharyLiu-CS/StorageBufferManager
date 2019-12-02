//
// Created by gloomy on 2019/11/30.
//

#include "DSMgr.h"

DataStorageManager::DSMgr::DSMgr(){
    if(this->OpenFile("./datafile.dbf") != 0)
        this->NewFile("./datafile.dbf");

}
DataStorageManager::DSMgr::~DSMgr() {
    fclose(this->currFile);
}
// OpenFile contain the function of open and read content
int DataStorageManager::DSMgr::OpenFile(std::string filename){
    this->currFile = fopen(filename.c_str(),"rb+");
    if(this->currFile == NULL)
        return -1;
    // start to read the file contents;
    if(fread(&this->numPages, sizeof(int),1,this->currFile) != 1){
        std::cerr << "faile to load db's numPages!" << std::endl;
        std::cerr << "so the numPages is " << this->numPages << std::endl;
        return -2;
    }
    //read the page_use_bit
    if(fread(this->pages_use_bit,sizeof(int),MAXPAGES,this->currFile) != MAXPAGES){
        std::cerr << "failed to load db's pages_use_bit!" << std::endl;
        return -2;
    }
    //read the page_offset
    if(fread(this->pages_offset,sizeof(long long),MAXPAGES,this->currFile) != MAXPAGES){
        std::cerr << "failed to load db's pages_offset!" << std::endl;
        return -2;
    }
    this->read_io_count = 0;
    this->write_io_count = 0;

    return 0;
}
int DataStorageManager::DSMgr::NewFile(std::string filename) {
    this->currFile = fopen(filename.c_str(),"wb+");
    if(this->currFile == NULL)
        return -1;
    this->numPages = MAXPAGES;
    this->write_io_count = 0;
    this->read_io_count = 0;
    // write the numPage info
    fwrite(&this->numPages,sizeof(int),1,this->currFile);

    //init the pages_use_bit and write it to db
    for(int i = 0; i < MAXPAGES; i++)
        this->pages_use_bit[i] = 0;
    fwrite(this->pages_use_bit,sizeof(int),MAXPAGES,this->currFile);

    //init the pages_offset and write it to db
    size_t pos = ftell(this->currFile);
    //init the pages_offset and write it to db
    for(int i = 0; i < MAXPAGES; i++)
        this->pages_offset[i] = 2*pos+ sizeof(pages_offset) + FRAMESIZE*i;
    fwrite(this->pages_offset,sizeof(long long),MAXPAGES,this->currFile);

    //init the data
    for(int i = 0; i < MAXPAGES; i++){
        bFrame write_page;
        sprintf(write_page.field,"%s%d%s","page_id: " ,i , " data: this is just test!");
        this->WritePage(i,write_page);
    }
    return 0;
}

int DataStorageManager::DSMgr::CloseFile(){
    if(fclose(this->currFile))
        return -1;
    else return 0;

}

int DataStorageManager::DSMgr::ReadPage(int page_id, bFrame &read_frame){
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
    if(this->Seek( offset, 0) == -1) {
        std::cerr << "wrong to seek file!" << std::endl;
        return -1;
    }
    if(fwrite(&write_frame, sizeof(write_frame), 1, currFile) != 1) {
        std::cerr << "DSMgr: Write page failed" << std::endl;
        return -1;
    }
    write_io_count++;
    return 0;
}

int DataStorageManager::DSMgr::Seek(long long  offset, long long pos){
    if(fseek(this->currFile,offset+pos, SEEK_SET))
        return -1;
    else
        return 0;

}

FILE *DataStorageManager::DSMgr::GetFile(){
    return this->currFile;
}

void DataStorageManager::DSMgr::IncNumPages(){
    this->numPages++;
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

int DataStorageManager::DSMgr::GetIOCount() {
    return write_io_count + read_io_count;
}