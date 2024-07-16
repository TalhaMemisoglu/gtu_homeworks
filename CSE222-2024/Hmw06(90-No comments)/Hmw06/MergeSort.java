public class MergeSort extends SortAlgorithm {
	
	public MergeSort(int input_array[]) {
		super(input_array);
	}
	
	private void merge(int[] arr1, int[] arr2, int[] output){
        int i = 0;
        int j = 0;
        int k = 0;
        while(i < arr1.length && j <arr2.length){
            if(arr1[i] <= arr2[j]){
                output[k++] = arr1[i++];
            } else {
                output[k++] = arr2[j++];
            }
            comparison_counter++;
        }

        while(i < arr1.length){
            output[k++] = arr1[i++];
        }
        while(j < arr2.length){
            output[k++] = arr2[j++];
        }
    }

    private void sort(int[] arr){
        if(arr.length > 1){
            int halfSize = arr.length / 2;
            int[] leftArr = new int[halfSize];
            int[] rightArr = new int[arr.length - halfSize];
            System.arraycopy(arr, 0, leftArr, 0, halfSize);
            System.arraycopy(arr, halfSize, rightArr, 0, arr.length - halfSize);

            sort(leftArr);
            sort(rightArr);
            
            merge(leftArr, rightArr, arr);
        }
    }
    
    @Override
    public void sort() {
    	sort(arr);
    }
    
    @Override
    public void print() {
    	System.out.print("Merge Sort\t=>\t");
    	super.print();
    }
}
