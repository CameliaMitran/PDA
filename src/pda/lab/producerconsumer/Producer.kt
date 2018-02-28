package pda.lab.producerconsumer

class Producer(private val buffer: SharedBuffer) : Thread() {
    //var for mutable properties
    //companion is equivalent to java static keyword
    companion object {
        private var counter: Int = 0
    }

    override fun run() {
        for (i in 1..10) {
            println("" + buffer.put(generateMessage()) + " is added to the buffer.")
        }
    }

    private fun generateMessage(): String {
        val message= "Message " + counter
        counter++
        return message
    }
}