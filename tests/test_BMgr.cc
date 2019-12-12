
#include "../buffer/BMgr.h"
#include <gtest/gtest.h>
#include "../buffer/BMgr.h"

TEST(BufferManagerTest,FixPage){
    auto bmgr = new BufferManager::BMgr();
    for(int i = 0 ;i < BUFFERSIZE; i++)
        bmgr->FixPage(i,0);
    for(int i = 0 ;i < MAXPAGES; i++)
        bmgr->FixPage(i%BUFFERSIZE,1);
    delete bmgr;
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}