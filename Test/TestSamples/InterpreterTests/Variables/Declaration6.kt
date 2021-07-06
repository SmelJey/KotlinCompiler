fun main() {
	val a = 1..2
	val b = 1..2
	val c = a
	println(a == b)
	println(a === b)
	println(a === c)
}