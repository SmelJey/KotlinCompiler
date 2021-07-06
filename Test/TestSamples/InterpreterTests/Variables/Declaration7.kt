fun main() {
	val a = arrayOf<Int>(1,2)
	val b = a
	a[0]++
	println(b[0])
	var c = a[0]
	c++
	println(c)
	println(a[0])
}