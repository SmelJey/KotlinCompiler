class A {
	var a = 6
}

class B {
	var b = A()
}

fun main() {
	var b = B()
	println(b.b.a)
}
