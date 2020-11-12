#ifndef DRAWINGCMP
#define DRAWINGCMP
    
void DrawingCmp( const char* outputfilename, int size,const char** fitfilenames);

#if defined(__CLING__)
#include "DrawingCmp.cpp"
#endif

#endif