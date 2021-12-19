#include "iostream"

int main(int argc, char* argv[]) {
    int words = 0;
    bool lastSeparator = true;

    for(char* it = argv[1]; *it; ++it) {
        bool currentSeparator = *it == ' ' || *it == '\t' || *it == '\n' || *it == '\r';
        if (!currentSeparator && lastSeparator)
            ++words;
        lastSeparator = currentSeparator;
    }

    std::cout<<"Number of words: "<<words<<std::endl;
}
