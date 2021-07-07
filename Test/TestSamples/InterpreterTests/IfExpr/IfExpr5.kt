fun main() {
	val a = arrayOf<Boolean>(true, false)
	if (a[0]) {
		if (!a[1])
			println("true")
	} else {
		println("0 false")
	}
}