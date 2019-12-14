//
// Created by GLOOMY on 2019/11/30.
//

#include "BMgr.h"

BufferManager::BMgr::BMgr(){
    std::cout << "Initial Buffer Manager......"<<std::endl;
    std::cout << "Page Size: " << PAGESIZE << " Cache Capacity: " << BUFFERSIZE << std::endl;
    // initalize the buffer pool
    memset(this->buff_pool, 0, sizeof(buff_pool));
    // initalize the Hash table
    memset(this->ftop, 0, sizeof(ftop));
    memset(this->ptof, 0, sizeof (ptof));
    this->num_free = BUFFERSIZE;
    this->operation_count = 0;
    this->hit_count = 0;
    this->priority_queue = std::list<int>();
    this->dsMgr = new DataStorageManager::DSMgr();
}
BufferManager::BMgr::~BMgr() {
    this->WriteDirtys();
    std::cout << "operation count is: " << this->operation_count << std::endl;
    std::cout << "actual io count is: " << this->dsMgr->GetIOCount()<<"  reduce io rate: " << 1 - (this->dsMgr->GetIOCount()+0.0)/this->operation_count << std::endl;
    std::cout << "actual write io count is: " << this->dsMgr->GetWriteIOCount() << std::endl;
    std::cout << "actual read io count is: " << this->dsMgr->GetReadIOCount() << std::endl;
    std::cout << "cache hit time is: " << this->hit_count << " cache hit rate: " << (this->hit_count + 0.0)/this->operation_count << std::endl;
    delete this->dsMgr;

}
// Interface functions
int BufferManager::BMgr::FixPage(int page_id, int prot){
    this->operation_count++;
    while(page_id >= this->dsMgr->GetNumPages())
        FixNewPage();
    BCB *pb = ptof[Hash(page_id)];
    //try to find it in the buffer
    while(pb) {
        if(pb->page_id == page_id) {
            break;
        }
        pb = pb->next;
    }
    if (pb == nullptr) {
        int victim_frame_id = SelectVictim();
        ftop[victim_frame_id] = page_id;
        //judge whether the ftop is inited
        auto newBCB = new BCB{victim_frame_id, page_id, 1, prot, nullptr};
        if (ptof[Hash(page_id)] == nullptr) {
            ptof[Hash(page_id)] = newBCB;
            pb = ptof[Hash(page_id)];
        } else {
            pb = ptof[Hash(page_id)];
            while (pb->next != nullptr)
                pb = pb->next;
            pb->next = newBCB;
            pb = pb->next;
        }
        if( prot == 0) {
            this->dsMgr->ReadPage(page_id, buff_pool[pb->frame_id]);
        }else if(prot == 1){
            bFrame write_page;
            sprintf(write_page.field, "%s%d%s", "page_id: ", pb->frame_id, " data: this is just test!--version2");
            buff_pool[pb->frame_id] = write_page;
        }
        this->priority_queue.push_front(pb->frame_id);
    }else {
        this->hit_count ++;
        this->RemoveLRUEle(pb->frame_id);
        this->priority_queue.push_front(pb->frame_id);
        if(prot == 1){
            bFrame write_page;
            sprintf(write_page.field, "%s%d%s", "page_id: ", pb->frame_id, " data: this is just test!--version2");
            buff_pool[pb->frame_id] = write_page;
            pb->dirty = 1;
        }

    }



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
    if(nff > 0) {               // Buffer has empty
        --num_free;
        int frame_id = BUFFERSIZE - nff;
        return frame_id;
    }else {                     // Buffer is full
        int victim_frame_id = this->priority_queue.back();
        int page_id = ftop[victim_frame_id];
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
        if(pb->dirty == 1) {
            this->dsMgr->WritePage(pb->frame_id, buff_pool[pb->frame_id]);
        }
        this->RemoveBCB(ptof[Hash(page_id)], page_id);
        this->RemoveLRUEle(victim_frame_id);
        return victim_frame_id;
    }

}
int BufferManager::BMgr::Hash(int page_id){
    return page_id % BUFFERSIZE;
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
    for(int i = 0; i < BUFFERSIZE; ++i) {
        BCB* pb = ptof[i];
        while(pb) {
            if(pb->dirty == 1) {
                this->dsMgr->WritePage(pb->frame_id, buff_pool[pb->frame_id]);
            }
            pb = pb->next;
        }
    }
}

bFrame BufferManager::BMgr::GetFrame(int frame_id){
    return buff_pool[frame_id];
}
