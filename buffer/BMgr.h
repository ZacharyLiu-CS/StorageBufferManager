//
// Created by gloomy on 2019/11/30.
//

#ifndef STORAGEBUFFERAMANAGER_BMGR_H
#define STORAGEBUFFERAMANAGER_BMGR_H

#include "../include/env.h"
#include "Buffer.h"
namespace BufferManager {
    class BMgr {
    public:
        BMgr();

        // Interface functions
        int FixPage(int page_id, int prot);

        void FixNewPage();

        int UnfixPage(int page_id);

        int NumFreeFrames();

        // Internal Functions
        int SelectVictim();

        int Hash(int page_id);

        void RemoveBCB(BCB *ptr, int page_id);

        void RemoveLRUEle(int frid);

        void SetDirty(int frame_id);

        void UnsetDirty(int frame_id);

        void WriteDirtys();

        void PrintFrame(int frame_id);

    private:
        // Hash Table
        int ftop[DEFBUFSIZE];
        BCB *ptof[DEFBUFSIZE];
    };
}
#endif //STORAGEBUFFERAMANAGER_BMGR_H
