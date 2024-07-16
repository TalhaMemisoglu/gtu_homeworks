public class SelectionSort extends SortAlgorithm {

	public SelectionSort(int input_array[]) {
		super(input_array);
	}

    @Override
    public void sort() {
        int minVal;
        for(int i=0; i < arr.length; i++){
            minVal = i;
            for(int j = (i + 1); j<arr.length; j++){
                if(arr[j] < arr[minVal]){
                    minVal = j;
                }
                comparison_counter++;
            }
            if(i != minVal) //If minimum value is already in correct position.
                swap(i, minVal);
        }
    }

    @Override
    public void print() {
    	System.out.print("Selection Sort\t=>\t");
    	super.print();
    }
}
