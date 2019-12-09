//
// Created by gloomy on 2019/10/9.
//

#ifndef STORAGEBUFFERAMANAGER_BUFFER_H
#define STORAGEBUFFERAMANAGER_BUFFER_H

#include "../include/env.h"

struct Frame{
    int frame_id;
    int offset;
};
struct  BCB{
    BCB();
    int frame_id;
    int page_id;
    int count;
    int dirty;
    BCB *next;
};
#endif //STORAGEBUFFERAMANAGER_BUFFER_H
