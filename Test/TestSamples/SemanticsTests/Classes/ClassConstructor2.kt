class A {
	fun test() : Int = 2
}

fun test() : A {
	return A()
}

fun main() {
	val a = test().test()
}