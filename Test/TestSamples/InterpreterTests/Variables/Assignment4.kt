fun main() {
	val a = arrayOf<Int>(1,2)
	val b = a
	a[0] = 2 + 3
	println(b[0])
	var c = a[0]
	c = a[1]
	println(c)
	println(a[0])
}