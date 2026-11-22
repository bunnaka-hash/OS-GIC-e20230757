import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class PoolDemo {

    static class MyTask implements Runnable {
        private final int taskNumber;

        public MyTask(int number) {
            this.taskNumber = number;
        }

        @Override
        public void run() {
            System.out.println(
                "Task " + taskNumber +
                " is running on " +
                Thread.currentThread().getName()
            );

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            System.out.println("Task " + taskNumber + " completed.");
        }
    }

    public static void main(String[] args) {
        System.out.println("Main thread started.");

        ExecutorService executor = Executors.newFixedThreadPool(2);

        for (int i = 1; i <= 6; i++) {
            executor.submit(new MyTask(i));
        }

        executor.shutdown();

        try {
            if (!executor.awaitTermination(10, TimeUnit.SECONDS)) {
                executor.shutdownNow();
            }
        } catch (InterruptedException e) {
            executor.shutdownNow();
            Thread.currentThread().interrupt();
        }

        System.out.println("All tasks completed.");
    }
}
