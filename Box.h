#ifndef BOX_H_INCLUDED
#define BOX_H_INCLUDED

class Box {
public:
    float boxWidth;
    float boxHeight;
    float boxDepth;

    Box(){
        boxWidth = 1.0f;
        boxHeight = 1.0f;
        boxDepth = 3.0f;
    }
};

#endif // BOX_H_INCLUDED