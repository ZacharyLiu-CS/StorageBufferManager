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

        int OpenFile(std::string filename);

        int NewFile(std::string filename);

        int CloseFile();

        int ReadPage(int page_id, bFrame &read_frame);

        int WritePage(int page_id, bFrame &write_frame);

        int Seek(long long offset, long long pos);

        FILE *GetFile();

        void IncNumPages();

        int GetNumPages();

        void SetUse(int page_id, int use_bits);

        int GetUse(int page_id);

        int GetIOCount();


    private:
        ~DSMgr();
        FILE * currFile;
        int numPages;
        int pages_use_bit[MAXPAGES];
        long long  pages_offset[MAXPAGES];
        int read_io_count;
        int write_io_count;

    };

}
#endif //STORAGEBUFFERAMANAGER_DSMGR_H
