fun main() {
	val a = arrayOf<Int>(1, 2)
	var b = arrayOf<Int>(1, 2)
	b = a
	println(a == b)
	println(a === b)
}