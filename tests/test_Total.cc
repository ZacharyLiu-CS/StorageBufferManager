//
// Created by zhenliu on 12/5/19.
//

#include "../buffer/BMgr.h"
#include <gtest/gtest.h>

TEST(BufferManagerTotalTest,ZipfDistribution){
    FILE *test_file = fopen("data-5w-50w-zipf.txt","rw");
    ASSERT_TRUE(test_file != NULL);
    int request_type,request_page;
    auto bmgr = new BufferManager::BMgr();
    while(fscanf(test_file,"%d,%d\n",&request_type,&request_page) ==2){
        bmgr->FixPage(request_page,request_type);
    }
    delete bmgr;
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
