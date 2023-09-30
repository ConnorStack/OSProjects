/*
 * Job of the class is to generate random double arrays
 * File: BuildRandomDoubleArray.java
 * Author: Connor Stack
 */
package prog_j.threads;

import java.util.Random;

public class BuildRandomDoubleArray{
    private double [] randomArray;
    Random rand = new Random();

    public BuildRandomDoubleArray(int length){
        randomArray = new double[length];
        this.randomArray = generateRandArr(randomArray);

    }
    private double[] generateRandArr(double[] randomArray){
        for(int i = 0; i < randomArray.length; i++){
            randomArray[i] = rand.nextDouble() * 1000;
        }

        return randomArray;
    }

    public double[] getRandomArray(){
        return this.randomArray;
    }
}