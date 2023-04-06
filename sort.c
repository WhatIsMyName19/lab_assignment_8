#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void swap (int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void heapify (int arr[], int size, int i)
{
	if (size == 1)
		return;
	
	int largest = i;
	int lChild = 2 * i + 1;
	int rChild = 2 * i + 2;
	
	if (lChild < size && arr[lChild] > arr[largest])
		largest = lChild;
	
	if (rChild < size && arr[rChild] > arr[largest])
		largest = rChild;
	
	if (largest != i)
	{
		swap(&arr[i], &arr[largest]);
		heapify(arr, size, largest);
	}
}

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{	
	// Sorts the array "arr" to become a max heap.
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(arr, n, i);
	}
	
	for (int i = n - 1; i >= 0; i--)
	{
		swap(&arr[0], &arr[i]);
		
		heapify(arr, i, 0);
	}
}

void merge(int pData[], int l, int m, int r)
{
	int lLength = (m - l) + 1;
	int rLength = r - m;
	
	int *tempL = (int*) malloc(lLength * sizeof(int));
	int *tempR = (int*) malloc(rLength * sizeof(int));
	
	extraMemoryAllocated += lLength * sizeof(int) + rLength * sizeof(int);
	
	int i, j, k;	
	
	for(i = 0; i < lLength; i++)
		tempL[i] = pData[l + i];
	
	for(j = 0; j < rLength; j++)
		tempR[j] = pData[m + 1 + j];
	
	i = 0;
	j = 0;
	
	for (k = l; k <= r; k++)
	{
		if ((i < lLength) && (j >= rLength || (tempL[i] <= tempR[j])))
		{
			pData[k] = tempL[i];
			i++;
		}
		
		else
		{
			pData[k] = tempR[j];
			j++;
		}
	}
	
	free(tempL);
	free(tempR);
	
	extraMemoryAllocated -= lLength * sizeof(int) + rLength * sizeof(int);
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	if (l < r)
	{
		int m = l + (r - l) / 2;
		
		mergeSort(pData, l, m);
		mergeSort(pData, m + 1, r);
		
		merge(pData, l, m, r);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// NOTE!!!: This method accesses out of bounds array indexes for the first 3 input text files!
// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}
