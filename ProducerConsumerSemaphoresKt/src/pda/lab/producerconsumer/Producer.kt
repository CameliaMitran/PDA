package pda.lab.producerconsumer

class Producer(private val buffer: SharedBuffer) : Thread() {
    //var for mutable properties
    //companion is equivalent to java static keyword
    companion object {
        private var counter: Int = 0
    }

    override fun run() {
        while(true){
            val item = generateMessage()
            buffer.semaphoreFree.release()//--
            buffer.put(item)
            buffer.semaphoreFull.acquire()//++
        }
    }

    private fun generateMessage(): String {
        val message= "Message " + counter
        counter++
        return message
    }
}