fun main() {
	println(4 in arrayOf<Int>(1, 2, 4, 5))
	println(3 in arrayOf<Int>(1, 5))
	println("aba" in arrayOf<String>("a", "aba"))
	println(2.4 in arrayOf<Double>(1.2, 2.4,))
}