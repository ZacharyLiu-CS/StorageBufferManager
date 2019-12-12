//
// Created by gloomy on 2019/11/30.
//

#ifndef STORAGEBUFFERAMANAGER_BMGR_H
#define STORAGEBUFFERAMANAGER_BMGR_H

#include "../include/env.h"
#include "Buffer.h"
#include "../storage/DSMgr.h"
namespace BufferManager {

    class BMgr {
    public:
        BMgr();
        ~BMgr();

        // Interface functions
        int FixPage(int page_id, int prot);

        void FixNewPage();

        int UnfixPage(int page_id);

        int NumFreeFrames();

        // Internal Functions
        int SelectVictim();

        int Hash(int page_id);

        void RemoveBCB(BCB *ptr, int page_id);

        void RemoveLRUEle(int frame_id);

        void SetDirty(int frame_id);

        void UnsetDirty(int frame_id);

        void WriteDirtys();

	bFrame GetFrame(int frame_id);
    private:
        // Hash Table
        int ftop[BUFFERSIZE];
        BCB *ptof[BUFFERSIZE];
        bFrame buff_pool[BUFFERSIZE];
        std::list<int> priority_queue;
        DataStorageManager::DSMgr *dsMgr;
        int num_free;
        int operation_count;
    };
}
#endif //STORAGEBUFFERAMANAGER_BMGR_H
