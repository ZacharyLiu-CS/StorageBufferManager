//
// Created by gloomy on 2019/11/30.
//

#include "DSMgr.h"
#include <memory.h>
DataStorageManager::DSMgr::DSMgr(){
    this->fileName = "./data.dbf";
    if(this->OpenFile(this->fileName) != 0)
        this->NewFile(this->fileName);

}
DataStorageManager::DSMgr::~DSMgr() {
    this->Seek(0,0);
    fwrite(&this->numPages,sizeof(int),1,this->currFile);
    fwrite(this->pages_use_bit,sizeof(int),this->numPages,this->currFile);
    fwrite(this->pages_offset,sizeof(long long),this->numPages,this->currFile);
    delete [] this->pages_use_bit;
    delete [] this->pages_offset;
    this->CloseFile();
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
    this->pages_use_bit = (int*)malloc(sizeof(int) * this->numPages);
    if(fread(this->pages_use_bit,sizeof(int),this->numPages,this->currFile) != (long unsigned int)this->numPages){
        std::cerr << "failed to load db's pages_use_bit!" << std::endl;
        return -2;
    }
    //read the page_offset
    this->pages_offset = (long long*)malloc(sizeof(long long) * this->numPages);
    if(fread(this->pages_offset,sizeof(long long),this->numPages,this->currFile) !=(long unsigned int) this->numPages){
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
    // write the numPage info

    fwrite(&this->numPages,sizeof(int),1,this->currFile);

    this->pages_use_bit = (int*)malloc(sizeof(int) * this->numPages);
    //init the pages_use_bit and write it to db
    for(int i = 0; i < MAXPAGES; i++)
        this->pages_use_bit[i] = 0;
    fwrite(this->pages_use_bit,sizeof(int),MAXPAGES,this->currFile);

    //init the pages_offset and write it to db
    size_t pos = ftell(this->currFile);
    //init the pages_offset and write it to db
    this->pages_offset = (long long*)malloc(sizeof(long long) * this->numPages);
    for(int i = 0; i < MAXPAGES; i++)
        this->pages_offset[i] = 3*pos+ sizeof(long long)*MAXPAGES + FRAMESIZE*i;
    fwrite(this->pages_offset,sizeof(long long),MAXPAGES,this->currFile);

    //init the data
    for(int i = 0; i < MAXPAGES; i++){
        bFrame write_page;
        memset(write_page.field,0,FRAMESIZE);
        sprintf(write_page.field,"%s%d%s","page_id: " ,i , " data: this is just test!");
        this->WritePage(i,write_page);
    }
    this->write_io_count = 0;
    this->read_io_count = 0;
    return 0;
}

int DataStorageManager::DSMgr::CloseFile(){
    if(fclose(this->currFile))
        return -1;
    else return 0;
}

int DataStorageManager::DSMgr::ReadPage(int page_id, bFrame &read_frame){
    long long offset = this->pages_offset[page_id];
    fseek(currFile, offset, SEEK_SET);
    if(fread(&read_frame, sizeof (read_frame), 1, currFile) != 1) {
        std::cerr << "DSMgr: Read page failed" << std::endl;
        return -1;
    }
    read_io_count ++ ;
    return 0;
}

int DataStorageManager::DSMgr::WritePage(int page_id, bFrame &write_frame){
    long long offset = this->pages_offset[page_id];
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

int DataStorageManager::DSMgr::IncNumPages(){
    this->pages_offset = (long long *)realloc(this->pages_offset,sizeof(*this->pages_offset) * (this->numPages+1));
    this->pages_use_bit = (int *)realloc(this->pages_use_bit,sizeof(int)*(this->numPages+1));
    this->pages_use_bit[this->numPages] = 0;
    this->pages_offset[this->numPages] = pages_offset[this->numPages-1] + FRAMESIZE;
    long long offset = pages_offset[this->numPages] ;
    this->Seek(offset,0);
    bFrame write_page;
    memset(write_page.field,0,FRAMESIZE);
    sprintf(write_page.field,"%s%d%s","page_id: " ,this->numPages, " data: this is just test!");
    this->WritePage(this->numPages,write_page);
    this->numPages++;
    return this->numPages-1;
}

int DataStorageManager::DSMgr::GetNumPages(){
    return this->numPages;
}

void DataStorageManager::DSMgr::SetUse(int index, int use_bits){
    this->pages_use_bit[index] = use_bits;
}
int DataStorageManager::DSMgr::GetUse(int index) {
    return this->pages_use_bit[index];
}

int DataStorageManager::DSMgr::GetIOCount() {
    return this->write_io_count + this->read_io_count;
}

int DataStorageManager::DSMgr::GetWriteIOCount() {
    return this->write_io_count;
}

int DataStorageManager::DSMgr::GetReadIOCount() {
    return this->read_io_count;
}