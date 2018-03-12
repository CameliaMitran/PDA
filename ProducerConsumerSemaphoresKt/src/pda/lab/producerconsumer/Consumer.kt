package pda.lab.producerconsumer

class Consumer(private val buffer: SharedBuffer) : Thread(){

    override fun run() {
        while(true){
            buffer.semaphoreFull.acquire()//--
            val item = buffer.get()
            buffer.semaphoreFree.release()//++
            println(item+ " was removed from the buffer")
        }
    }
}
