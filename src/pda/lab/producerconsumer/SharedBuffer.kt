package pda.lab.producerconsumer

import java.util.*

//constructor variable
class SharedBuffer(private val bufferSize: Int) {
    //val for immutable properties
    private val bufferList = ArrayList<String>()
    private val lock = java.lang.Object()

    fun isFull(): Boolean { return bufferList.size == bufferSize}

    fun isEmpty(): Boolean { return bufferList.isEmpty()}

    @Synchronized
    fun put(message: String) = synchronized(lock) {
        while (isFull()){
            print(message)
            lock.wait()
        }
        //Thread.sleep(Random().nextInt(100).toLong())
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

