class A {
	var a = 1

	fun test() {
		a++
	}
}

fun main() {
	val a = A()
	a.test()
	println(a.a)
}