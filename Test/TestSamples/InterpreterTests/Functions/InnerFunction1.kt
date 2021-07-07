fun test() : Int = 5

fun main() {
	fun test2(x: Int) {
		println(x)
	}

	test2(test())
}