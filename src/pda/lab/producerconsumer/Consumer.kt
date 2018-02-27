package pda.lab.producerconsumer

class Consumer(private val buffer: SharedBuffer) : Thread(){
    override fun run() {
        for (i in 1..10) {
            println("Message $buffer.get() is retrieved from the buffer.")
        }
    }
}