fun main() {
	val a = true;
	if (a) {
		println("y")
		println(a)
		val a = false
		if (a) {
			println("y")
			println(a)
		} else {
			println("n")
			println(a)
		}
	} else {
		println("n");
	}
	println(a)
}