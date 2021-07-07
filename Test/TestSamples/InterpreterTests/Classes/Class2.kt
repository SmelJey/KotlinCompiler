class A {
	val a = if (true) {
		println("a init")
		2
	} else {
		3
	}
}

fun main() {
	println("main start")
	val a = A()
	println("test")
}