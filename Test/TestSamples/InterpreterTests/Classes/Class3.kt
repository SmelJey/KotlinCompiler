class A {
	var a = 1
}

fun main() {
	val a = A()
	val b = A()
	val c = a
	
	println(a == b)
	println(a == c)
	println(a === b)
	println(a === c)
}