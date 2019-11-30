//
// Created by gloomy on 2019/11/30.
//

#ifndef STORAGEBUFFERAMANAGER_DSMGR_H
#define STORAGEBUFFERAMANAGER_DSMGR_H

#include <string>
#include <iostream>
#include "../buffer/Buffer.h"

namespace DataStorageManager {
    class DSMgr {
    public:
        DSMgr();

        int OpenFile(std::string filename);

        int CloseFile();

        int ReadPage(int page_id, bFrame &read_frame);

        int WritePage(int page_id, bFrame &write_frame);

        int Seek(int offset, int pos);

        FILE *GetFile();

        void IncNumPages();

        int GetNumPages();

        void SetUse(int index, int use_bits);

        int GetUse(int index);

    private:
        ~DSMgr();
        FILE * currFile;
        int numPages;
        int pages_use_bit[MAXPAGES];
        int pages_offset[MAXPAGES];
        int read_io_count;
        int write_io_count;

    };

}
#endif //STORAGEBUFFERAMANAGER_DSMGR_H
