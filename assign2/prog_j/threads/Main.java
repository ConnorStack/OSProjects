/*
 * Job of the class is to demonstrate thread vs non thread runtime
 * File: Main.java
 * Author: Connor Stack
 */
package prog_j.threads;

public class Main {
    public static void main(String[] args) {

        if (!Validate.isArgsValid(args)) {
            System.out.println("Invalid input");
            return;
        }
        int n = Validate.getArgValue();
        int half_n = n / 2;
        n *= 2;

        // ArraySortBuilder takes size n and creates an object of BuildRandomDoubleArray
        // which creates a random double array
        // it then sends the random array into the constructor of SortAvgThread, which
        // preps the object with everything it needs before sorting
        // admittedly arraySortBuilder is not the most descriptive name, but the code
        // duplication was getting bonkers
        // and I couldn't think of a better name

        SortAvgThread firstHalfSorter = arraySortBuilder(half_n);
        SortAvgThread secondHalfSorter = arraySortBuilder(half_n);

        // Start non-threaded clock and display results
        long nonThreadTime = System.nanoTime();

        firstHalfSorter.sortArrNonThread();
        secondHalfSorter.sortArrNonThread();

        MergeAvgThread mergeAvgThread = new MergeAvgThread(firstHalfSorter.getSortedArray(),
                secondHalfSorter.getSortedArray());

        mergeAvgThread.nonThreadedMergeAvgThread();
        long endNonThreadTime = System.nanoTime();
        System.out.println(
                "Sorting is done in " + getElapsedTime(nonThreadTime, endNonThreadTime) + "ms when one thread is used");

        // As earlier in the program, arraySortBuilder will create a couple more objects
        // that will contain random double arrays
        SortAvgThread threadedSorterFirstHalf = arraySortBuilder(half_n);
        SortAvgThread threadedSorterSecondHalf = arraySortBuilder(half_n);

        // Start threaded clock and display results
        long threadedTime = System.nanoTime();
        startThread(threadedSorterFirstHalf, threadedSorterSecondHalf);

        try {
            threadedSorterFirstHalf.join();
            threadedSorterSecondHalf.join();

        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        MergeAvgThread threadedMergeAvg = new MergeAvgThread(threadedSorterFirstHalf.getSortedArrayByThread(),
                threadedSorterSecondHalf.getSortedArrayByThread());
        threadedMergeAvg.threadedMergeAvgThread();
        threadedMergeAvg.start();
        try {
            threadedMergeAvg.join();

        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        long endThreadedTime = System.nanoTime();
        System.out.println(
                "Sorting is done in " + getElapsedTime(threadedTime, endThreadedTime) + "ms when two threads are used");
    }

    //this method isn't very necessary, just cutting down on main() bloat
    public static void startThread(SortAvgThread threadedSorterFirstHalf, SortAvgThread threadedSorterSecondHalf) {
        threadedSorterFirstHalf.sortArrByThread();
        threadedSorterFirstHalf.start();
        threadedSorterSecondHalf.sortArrByThread();
        threadedSorterSecondHalf.start();
    }

    //For debugging
    public static void displayDoubleArr(double[] dArr) {
        System.out.println();
        for (int i = 0; i < dArr.length; i++) {
            System.out.println(dArr[i]);
        }

    }

    //Method to get elapsed time
    public static double getElapsedTime(long start, long end) {
        double elapsed = (end - start) / 1e6;
        return elapsed;
    }

    //The intention of this method is to prepare SortAvgThread with randomArray object, obfuscating some implementation that lead to main() bloat
    public static SortAvgThread arraySortBuilder(int n) {
        BuildRandomDoubleArray randomArray = new BuildRandomDoubleArray(n);
        SortAvgThread arraySorter = new SortAvgThread(randomArray.getRandomArray());
        return arraySorter;
    }

}
