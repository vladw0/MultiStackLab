#include "Stack.h"
#include "MultiStack.h"
int main(){
    TMultiStack<char> mStack(40,10);
    mStack.Push('t', 0);
    mStack.Push('q', 1);
    mStack.Push('w', 1);
    mStack.Push('e', 1);
    mStack.Push('r', 1);
    mStack.Push('y', 2);
    mStack.Push('u', 2);
    mStack.Push('i', 2);
    mStack.Push('o', 3);
    mStack.Push('p', 3);
    mStack.Push('a', 4);
    mStack.Push('a', 4);
    mStack.Push('s', 5); 
    mStack.Push('d', 6); 
    mStack.Push('f', 7); 
    mStack.Push('g', 7); 
    mStack.Push('h', 7); 
    mStack.Push('j', 8); 
    mStack.Push('k', 8); 
    mStack.Push('l', 9); 
    mStack.Push('z', 9); 

    mStack.Push('x', 1); 

    return 0; 
}