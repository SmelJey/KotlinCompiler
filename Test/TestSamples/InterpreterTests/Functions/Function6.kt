fun test(a : Array<Int>) {
	a[0]++
	a[1]++
	a[2] = 5
}

fun main() {
	val a = arrayOf<Int>(1, 2, 3)
	println(a[0])
	println(a[1])
	println(a[2])
}