fun test(a : Int, b : String) : Int = a

fun main() {
	val a = 1.2
	val b = 3.0
	test(a.toInt(), b.toString())
}