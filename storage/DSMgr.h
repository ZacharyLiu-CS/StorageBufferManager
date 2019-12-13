//
// Created by gloomy on 2019/11/30.
//

#ifndef STORAGEBUFFERAMANAGER_DSMGR_H
#define STORAGEBUFFERAMANAGER_DSMGR_H

#include "../buffer/Buffer.h"

namespace DataStorageManager {

    class DSMgr {
    public:
        DSMgr();

        ~DSMgr();

        int OpenFile(std::string filename);

        int NewFile(std::string filename);

        int CloseFile();

        int ReadPage(int page_id, bFrame &read_frame);

        int WritePage(int page_id, bFrame &write_frame);

        int Seek(long long offset, long long pos);

        FILE *GetFile();

        int IncNumPages();

        int GetNumPages();

        void SetUse(int page_id, int use_bits);

        int GetUse(int page_id);

        int GetIOCount();

        int GetWriteIOCount();

        int GetReadIOCount();

    private:
        std::string fileName;
        FILE * currFile;
        int numPages;
        int *pages_use_bit;
        long long  *pages_offset;
        int read_io_count;
        int write_io_count;

    };

}
#endif //STORAGEBUFFERAMANAGER_DSMGR_H
