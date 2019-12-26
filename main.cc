#include "buffer/BMgr.h"
int main() {
    FILE *test_file = fopen("../tests/data-5w-50w-zipf.txt","rw");
    int request_type,request_page;
    auto bmgr = new BufferManager::BMgr();
    int operation_count = 0;
    while(fscanf(test_file,"%d,%d\n",&request_type,&request_page) ==2){
        bmgr->FixPage(request_page,request_type);
	operation_count += 1;
	if(operation_count %100 == 0){
	    printf("\33[2K\r");
	    printf("Running Progress:%3d%% ....                   [",operation_count*100/500000);
	    for(int i = 0; i < 20*operation_count/500000;i++)
		printf(">");
	    for(int i = 0; i < 20- 20*operation_count/500000;i++)
		printf("-");
	    printf("]");
	    if( operation_count == 500000)
		printf("comlete!");
	   fflush(stdout);
	}
    }
    printf("\n");
    delete bmgr;

    return 0;
}
