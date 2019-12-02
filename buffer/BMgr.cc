//
// Created by GLOOMY on 2019/11/30.
//

#include "BMgr.h"

BufferManager::BMgr::BMgr(){

}
// Interface functions
int BufferManager::BMgr::FixPage(int page_id, int prot){

}
void BufferManager::BMgr::FixNewPage(){

}
int BufferManager::BMgr::UnfixPage(int page_id){

}
int BufferManager::BMgr::NumFreeFrames(){

}
// Internal Functions
int BufferManager::BMgr::SelectVictim(){

}
int BufferManager::BMgr::Hash(int page_id){

}
void BufferManager::BMgr::RemoveBCB(BCB * ptr, int page_id){

}
void BufferManager::BMgr::RemoveLRUEle(int frid){

}
void BufferManager::BMgr::SetDirty(int frame_id){

}
void BufferManager::BMgr::UnsetDirty(int frame_id){

}
void BufferManager::BMgr::WriteDirtys(){

}
void BufferManager::BMgr::PrintFrame(int frame_id){

}

// Hash Table
int ftop[DEFBUFSIZE];
BCB* ptof[DEFBUFSIZE];