fun main() {
	val a = arrayOf<Int>(1, 2)
	val b = arrayOf<Int>(1, 2)
	val c = a
	println(a == b)
	println(a === b)
	println(a === c)
}