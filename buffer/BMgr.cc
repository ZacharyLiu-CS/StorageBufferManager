//
// Created by GLOOMY on 2019/11/30.
//

#include "BMgr.h"

BufferManager::BMgr::BMgr(){
    // initalize the buffer pool
    memset(this->buff_pool, 0, sizeof(buff_pool));
    // initalize the Hash table
    memset(this->ftop, 0, sizeof(ftop));
    memset(this->ptof, 0, sizeof (ptof));
    this->num_free = DEFBUFSIZE;
    this->dsMgr = new DataStorageManager::DSMgr();
}
BufferManager::BMgr::~BMgr() {
    this->WriteDirtys();
    delete this->dsMgr;
    std::cout << "cache miss rate:\t" << (this->cache_miss_time+0.0)/this->operation_count << std::endl;

}
// Interface functions
int BufferManager::BMgr::FixPage(int page_id, int prot){
    this->operation_count++;
    while(page_id >= this->dsMgr->GetNumPages())
        FixNewPage();
    BCB *pb = ptof[Hash(page_id)];
    while(pb) {
        if(pb->page_id == page_id) {
            break;
        }
        pb = pb->next;
    }
    if(pb == nullptr) {         // page not in buffer
        int victim = SelectVictim();
        this->cache_miss_time++;
        this->dsMgr->ReadPage(page_id, buff_pool[victim]);
        ftop[victim] = page_id;
        if(ptof[Hash(page_id)] == nullptr) {
            ptof[Hash(page_id)] = new BCB();
            pb = ptof[Hash(page_id)];
            pb->page_id = page_id;
            pb->frame_id = victim;
            pb->count = 1;
            pb->dirty = prot;
            pb->next = nullptr;
        }else {
            pb = ptof[Hash(page_id)];
            while(pb->next != nullptr) {
                pb = pb->next;
            }
            pb->next = new BCB;
            pb = pb->next;
            pb->page_id = page_id;
            pb->frame_id = victim;
            pb->count = 1;
            pb->dirty = prot;
            pb->next = nullptr;
        }
    }
    this->priority_queue.push_back(pb->frame_id);
    return pb->frame_id;
}
void BufferManager::BMgr::FixNewPage(){
    this->dsMgr->IncNumPages();
}
int BufferManager::BMgr::UnfixPage(int page_id){
    BCB* pb = ptof[Hash(page_id)];
    while(pb) {
        if(pb->page_id == page_id) {
            break;
        }
        pb = pb->next;
    }
    if(pb != nullptr) {
        if(pb->count > 0) {
            --(pb->count);
            return pb->frame_id;
        }else {
            std::cerr << "BMgr: UnfixPage failed: pin-count < 0" << std::endl;
        }
    }else {
        std::cerr << "BMgr: UnfixPage failed: page_id not found" << std::endl;
    }
    return 0;
}
int BufferManager::BMgr::NumFreeFrames(){
    return num_free;
}
// Internal Functions
int BufferManager::BMgr::SelectVictim(){
    int nff = this->NumFreeFrames();
    if(nff > 0) {               // Buffer 有空余
        --num_free;
        int frame_id = DEFBUFSIZE - nff;
        return frame_id;
    }else {                     // Buffer 已满
        int vic = this->priority_queue.back();
        int page_id = ftop[vic];
        BCB* pb = ptof[Hash(page_id)];
        while(pb) {
            if(pb->page_id == page_id) {
                break;
            }
            pb = pb->next;
        }
        if(pb == nullptr) {
            std::cerr << "BMgr: Select Victim failed: BCB not found" << std::endl;
            return -1;
        }
        if(pb->dirty) {

            this->dsMgr->WritePage(pb->frame_id, buff_pool[pb->frame_id]);
        }
        RemoveBCB(ptof[Hash(page_id)], page_id);
        RemoveLRUEle(vic);
        return vic;
    }

}
int BufferManager::BMgr::Hash(int page_id){
    return page_id % DEFBUFSIZE;
}
void BufferManager::BMgr::RemoveBCB(BCB * ptr, int page_id){
    if(ptr == nullptr) {
        std::cerr << "BMgr: Remove BCB failed: page_id not found" << std::endl;
        return;
    }
    BCB* pre = nullptr;
    while(ptr->page_id != page_id && ptr != nullptr) {
        pre = ptr;
        ptr = ptr->next;
    }
    if(pre == nullptr) {    // page_id is the first
        ptof[Hash(page_id)] = ptr->next;
        delete ptr;
    }else {
        pre->next = pre->next->next;
        delete ptr;
    }


}
void BufferManager::BMgr::RemoveLRUEle(int frame_id){
    this->priority_queue.remove(frame_id);
}
void BufferManager::BMgr::SetDirty(int frame_id){
    int page_id = ftop[frame_id];
    BCB* pb = ptof[Hash(page_id)];
    while(pb) {
        if(pb->frame_id == frame_id) {
            break;
        }
        pb = pb->next;
    }
    if(pb == nullptr) {
        std::cerr << "BMgr: Set Dirty failed: frame_id not found" << std::endl;
        return;
    }
    pb->dirty = 1;
}
void BufferManager::BMgr::UnsetDirty(int frame_id){
    int page_id = ftop[frame_id];
    BCB* pb = ptof[Hash(page_id)];
    while(pb) {
        if(pb->frame_id == frame_id) {
            break;
        }
        pb = pb->next;
    }
    if(pb == nullptr) {
        std::cerr << "BMgr: Unset Dirty failed: frame_id not found" << std::endl;
        return;
    }
    pb->dirty = 0;
}
void BufferManager::BMgr::WriteDirtys(){
    // Traverse the BCB list
    for(int i = 0; i < DEFBUFSIZE; ++i) {
        BCB* pb = ptof[i];
        while(pb) {
            if(pb->dirty == 1) {
                this->dsMgr->WritePage(pb->frame_id, buff_pool[pb->frame_id]);
            }
            pb = pb->next;
        }
    }
}
void BufferManager::BMgr::PrintFrame(int frame_id){
    std::cout << buff_pool[frame_id].field << std::endl;
}
