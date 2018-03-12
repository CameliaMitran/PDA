package pda.lab.producerconsumer

import java.util.*
import java.util.concurrent.Semaphore

//constructor variable
class SharedBuffer(@Volatile var bufferSize: Int,
                   @Volatile var semaphoreFull: Semaphore,
                   @Volatile var semaphoreFree: Semaphore) {
    //val for immutable properties
    private val bufferList = ArrayList<String>()

    fun put(message: String) {
        bufferList.add(message)
    }

    @Synchronized
    fun get(): String {
        val message: String
        message = bufferList.removeAt(0)
    }
}

