
#include "../buffer/BMgr.h"
#include <gtest/gtest.h>

TEST(BufferManagerTest,FixPage){
    auto bmgr = new BufferManager::BMgr();
    for(int i = 0 ;i < BUFFERSIZE; i++){
        bmgr->FixPage(i,0);
    }
    for(int i = 0 ;i < BUFFERSIZE; i++)
        bmgr->FixPage(i,1);
    delete bmgr;
    bmgr = new BufferManager::BMgr();
    bFrame read_frame_expected;
    for(int i = 0; i < BUFFERSIZE; i++){
	int frame_id = bmgr->FixPage(i,0);
	auto read_frame = bmgr->GetFrame(frame_id);
	sprintf(read_frame_expected.field, "%s%d%s", "page_id: ", i, " data: this is just test!--version2");
	ASSERT_EQ(0,strcmp(read_frame_expected.field,read_frame.field));
    }
    delete bmgr;
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
