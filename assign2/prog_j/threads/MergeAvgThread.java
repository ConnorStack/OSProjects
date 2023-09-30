package prog_j.threads;

public class MergeAvgThread extends Thread {
    // --- This is the same comment in SortAvgThread
    //This is my general incompetence with threads, but I make a member variable for threaded and non threaded seperately
    //With a new object these values could be reset and the time for execution would still be accurate for each case, 
    //But I wanted to be more explicit with which arrays were being called where, so I knew exactly what was being called.
    //I didn't want to accidentally share data and have runtime be skewed somehow, but this is probably overkill.
    double[] unmergedFirstHalf;
    double[] unmergedSecondHalf;
    double[] mergedArray;
    double[] mergedArrayByThread;
    int fullLen;
    double average;
    double averageByThread;

    //Constructor just sets lengths and prets arrays with their proper size
    public MergeAvgThread(double[] firstHalf, double[] secondHalf) {
        this.fullLen = firstHalf.length + secondHalf.length;

        this.unmergedFirstHalf = firstHalf;
        this.unmergedSecondHalf = secondHalf;

        this.mergedArray = new double[fullLen];
        this.mergedArrayByThread = new double[fullLen];
    }

    // This is the implementation for a non threaded method
    public void nonThreadedMergeAvgThread() {
        mergedArray = mergeArrays(unmergedFirstHalf, unmergedSecondHalf);
        average = calcAverage(mergedArray);
    }

    // This is the implementation for a threaded method
    public void threadedMergeAvgThread() {
        this.mergedArrayByThread = mergedArrayByThread;

    }

    //I suppose this is like javas pthread_create
    public void run() {
        mergedArrayByThread = mergeArrays(unmergedFirstHalf, unmergedSecondHalf);
        averageByThread = calcAverage(mergedArrayByThread);
    }

    //merge array algorithm
    private double[] mergeArrays(double[] firstHalf, double[] secondHalf) {
        double total = 0;
        int n, m;
        //corresponds to first half
        n = 0;
        //corresponds to second half
        m = 0;

        for (int i = 0; i < fullLen; i++) {
            
            //If n is less than firstHalf.length, then we expect more n indexed values to be merged.
            //Now one of two things is possible, either values indexed from m are full (in which case we know all others will be n indexed)
            //Or our standard case, n index value is less than m index value, so it should be placed in the merged array
            //m == second half length means m index is full or firsthalf[n] < secondHalf[m]
            if (n < firstHalf.length && (m == secondHalf.length || firstHalf[n] < secondHalf[m])) {
                mergedArray[i] = firstHalf[n];
                n++;
            } else if (m < secondHalf.length) {
                mergedArray[i] = secondHalf[m];
                m++;
            }
        }

        return mergedArray;
    }

    public double[] getMergedArray() {
        return this.mergedArray;
    }

    //average calculation as per instructions
    private double calcAverage(double[] array) {
        double total = 0.0;
        int count = 0;
        double average = 0.0;
        for (int i = 0; i < array.length; i++) {
            total += array[i];
            count++;
        }
        if (count == 0) {
            return -1;
        }
        average = total / count;
        return average;

    }

    public double getAverage() {
        return this.average;
    }

    public double getAverageByThread() {
        return this.averageByThread;
    }

}
