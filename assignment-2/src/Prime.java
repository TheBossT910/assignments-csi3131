import java.util.Scanner;

public class Prime {

    public static void main(String[] args) {
    	String userInput = args[0];

        int value;

        try {
            value = Integer.parseInt(userInput);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Expected an integer, got: " + userInput);
        }

        Thread primeThread = new Thread(() -> {
            for (int i = 2; i <= value; i++) {
                if (isPrime(i)) {
                    System.out.println(i);
                }
            }
        });

        primeThread.start();

        try {
            primeThread.join(); // wait for thread to finish
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public static boolean isPrime(int n) {
        if (n < 2) {
            return false;
        }

        for (int i = 2; i*i <= n; i++) {
            if (n % i == 0) {
                return false;
            }
        }

        return true;
    }
}