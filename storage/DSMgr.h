//
// Created by gloomy on 2019/11/30.
//

#ifndef STORAGEBUFFERAMANAGER_DSMGR_H
#define STORAGEBUFFERAMANAGER_DSMGR_H

#include <string>
#include "../buffer/Buffer.h"
namespace DataStorageManager {
    class DSMgr {
    public:
        DSMgr();

        int OpenFile(std::string filename);

        int CloseFile();

        bFrame ReadPage(int page_id);

        int WritePage(int page_id, bFrame frm);

        int Seek(int offset, int pos);

        FILE *GetFile();

        void IncNumPages();

        int GetNumPages();

        void SetUse(int index, int use_bits);

    private:
        ~DSMgr();
        FILE * currFile;
        int numPages;
        int pages_use_bit[MAXPAGES];
        int pages_offset[MAXPAGES];
        int io_count;

    };

}
#endif //STORAGEBUFFERAMANAGER_DSMGR_H
