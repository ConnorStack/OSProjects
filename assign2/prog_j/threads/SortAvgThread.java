package prog_j.threads;

public class SortAvgThread extends Thread {

    double[] unsortedArray;
    double[] unsortedArrayByThread;
    double[] sortedArray;
    double[] sortedArrayByThread;
    double average;
    double averageByThread;
    int length;

    public SortAvgThread(double[] array) {
        this.unsortedArray = array;
        this.unsortedArrayByThread = array;
    }

    public void sortArrNonThread() {
        this.sortedArray = sortArray(unsortedArray);
        this.average = calcAverage(sortedArray);
    }

    public void sortArrByThread() {
        this.sortedArrayByThread = unsortedArray;
    }

    public void run() {
        sortedArrayByThread = sortArray(unsortedArrayByThread);
        averageByThread = calcAverage(sortedArrayByThread);
    }

    private double[] sortArray(double[] sortedArray) {

        int i;
        for (int j = 1; j < sortedArray.length; j++) {
            double insertVal = sortedArray[j];
            i = j - 1;

            while (i >= 0 && insertVal < sortedArray[i]) {
                sortedArray[i + 1] = sortedArray[i];
                i = i - 1;
            }
            sortedArray[i + 1] = insertVal;
        }

        return sortedArray;
    }

    public double[] getSortedArray() {
        return this.sortedArray;
    }

    public double[] getSortedArrayByThread() {
        return this.sortedArrayByThread;
    }

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

}
