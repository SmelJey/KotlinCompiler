fun main() {
	val a = arrayOf<Array<Int>>(arrayOf<Int>(1, 2), arrayOf<Int>(3, 4), arrayOf<Int>(5, 6))
	val b = a[0]
	var c = a[1]
	val d = a[2]
	println(a[0][0] == b[0])
	println(a[0] === b)
	println(a[2][1] == d[1])
	
	val e = b[1]
	println(e == a[0][1])
	println(e === a[0][1])
	
	c = arrayOf<Int>(7)
	println(c === a[1])
	
	d[1] = 10
	println(d === a[2])
	println(a[2][1])
}