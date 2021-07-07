fun test() : Int = 1

fun test2(a : Int) : Int = a

fun main() {
	test2(test())
}
