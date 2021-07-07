var a = 1

fun test() : Int = a++

fun main() {
	var a = 0

	fun test2() : Int {
		return a--
	}
	
	fun test3(x : Int) : Int {
		var a = x
		return ++a
	}
	
	println(test())
	println(test2())
	println(test3(10))
	
	fun testAll() {
		println(test())
		println(test2())
		println(test3(10))
	}
	
	testAll()
}