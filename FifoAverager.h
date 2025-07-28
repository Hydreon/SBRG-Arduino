// FifoAverager.h
#ifndef FIFOAVERAGER_H
#define FIFOAVERAGER_H

class FifoAverager {
public:
    FifoAverager();  // Constructor
    void addNumber(float number);
    float getAverage();
    float getRange();
    void reset();

private:
    float numbers[10];
    int index;
    int count = 0;
};

#endif // FIFOAVERAGER_H
