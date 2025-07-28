// FifoAverager.cpp
#include "FifoAverager.h"

FifoAverager::FifoAverager() {
    reset(); // Initialize the FIFO and reset all elements to zero
}

void FifoAverager::addNumber(float number) {
    numbers[index] = number; // Add the new number at the current index
    index = (index + 1) % 10; // Increment index and wrap around using modulo
    if (count < 10) {
        count++; // Only increment count until it reaches 10
    }
}

float FifoAverager::getAverage() {
    float sum = 0;
    for (int i = 0; i < count; i++) {
        sum += numbers[i]; // Sum all numbers
    }
    
    // If the FIFO has not yet been filled up, return zero.
    return count < 10 ? 0 : sum / count; // Calculate average
}

float FifoAverager::getRange() {
  // Range is the highest minus the lowest value of the numbers.
    float max  = -100.0;
    float min = 200.0;

    for (int i = 0; i < count; i++) {
        if (numbers[i] < min ) min = numbers[i];
        if (numbers[i] > max ) max = numbers[i];
    }
    
    return max - min;  // Return range.
}


void FifoAverager::reset() {
    for (int i = 0; i < 10; i++) {
        numbers[i] = 0; // Reset all elements to zero
    }
    index = 0; // Reset index
    count = 0; // Reset count
}
