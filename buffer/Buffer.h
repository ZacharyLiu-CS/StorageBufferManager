//
// Created by gloomy on 2019/10/9.
//

#ifndef STORAGEBUFFERAMANAGER_BUFFER_H
#define STORAGEBUFFERAMANAGER_BUFFER_H

#include "../include/env.h"

struct bFrame{
    char field[FRAMESIZE];
};
bFrame buf[DEFBUFSIZE];
struct  Frame{
    int frame_id;
    int offset;

};
struct BCB{
    BCB();
    int page_id;
    int frame_id;
    int count;
    int dirty;
    BCB *next;
};
#endif //STORAGEBUFFERAMANAGER_BUFFER_H
