//
// Created by zhenliu on 12/5/19.
//

#include "../buffer/BMgr.h"
#include <gtest/gtest.h>

TEST(BufferManagerTotalTest,ZipfDistribution){
    FILE *test_file = fopen("data-5w-50w-zipf.txt","rw");
    ASSERT_TRUE(test_file != NULL);
    int read_type,read_page;
    while(fscanf(test_file,"%d,%d\n",&read_type,&read_page) ==2){
        bFrame page_content;
        memset(page_content.field,0,FRAMESIZE);
        sprintf(page_content.field,"%s%d%s","page_id: " ,read_page , " data: this is just test!");
        if(read_type == 0)
            printf("the operation type is read and the page id is: %d\n",read_page);
        else
            printf("the operation type is write and the page id is: %d\n",read_page);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}