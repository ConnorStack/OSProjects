package prog_j.threads;

public class MergeAvgThread extends Thread {

    double[] unmergedFirstHalf;
    double[] unmergedSecondHalf;

    double[] mergedArray;
    double[] mergedArrayByThread;
    int fullLen;
    double average;
    double averageByThread;

    public MergeAvgThread(double[] firstHalf, double[] secondHalf) {
        this.fullLen = firstHalf.length + secondHalf.length;

        this.unmergedFirstHalf = firstHalf;
        this.unmergedSecondHalf = secondHalf;

        this.mergedArray = new double[fullLen];
        this.mergedArrayByThread = new double[fullLen];
        // this.mergedArray = mergeArrays(firstHalf, secondHalf); method to thread
    }

    public void nonThreadedMergeAvgThread() {
        mergedArray = mergeArrays(unmergedFirstHalf, unmergedSecondHalf);
        average = calcAverage(mergedArray);
    }

    public void threadedMergeAvgThread() {
        this.mergedArrayByThread = mergedArrayByThread;

    }

    public void run() {
        mergedArrayByThread = mergeArrays(unmergedFirstHalf, unmergedSecondHalf);
        averageByThread = calcAverage(mergedArrayByThread);
    }

    private double[] mergeArrays(double[] firstHalf, double[] secondHalf) {
        int n, m;
        n = 0;
        m = 0;

        for (int i = 0; i < fullLen; i++) {
            if (n < firstHalf.length && (m == secondHalf.length || firstHalf[n] <= secondHalf[m])) {
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
