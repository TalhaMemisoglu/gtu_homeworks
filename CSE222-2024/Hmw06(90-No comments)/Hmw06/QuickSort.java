public class QuickSort extends SortAlgorithm {

	public QuickSort(int input_array[]) {
		super(input_array);
	}
	
    private int partition(int first, int last){
        //better solution for sorted array.
        int middle = (first + last) / 2;
        if(arr[middle] < arr[first]){
            swap(first, middle);
        }
        if(arr[last] < arr[middle]){
            swap(middle, last);
        }
        if(arr[middle] < arr[first]){
            swap(first, middle);
        }
        swap(first, (first + last)/2);
        //Unbalancing was reduced.

        int pivot = arr[first];
        int up = first;
        int down = last;
        do{
            while((up < last) && (pivot >= arr[up])){
                up++;
                comparison_counter++;
            }
            while(pivot < arr[down]){
                down--;
                comparison_counter++;
            }
            if(up < down) {
                swap(up, down);
            }
        } while(up < down);
        swap(first, down);
        return down;
    }

    private void sort(int first, int last){
        if(first < last){
            int pivIndex = partition(first, last);
            sort(first, pivIndex - 1);
            sort(pivIndex + 1, last);
        }
    }

    @Override
    public void sort() {
    	sort(0, arr.length - 1);
    }

    @Override
    public void print() {
    	System.out.print("Quick Sort\t=>\t");
    	super.print();
    }
}