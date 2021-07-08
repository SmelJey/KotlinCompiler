class A {
	fun test() {
		println("test method")
	}
}

fun main() {
	val a = A()
	println("main enter")
	a.test()
	println("main exit")
}