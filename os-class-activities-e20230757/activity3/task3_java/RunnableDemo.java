public class RunnableDemo {

    static class MyTask implements Runnable {
        private final int taskNumber;

        public MyTask(int number) {
            this.taskNumber = number;
        }

        @Override
        public void run() {
            System.out.println(
                "Task " + taskNumber +
                " is running on thread: " +
                Thread.currentThread().getName()
            );

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            System.out.println("Task " + taskNumber + " finished.");
        }
    }

    public static void main(String[] args) {
        System.out.println("Main thread started.");

        for (int i = 1; i <= 4; i++) {
            Thread thread = new Thread(new MyTask(i));
            thread.start();
        }

        System.out.println("Main thread finished creating tasks.");
    }
}
