package pda.lab.producerconsumer
//Testing git commit

fun main(args : Array<String>) {
    val buffer = SharedBuffer(5)

    Producer(buffer).start()
    Producer(buffer).start()
    Producer(buffer).start()
    Consumer(buffer).start()
    Consumer(buffer).start()
    Consumer(buffer).start()
}