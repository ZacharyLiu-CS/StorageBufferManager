//
// Created by GLOOMY on 2019/11/30.
//

#include "../storage/DSMgr.h"
#include <gtest/gtest.h>

TEST(DataStorageManagerTest,Construction){
    auto dsmgr = DataStorageManager::DSMgr();
    ASSERT_EQ(dsmgr.GetNumPages(),dsmgr.GetNumPages());
}


TEST(DataStorageManagerTest,ReadPage){
    bFrame read_page;
    auto dsmgr = DataStorageManager::DSMgr();
    dsmgr.ReadPage(0,read_page);
    ASSERT_EQ(0,strcmp(read_page.field,"page_id: 0 data: this is just test!"));
    dsmgr.ReadPage(MAXPAGES-1,read_page);
    ASSERT_EQ(0,strcmp(read_page.field,"page_id: 49999 data: this is just test!"));

}

TEST(DataStorageManagerTest,WritePage){
    bFrame write_page_100{"page_id: 100 data: this is just test!---version2"};
    bFrame write_page_101{"page_id: 101 data: this is just test!---version2"};
    auto dsmgr = DataStorageManager::DSMgr();
    ASSERT_EQ(0,dsmgr.WritePage(100,write_page_100));
    ASSERT_EQ(0,dsmgr.WritePage(101,write_page_101));
    bFrame read_frame;
    dsmgr.ReadPage(100,read_frame);
    ASSERT_EQ(0,strcmp(read_frame.field,"page_id: 100 data: this is just test!---version2"));
    dsmgr.ReadPage(101,read_frame);
    ASSERT_EQ(0,strcmp(read_frame.field,"page_id: 101 data: this is just test!---version2"));

}

TEST(DataStorageManagerTest,ReadCount){
    auto dsmgr = DataStorageManager::DSMgr();
    bFrame read_frame;
    for(int i = 1000; i < 2000; i++){
        dsmgr.ReadPage(i,read_frame);
    }
    ASSERT_EQ(1000,dsmgr.GetIOCount());
}
TEST(DataStorageManagerTest,WriteCount){
    auto dsmgr = DataStorageManager::DSMgr();
    for(int i = 100; i <200;i++) {
        bFrame write_page;
        sprintf(write_page.field, "%s%d%s", "page_id: ", i, " data: this is just test!--version2");
        dsmgr.WritePage(i,write_page);
    }
    ASSERT_EQ(100,dsmgr.GetIOCount());

}
TEST(DataStorageManagerTest,IncNumPages){
    auto dsmgr = DataStorageManager::DSMgr();
    int numpages = dsmgr.GetNumPages();
    dsmgr.IncNumPages();
    ASSERT_EQ(numpages+1,dsmgr.GetNumPages());
    bFrame read_frame;
    dsmgr.ReadPage(numpages,read_frame);
    char expetedstr[FRAMESIZE];
    sprintf(expetedstr,"page_id: %d data: this is just test!",numpages);
    ASSERT_EQ(0,strcmp(read_frame.field,expetedstr));

}
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
