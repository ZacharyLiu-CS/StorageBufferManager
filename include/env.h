//
// Created by gloomy on 2019/11/30.
//

#ifndef STORAGEBUFFERAMANAGER_ENV_H
#define STORAGEBUFFERAMANAGER_ENV_H

#include <bits/stdc++.h>

#define MAXPAGES 50000  // define the disk max pages nums
#define FRAMESIZE 100  // define the frame max size
#define PAGESIZE 4096   // define the page size
#define DEFBUFSIZE 1024 // define the buffer size

// the bFrame is not just the data stored in buffer but also the data in database file
struct bFrame{
    char field[FRAMESIZE];
};

#endif //STORAGEBUFFERAMANAGER_ENV_H
