#include <Windows.h>
#include "MergeSort.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "Russian");

	int numCores = coreNumber();
	std::cout << "\nКоличество ядер CPU: " << numCores << std::endl;
	int maxThreads = numCores;
	int activeThreads = 0;

	std::vector<int> largeVector;
	const long long size = 4'000'000;

	largeVectorFill(largeVector, size);

	mergeSort(largeVector, 0, largeVector.size() - 1, maxThreads, activeThreads);

	printArray(largeVector);

	return 0;
}