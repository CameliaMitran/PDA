package pda.lab.producerconsumer

class Producer(private val buffer: SharedBuffer) : Thread() {
    //var for mutable properties
    var counter: Int = 0

    override fun run() {
        for (i in 1..10) {
            println("Message $buffer.put() is retrieved from the buffer.")
        }
    }
}