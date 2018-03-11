package pda.lab.producerconsumer

import java.util.concurrent.Semaphore


fun main(args : Array<String>) {
    val buffer = SharedBuffer(2, Semaphore(0), Semaphore(2))

    Producer(buffer).start()
    Producer(buffer).start()
    Consumer(buffer).start()
    Consumer(buffer).start()
}