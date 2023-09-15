﻿#include "MergeSort.h"
#include <cstdlib>
#include <future>
#include <mutex>

std::mutex activeThreadsMutex;

void largeVectorFill(std::vector<int>& largeVector, long long size)
{
    largeVector.reserve(size);

    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "\nЗаполняем массив случайными числами" << std::endl;

    for (int i = 0; i < size; i++)
    {
        largeVector.push_back(std::rand() % 101);
    }

    std::cout << "Массив размером [" << size << "] заполнен случайными числами" << std::endl;
}

void merge(std::vector<int>& array, int left, int middle, int right)
{
    int sizeLeft = middle - left + 1;
    int sizeRight = right - middle;

    std::vector<int> leftArray(sizeLeft);
    std::vector<int> rightArray(sizeRight);

    for (int i = 0; i < sizeLeft; ++i)
        leftArray[i] = array[left + i];
    for (int j = 0; j < sizeRight; ++j)
        rightArray[j] = array[middle + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < sizeLeft && j < sizeRight) {
        if (leftArray[i] <= rightArray[j]) {
            array[k] = leftArray[i];
            ++i;
        }
        else {
            array[k] = rightArray[j];
            ++j;
        }
        ++k;
    }

    while (i < sizeLeft) {
        array[k] = leftArray[i];
        ++i;
        ++k;
    }

    while (j < sizeRight) {
        array[k] = rightArray[j];
        ++j;
        ++k;
    }
}

void mergeSort(std::vector<int>& array, int left, int right, int& maxThreads, int& activeThreads)
{
    if (left < right) {
        int middle = left + (right - left) / 2;

        std::unique_lock<std::mutex> mutexLock(activeThreadsMutex);
        if (activeThreads < maxThreads && right - left > 10000) {
            ++activeThreads;
            mutexLock.unlock();

            std::future<void> leftFuture = std::async(std::launch::async, [&] {
                mergeSort(array, left, middle, maxThreads, activeThreads);
                });
            std::future<void> rightFuture = std::async(std::launch::async, [&] {
                mergeSort(array, middle + 1, right, maxThreads, activeThreads);
                });

            leftFuture.get();
            rightFuture.get();

            mutexLock.lock();
            --activeThreads;
        }
        else {
            mutexLock.unlock();

            mergeSort(array, left, middle, maxThreads, activeThreads);
            mergeSort(array, middle + 1, right, maxThreads, activeThreads);
        }

        merge(array, left, middle, right);
    }
}

int coreNumber()
{
    std::cout << "\nВычисляем количество ядер CPU..." << std::endl;
    int numCores = std::thread::hardware_concurrency();
    if (numCores > 0) {

        return numCores;
    }
    else {
        std::cout << "\nОШИБКА: вычисления количества ядер CPU\n" << std::endl;
        return 0;
    }
}

void printArray(const std::vector<int>& array)
{
    for (const int& num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}