class A {
	var a = 5
}

fun test(a : A) {
	a.a = 1
}

fun main() {
	var a = A()
	test(a)
	println(a.a)
}
