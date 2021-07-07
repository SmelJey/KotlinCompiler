fun main() {
	val a = true;
	val b = if (a) {
		println("y")
		println(a)
		val a = false
		if (a) {
			println("y")
			println(a)
            12
		} else {
			println("n")
			println(a)
            -12
		}
	} else {
		println("n");
        -10
	}
	println(a)
    println(b)
}