package pda.lab.producerconsumer

import java.util.*
import java.util.concurrent.Semaphore

//constructor variable
class SharedBuffer(@Volatile var bufferSize: Int,
                   @Volatile var semaphoreFull: Semaphore,
                   @Volatile var semaphoreFree: Semaphore) {
    //val for immutable properties
    private val bufferList = ArrayList<String>()
    private val lock = java.lang.Object()

    @Synchronized
    fun isFull(): Boolean { return bufferList.size == bufferSize}

    @Synchronized
    fun isEmpty(): Boolean { return bufferList.isEmpty()}

    fun put(message: String) = synchronized(lock) {
        while (isFull()){
            print(message)
            lock.wait()
        }
        bufferList.add(message)
        println("$message was added to the buffer.")
        lock.notifyAll()
    }

    @Synchronized
    fun get(): String = synchronized(lock){
        val message: String
        while (isEmpty()){
            println("Buffer is empty; waiting for producer")
            lock.wait()
        }
        message = bufferList.removeAt(0)
        println("$message was removed from buffer.")
        lock.notifyAll()
        return message
    }
}

