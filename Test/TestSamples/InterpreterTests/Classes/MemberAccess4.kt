class A {
	var a = arrayOf<Int>(1, 2, 3)
}

fun main() {
	var a = A()
	a.a[2] = 4
	
	println(a.a[0])
	println(a.a[1])
	println(a.a[2])
}
