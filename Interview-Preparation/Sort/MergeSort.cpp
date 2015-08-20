#include<iostream>
#define SIZE(X) (sizeof(X)/sizeof(X[0]))

using namespace std;

void merge(int original[], int temp[], int low, int mid,int  high){
	for(int i = low; i <= high; i++){
		temp[i] = original[i];
	}

	int leftlist = low;
	int rightlist = mid+1;
	int cur = low;
	while(leftlist <= mid && rightlist <= high){
		if(temp[leftlist] <= temp[rightlist]){
			original[cur] = temp[leftlist];
			leftlist++;
		}
		else{
			original[cur] = temp[rightlist];
			rightlist++;
		}
		cur++;
	}
	while(leftlist <= mid){
		original[cur] = temp[leftlist];
		cur++;
		leftlist++;
	}
}
void mergeSort(int original[], int temp[], int low, int high){
	if(low >= high){
		return;
	}
	int mid = (low+high)/2;
	mergeSort(original, temp, low, mid);
	mergeSort(original, temp, mid+1, high);
	merge(original, temp, low, mid, high);
}

void quickSort(int values[], int low, int high){
	int pivot = values[(low+high)/2];
	int i = low;
	int j = high;

	while(i <= j){

		while(values[i] < pivot){
			i++;
		}

		while(values[j] > pivot){
			j--;
		}

		if(i <= j){
			int temp = values[i];
			values[i] = values[j];
			values[j] = temp;
			i++;
			j--;
		}
	}

	if(low < j)
		quickSort(values, low, j);
	if(i < high)
		quickSort(values, i, high);
}

void quickSort1(int arr[], int left, int right) {
      int i = left, j = right;
      int tmp;
      int pivot = arr[(left + right) / 2];

      /* partition */
      while (i <= j) {
            while (arr[i] < pivot)
                  i++;
            while (arr[j] > pivot)
                  j--;
            if (i <= j) {
                  tmp = arr[i];
                  arr[i] = arr[j];
                  arr[j] = tmp;
                  i++;
                  j--;
            }
      };

      /* recursion */
      if (left < j)
            quickSort(arr, left, j);
      if (i < right)
            quickSort(arr, i, right);
}

void Sort(int original[], int size){
	int temp[size];
	mergeSort(original, temp, 0, size-1);
}

int main(){
	int array[] = {10, 1, 5, 29, 50, 8};
	//Sort(array,SIZE(array));
	quickSort(array, 0, SIZE(array)-1);
	for(int i = 0; i < 6; i++)
		cout<<array[i]<<"  ";
	return(1);
}
