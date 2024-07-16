public class BubbleSort extends SortAlgorithm {

	public BubbleSort(int input_array[]) {
		super(input_array);
	}
	
    @Override
    public void sort() {
    	for(int i = 0; i < (arr.length - 1); i++){
            for(int j = 0; j < (arr.length - 1) - i; j++){
                if(arr[j] > arr[j + 1]){
                    swap(j, j + 1);
                }
                comparison_counter++;
            }
            if(swap_counter == 0) //If array is already sorted.
                break;
        }
    }
    
    @Override
    public void print() {
    	System.out.print("Bubble Sort\t=>\t");
    	super.print();
    }
}
