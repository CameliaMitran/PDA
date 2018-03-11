package pda.lab.producerconsumer

class Consumer(private val buffer: SharedBuffer) : Thread(){

    override fun run() {
        while(true){
            buffer.semaphoreFull.release()//--
            val item = buffer.get()
            buffer.semaphoreFree.acquire()//++
            println(item+ " was removed from the buffer")
        }
    }
}