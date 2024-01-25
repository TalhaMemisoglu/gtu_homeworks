#ifndef OSMANAGER_H
#define OSMANAGER_H

#include "file.h"

namespace MemOS
{
    class OSManager
    {
    public:
        OSManager();
        void start();
    private:
        file* crloc;
        file* root;
    };
}

#endif //OSMANAGER_H