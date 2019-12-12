#include "buffer/BMgr.h"
int main() {
     FILE *test_file = fopen("../tests/data-5w-50w-zipf.txt","rw");
    int request_type,request_page;
    auto bmgr = new BufferManager::BMgr();
    while(fscanf(test_file,"%d,%d\n",&request_type,&request_page) ==2){
        bmgr->FixPage(request_page,request_type);
    }
    delete bmgr;

    return 0;
}