//
// Created by sharvil on 17/9/18.
//

#include "Session.h"


Session::Session(int numberOfPapers) {
    this->numberOfPapers = numberOfPapers;
    this->papers = new int[numberOfPapers];
}

