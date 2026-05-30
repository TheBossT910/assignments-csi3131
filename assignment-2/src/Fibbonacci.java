import java.util.Scanner;

public class Fibbonacci {

	public static void main(String[] args) {
        String userInput = args[0];

        int value;

        try {
            value = Integer.parseInt(userInput);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Expected an integer, got: " + userInput);
        }
        
        if (value <= 0) {
            throw new IllegalArgumentException(
                "Number of Fibonacci values must be positive");
        }
        
        int[] sequence = new int[value];
        
        Thread fibbThread = new Thread(() -> {
        	sequence[0] = 0;
        	if (value > 1) {
        		sequence[1] = 1;
        	}
            for (int i = 2; i < value; i++) {
                sequence[i] = sequence[i-1] + sequence[i-2];
            }
        });
        
        fibbThread.start();

        try {
            fibbThread.join(); // wait for thread to finish
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        for (int i = 0; i < value; i++) {
        	System.out.print(sequence[i] + " ");
        }
        System.out.println();

	}

}
