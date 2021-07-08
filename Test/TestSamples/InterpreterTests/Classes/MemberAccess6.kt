class A {
	var a = 6
}

class B {
	var b = A()
	var c = 12
}

fun extraConstructor() : B {
	var b = B()
	var a = A()
	a.a = -1
	b.b = a
	b.c = -12
	return b
}

fun main() {
	var b = extraConstructor()
	println(b.b.a)
	println(b.c)
}
