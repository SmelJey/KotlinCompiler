fun main() {
	val arr = arrayOf<Array<Int>>(arrayOf<Int>(1, 2, 3), arrayOf<Int>(4, 5, 6), arrayOf<Int>(7, 8, 9))
	for (row in arr) {
		for (col in row) {
			println(col)
		}
	}
}