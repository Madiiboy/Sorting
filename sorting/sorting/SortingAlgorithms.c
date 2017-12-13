#define _CRT_SECURE_NO_WARNINGS
#include "SortingAlgorithms.h"
#include "Statistics.h"
#include "Array.h"
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#define FALSE 0
#define TRUE 1

int isImplemented(SortingAlgorithm algorithm)
{
	switch (algorithm)
	{
      case BUBBLE_SORT:
//      case INSERTION_SORT:
//      case SELECTION_SORT:
//      case QUICK_SORT:
//      case MERGE_SORT:
            return 1;
        default:
            return 0;
	}
}


/******************************************************************************************/
/* Era algoritmer har: */

static void bubbleSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{
	//tidskomplexitet o(n^2)
	//en bubbelsort funktion som sorterar efter storlek
	int  i, k;
	
	//loopar genom alla nummer i arrayen
	for (i = 0; lessThan(i, size-1, statistics); i++)
	{

		//loppar genom alla element framför element i
		for (k = 0; lessThan(k, size-1-i, statistics); k++)
		{
			if (greaterThan(arrayToSort[k], arrayToSort[k + 1], statistics))
			{
				swapElements(&arrayToSort[k], &arrayToSort[k+1], statistics);
			}
		}
	}


}

static void insertionSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{
	//tidskomplexitet o(n^2)
	int i, j;

	for (i = 1; lessThan(i, size, statistics); i++)
	{
		j = i;

		//sätter in elementet på rätt plats i arrayen

		while ( lessThan( arrayToSort[j], arrayToSort[j-1], statistics ) && greaterThan( j , 0, statistics))
		{
			swapElements(&arrayToSort[j], &arrayToSort[j - 1], statistics);
			j--;
		}
		
	}


}

static void selectionSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{
	int i, j;
	int* pArray; //pekare till elementet i arrayToSort

	for (i = 0; lessThan(i, size - 1, statistics); i++)
	{
		pArray = &arrayToSort[i];

		for (j = i; lessThan(j, size, statistics); j++)
		{
			if (lessThan(arrayToSort[j], *pArray, statistics))
				pArray = &arrayToSort[j];
		}
				swapElements(&arrayToSort[i], pArray, statistics);
	}


}

static void mergeSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{


}

static void quickSort(ElementType* arrayToSort, size_t size, Statistics* statistics)
{
	quick(arrayToSort, 0, size - 1, statistics);
}

void quick(ElementType* arrayToSort, int start, int end, Statistics* statistics) {

	if (!equalTo(end, -1, statistics)) {

		if (lessThan(start, end, statistics))
		{

			int pivot = partition(arrayToSort, start, end, statistics);
			quick(arrayToSort, start, pivot - 1, statistics);
			quick(arrayToSort, pivot + 1, end, statistics);
		}
	}
}

//tar sista elementet som pivot och lägger alla mindre värden till vänster om pivot och större till höger
int partition(ElementType* arrayToSort, int start, int end, Statistics* statistics) {

	int pivot = arrayToSort[end];
	int i = start, j;

	for (j = start; lessThan(j, end, statistics); j++)
	{
		if (lessThanOrEqualTo(arrayToSort[j], pivot, statistics))
		{
			swapElements(&arrayToSort[j], &arrayToSort[i], statistics);
			i++;
		}

	}
	swapElements(&arrayToSort[i], &arrayToSort[end], statistics);
	return i;
}
/******************************************************************************************/


char* getAlgorithmName(SortingAlgorithm algorithm)
{
	/* Ar inte strangen vi allokerar lokal for funktionen?
	   Nej, inte i detta fall. Strangkonstanter ar ett speciallfall i C */
	switch (algorithm)
	{
        case BUBBLE_SORT:	 return "  Bubble sort ";
        case SELECTION_SORT: return "Selection sort";
        case INSERTION_SORT: return "Insertion sort";
        case MERGE_SORT:	 return "  Merge sort  ";
        case QUICK_SORT:	 return "  Quick sort  ";
        default: assert(0 && "Ogiltig algoritm!"); return "";
	}
}

// Sorterar 'arrayToSort' med 'algorithmToUse'. Statistik for antal byten och jamforelser hamnar i *statistics
static void sortArray(ElementType* arrayToSort, size_t size, SortingAlgorithm algorithmToUse, Statistics* statistics)
{
	if(statistics != NULL)
		resetStatistics(statistics);

	switch (algorithmToUse)
	{
	case BUBBLE_SORT:	 bubbleSort(arrayToSort, size, statistics); break;
	case SELECTION_SORT: selectionSort(arrayToSort, size, statistics); break;
	case INSERTION_SORT: insertionSort(arrayToSort, size, statistics); break;
	case MERGE_SORT:	 mergeSort(arrayToSort, size, statistics); break;
	case QUICK_SORT:	 quickSort(arrayToSort, size, statistics); break;
	default:
		assert(0 && "Ogiltig algoritm!");
	}
}

// Forbereder arrayer for sortering genom att allokera minne for dem, samt intialisera dem
static void prepareArrays(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], const unsigned int sizes[])
{
	assert(isImplemented(algorithm));

	int i;
	int totalArraySize;

	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		totalArraySize = sizeof(ElementType)*sizes[i];
		sortingArray[i].arrayToSort = malloc(totalArraySize);
		memcpy(sortingArray[i].arrayToSort, arrays[i], totalArraySize);

		sortingArray[i].algorithm = algorithm;
		sortingArray[i].arraySize = sizes[i];
		resetStatistics(&sortingArray[i].statistics);
	}
}

// Sorterar arrayerna
static void sortArrays(SortingArray toBeSorted[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		SortingArray* current = &toBeSorted[i];
		sortArray(current->arrayToSort, current->arraySize, current->algorithm, &current->statistics);
	
		if (!isSorted(current->arrayToSort, current->arraySize))
		{
			printf("Fel! Algoritmen %s har inte sorterat korrekt!\n", getAlgorithmName(current->algorithm));
			printf("Resultatet är: \n");
			printArray(current->arrayToSort, current->arraySize, stdout);
			assert(0); // Aktiveras alltid
		}
	}
}

void printResult(SortingArray sortedArrays[], FILE* file)
{
	assert(file != NULL);

	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		fprintf(file, "%4d element: ", sortedArrays[i].arraySize);
		printStatistics(&sortedArrays[i].statistics, file);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void sortAndPrint(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], unsigned int sizes[], FILE* file)
{
	assert(file != NULL);

	prepareArrays(sortingArray, algorithm, arrays, sizes);
	sortArrays(sortingArray);
	printResult(sortingArray, file);
}

void freeArray(SortingArray sortingArray[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		if (sortingArray[i].arrayToSort != NULL)
			free(sortingArray[i].arrayToSort);
		sortingArray[i].arrayToSort = NULL;
		sortingArray[i].arraySize = 0;
		resetStatistics(&sortingArray[i].statistics);
	}
}
