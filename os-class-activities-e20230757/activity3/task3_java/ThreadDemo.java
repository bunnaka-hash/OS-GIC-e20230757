public class ThreadDemo {

    static class MyThread extends Thread {
        private final int threadNumber;

        public MyThread(int number) {
            this.threadNumber = number;
        }

        @Override
        public void run() {
            System.out.println(
                "Thread " + threadNumber +
                " is running. Thread ID: " + Thread.currentThread().threadId()
            );

            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            System.out.println("Thread " + threadNumber + " finished.");
        }
    }

    public static void main(String[] args) {
        System.out.println("Main thread started.");

        for (int i = 1; i <= 4; i++) {
            MyThread thread = new MyThread(i);
            thread.start();
        }

        System.out.println("Main thread finished creating workers.");
    }
}

