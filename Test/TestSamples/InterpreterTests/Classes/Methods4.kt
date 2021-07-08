class A {
	var a = arrayOf<Int>(1, 2)

	fun test() : Array<Int> {
		a[0] *= 2
		a[1] *= 2
		return a
	}
}

fun main() {
	val a = A()
	val arr = a.test()
	println(arr[0])
	println(a.a[0])
	a.test()
	println(arr[0])
}