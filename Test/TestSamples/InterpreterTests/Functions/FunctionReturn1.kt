fun test() {
	println("a")
	return
	println("b")
}

fun main() {
	println("first")
	test()
	println("second")
}